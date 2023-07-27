/**
 *  midi_guitar
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2015 by Michael Kohn
 *
 */

#ifndef _AUDIO_DEVICE_H
#define _AUDIO_DEVICE_H

#include <stdint.h>
#ifdef ALSA
#include <alsa/asoundlib.h>
#endif

#include "float.h"
#include "AudioInput.h"

class AudioDevice : public AudioInput
{
public:
  AudioDevice(const char *filename, int gain);
  virtual ~AudioDevice();
  virtual int init();
  virtual int read_data(FLOAT *samples, int count);
  virtual int read_data(int *samples, int count);
  virtual int get_sample_rate() { return 44100; }

private:
#ifdef ALSA
  snd_mixer_t *mixer;
  snd_pcm_t *pcm;
  snd_pcm_hw_params_t *params;
  snd_pcm_uframes_t frames;
#endif
  int status;
  float divisor;
  int gain;
  int16_t buffer[8192];
};

#endif

