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

AudioDevice::AudioDevice(const char *device) : mixer(NULL), pcm(NULL)
{
  do
  {
    status = snd_pcm_open(&pcm, device, SND_PCM_STREAM_CAPTURE, 0);
    if (status < 0)
    {
      printf("ERROR: snd_pcm_open %s\n", snd_strerror(status));
      break;
    }

    status = snd_mixer_open(&mixer, 0);
    if (status < 0)
    {
      printf("ERROR: snd_mixer_open %s\n", snd_strerror(status));
      break;
    }

    if (snd_mixer_attach(mixer, device) < 0)
    {
      printf("ERROR: snd_mixer_attach()\n");
      break;
    }
  } while(0);
}

AudioDevice::~AudioDevice()
{
  if (params != NULL) { snd_pcm_hw_params_free(params); }
  if (pcm != NULL) { snd_pcm_close(pcm); }
  if (mixer != NULL) { snd_mixer_close(mixer); }
}

int AudioDevice::init()
{
  unsigned int rate = 44100;
  //unsigned int rate = 48000;
  //int format = AFMT_S16_LE;
  const int channels = 1;
  //snd_pcm_chmap_t chmap = SND_CHMAP_MONO;

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

  if (snd_pcm_hw_params_any(pcm, params) < 0)
  {
    printf("ERROR: snd_pcm_hw_params_any()\n");
    return -1;
  }

  if ((status = snd_pcm_hw_params_set_access(pcm, params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0)
  {
    printf("ERROR: Can't set interleaved mode. %s\n", snd_strerror(status));
    return -1;
  }
 
  if ((status = snd_pcm_hw_params_set_format(pcm, params, SND_PCM_FORMAT_S16_LE)) < 0)
  {
    printf("ERROR: Can't set format. %s\n", snd_strerror(status));
    return -1;
  }
 
  if ((status = snd_pcm_hw_params_set_channels(pcm, params, channels)) < 0)
  {
    printf("ERROR: Can't set channels number. %s\n", snd_strerror(status));
    return -1;
  }

#if 0
  if ((status = snd_pcm_set_chmap(pcm, &chmap)) < 0)
  {
    printf("ERROR: snd_pcm_set_chmap. %s\n", snd_strerror(status));
    return -1;
  }
#endif
 
  if ((status = snd_pcm_hw_params_set_rate_near(pcm, params, &rate, 0)) < 0)
  {
    printf("ERROR: Can't set rate. %s\n", snd_strerror(status));
    return -1;
  }
 
  if ((status = snd_pcm_hw_params(pcm, params)) < 0)
  {
    printf("ERROR: Can't set harware parameters. %s\n", snd_strerror(status));
    return -1;
  }

  if ((status = snd_pcm_prepare(pcm)) < 0)
  {
    printf ("ERROR: snd_pcm_prepare %s\n", snd_strerror(status));
    return -1;
  }

  printf("PCM name: '%s'\n", snd_pcm_name(pcm));
  printf("PCM state: %s\n", snd_pcm_state_name(snd_pcm_state(pcm)));

  unsigned int tmp; 
  snd_pcm_hw_params_get_channels(params, &tmp);
  printf("channels: %i\n", tmp);

  if (tmp == 1) { printf("(mono)\n"); }
  else if (tmp == 2) { printf("(stereo)\n"); }

#if 0
  snd_pcm_chmap_t *chmap;
  chmap = snd_pcm_get_chmap(pcm);
  printf("PCM pos: %s\n", snd_pcm_chmap_name((snd_pcm_chmap_position)chmap->pos[0]));
#endif
  snd_pcm_chmap_query_t **chmap_query;
  chmap_query = snd_pcm_query_chmaps(pcm);

  snd_pcm_chmap_query_t **p;
  for (p = chmap_query; *p != NULL; p++)
  {
    printf("type=%s  channels=%d\n",
      snd_pcm_chmap_type_name((*p)->type),
      (*p)->map.channels);
  }

  snd_pcm_hw_params_get_rate(params, &tmp, 0);
  printf("rate: %d bps\n", tmp);

  // Mixer code
  if (snd_mixer_selem_register(mixer, NULL, NULL) < 0)
  {
    printf("ERROR: snd_mixer_selem_register()\n");
    return -1;
  }

  if (snd_mixer_load(mixer) < 0)
  {
    printf("ERROR: snd_mixer_load()\n");
    return -1;
  }

  //snd_mixer_selem_id_t *sid;
  snd_mixer_elem_t *elem;

  for (elem = snd_mixer_first_elem(mixer);
       elem != NULL;
       elem = snd_mixer_elem_next(elem))
  {
    if (snd_mixer_selem_is_enum_capture(elem))
    {
      printf("Found capture\n");
    }
      else
    {
      printf("NOT capture\n");
    }

    if (snd_mixer_selem_has_common_volume(elem))
    {
      printf("TRUE\n");
    }
      else
    {
      printf("FALSE\n");
    }

    if ((status = snd_mixer_selem_set_capture_volume_all(elem, 1000)) < 0)
    {
      printf("ERROR: snd_mixer_selem_set_capture_volume_all() %s\n", snd_strerror(status));
    }
  }

  return 0;
}

int AudioDevice::read_data(FLOAT *samples, int count)
{
  snd_pcm_sframes_t n;
  int len;

  len = 0; 
  while(len < 8192)
  {
    n = snd_pcm_readi(pcm, ((uint8_t *)buffer) + len, 8192 - len);
    if (n < 0)
    {
      printf("snd_pcm_readi() failed %d  %s\n", len, snd_strerror(n));
      return -1;
    }
    len += n;
  }

  for (n = 0; n < 8192; n++)
  {
    //FLOAT sample = (FLOAT)buffer[n] / 32767.0;
    FLOAT sample = (FLOAT)buffer[n] / 1000.0;

    if (sample > 1.0) { sample = 1.0; }
      else
    if (sample < -1.0) { sample = -1.0; }

    samples[n] = sample;
//printf("samples[n]=%f  %d\n", samples[n], buffer[n]);
  }

  return 0;
}

