/** @file
 * modbus.cpp
 *
 *  Created on: Feb 8, 2022
 *      Author: mhg
 *
 *  Sep 10,2024 MHG Port for Kinetis K22
 *
 */
#include "modbus.h"
#include "global_data.h"
#include "c_hal.h"  // send, receive
#include <string.h> // memcpy
#include <assert.h>
#include "crc.h"
#include "c_modbus.h"

// Globals
modbus_read_t modbus_read_request;
modbus_write_t modbus_write_request;
modbus_write_read_t modbus_write_read_request;

bool modbus_device_busy;
uint16_t modbus_multiple_read_max  = MODBUS_MULTIPLE_RD_MAX;
uint8_t modbus_tx_buffer[MODBUS_SERIAL_BUFFER_SIZE];
static uint8_t modbus_rx_buffer[MODBUS_SERIAL_BUFFER_SIZE];
static uint8_t modbus_tx_exception[10]; // enough to hold an exception reply

static uint8_t  *slave_address = &modbus_rx_buffer[0];
static uint8_t  *function_code = &modbus_rx_buffer[1];
//static uint16_t *reg_address = (uint16_t *)&modbus_rx_buffer[2];  // Big endian
static uint16_t reg_address;
static uint16_t quantity; // = (uint16_t *)&modbus_rx_buffer[4];  // read & write multiple
static int16_t *write_reg_value;


void SendModbus(uint16_t length) {
  hal->Send485(modbus_tx_buffer, length);
}

void RequestResponse(modbus_read_t *rr) {
  rr->received = true;  // return 0 (exception) if valid
  rr->self_address = *slave_address;
  rr->function_code = *function_code;
  rr->quantity = quantity;
  rr->address = reg_address;
}

void RequestResponse(modbus_write_t *rr) {
  rr->received = true;
  rr->self_address = *slave_address;
  rr->function_code = *function_code;
  rr->address = reg_address;
  rr->quantity = *function_code == WRITE_MULTIPLE_REGISTERS ? quantity : 1;
  rr->data = write_reg_value;
}


void RequestResponseWriteMultipleRegisters(modbus_write_read_t *rr) {
  rr->received = true;
  rr->self_address = *slave_address;      // 0
  rr->function_code = *function_code;     // 1
  rr->rd_address =   *((uint16_t *)&modbus_rx_buffer[2]);
  rr->rd_quantity =  *((uint16_t *)&modbus_rx_buffer[4]);
  rr->wr_address =   *((uint16_t *)&modbus_rx_buffer[6]);
  rr->wr_quantity =  *((uint16_t *)&modbus_rx_buffer[8]);
  rr->wr_data   =     &modbus_rx_buffer[11];
}


/**
 * Verify frame length for required message
 *
 * The size of all functions need to be provided, even if not implemented
 *
 */
