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


#ifndef _MIDI_DEVICE_H
#define _MIDI_DEVICE_H

#include <stdint.h>
#include <alsa/asoundlib.h>

#include "Midi.h"

class MidiDevice : public Midi
{
public:
  MidiDevice(const char *device);
  virtual ~MidiDevice();

  virtual int init();
  virtual void write_midi_note_on(Note *note);
  virtual void write_midi_note_off(Note *note);

private:
  snd_rawmidi_t *read_handle;
  snd_rawmidi_t *write_handle;
};

#endif

