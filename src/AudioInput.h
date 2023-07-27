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

#ifndef _AUDIO_INPUT_H
#define _AUDIO_INPUT_H

#include <stdint.h>

#include "float.h"

class AudioInput
{
public:
  AudioInput() { }
  AudioInput(const char *filename) { }
  virtual ~AudioInput();
  virtual int init() = 0;
  virtual int read_data(FLOAT *samples, int count) = 0;
  virtual int read_data(int *samples, int count) = 0;
  virtual int get_sample_rate() = 0;
};

#endif

