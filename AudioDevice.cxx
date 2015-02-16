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
  status = snd_pcm_open(&pcm_handle, device, SND_PCM_STREAM_CAPTURE, 0);

  if (status < 0)
  {
    printf("ERROR: snd_pcm_open %s\n", snd_strerror(status));
  }
}

AudioDevice::~AudioDevice()
{
  if (params != NULL) { snd_pcm_hw_params_free(params); }
  if (pcm_handle != NULL) { snd_pcm_close(pcm_handle); }
}

int AudioDevice::init()
{
  unsigned int rate = 44100;
  //unsigned int rate = 48000;
  //int format = AFMT_S16_LE;
  const int channels = 1;

  if (status < 0)
  {
    printf("status < 0 error\n");
    return -1;
  }

  if (snd_pcm_hw_params_malloc(&params) < 0)
  {
    printf("ERROR: snd_pcm_hw_params_malloc()\n");
    return -1;
  }

  if (snd_pcm_hw_params_any(pcm_handle, params) < 0)
  {
    printf("ERROR: snd_pcm_hw_params_any()\n");
    return -1;
  }

  if ((status = snd_pcm_hw_params_set_access(pcm_handle, params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0)
  {
    printf("ERROR: Can't set interleaved mode. %s\n", snd_strerror(status));
    return -1;
  }
 
  if ((status = snd_pcm_hw_params_set_format(pcm_handle, params, SND_PCM_FORMAT_S16_LE)) < 0)
  {
    printf("ERROR: Can't set format. %s\n", snd_strerror(status));
    return -1;
  }
 
  if ((status = snd_pcm_hw_params_set_channels(pcm_handle, params, channels)) < 0)
  {
    printf("ERROR: Can't set channels number. %s\n", snd_strerror(status));
    return -1;
  }
 
  if ((status = snd_pcm_hw_params_set_rate_near(pcm_handle, params, &rate, 0)) < 0)
  {
    printf("ERROR: Can't set rate. %s\n", snd_strerror(status));
    return -1;
  }
 
  if ((status = snd_pcm_hw_params(pcm_handle, params)) < 0)
  {
    printf("ERROR: Can't set harware parameters. %s\n", snd_strerror(status));
    return -1;
  }

  if ((status = snd_pcm_prepare(pcm_handle)) < 0)
  {
    printf ("ERROR: snd_pcm_prepare %s\n", snd_strerror(status));
    return -1;
  }

  printf("PCM name: '%s'\n", snd_pcm_name(pcm_handle));
  printf("PCM state: %s\n", snd_pcm_state_name(snd_pcm_state(pcm_handle)));

  unsigned int tmp; 
  snd_pcm_hw_params_get_channels(params, &tmp);
  printf("channels: %i ", tmp);
 
  if (tmp == 1) { printf("(mono)\n"); }
  else if (tmp == 2) { printf("(stereo)\n"); }

  snd_pcm_hw_params_get_rate(params, &tmp, 0);
  printf("rate: %d bps\n", tmp);

  return 0;
}

int AudioDevice::read_data(FLOAT *samples, int count)
{
  snd_pcm_sframes_t n;
  int len;

  len = 0; 
  while(len < 8192)
  {
    n = snd_pcm_readi(pcm_handle, ((uint8_t *)buffer) + len, 8192 - len);
    if (n < 0)
    {
      printf("snd_pcm_readi() failed %d  %s\n", len, snd_strerror(n));
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

