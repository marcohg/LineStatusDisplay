# Board Support Package
8/23/2024
## project wizard
c  project, sdk console UART
Redlib (nohost-nf) this is the default
redirect sdk PRINT -> printf
Redirect print/scanf UART
Note:  Newlib (none) fails to provide ->  _close, _fstat,

## Intentions:
- Just provide BSP for encoder and 485
-  Provide the Debug UART1  'as is'  for tracing
- Test encoder counter hardware
- Test Modbus UART0 
    - DE using RTS
    - Idle interrupt. Removed, as it only provides 10, 11 bits delay.
    - Add a PIT ch0 to control frame receive, fixed to 5ms for now


Is acceptable to have TX blocking.

## Wiring
Modbus UART0
PTD4	UART0_RTS
PTA1	UART0_RX
PTA2	UART0_TX

Debug UART1
PTE0    UART1_TX
PTE1    UART1_RX

Encoder
PTB18   FTM2_CH0    QDA
PTB19   FTM2_CH1    QDB

PTD5    Test Point

