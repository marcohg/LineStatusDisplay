#include <stdio.h>
#include <modbus.h>
#include <errno.h>
// # apt-get install libmodbus-dev libmodbus5
// $ gcc -O2 -Wall -I /usr/include/modbus example.c -o test -L/usr/local/lib/modbus -lmodbus

#define SLAVE_ADDRESS 2
int main(int argc, char *argv[])
{
  printf("Test libmodbus 2\n");
  char serial_port[50] = "/dev/ttyUSB0";  // default
  if (argc > 1)
  {
    // printf("arg: %s\n",argv[1]);
    printf("Will use %s instead %s (default)\n", argv[1],serial_port);
    sprintf(serial_port,"%s",argv[1]);
    // return -1;
  }
  modbus_t *ctx;
  uint16_t tab_reg[64];
  int rc;
  int i;

  ctx = modbus_new_rtu(serial_port, 115200, 'N', 8, 1);
  if (ctx == NULL)
  {
    fprintf(stderr, "Unable to create the libmodbus context\n");
    return -2;
  }
  modbus_set_slave(ctx, SLAVE_ADDRESS);

  if (modbus_connect(ctx) == -1)
  {
    fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
    modbus_free(ctx);
    return -3;
  }

  rc = modbus_read_registers(ctx, 0, 5, tab_reg);

  if (rc == -1)
  {
    printf("failed man!\n");
    fprintf(stderr, "%s\n", modbus_strerror(errno));
    return -4;
  }

  for (i = 0; i < rc; i++)
  {
    printf("reg[%d]=%d (0x%X)\n", i, tab_reg[i], tab_reg[i]);
  }

  modbus_close(ctx);
  modbus_free(ctx);
  return 0;
}