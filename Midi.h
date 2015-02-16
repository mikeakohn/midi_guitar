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


#ifndef _MIDI_H
#define _MIDI_H

#include <stdint.h>

struct Note
{
  uint8_t volume;
  uint8_t pitch;
  int division_delay;
  int midi_channel;
};

class Midi
{
public:
  Midi();
  virtual ~Midi();

  virtual int init() = 0;
  virtual void write_midi_note_on(Note *note) = 0;
  virtual void write_midi_note_off(Note *note) = 0;

  int status;
};

#endif

