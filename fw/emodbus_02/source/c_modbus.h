
/**!
 *  External communication is Modbus using RTU class UART
 */
#ifndef MODBUS_REFACTOR_H_
#define MODBUS_REFACTOR_H_

#include "typedefs.h"
class c_modbus {
 public:
  c_modbus();

  // This is the only public function I need to reply
//  bool NotDefined(uint16_t mb_addrs);
  bool DataOutOfLimits(uint16_t mb_addrs, int16_t data);
  bool DataIsWriteProtected( uint16_t mb_addrs);
  bool AddressNotDefined(uint16_t address);

  void ReplyReadRequest(void);
  void ReplyWriteRequest(void);
  static uint16_t modbus_max_addresable;

  inline uint16_t swap_bytes(uint16_t v) {
    return v >> 8 | v << 8;
  }

//  inline uint32_t swap_bytes(uint32_t &v) {
//    uint
//    return *v >> 8 | *v << 8;
//  }


 private:
  static uint16_t std_size;    // LUT members access 16bit with restrictions (valid, protected, etc)
  static uint16_t ext_size;    // Maximum size.
//  void GetDataFromLUT(uint8_t *dest, uint16_t mb_addr, uint16_t qty);
  uint16_t GetDataStdLUT(uint16_t mb_addr);
  int SetDataStdLUT(uint16_t param_index, int16_t data);
  int SetDataExtLUT(uint16_t mb_addrs, int16_t *v);
  int16_t *ConsumeFromModbusDataBuffer(uint16_t &mb_address, int16_t *data);

};

extern c_modbus *modbus;


#endif
