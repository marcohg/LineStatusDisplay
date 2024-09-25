#include "modbus.h"
#include "c_modbus.h"
#include "crc.h"
#include "global_data.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>

uint16_t c_modbus::modbus_max_addresable;
uint16_t c_modbus::std_size = MODBUS_STD_TBL_SIZE;    // LUT members access 16bit with restrictions (valid, protected, etc)
uint16_t c_modbus::ext_size = sizeof(ext_lut)/sizeof(ext_lut[0]);    // Maximum size.


c_modbus::c_modbus() {
  modbus_max_addresable =  std_size + 2* ext_size;  // 22,23
}

/// ReadWriteProtected when unlock protection
bool c_modbus::DataIsWriteProtected(uint16_t address) {
  if (address < std_size) {
    std_t::access_t a = std_lut[address].access;
    return
        a == std_t::RO  ||
        a == std_t::RWP;
  } else {
    ext_t::access_t a = ext_lut[(address - std_size)/2].access;
    return a == ext_t::RO || a == ext_t::RWP ;
  }

}

bool c_modbus::AddressNotDefined(uint16_t address) {
  int32_t *p = address < std_size ? std_lut[address].data.i32 :
      ext_lut[(address - std_size)/2].data.i32;
    return p == nullptr;
}


// checking 12/5/2022: pulse range
bool c_modbus::DataOutOfLimits(uint16_t address, int16_t data) {
  if (address < std_size)
    return false;  // Extended data has no limits

  //  const std_t *p = &std_lut[address];
//  if (p->access == std_t::ReadWriteScaledInt
//      || p->access == std_t::ReadWriteScaledUint
//      || p->access == std_t::ReadWriteScaledFloat) {
//    float value = data / (*p->scale);
//    return !(value <= *p->max && value >= *p->min);
//  } else
    return false; // fake-it
}



uint16_t c_modbus::GetDataStdLUT(uint16_t mb_addr) {
  uint16_t u16;  // 16-bit result
  float s = *std_lut[mb_addr].scale;
  if (std_lut[mb_addr].type == std_t::u32) {
    uint32_t u32 = *std_lut[mb_addr].data.u32 * s;
    u16 = swap_bytes(u32);  // test > 32767
  } else if(std_lut[mb_addr].type == std_t::i32) {
    int32_t i = *std_lut[mb_addr].data.i32 * s;
    u16 = swap_bytes(i); }
  else {
    float f = *std_lut[mb_addr].data.f32 * s;
    u16 = swap_bytes(f);
  }
  return u16;
}

// mb_address == table index, std_lut starts at 0
int c_modbus::SetDataStdLUT(uint16_t param_index, int16_t data) {  // SetStdParameterData(uint16_t param_index, int16_t data);
  data = swap_bytes(data);  // MB is Big endian
  float min = *std_lut[param_index].min;
  float max = *std_lut[param_index].max;
  float s = *std_lut[param_index].scale;

  if (std_lut[param_index].type == std_t::i32) {
    int32_t i32 = data / s;
    if (i32 >= min && i32 <= max) {
      *std_lut[param_index].data.u32 = i32;
    } else
      return EXCEPTION_ILLEGAL_DATA_VALUE;
  } else if (std_lut[param_index].type == std_t::u32) {
    uint32_t u32 = (uint16_t) data / s;
    if (u32 >= min && u32 <= max) {
      *std_lut[param_index].data.u32 = u32;
    } else
      return EXCEPTION_ILLEGAL_DATA_VALUE;
  } else {
    float f32 = min >= 0 ? (float)(uint16_t)data/ s :(float)data/ s;
    if (f32 >= min && f32 <= max) {
      *std_lut[param_index].data.f32 = f32;
    } else
      return EXCEPTION_ILLEGAL_DATA_VALUE;
  }

  return 0;

}

