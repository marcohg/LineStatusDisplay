# Build libmodbus 
This library usaes autotools.  
- configure.ac defines the required configuration for the project
- Makefile.am the compilation steps 
![alt text](image.png)

libmodbus  
install automake, autoconf, libtool and a C compiler 

FIRST: the provided scrips are not valid, run ./autogen.sh (don't source)  
This generates configure.ac and Makefile.am.

make install  
Libraries have been installed in:
   /usr/local/lib

Build Log:

If you ever happen to want to link against installed libraries
in a given directory, LIBDIR, you must either use libtool, and
specify the full pathname of the library, or use the '-LLIBDIR'
flag during linking and do at least one of the following:
   - add LIBDIR to the 'LD_LIBRARY_PATH' environment variable
     during execution
   - add LIBDIR to the 'LD_RUN_PATH' environment variable
     during linking
   - use the '-Wl,-rpath -Wl,LIBDIR' linker flag
   - have your system administrator add LIBDIR to '/etc/ld.so.conf'
