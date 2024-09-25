# Quadrature Encoder Through Modbus RTU
Using K22 to read the Encoder counts ans speed using Modbus RTU

## Initial Commit 
- Create c++ project using wizard: NewLib(nohost), redirect SDK PRINTF->C printf, printf/scanf->UART
- Use Config tools to add drivers and generate the *.mex file
	- PIT - End of Frame detection - 5ms 
	- UART0 - Interrupt
	- Use UART0 RTS for 485 control 
- git add:
	.gitignore .cproject .project readme.md  
	board/ CMSIS/ component/ device/ drivers/ middleware/ source/ startup/ utilities/   
	.settings/org.eclipse.core.resources.prefs  

## Using target PCB board
  - Use wizard to create a K22FN256VHL120 C++ using 2.16 SDK, note: MCG clock is FEI (internal)
  - Use emodbus_01 configuration tools file and source files
  - Clock got stuck. Set clock by comparison using CLK tool. Resonator has 8pF load
  - Use clock configuration by hand (mex updated)
  	PEE, Core 80MHz, System 80MHz, Bus 40MHz, Flash 20MHZ, Disable RTC (32KHz)
 
 	
  