// Get data from Modbus buffer (Big endian) and write to ext table
//
int c_modbus::SetDataExtLUT(uint16_t mb_addrs, int16_t *v) {

    uint32_t data = swap_bytes( *v ) + (swap_bytes(*(v+1)) << 16);

    switch (ext_lut[(mb_addrs - std_size)/2].access) {
      case ext_t::f32:
        *ext_lut[ (mb_addrs - std_size)/2].data.f32 = (float)data;
        break;
      case ext_t::i32:
        *ext_lut[(mb_addrs - std_size)/2].data.i32 = (int32_t)data;
        break;
      case ext_t::u32:
        *ext_lut[(mb_addrs - std_size)/2].data.u32 = (uint32_t)data;
        break;

      default:
        printf("Error: Undefined datatype \r\n");
        return EXCEPTION_ILLEGAL_DATA_VALUE;
        break;
    }
    return 0;
}

/*
 * Build and send Modbus read request for Read multiple FNC 3
 *
 */
void c_modbus::ReplyReadRequest(void) {
  modbus_read_t &request = modbus_read_request;
  if(!request.received)
    return;

  request.received = false;  // serviced
  modbus_tx_buffer[0] = request.self_address;
  modbus_tx_buffer[1] = request.function_code;
  modbus_tx_buffer[2] = request.quantity * 2;

  for (int i = 0; i < request.quantity; ++i) {
    uint8_t *pdst = &modbus_tx_buffer[3] + i * 2;
    if ((request.address + i) < std_size)  // Single 16 bits
      *((uint16_t*) pdst) = GetDataStdLUT(request.address + i);
    else {
      uint8_t *psrc = (uint8_t*) ext_lut[((request.address - std_size) + i) / 2].data.u32;
      // Value 0x12345678 in ARM little endian  [56][78][12][34] to Modbus 16bit Big endian, bytes ABCD -> BADC
      pdst[0] = psrc[1];
      pdst[1] = psrc[0];
      pdst[2] = psrc[3];
      pdst[3] = psrc[2];
      ++i;  // Moving double words
    }
  }
  uint16_t CRC = GetCRC(modbus_tx_buffer, request.quantity * 2 + 3);
  *((uint16_t *) &modbus_tx_buffer[request.quantity * 2 + 3]) = CRC;
  SendModbus(request.quantity * 2 + 5);
}

/// Consume the modbus received data and update next address depending on STD or EXT
int16_t *c_modbus::ConsumeFromModbusDataBuffer(uint16_t &mb_address, int16_t *data) {
  mb_address +=  1;
  ++data;
  if(mb_address < std_size)
    return data;
  mb_address += 1;
  ++data;
    return data;
}
/*
 * Build and send Modbus write requests for single 0x06 and multiple 0x10
 *
 */

void c_modbus::ReplyWriteRequest(void) {
  modbus_write_t &request = modbus_write_request;
  if(!request.received)
    return;
  request.received = false;  // serviced
  modbus_tx_buffer[0] = request.self_address;
  modbus_tx_buffer[1] = request.function_code;
  *((uint16_t *) &modbus_tx_buffer[2]) = request.address;


  int16_t *data = request.data;
  uint8_t qty = 1;  // assume single
//  uint16_t index = request.address;
  if (request.function_code == WRITE_SINGLE_REGISTER)
    *((int16_t *) &modbus_tx_buffer[4]) = swap_bytes(*data);              // Single: FNC A1 A2 V1 V2
  else {
    qty = request.quantity;
    *((int16_t *) &modbus_tx_buffer[4]) = swap_bytes(request.quantity);   // Multiple: FNC A1 A2 Q1 Q2
  }

  uint16_t len = 6;
  for (uint16_t mb_address = request.address; mb_address < request.address + qty;) {
    int exception = mb_address < std_size ? SetDataStdLUT(mb_address, *data) :  // handle cases is std regions
          SetDataExtLUT(mb_address, data);

    if ( exception > 0 ) {  // SetStdParameterData(uint16_t param_index, int16_t data); < 0 error
      modbus_tx_buffer[1] |= 0x80;  // FNC exception: read-only parameter
//      EXCEPTION_ILLEGAL_DATA_VALUE
      modbus_tx_buffer[2] = exception;  // 2;      // , CNX BLDC Drive Parameters
      len = 3;
      break;
    }
//    if(exception >= 0)
//      spawn(mb_address);
    data = modbus->ConsumeFromModbusDataBuffer(mb_address, data);
  }
  *((uint16_t *) &modbus_tx_buffer[len]) = GetCRC(modbus_tx_buffer, len);
  SendModbus(len + 2);

}
