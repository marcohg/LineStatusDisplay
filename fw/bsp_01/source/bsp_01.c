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
#define DEMO_RING_BUFFER_SIZE 256
uint8_t rx_buffer[DEMO_RING_BUFFER_SIZE];
volatile uint16_t rx_index;
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
          rx_buffer[rx_index++] = data;
          if(rx_index == DEMO_RING_BUFFER_SIZE)
        	  rx_index = 0;	// Overrun
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
  frame_received = true;	// Frame Detected Callback here.
  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}

void FrameReply(void) {
	//		// In 485 we should disable receive, and TX when idle

	volatile uint16_t count = rx_index;
	UART_EnableInterrupts(UART0_PERIPHERAL, 0);	// disable (RX, OVR) interrupts
	for(uint16_t i =0; i< count; ++i) {
		while(!(kUART_TxDataRegEmptyFlag & UART_GetStatusFlags(UART0_PERIPHERAL)));	// caution
		UART_WriteByte(UART0_PERIPHERAL, rx_buffer[i]);
	}
	rx_index = 0;
	GPIO_PortClear(GPIOD, 1U << 5);	// PTD5
	/// \todo Flush RX buffer
	UART_EnableInterrupts(UART0_PERIPHERAL, kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable);
}

/*
 * @brief 47.3.14 UART Modem Register (UARTx_MODEM)
 */
static void UART_Enable_RTS485(UART_Type *base) {
	base->MODEM |= UART_MODEM_TXRTSE_MASK;		// Assert RTS one-bit before the start, de-assert one bit after stop
	base->MODEM |= UART_MODEM_TXRTSPOL_MASK;	// RTS Active high
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
	UART_Enable_RTS485(UART0_PERIPHERAL);
	/* Force the counter to be placed into memory. */
	volatile static int i = 0;
	uint32_t	count =0;
	/* Enter an infinite loop, just incrementing a counter. */
	while (1) {
		i++;
		if(frame_received) {//u != rx_idle_line ) {
			FrameReply();
			frame_received = false;
		}
		count = FTM_GetQuadDecoderCounterValue(FTM2_PERIPHERAL);
		if(i % 5000 )
			PRINTF("Count %ld\r\n", count);
	}
	return 0;
}
