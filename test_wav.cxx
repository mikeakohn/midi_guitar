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
#include <math.h>

#include "DCT.h"
#include "MidiFile.h"
#include "MidiMap.h"
#include "WAV.h"
#include "float.h"

int main(int argc, char *argv[])
{
  FILE *in,*out;
  DCT *dct;
  WAV *wav;
  MidiMap *midi_map;
  MidiFile *midi_file;
  uint8_t midi_notes[128];
  FLOAT buffer[SAMPLES];
  FLOAT dcts[DCT_LEN];
  int last_note = 0;
  int divisions = 0;
  int division_len = (SAMPLES * 240) / 44100;
  int next_note;

  if (argc != 2 && argc != 3)
  {
    printf("Usage: %s <filename.wav> <optional: out.mid>\n", argv[0]);
    exit(0);
  }

  in = fopen(argv[1], "rb");
  if (in == NULL)
  {
    printf("Could not open file %s for reading.\n", argv[1]);
    exit(1);
  }

  if (argc == 3)
  {
    out = fopen("out.mid", "wb");
    if (out == NULL)
    {
      printf("Could not open file %s for writing.\n", argv[2]);
      fclose(in);
      exit(1);
    }

    midi_file = new MidiFile(out, argv[2], 60, 240);
  }
    else
  {
    out = NULL;
    midi_file = NULL;
  }
  
  wav = new WAV(in);
  wav->read_headers();

  midi_map = new MidiMap(wav->get_sample_rate());

  dct = new DCT();
#ifdef COS_LOOKUP
  dct->init_cos_lookup();
#endif

  int samples = 0;

  while(1)
  {
    printf("------- %f seconds --------\n", (float)samples / (float)wav->get_sample_rate());
    if (wav->read_data(buffer, SAMPLES) != 0) { break; }
#ifndef COS_LOOKUP
    dct->compute_dct_ii(buffer, dcts, SAMPLES);
#else
    dct->compute_dct_ii_cos_lookup(buffer, dcts, SAMPLES);
#endif

    midi_map->dct_to_midi(dcts, midi_notes, DCT_LEN);
    midi_map->print_notes(midi_notes);

    if (midi_file != NULL)
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
        Note note;
        note.volume = 127;
        note.midi_channel = 1;

        if (last_note != 0)
        {
          printf("NOTE OFF: %d  divisions=%d\n", last_note, divisions);
          note.pitch = last_note;
          note.division_delay = divisions;
          midi_file->write_midi_note_off(&note);
        }

        printf("NOTE ON: %d\n", next_note);
        note.pitch = next_note;
        note.division_delay = 0;
        midi_file->write_midi_note_on(&note);

        divisions = 0;
        last_note = next_note;
      }

      divisions += division_len;
    }

    samples += SAMPLES;
  }

  if (midi_file != NULL)
  {
    midi_file->write_midi_footer();
    delete midi_file;
  }

  delete wav;
  delete dct;
  delete midi_map;

  fclose(in);
  fclose(out);

  return 0;
}


