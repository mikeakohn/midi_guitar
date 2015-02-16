/**
 *  midi_guitar
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2015 by Michael Kohn
 *
 * AudioDevice.cxx: Read sound data from /dev/dsp
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/soundcard.h>

#include "AudioDevice.h"

AudioDevice::AudioDevice(const char *device)
{
  in = open(device, O_RDONLY);
}

AudioDevice::~AudioDevice()
{
  close(in);
}

int AudioDevice::init()
{
  int rate = 44100;
  int format = AFMT_S16_LE;
  int channels = 1;

  if (in == -1)
  {
    printf("in = -1\n");
    return -1;
  }

  if (ioctl(in, SNDCTL_DSP_SETFMT, &format) == -1)
  {
    printf("SNDCTL_DSP_SETFMT error\n");
    return -1;
  }

  if (ioctl(in, SNDCTL_DSP_SPEED, &rate) == -1)
  {
    printf("SNDCTL_DSP_SPEED error\n");
    return -1;
  }

  if (ioctl(in, SNDCTL_DSP_CHANNELS, &channels) == -1)
  {
    printf("SNDCTL_DSP_CHANNELS error\n");
    return -1;
  }

  return 0;
}

int AudioDevice::read_data(FLOAT *samples, int count)
{
  int n,len;

  len = 0; 
  while(len < 8192 * 2)
  {
    n = read(in, ((uint8_t *)buffer) + len, (8192 * 2) - len);
    if (n < 0)
    {
      printf("read() failed %d/%d\n", n, len);
      return -1;
    }
    len += n;
  }

  for (n = 0; n < 8192; n++)
  {
    samples[n] = (FLOAT)buffer[n] / 32767.0;
  }

  return 0;
}

