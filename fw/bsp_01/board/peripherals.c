/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Peripherals v15.0
processor: MK22FN512xxx12
package_id: MK22FN512VLH12
mcu_data: ksdk2_0
processor_version: 16.1.0
board: FRDM-K22F
functionalGroups:
- name: BOARD_InitPeripherals
  UUID: 98ea2628-3546-4839-ae33-e459990de35e
  called_from_default_init: true
  selectedCore: core0
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/

/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
component:
- type: 'system'
- type_id: 'system'
- global_system_definitions:
  - user_definitions: ''
  - user_includes: ''
  - global_init: ''
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/

/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
component:
- type: 'gpio_adapter_common'
- type_id: 'gpio_adapter_common'
- global_gpio_adapter_common:
  - quick_selection: 'default'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/

/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
component:
- type: 'uart_cmsis_common'
- type_id: 'uart_cmsis_common'
- global_USART_CMSIS_common:
  - quick_selection: 'default'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

/***********************************************************************************************************************
 * Included files
 **********************************************************************************************************************/
#include "peripherals.h"

/***********************************************************************************************************************
 * BOARD_InitPeripherals functional group
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * NVIC initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'NVIC'
- type: 'nvic'
- mode: 'general'
- custom_name_enabled: 'false'
- type_id: 'nvic'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'NVIC'
- config_sets:
  - nvic:
    - interrupt_table:
      - 0: []
      - 1: []
    - interrupts: []
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

/* Empty initialization function (commented out)
static void NVIC_init(void) {
} */

/***********************************************************************************************************************
 * UART0 initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'UART0'
- type: 'uart'
- mode: 'interrupts'
- custom_name_enabled: 'false'
- type_id: 'uart_2.5.1'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'UART0'
- config_sets:
  - uartConfig_t:
    - uartConfig:
      - clockSource: 'BusInterfaceClock'
      - clockSourceFreq: 'GetFreq'
      - baudRate_Bps: '115200'
      - parityMode: 'kUART_ParityDisabled'
      - dataBitsCount: 'kUART_EightDataBits'
      - enableMatchAddress1: 'false'
      - matchAddress1: '0'
      - enableMatchAddress2: 'false'
      - matchAddress2: '0'
      - txFifoWatermark: '0'
      - rxFifoWatermark: '1'
      - idleType: 'kUART_IdleTypeStopBit'
      - enableTx: 'true'
      - enableRx: 'true'
  - interruptsCfg:
    - interrupts: 'kUART_RxDataRegFullInterruptEnable kUART_RxOverrunInterruptEnable'
    - interrupt_vectors:
      - enable_rx_tx_irq: 'true'
      - interrupt_rx_tx:
        - IRQn: 'UART0_RX_TX_IRQn'
        - enable_interrrupt: 'enabled'
        - enable_priority: 'false'
        - priority: '0'
        - enable_custom_name: 'false'
      - enable_err_irq: 'false'
      - interrupt_err:
        - IRQn: 'UART0_ERR_IRQn'
        - enable_interrrupt: 'enabled'
        - enable_priority: 'false'
        - priority: '0'
        - enable_custom_name: 'false'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */
const uart_config_t UART0_config = {
  .baudRate_Bps = 115200UL,
  .parityMode = kUART_ParityDisabled,
  .txFifoWatermark = 0U,
  .rxFifoWatermark = 1U,
  .idleType = kUART_IdleTypeStopBit,
  .enableTx = true,
  .enableRx = true
};

static void UART0_init(void) {
  UART_Init(UART0_PERIPHERAL, &UART0_config, UART0_CLOCK_SOURCE);
  UART_EnableInterrupts(UART0_PERIPHERAL, kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable);
  /* Enable interrupt UART0_SERIAL_RX_TX_IRQN request in the NVIC */
  EnableIRQ(UART0_SERIAL_RX_TX_IRQN);
}

/***********************************************************************************************************************
 * PIT initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'PIT'
- type: 'pit'
- mode: 'LPTMR_GENERAL'
- custom_name_enabled: 'false'
- type_id: 'pit_2.0.3'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'PIT'
- config_sets:
  - fsl_pit:
    - enableRunInDebug: 'false'
    - timingConfig:
      - clockSource: 'BusInterfaceClock'
      - clockSourceFreq: 'GetFreq'
    - channels:
      - 0:
        - channel_id: 'CHANNEL_0'
        - channelNumber: '0'
        - enableChain: 'false'
        - timerPeriod: '5ms'
        - startTimer: 'false'
        - enableInterrupt: 'true'
        - interrupt:
          - IRQn: 'PIT0_IRQn'
          - enable_interrrupt: 'enabled'
          - enable_priority: 'false'
          - priority: '0'
          - enable_custom_name: 'false'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */
const pit_config_t PIT_config = {
  .enableRunInDebug = false
};