static bool CheckCorrectFrameSize(uint16_t rx_bytes) {

  if (rx_bytes <= 4)
    return false;

  switch (*function_code) {
    case  READ_COILS                        : // Read Coils                  0x01
    case  READ_DISCRETE_INPUTS              : // Read Discrete Inputs        0x02
    case  READ_HOLDING_REGISTERS            : // Read Holding Registers      0x03
    case  READ_INPUT_REGISTERS              : // Read Input Registers        0x04
    case  WRITE_SINGLE_COIL                 : // Write Single Coil           0x05
    case  WRITE_SINGLE_REGISTER             : // Write Single Register       0x06
    case  DIAGNOSTICS                       : // Diagnostics                 0x08
      return rx_bytes == 8;  // ID(1) + function(1) + Address/sub-function(2) + Quantity/Value/Data(2) + CRC(2)

    case  READ_EXCEPTION_STATUS             : // Read Exception Status       0x07
    case  GET_COMM_EVENT_COUNTER            : // Get COMM Event Counter      0x0B
    case  GET_COMM_EVENT_LOG                : // Get COMM Event Log          0x0C
    case  REPORT_SLAVE_ID                   : // Report Slave ID             0x11
      return rx_bytes == 4;  // ID(1) + function(1) +  CRC(2) remains.
    break;

    case  WRITE_MULTIPLE_COILS              : // Write Multiple Coils: // Write Multiple Coils0x0F: // Write Multiple Coils - various length - write 1byte each
      return rx_bytes == 9;  // ID(1) + function(1) +  address(2) + quantity(2) + byte Count(1*N) + [output value(1*N)] + CRC(2)                 //
    break;

    case  WRITE_MULTIPLE_REGISTERS          : // Write Multiple registers : // Write Multiple registers = 9 + 2*qty, One register = 11
      return rx_bytes >= 11;  // ID(1) + function(1) +  address(2) + quantity(2) + byte count(N) + [output value(2*N)] + CRC(2)
    break;

    case  READ_FILE_RECORD                  : // Read File Record - various length - Byte count * 7 + CRC(2)
      return rx_bytes == 11; // ID(1) + function(1) + Byte count(1) + Ref.type(1) + file#(2) + record#(2) + record length(2) + CRC(2)
              // Byte count + 1(self) + CRC(2)
    break;

    case  WRITE_FILE_RECORD                 : // Write File Record
      return rx_bytes == 14; // ID(1) + function(1) + request length(1) + Ref.type(1) + file#(2) +record#(2) + record length(2) + record data(2*N) + CRC(2)
              // request length + 1(self) + CRC(2)
    break;
    case  MASK_WRITE_REGISTER               : // Mask Write Register 0x16
      return rx_bytes == 10; // ID(1) + function(1) + ref.address(2) + and mask(2) + or mask(2) + CRC(2)
    break;

    case  READ_WRITE_MULTIPLE_REGISTERS     : // 0x17 // Read/Write Multiple registers: // Read/Write Multiple registers
      return rx_bytes == 13 + *(uint16_t *)&modbus_rx_buffer[8]*2;
//      return rx_bytes == 15; // ID(1) + function(1) + Read Start Address(2) + Quantity to Read(2) + Write Starting address(2) + Quantity to Write(2) + Write byte count(1) + Write Registers Values(2*N) + CRC(2)
              // 9 + (2*Write Byte Count) + CRC(2)
    break;
    case  READ_FIFO_QUEUE                   : // 0x18 // Read FIFO Queue 0x18: // Read FIFO Queue
      return rx_bytes == 6;  // ID(1) + function(1) + FIFO Pointer Address(2) + CRC(2)
    break;
    case  ENCAPSULATED_INTERFACE_TRANSPORT  : // 0x2B: // Encapsulated Interface Transport ** 2B+0D (CAN open), 2B+0E (Read Device ID) can be.
      return rx_bytes == 6;  // ID(1) + function(1) + MEI type(1) + MEI data(N) + CRC(2)
    break;

    default:
      break;
  }
  return false;
}

/**
 * Exception code 1: Unimplemented FNC
 *
 */
static bool FunctionCodeNotSupported(void) {
  if (*function_code == READ_HOLDING_REGISTERS
      || *function_code == WRITE_SINGLE_REGISTER
      || *function_code == WRITE_MULTIPLE_REGISTERS
//      || *function_code == READ_WRITE_MULTIPLE_REGISTERS
      )
    return false;
  return true;
}

/**
 * Exception code 2: Illegal address
 * - R/W out of modbus_max_addresable range (default is 0-255)
 * - Write to
 *  1) write to unused (address not defined)
 *  2) read only
 *  3) protected address
 *  03 rmultiple, 06 wsingle, 16 wmultiple
 *
 *  \todo Rewrite this function -
 *  Take lut and generate a min-modbus_max_addresable addresses
 */
static bool AddressIsNotValid(void) {
  uint16_t qty = *function_code == WRITE_SINGLE_REGISTER ? 1 : quantity;
  // Address R/W valid range

  bool write_operation =
      *function_code == READ_WRITE_MULTIPLE_REGISTERS
      || *function_code == WRITE_MULTIPLE_REGISTERS
      || *function_code == WRITE_SINGLE_REGISTER;

//  if( *function_code == READ_WRITE_MULTIPLE_REGISTERS)
//    return false; // fake-it
//  else
//  if ((reg_address + qty) > c_modbus::modbus_max_addresable)  // R/W out of range
//    return true;
//  else
//    if(*function_code == READ_HOLDING_REGISTERS) // we can read read, host knows what will do (-1 in low range)
//      return false;

  // Valid write access
  bool not_valid = false;
  for (uint16_t i = 0; i < qty; ++i) {
    // Test not defined first to break
    if( modbus->AddressNotDefined(reg_address + i) || // R/W
        write_operation && modbus->DataIsWriteProtected(reg_address + i) ) {
      not_valid = true;
      break;
    }
    // Parameter is > 16bit
    if ((reg_address + i) >= MODBUS_STD_TBL_SIZE)
      ++i;
  }
  return not_valid;
}

