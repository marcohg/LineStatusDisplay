/** @file
 * c_hal.h
 * Created on: Nov 18, 2021
 * Author: mhg
 *
*/
#ifndef C_HAL_H_
#define C_HAL_H_
#include "typedefs.h"
#define RX485_BUFFER_SIZE 256
#define TX485_BUFFER_SIZE 256


class c_hal {
 public:
  c_hal();  // Std constructor
  uint16_t *encoder_position; //  uint16_t GetEncoderPosition(void);
  void CreatePositionTable(void);
  void ResetEncoderPosition(void);

  int getchar(void);
  uint64_t GetRuntime(void);

  // Provide Test Point
  static void Set_TP1();
  static void Clear_TP1();
  static void Toggle_TP1();


  static void Send485(uint8_t *data, uint16_t length);
  uint16_t GetRxBuffer485(uint8_t *dst);
  static uint16_t rx_index;

  static void TxIsrMode485(void);
  static void RxIsrMode485(void);
  static bool Sending;

 private:
  void SetRTS485Control(void);

};

extern c_hal *hal;




#endif /* HAL_H_ */
