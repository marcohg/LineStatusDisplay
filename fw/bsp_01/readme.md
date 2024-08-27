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

## Wiring - bsp_01
Modbus UART0
J2-6    PTD4	UART0_RTS
J2-4    PTA1	UART0_RX
J1-8    PTA2	UART0_TX

Debug UART1
PTE0    UART1_TX
PTE1    UART1_RX

Encoder
J1-12   PTB18   FTM2_CH0    QDA
J2-2    PTB19   FTM2_CH1    QDB

PTD5    Test Point

## Modbus frames
use modpoll
modpoll -a 2 -1 -0 -r 1 -c 2 -p none -b 115200 /dev/ttyUSB0

A   F   Start   Qty     CRC      
0x2 0x3 0x0 0x1 0x0 0x2 0x95 0xf8	