/**
 * Exception code 3: Bad Data (Illegal Data Value)
 * - Read more than MODBUS_MULTIPLE_RD_MAX parameters MODBUS_MULTIPLE_RD_MAX
 * - Write invalid data (min/max) for specific parameter.
 *
 */
static bool BadData(void) {
  if (*function_code == READ_HOLDING_REGISTERS && quantity > modbus_multiple_read_max)  // read more than 5
    return true;

  if( reg_address >= MODBUS_STD_TBL_SIZE)  // We don't check extended range until sw is close to complete
    return false;

  if (*function_code == WRITE_SINGLE_REGISTER  && modbus->DataOutOfLimits(reg_address, *write_reg_value))
    return true;

  if(*function_code == WRITE_MULTIPLE_REGISTERS ) { // Add support for FNC 16, note: We abort the whole range on 1st invalid,
    for(int i =0; i< quantity; ++i)
      if(modbus->DataOutOfLimits(reg_address + i, write_reg_value[i]))
        return true;
  }
  return false;

}

static void SendExceptionResponse(uint8_t code) {
  modbus_tx_exception[0] = *slave_address;
  modbus_tx_exception[1] = *function_code | 0x80;
  modbus_tx_exception[2] = code;
  uint16_t CRC = GetCRC(modbus_tx_exception, 3);
  *((uint16_t *) &modbus_tx_exception[3]) = CRC;
  hal->Send485(modbus_tx_exception, 5);
}

/**
 *  Process the frame stored in RX buffer
 *
 *  - function continues the RX_ERROR isr (timeout matches RTU gap)
 *  - Verify message integrity: size and checksum
 *  - Message is ignored if malformed or not intended for this device address,
 *  - Valid message directed to this station is always responded
 *
 *  Exceptions 1,2,3 responses are generated here
 *  1 - Unimplemented command
 *  2 - Illegal address: access out of range or a write to unused or `protected`
 *  3 - Bad data: more than MODBUS_MULTIPLE_RD_MAX, invalid data (out of limits)
 *  6 - Command not allowed: device is busy
 *
 *  References:
 *  - Section 7 - MODBUS Exception Responses - MODBUS Application Protocol Specification V1.1b3
 *  - Figure 9 - MODBUS Transaction state diagram - MODBUS Application Protocol v1.1b
 *  - Calnetix - BLDC Drive Parameters
 */
void OnModbusFrameReceived(void) {

  uint16_t frame_size = hal->GetRxBuffer485(modbus_rx_buffer); // ring->linear returns nbr

  // --- Discard message criteria
  if (!CheckCorrectFrameSize(frame_size))
    return;
  else {
    reg_address = (modbus_rx_buffer[2] << 8) + modbus_rx_buffer[3];    // Little endian
    quantity =    (modbus_rx_buffer[4] << 8) + modbus_rx_buffer[5];    // Little endian
    write_reg_value = *function_code == WRITE_SINGLE_REGISTER ?
         (int16_t *)&modbus_rx_buffer[4] : // single write
         (int16_t *)&modbus_rx_buffer[7]; // multiple write

    uint16_t &CRC = *((uint16_t *) &modbus_rx_buffer[frame_size - 2]);
    if (CRC == GetCRC(modbus_rx_buffer, frame_size - 2)) {
      if (MODBUS_LOCAL_ADDRESS != *slave_address)
        return; // not for this VSD
    } else
      return; // wrong CRC
  }
  // --- Modbus response required
  int exception_code = 0;

  // Exceptions 1,2,3
  if (FunctionCodeNotSupported())
    exception_code = EXCEPTION_ILLEGAL_FUNCTION;
  else if (AddressIsNotValid())
    exception_code = EXCEPTION_ILLEGAL_ADDRESS;
  else if (BadData())
    exception_code = EXCEPTION_ILLEGAL_DATA_VALUE;
  else if (modbus_device_busy)
    exception_code = EXCEPTION_SERVER_DEVICE_BUSY;
  else
      // Exception 6 may be generated by executing MB command 03, 06
    switch (*function_code) {
      case READ_HOLDING_REGISTERS:
        RequestResponse(&modbus_read_request);
        break;
      case WRITE_SINGLE_REGISTER:
      case WRITE_MULTIPLE_REGISTERS:
        RequestResponse(&modbus_write_request);
        break;
//      case READ_WRITE_MULTIPLE_REGISTERS:
//        RequestResponseWriteMultipleRegisters(&modbus_write_read_request);
//        break;
      default:
        assert(0);  // This should happen - catch
        return;
        break;
    }
  // Send exception response, if we have one
  if(exception_code)
    SendExceptionResponse(exception_code);
}
