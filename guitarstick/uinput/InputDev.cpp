#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <linux/uinput.h>

#include "InputDev.h"

InputDev::InputDev() :
  fd     { -1 },
  curr_x {  0 },
  curr_y {  0 },
  curr_z {  0 }
{
}

InputDev::~InputDev()
{
  destroy();
}

int InputDev::init()
{
  fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
  
  if (fd < 0)
  {
    return -1;
  }

  ioctl(fd, UI_SET_EVBIT, EV_KEY);
  
  ioctl(fd, UI_SET_KEYBIT, BTN_A);
  ioctl(fd, UI_SET_KEYBIT, BTN_B);
  ioctl(fd, UI_SET_KEYBIT, BTN_X);
  ioctl(fd, UI_SET_KEYBIT, BTN_Y);

  ioctl(fd, UI_SET_EVBIT, EV_ABS);
  
  if (setup_abs(fd, ABS_X,  -32767, 32767) != 0) { return -1; }
  if (setup_abs(fd, ABS_Y,  -32767, 32767) != 0) { return -1; }
  if (setup_abs(fd, ABS_Z,  -32767, 32767) != 0) { return -1; }

  struct uinput_setup setup = { 0 };

  snprintf(setup.name, sizeof(setup.name), "%s", "Joytar");
  setup.id.bustype = BUS_USB;
  setup.id.vendor  = 0x3;
  setup.id.product = 0x3;
  setup.id.version = 2;

  if (ioctl(fd, UI_DEV_SETUP, &setup))
  {
    perror("UI_DEV_SETUP");
    return -1;
  }
  
  if (ioctl(fd, UI_DEV_CREATE))
  {
    perror("UI_DEV_CREATE");
    return -1;
  }

  return 0;
}

void InputDev::destroy()
{
  if (fd == -1 ) { return; }

  if (ioctl(fd, UI_DEV_DESTROY))
  {
    printf("UI_DEV_DESTROY");
  }

  close(fd);
  fd = -1;
}

int InputDev::update(int x, int y, int z)
{
  struct input_event ev[4];
  memset(ev, 0, sizeof(ev));

  if (x < -100) { x = -32767; }
  if (x >  100) { x =  32767; }
  if (y < -100) { y = -32767; }
  if (y >  100) { y =  32767; }

  int n = 0;

  if (x != curr_x)
  {
    ev[n].type = EV_ABS;
    ev[n].code = ABS_X;
    ev[n].value = x;

    curr_x = x;

    n++;
  }

  if (y != curr_y)
  {
    ev[n].type = EV_ABS;
    ev[n].code = ABS_Y;
    ev[n].value = y;

    curr_y = y;

    n++;
  }

  if (z != curr_z)
  {
    ev[n].type = EV_ABS;
    ev[n].code = ABS_Z;
    ev[n].value = z;

    curr_z = z;

    n++;
  }

  if (n == 0) { return 0; }

  ev[n].type = EV_SYN;
  ev[n].code = SYN_REPORT;
  ev[n].value = 0;
  n++;

  printf("sending %d events (%d joystick, 1 sync)\n", n, n - 1);

  if (write(fd, ev, sizeof(input_event) * n) < 0)
  {
    perror("write");
    return -1;
  }

  return 0;
}

int InputDev::setup_abs(int fd, uint32_t channel, int min, int max)
{
  if (ioctl(fd, UI_SET_ABSBIT, channel))
  {
    perror("UI_SET_ABSBIT");
    return -1;
  }
  
  struct uinput_abs_setup abs = { 0 };

  abs.code = channel;
  abs.absinfo.minimum = min;
  abs.absinfo.maximum = max;

  if (ioctl(fd, UI_ABS_SETUP, &abs))
  {
    perror("UI_ABS_SETUP");
    return -1;
  }

  return 0;
}

