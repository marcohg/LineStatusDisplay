/*
 * Copyright 2016-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    bsp_01.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "fsl_gpio.h"	//setGpioOut"//

/* TODO: insert other definitions and declarations here. */
#define DEMO_RING_BUFFER_SIZE 16
uint8_t rx_buffer[DEMO_RING_BUFFER_SIZE];
volatile uint16_t rx_head;
volatile uint16_t rx_tail;
volatile uint16_t rx_count;
volatile uint16_t rx_idle_line;
bool frame_received = true;

/* UART0_RX_TX_IRQn interrupt handler */
void UART0_SERIAL_RX_TX_IRQHANDLER(void) {
  uint32_t intStatus;
  /* Reading all interrupt flags of status registers */
  intStatus = UART_GetStatusFlags(UART0_PERIPHERAL);

  /* Flags can be cleared by reading the status register and reading/writing data registers.
    See the reference manual for details of each flag.
    The UART_ClearStatusFlags() function can be also used for clearing of flags in case the content of data regsiter is not used.
    For example:
        status_t status;
        intStatus &= ~(kUART_RxOverrunFlag | kUART_NoiseErrorFlag | kUART_FramingErrorFlag | kUART_ParityErrorFlag);
        status = UART_ClearStatusFlags(UART0_PERIPHERAL, intStatus);
  */

  /* Place your code here */
  uint8_t data;
  /* If new data arrived. */
  if ((kUART_RxDataRegFullFlag | kUART_RxOverrunFlag) & intStatus)
  {
      data = UART_ReadByte(UART0_PERIPHERAL);
          rx_buffer[rx_head++] = data;
          if(rx_head == DEMO_RING_BUFFER_SIZE)
        	  rx_head = 0;
          ++rx_count;
          PIT_StopTimer(PIT_PERIPHERAL, PIT_CHANNEL_0);
          PIT_StartTimer(PIT_PERIPHERAL, PIT_CHANNEL_0);	// measure a gap between frames
  }
  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}

/* PIT0_IRQn interrupt handler */
void PIT_CHANNEL_0_IRQHANDLER(void) {
  uint32_t intStatus;
  /* Reading all interrupt flags of status register */
  intStatus = PIT_GetStatusFlags(PIT_PERIPHERAL, PIT_CHANNEL_0);
  PIT_ClearStatusFlags(PIT_PERIPHERAL, PIT_CHANNEL_0, intStatus);

  /* Place your code here */
  GPIO_PortSet(GPIOD, 1U << 5);	// PTD5
  PIT_StopTimer(PIT_PERIPHERAL, PIT_CHANNEL_0);	// One shot
  frame_received = true;	// Modbus Callback here
  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}

/*
 * @brief   Application entry point.
 */
int main(void) {

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
//    PIT_StopTimer(PIT_PERIPHERAL, PIT_CHANNEL_0);
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

	PRINTF("UART0 sends what received\r\n");

	/* Force the counter to be placed into memory. */
	volatile static int i = 0;
	/* Enter an infinite loop, just incrementing a counter. */
	volatile uint16_t u;
	while (1) {
		i++;
		if(frame_received) {//u != rx_idle_line ) {
			u = rx_idle_line;
			volatile int a =0;
			for(a=0; a < 200; ++a);	// a delay
			frame_received = false;
			GPIO_PortClear(GPIOD, 1U << 5);	// PTD5
//			PIT_StopTimer(PIT_PERIPHERAL, PIT_CHANNEL_0);
		}
//		// In 485 we should disable receive, and TX when idle
//		while (kUART_TxDataRegEmptyFlag & UART_GetStatusFlags(UART0_PERIPHERAL) && rx_count) {
//			UART_WriteByte(UART0_PERIPHERAL, rx_buffer[rx_tail++]);
//			if (rx_tail == DEMO_RING_BUFFER_SIZE)
//				rx_tail = 0;
//			if (rx_count)	// TX_RX_IRQ --- improve
//				--rx_count;
//		}
	}
	return 0;
}
