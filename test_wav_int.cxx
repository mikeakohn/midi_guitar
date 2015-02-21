/**
 *  midi_guitar
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2015 by Michael Kohn
 *
 * test_wav.cxx: Take a wav file as input, convert to DCT, and output
 *               a .mid file.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>

#include "AudioDevice.h"
#include "AudioInput.h"
#include "DCT.h"
#include "Midi.h"
#include "MidiDevice.h"
#include "MidiFile.h"
#include "NoteMap.h"
#include "WAV.h"
#include "float.h"

int main(int argc, char *argv[])
{
  DCT *dct;
  AudioInput *audio_input;
  NoteMap *note_map;
  Midi *midi;
  Note note;
  uint8_t midi_notes[128];
  int buffer[SAMPLES];
  int dcts[DCT_LEN];
  int last_note = 0;
  int divisions = 0;
  int division_len = (SAMPLES * 240) / 44100;
  int next_note;
  struct timeval tv_lookup,tv_start,tv_end;

  if (argc != 2 && argc != 3)
  {
    printf("Usage: %s <filename.wav> <optional: out.mid>\n", argv[0]);
    exit(0);
  }

  if (strncmp(argv[1], "hw:", 3) == 0 || strncmp(argv[1], "plughw:", 7) == 0)
  {
    audio_input = new AudioDevice(argv[1], 32);
  }
    else
  {
    audio_input = new WAV(argv[1]);
  }

  if (audio_input->init() != 0)
  {
    printf("Could not open file %s for reading\n", argv[1]);
    delete audio_input;
    exit(1);
  }

  if (argc == 3)
  {
    if (strncmp(argv[2], "hw:", 3) == 0 || strncmp(argv[2], "plughw:", 7) == 0)
    {
      midi = new MidiDevice(argv[2]);
    }
      else
    {
      midi = new MidiFile(argv[2], 60, 240);
    }

    if (midi->init() != 0)
    {
      printf("Could not open file %s for writing.\n", argv[2]);
      delete audio_input;
      exit(1);
    }
  }
    else
  {
    midi = NULL;
  }
  
  note_map = new NoteMap(audio_input->get_sample_rate());

  gettimeofday(&tv_lookup, NULL);

  dct = new DCT();
#ifdef COS_LOOKUP
  dct->init_cos_lookup_int();
#endif

  gettimeofday(&tv_start, NULL);

  int flags = fcntl(0, F_GETFL, 0);
  flags |= O_NONBLOCK;
  fcntl(0, F_SETFL, flags);

  int samples = 0;

  while(true)
  {
    printf("------- %f seconds --------\n", (float)samples / (float)audio_input->get_sample_rate());
    if (audio_input->read_data(buffer, SAMPLES) != 0) { break; }
#ifndef COS_LOOKUP
    dct->compute_dct_ii(buffer, dcts, SAMPLES);
#else
    dct->compute_dct_ii_cos_lookup(buffer, dcts, SAMPLES);
#endif

    note_map->dct_to_midi(dcts, midi_notes, DCT_LEN);
    note_map->print_notes(midi_notes);

    if (midi != NULL)
    {
      next_note = 0;

      int loudness = 0;

      for (int n = 0; n < 128; n++)
      {
        if (midi_notes[n] != 0)
        {
          // Pick lowest note
          //next_note = n;
          //break;

          // Pick loudest note
          if (midi_notes[n] > loudness)
          {
            next_note = n;
            loudness = midi_notes[n];
          }
          break;
        }
      }

      if (next_note != last_note && next_note != 0)
      {
        note.volume = 127;
        note.midi_channel = 1;

        if (last_note != 0)
        {
          printf("NOTE OFF: %d  divisions=%d\n", last_note, divisions);
          note.pitch = last_note;
          note.division_delay = divisions;
          midi->write_midi_note_off(&note);
        }

        printf("NOTE ON: %d\n", next_note);
        note.pitch = next_note;
        note.division_delay = 0;
        midi->write_midi_note_on(&note);

        divisions = 0;
        last_note = next_note;
      }

      divisions += division_len;
    }

    samples += SAMPLES;

    int ch = getchar();

    if (ch != -1) { break; }
  }

  gettimeofday(&tv_end, NULL);

  if (midi != NULL)
  {
    if (last_note != 0)
    {
      printf("NOTE OFF: %d  divisions=%d\n", last_note, divisions);
      note.pitch = last_note;
      note.division_delay = divisions;
      midi->write_midi_note_off(&note);
    }
    delete midi;
  }

  delete audio_input;
  delete dct;
  delete note_map;

  tv_lookup.tv_usec = tv_lookup.tv_usec % 1000000;
  tv_start.tv_usec = tv_start.tv_usec % 1000000;
  tv_end.tv_usec = tv_end.tv_usec % 1000000;

#ifdef COS_LOOKUP
  timersub(&tv_start, &tv_lookup, &tv_diff);
  printf("Lookup table time: %fs\n", TIME_TO_FLOAT(tv_diff));
#endif
  timersub(&tv_end, &tv_start, &tv_diff);
  printf("    DCT/MIDI time: %fs\n", TIME_TO_FLOAT(tv_diff));


  return 0;
}


