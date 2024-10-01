/*!
 * main.cpp
 */
#include "c_hal.h"
#include "global_data.h"
#include "c_modbus.h"
#include <stdio.h>
c_hal *hal;
c_modbus *modbus;

// Sample Data
uint32_t status				 = 0;
uint32_t serial_command= 0;
int32_t  total_counts  ;
int32_t  delta_counts  ;
uint32_t run_time      ;
uint32_t time_base_ms  = 1000; // 1 sec
//int32_t  encoder     ;
float    speed_monitor = 1.2345678f;

void which_dialect(void) {
  if (         __cplusplus == 202101L) printf("C++23\r");
      else if (__cplusplus == 202002L) printf("C++20\r");
      else if (__cplusplus == 201703L) printf("C++17\r");
      else if (__cplusplus == 201402L) printf("C++14\r");
      else if (__cplusplus == 201103L) printf("C++11\r");
      else if (__cplusplus == 199711L) printf("C++98\r");
      else printf("pre-standard C++ %d" , __cplusplus );
      printf("\n");
}

int main (void) {
//  // Test alignment
//  volatile uint16_t u16 = 0x1234;
//  volatile uint32_t u32 = 0x12345678;
//
//  uint8_t *pb = (uint8_t *)&u16;
//  pb[0] = 0;
//  pb = (uint8_t *)&u32;
//  pb[0] = 0;


	hal = new c_hal();
	modbus = new c_modbus();//   std_lut,  MODBUS_STD_TBL_SIZE,	                        ext_lut, ext_lut_size);

//	std_t *t = &std_lut[0];
//	for(int i =0; i < MODBUS_STD_TBL_SIZE; ++i) {
//	  if(t->access == std_t::ReadOnlyHigh)
//	    printf("is protected\r\n");
//	  ++t;
//	}

	volatile int i =0;
  which_dialect();

  printf("Hola Debug Port\r\n");
  int32_t limit = 20;
  uint32_t msec = run_time;
  bool count_up = true;
	while(1) {
	  if(msec != run_time && !(run_time % 1000) ) {
	    msec = run_time;

    }
	  modbus->ReplyWriteRequest();  // Modbus writes, target reads data
	  // execute program logic
	  modbus->ReplyReadRequest();   // Modbus reads al internals


	}


}
