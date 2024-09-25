
// Exceptions
#define EXCEPTION_ILLEGAL_FUNCTION       0x01  
#define EXCEPTION_ILLEGAL_ADDRESS        0x02  
#define EXCEPTION_ILLEGAL_DATA_VALUE     0x03  
#define EXCEPTION_SERVER_DEVICE_FAILURE  0x04  
#define EXCEPTION_ACKNOWLEDGE            0x05  
#define EXCEPTION_SERVER_DEVICE_BUSY     0x06  
#define EXCEPTION_MEMORY_PARITY_ERROR    0x08  

Exception | Description | Policy
---|---|---
## 1 Unimplemented FNC 
EXCEPTION_ILLEGAL_FUNCTION 
- Only functions 
  - 3 Read Mult, 
  - 6/16 Write single/multiple 

## 2 Illegal address 
EXCEPTION_ILLEGAL_ADDRESS  
- R/W out of 0-255 range(std size), or Indefined address (MH)
- write to 
  - 1)`read only` 
  - 2) protected address

## 3 Bad Data 
EXCEPTION_ILLEGAL_DATA_VALUE
- Read more than 5 params(can be unlocked), 
- write invalid data (min/max) for specific parameter. 
- If data accepted, an internal limit may be applied

## 6 Comm not allowed 
EXCEPTION_SERVER_DEVICE_BUSY
- Processor is busy 