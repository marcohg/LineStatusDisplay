/** @file
 * modbus.h
 *
 * @brief Modbus lower level interface
 *
 *  Created on: Feb 14, 2022
 *      Author: mhg
 *  Sep 10,2024	MHG	Use FNC defined names instead FNC code
 *   
 *
 */

#ifndef INCLUDE_MODBUS_H_
#define INCLUDE_MODBUS_H_

#include "typedefs.h"


#define READ_COILS                        0x01 // Read Coils
#define READ_DISCRETE_INPUTS              0x02 // Read Discrete Inputs
#define READ_HOLDING_REGISTERS            0x03 // Read Holding Registers
#define READ_INPUT_REGISTERS              0x04 // Read Input Registers
#define WRITE_SINGLE_COIL                 0x05 // Write Single Coil
#define WRITE_SINGLE_REGISTER             0x06 // Write Single Register
#define DIAGNOSTICS                       0x08 // Diagnostics
#define READ_EXCEPTION_STATUS             0x07 // Read Exception Status
#define GET_COMM_EVENT_COUNTER            0x0B // Get COMM Event Counter
#define GET_COMM_EVENT_LOG                0x0C // Get COMM Event Log
#define REPORT_SLAVE_ID                   0x11 // Report Slave ID
#define WRITE_MULTIPLE_COILS              0x0F // Write Multiple Coils
#define WRITE_MULTIPLE_REGISTERS          0x10 // Write Multiple registers
#define READ_FILE_RECORD                  0x14 // Read File Record
#define WRITE_FILE_RECORD                 0x15 // Write File Record
#define MASK_WRITE_REGISTER               0x16 // Mask Write Register
#define READ_WRITE_MULTIPLE_REGISTERS     0x17 // Read/Write Multiple registers
#define READ_FIFO_QUEUE                   0x18 // Read FIFO Queue
#define ENCAPSULATED_INTERFACE_TRANSPORT  0x2B // Encapsulated Interface Transport

// Exceptions
#define EXCEPTION_ILLEGAL_FUNCTION       0x01
#define EXCEPTION_ILLEGAL_ADDRESS        0x02
#define EXCEPTION_ILLEGAL_DATA_VALUE     0x03
#define EXCEPTION_SERVER_DEVICE_FAILURE  0x04
#define EXCEPTION_ACKNOWLEDGE            0x05
#define EXCEPTION_SERVER_DEVICE_BUSY     0x06
#define EXCEPTION_MEMORY_PARITY_ERROR    0x08




// Peripheral interface
void OnModbusFrameReceived(void);
// Application interface
extern modbus_read_t modbus_read_request;
extern modbus_write_t modbus_write_request;
extern modbus_write_read_t modbus_write_read_request;

void SendModbus(uint16_t length);
extern bool modbus_device_busy;   // \todo deprecate
extern uint8_t modbus_tx_buffer[];
extern uint16_t modbus_multiple_read_max;

#endif /* MODBUS_MODBUS_H_ */
