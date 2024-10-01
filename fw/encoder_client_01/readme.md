# Encoder Client
C++ client to read Encoder through modbus RTU

## Modbus
Using [libmodbus](https://libmodbus.org/) in Linux.
See the `../../doc/recipes/build_libmodbus.md` notes on building. 

## Compiler
Provide the following flags for compilation:
- Include `-I/usr/local/include/modbus`
- Library search path `-L/usr/local/lib`
- Libraries `-lmodbus`



Added c_modbus class
Arrange encoder registers
