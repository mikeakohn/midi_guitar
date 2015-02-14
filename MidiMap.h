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

#ifndef _MIDI_MAP
#define _MIDI_MAP

#include <stdint.h>

#include "float.h"

class MidiMap
{
public:
  MidiMap(int sample_rate);
  ~MidiMap() { }
  const char *get_name(int note) { return midi_names[note]; }
  void dct_to_midi(FLOAT *dct, uint8_t *midi_notes, int count);
  void print_notes(uint8_t *midi_notes);
  int get_note(int frequency) { return frequency_to_midi[frequency]; }


private:
  int frequency_to_midi[8192];
  static double midi_notes[];
  static const char *midi_names[];
  FLOAT freq_scale;
};

#endif
