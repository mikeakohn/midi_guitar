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

#include "float.h"
#include "AudioInput.h"

class AudioDevice : public AudioInput
{
public:
  AudioDevice(const char *filename);
  virtual ~AudioDevice();
  virtual int init();
  virtual int read_data(FLOAT *samples, int count);
  virtual int get_sample_rate() { return 44100; }

private:
  int in;
  int16_t buffer[8192];
};

#endif

