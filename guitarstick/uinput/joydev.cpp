#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "InputDev.h"
#include "Serial.h"

int main(int argc, char *argv[])
{
  uint8_t data[4];
  InputDev input_dev;
  Serial serial;
  int ptr = 0;

  if (argc != 2)
  {
    printf("Usage: %s </dev/ttyUSBx>\n", argv[0]);
    exit(0);
  }

  if (serial.init(argv[1]) != 0)
  {
    printf("Error: Cannot open serial port.\n");
    exit(1);
  }

  if (input_dev.init() != 0)
  {
    printf("Error: Cannot open joystick dev.\n");
    exit(1);
  }

  printf("Listening for data...\n");

  while (true)
  {
    uint8_t c = serial.read_byte();

    if (c == 0xff)
    {
      printf(" --- start ---\n");
      ptr = 0;
    }

    printf("%d\n", c);

    if (ptr < 4)
    {
      data[ptr++] = c;

      if (ptr == 4)
      {
        int x, y, z;

        x = (data[1] - 63) * 511;
        y = (data[2] - 63) * 511;
        z = (data[3] - 63) * 511;

        printf("%d, %d, %d\n", x, y, z);

        input_dev.update(x, y, z);
      }
    }
  }

  return 0;
}

