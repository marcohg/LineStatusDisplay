/*
 * c_hal.cpp
 * Hardware abstraction
 *
 * Put all mcuXpresso ConfigTools 'c' functions handlers
 *
 *
 */

#include "c_hal.h"
// K22
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "modbus.h" // OnModbusFrameReceived

// Peripherals - tools
#define FTM_QUAD_MODULE 0xFFFF

uint8_t rx_buffer[RX485_BUFFER_SIZE];
uint8_t tx_buffer[TX485_BUFFER_SIZE];
uint16_t c_hal::rx_index = 0;
bool c_hal::Sending;

//volatile uint32_t *c_hal::qcounter16;
//extern int32_t total_counts;
//extern uint32_t delta_counts;
#include "global_data.h"
uint32_t  *encoder   = (uint32_t *)&FTM2->CNT;

//static void UART_Enable_RTS485(UART_Type *base);
uint16_t tx_nbr;
uint16_t tx_index;
uint16_t tx_complete;
uint16_t tx_buffer_empty;

extern uint32_t run_time      ;
#ifdef __cplusplus  // Config tools 'c' callbacks
extern "C"
{
#endif

/* UART0_RX_TX_IRQn interrupt handler */
void UART0_SERIAL_RX_TX_IRQHANDLER(void) {
  uint32_t intStatus;
  /* Reading all interrupt flags of status registers */
  intStatus = UART_GetStatusFlags(UART0_PERIPHERAL);
  /* Place your code here */
  uint8_t data;
  if (c_hal::Sending) {
    if ((kUART_TransmissionCompleteFlag) & intStatus) {  // | kUART_TxDataRegEmptyFlag
      if (tx_index == tx_nbr) {
        tx_complete = true;
        c_hal::rx_index = 0;  //
//        c_hal::Clear_TP1();  // GPIO_PortClear(GPIOD, 1U << 5);  // PTD5
        c_hal::RxIsrMode485();

      } else
        UART_WriteByte(UART0_PERIPHERAL, tx_buffer[tx_index++]);
    }

  } else /* If new data arrived. */
  if ((kUART_RxDataRegFullFlag | kUART_RxOverrunFlag) & intStatus) {
    data = UART_ReadByte(UART0_PERIPHERAL);
    rx_buffer[c_hal::rx_index++] = data;
    if (c_hal::rx_index == RX485_BUFFER_SIZE)
      c_hal::rx_index = 0;

    PIT_StopTimer(PIT_PERIPHERAL, PIT_CHANNEL_0);
    PIT_StartTimer(PIT_PERIPHERAL, PIT_CHANNEL_0);  // measure a gap between frames
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
  c_hal::Set_TP1();

  PIT_StopTimer(PIT_PERIPHERAL, PIT_CHANNEL_0); // One shot
  OnModbusFrameReceived();
  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}
static bool IsAtCenter(uint32_t v) {
  const static uint32_t low_limit = FTM_QUAD_MODULE/2 - FTM_QUAD_MODULE/4;
  const static uint32_t high_limit = FTM_QUAD_MODULE/2 + FTM_QUAD_MODULE/4;
  return v >= low_limit && v <= high_limit;
}
/* PIT1_IRQn interrupt handler */
void PIT_CHANNEL_1_IRQHANDLER(void) {
  uint32_t intStatus;
  /* Reading all interrupt flags of status register */
  intStatus = PIT_GetStatusFlags(PIT_PERIPHERAL, PIT_CHANNEL_1);
  PIT_ClearStatusFlags(PIT_PERIPHERAL, PIT_CHANNEL_1, intStatus);

  /* Place your code here */
  static uint16_t runtime_rate;
  if(++runtime_rate >= 100) { // 0.1 sec ticks
    runtime_rate = 0;
    ++run_time;
  }
  c_hal::Toggle_TP1();


  // Get Counts - Delta counts should be lower than FTM_QUAD_MODULE counts 
  volatile uint16_t counts = *encoder;  // Assume +direction for now
  static uint16_t counts_previous = counts;
  static uint16_t time_base = 0;
  // Assume +count direction, 1000ms time base
  if (++time_base >= time_base_ms) {
    time_base = 0;
    // TBD counts == FTM_QUAD_MODULE
    bool count_at_center =  IsAtCenter(counts) && IsAtCenter(counts_previous);
    bool count_pos_rollover = counts_previous > FTM_QUAD_MODULE / 2 && counts < FTM_QUAD_MODULE / 2;
    bool count_neg_rollover = counts_previous < FTM_QUAD_MODULE / 2 && counts > FTM_QUAD_MODULE / 2;

    if(count_pos_rollover && !count_at_center)
      delta_counts = (int)counts + (int)(FTM_QUAD_MODULE - counts_previous) + 1;
    else if(count_neg_rollover && !count_at_center) {
      delta_counts = (int)counts -(int)FTM_QUAD_MODULE - counts_previous - 1;
    } else {
      delta_counts = (int)counts - (int)counts_previous;
    }
    total_counts += delta_counts;
    counts_previous = counts;
  }
  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
   Store immediate overlapping exception return operation might vector to incorrect interrupt. */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
  __DSB();
#endif
}

#ifdef __cplusplus
}
#endif


void c_hal::Set_TP1()    { GPIO_PortSet(GPIOC, 1U << 7);    } // PTD5
void c_hal::Clear_TP1()  { GPIO_PortClear(GPIOC, 1U << 7);  }
void c_hal::Toggle_TP1() { GPIO_PortToggle(GPIOC, 1U << 7); }

/**
 * \brief hal constructor sets up the hardware for the application
 */
c_hal::c_hal() {
  /* Init board hardware. */
  BOARD_InitBootPins();
  BOARD_InitBootClocks();
  BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
  /* Init FSL debug console. */
  BOARD_InitDebugConsole();
#endif
  SetRTS485Control();
  RxIsrMode485();
}

/*
 * @brief Enable485Driver
 * Use RTS pin to control 485 transceiver see 47.3.14 UART Modem Register (UARTx_MODEM)
 */
void c_hal::SetRTS485Control(void) {

  UART0_PERIPHERAL->MODEM |= UART_MODEM_TXRTSE_MASK;    // Assert RTS one-bit before the start, de-assert one bit after stop
  UART0_PERIPHERAL->MODEM |= UART_MODEM_TXRTSPOL_MASK;  // RTS Active high
}

void c_hal::TxIsrMode485(void) {
  UART_DisableInterrupts(UART0_PERIPHERAL, kUART_AllInterruptsEnable);
  Sending = true;
  UART_EnableInterrupts(
      UART0_PERIPHERAL,
//      kUART_TxDataRegEmptyInterruptEnable  |
      kUART_TransmissionCompleteInterruptEnable);

}
void c_hal::RxIsrMode485(void) {
  /// \todo Flush RX buffer
  UART_DisableInterrupts(UART0_PERIPHERAL, kUART_AllInterruptsEnable);
  Sending = false;
  UART_EnableInterrupts( UART0_PERIPHERAL,
      kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable);
}


void c_hal::Send485(uint8_t *data, uint16_t length) {

    assert(length);
    memcpy(tx_buffer, data, length);
    //    TransmitEnable(true); // or flush
//    Set_TP1();// GPIO_PortSet(GPIOD, 1U << 5);  // PTD5
    while (!(kUART_TxDataRegEmptyFlag & UART_GetStatusFlags(UART0_PERIPHERAL)));  // caution
    tx_nbr = length;
    tx_index = 1;
    tx_complete = false;
    tx_buffer_empty = false;
    UART_WriteByte(UART0_PERIPHERAL, data[0]);  // flags are erased
    TxIsrMode485();  // need to re-write - when complete
}

uint16_t c_hal::GetRxBuffer485(uint8_t *dst) {
  uint16_t count = rx_index;
  memcpy(dst, rx_buffer, count);
  rx_index = 0;
  return count;
}


