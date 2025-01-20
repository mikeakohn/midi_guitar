#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "Serial.h"

Serial::Serial() : fd { -1 }
{
}

Serial::~Serial()
{
  destroy();
}

int Serial::init(const char *device)
{
  struct termios newtio;

  fd = open(device, O_RDWR | O_NOCTTY);

  if (fd < 0)
  {
    return -1;
  }

  tcgetattr(fd, &oldtio);

  memset(&newtio, 0, sizeof(struct termios));

  newtio.c_cflag     = B9600 | CS8 | CLOCAL | CREAD;
  newtio.c_iflag     = IGNPAR;
  newtio.c_oflag     = 0;
  newtio.c_lflag     = 0;
  newtio.c_cc[VTIME] = 0;
  newtio.c_cc[VMIN]  = 1;

  tcflush(fd, TCIFLUSH);
  tcsetattr(fd, TCSANOW, &newtio);

  return 0;
}

void Serial::destroy()
{
  if (fd == -1 ) { return; }

  tcsetattr(fd, TCSANOW, &oldtio);

  close(fd);
  fd = -1;
}

uint8_t Serial::read_byte()
{
  fd_set readset;
  struct timeval tv;
  uint8_t ch;
  int n;

  while (true)
  {
    FD_ZERO(&readset);
    FD_SET(fd, &readset);

    tv.tv_sec = 2;
    tv.tv_usec = 0;
    n = select(fd + 1, &readset, NULL, NULL, &tv);

    if (n == -1) { return -1; }
    if (n == 0) { continue; }

    n = read(fd, &ch, 1);

    if (n == 1)
    {
      return ch;
    }
  }
}

struct termios Serial::oldtio;

