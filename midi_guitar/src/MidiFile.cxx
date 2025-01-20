/**
 *  midi_guitar
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2015 by Michael Kohn
 *
 * MidiFile.cxx: Module for creating .mid files.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "MidiFile.h"

MidiFile::MidiFile(const char *filename, int bpm, int divisions) :
  bpm(bpm),
  divisions(divisions),
  time_signature_beats(4),
  time_signature_base(4)
{
  out = fopen(filename, "wb");
}

MidiFile::~MidiFile()
{
  if (out != NULL)
  {
    write_midi_footer();
    fclose(out);
  }
}

int MidiFile::init()
{
  if (out == NULL) { return -1; }

  write_midi_header("dct");
  write_midi_bpm();

  return 0;
}

void MidiFile::write_midi_header(const char *song_name)
{
  const char *comment = "Created by Michael Kohn; http://www.mikekohn.net/";

  fprintf(out,"MThd");
  write_int32(6);         // length (always 6)
  write_int16(0);         // 0=single track, 1=multiple track, 2=multiple song
  write_int16(1);         // number of track chunks
  write_int16(divisions); // divisions

  fprintf(out, "MTrk");
  marker = ftell(out);
  write_int32(0);         // track length (start with 0 and fill in later)

  if (song_name[0] != 0)
  {
    write_var(0);         // time=0
    putc(0xff, out);      // 0xff=meta event
    putc(0x03, out);      // 0x03=sequence or track name
    write_var(strlen(song_name));
    fprintf(out, "%s", song_name);
  }

  write_var(0);           // time=0
  putc(0xff, out);        // 0xff=meta event
  putc(0x01, out);        // 0x01=text event
  write_var(strlen(comment));

  fprintf(out, "%s", comment);
}

void MidiFile::write_midi_note_on(Note *note)
{
  write_var(note->division_delay);
  putc(0x90 + note->midi_channel, out);
  putc(note->pitch, out);
  putc(note->volume, out);
}

void MidiFile::write_midi_note_off(Note *note)
{
  //int d;
  //d = (int)((float)divisions * ((float)note->duration / (float)(60000000 / bpm)));

  write_var(note->division_delay);
  putc(0x80 + note->midi_channel, out);
  putc(note->pitch, out);
  putc(0, out);
}

void MidiFile::write_midi_footer()
{
  long i;

  write_var(0);        // time=now
  putc(0xff, out);     // 0xff=meta event
  putc(0x2f, out);     // 0x2f=end of track
  putc(0x00, out);

  i = ftell(out);
  fseek(out, marker, 0);
  write_int32((i - marker) - 4);
  fseek(out, i, 0);
}

void MidiFile::write_midi_bpm()
{
  int d;

  write_var(0);       // time=now
  putc(0xff, out);    // 0xff=meta event
  putc(0x51, out);    // 0x51=tempo setting
  putc(0x03, out);
  d = 60000000 / bpm;
  putc(d >> 16, out);
  putc((d >> 8) & 0xff, out);
  putc(d & 0xff, out);
}

void MidiFile::write_midi_time_signature()
{
  int d;

  d = time_signature_base;

  switch(d)
  {
    case 1: d = 0; break;
    case 2: d = 1; break;
    case 4: d = 2; break;
    case 8: d = 3; break;
    case 16: d = 4; break;
    case 32: d = 5; break;
    default: return;
  }

  write_var(0);      // 0=now
  putc(0xff, out);   // 0xff=meta event
  putc(0x58, out);   // 0x58=time signature
  putc(0x04, out);

  putc(time_signature_beats, out);
  putc(d, out);

  if (d==3)
  {
    putc(divisions / 3, out);
  }
    else
  {
    putc(divisions, out);
  }

  putc(8, out);
}

int MidiFile::write_int32(int n)
{
  putc(((n >> 24) & 0xff), out);
  putc(((n >> 16) & 0xff), out);
  putc(((n >> 8) & 0xff), out);
  putc((n & 0xff), out);

  return 0;
}

int MidiFile::write_int16(int n)
{
  putc(((n >> 8) & 0xff), out);
  putc((n & 0xff), out);

  return 0;
}

void MidiFile::write_var(int i)
{
  int t,k;

  t = 7;
  while((i >> t) != 0)
  {
    t = t + 7;
  }

  t = t - 7;
  for (k = t; k >= 0; k = k - 7)
  {
    if (k != 0)
    { putc(((i >> k) & 0x7f) + 0x80, out); }
      else
    { putc(((i >> k) & 0x7f), out); }
  }
}


