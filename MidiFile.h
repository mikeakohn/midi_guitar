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


#ifndef _MIDI_FILE_H
#define _MIDI_FILE_H

#include <stdint.h>

struct Note
{
  uint8_t volume;
  uint8_t pitch;
  int division_delay;
  int midi_channel;
};

class MidiFile
{
public:
  MidiFile(FILE *out, const char *song_name, int bpm, int divisions);
  ~MidiFile();

  void write_midi_header(const char *song_name);
  void write_midi_note_on(Note *note);
  void write_midi_note_off(Note *note);
  void write_midi_footer();
  void write_midi_bpm();
  void write_midi_time_signature();

private:
  FILE *out;
  int bpm;
  int divisions;
  int time_signature_beats;
  int time_signature_base;
  long marker;

  int write_int32(int n);
  int write_int16(int n);
  void write_var(int i);

};

#endif