static void PIT_init(void) {
  /* Initialize the PIT. */
  PIT_Init(PIT_PERIPHERAL, &PIT_config);
  /* Set channel 0 period to N/A. */
  PIT_SetTimerPeriod(PIT_PERIPHERAL, PIT_CHANNEL_0, PIT_CHANNEL_0_TICKS);
  /* Enable interrupts from channel 0. */
  PIT_EnableInterrupts(PIT_PERIPHERAL, PIT_CHANNEL_0, kPIT_TimerInterruptEnable);
  /* Enable interrupt PIT_CHANNEL_0_IRQN request in the NVIC */
  EnableIRQ(PIT_CHANNEL_0_IRQN);
}

/***********************************************************************************************************************
 * FTM2 initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'FTM2'
- type: 'ftm'
- mode: 'QuadratureDecoder'
- custom_name_enabled: 'false'
- type_id: 'ftm_2.6.0'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'FTM2'
- config_sets:
  - ftm_main_config:
    - ftm_config:
      - clockSource: 'kFTM_SystemClock'
      - clockSourceFreq: 'GetFreq'
      - timerPrescaler: '1'
      - systemClockSource: 'BusInterfaceClock'
      - systemClockSourceFreq: 'mirrored_value'
      - faultMode: 'kFTM_Fault_Disable'
      - inputFilterPeriod: '1'
      - faultInputs:
        - 0:
          - enableFaultInput: 'false'
          - faultLevelVal: 'low'
          - useFaultFilter: 'false'
        - 1:
          - enableFaultInput: 'false'
          - faultLevelVal: 'low'
          - useFaultFilter: 'false'
        - 2:
          - enableFaultInput: 'false'
          - faultLevelVal: 'low'
          - useFaultFilter: 'false'
        - 3:
          - enableFaultInput: 'false'
          - faultLevelVal: 'low'
          - useFaultFilter: 'false'
      - deadTimePrescale: 'kFTM_Deadtime_Prescale_1'
      - deadTimePeriod: '0'
      - pwmSyncMode: 'kFTM_SoftwareTrigger'
      - swTriggerResetCount: 'true'
      - hwTriggerResetCount: 'false'
      - reloadPoints: ''
      - extTriggers: ''
      - chnlInitState: ''
      - chnlPolarity: ''
      - bdmMode: 'kFTM_BdmMode_0'
      - useGlobalTimeBase: 'false'
    - timer_interrupts: ''
    - enable_irq: 'false'
    - ftm_interrupt:
      - IRQn: 'FTM2_IRQn'
      - enable_interrrupt: 'enabled'
      - enable_priority: 'false'
      - priority: '0'
      - enable_custom_name: 'false'
    - EnableTimerInInit: 'true'
    - quick_selection: 'QuickSelectionDefault'
  - ftm_quadrature_decoder_mode:
    - timerModuloVal: '1000'
    - timerInitVal: '0'
    - ftm_quad_decoder_mode: 'kFTM_QuadPhaseEncode'
    - ftm_phase_a_params:
      - enablePhaseFilter: 'false'
      - phaseFilterPeriod: '1'
      - phasePolarity: 'kFTM_QuadPhaseNormal'
    - ftm_phase_b_params:
      - enablePhaseFilter: 'false'
      - phaseFilterPeriod: '1'
      - phasePolarity: 'kFTM_QuadPhaseNormal'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */
const ftm_config_t FTM2_config = {
  .prescale = kFTM_Prescale_Divide_1,
  .faultMode = kFTM_Fault_Disable,
  .faultFilterValue = 0,
  .deadTimePrescale = kFTM_Deadtime_Prescale_1,
  .deadTimeValue = 0,
  .pwmSyncMode = kFTM_SoftwareTrigger,
  .swTriggerResetCount = true,
  .hwTriggerResetCount = false,
  .reloadPoints = 0,
  .extTriggers = 0,
  .chnlInitState = 0,
  .chnlPolarity = 0,
  .bdmMode = kFTM_BdmMode_0,
  .useGlobalTimeBase = false
};
const ftm_phase_params_t FTM2_phaseAParams = { 
  .enablePhaseFilter = false,
  .phasePolarity = kFTM_QuadPhaseNormal

};
const ftm_phase_params_t FTM2_phaseBParams = { 
  .enablePhaseFilter = false,
  .phasePolarity = kFTM_QuadPhaseNormal

};

static void FTM2_init(void) {
  FTM_Init(FTM2_PERIPHERAL, &FTM2_config);
/* Initialization of the timer initial value and modulo value */
  FTM_SetQuadDecoderModuloValue(FTM2_PERIPHERAL, 0,1000);
  FTM_SetupQuadDecode(FTM2_PERIPHERAL, &FTM2_phaseAParams, &FTM2_phaseBParams, kFTM_QuadPhaseEncode);
  FTM_StartTimer(FTM2_PERIPHERAL, kFTM_SystemClock);
}

/***********************************************************************************************************************
 * Initialization functions
 **********************************************************************************************************************/
void BOARD_InitPeripherals(void)
{
  /* Initialize components */
  UART0_init();
  PIT_init();
  FTM2_init();
}

/***********************************************************************************************************************
 * BOARD_InitBootPeripherals function
 **********************************************************************************************************************/
void BOARD_InitBootPeripherals(void)
{
  BOARD_InitPeripherals();
}
