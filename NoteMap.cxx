/**
 *  midi_guitar
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2015 by Michael Kohn
 *
 * NoteMap.cxx: Take a series dct's and output a series of midi notes and
 *              their volume.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "DCT.h"
#include "NoteMap.h"

double NoteMap::midi_notes[] =
{
  0,  // 0
  0,  // 1
  0,  // 2
  0,  // 3
  0,  // 4
  0,  // 5
  0,  // 6
  0,  // 7
  0,  // 8
  0,  // 9
  0,  // 10
  0,  // 11
  16.35,  // 12 C0
  17.32,  // 13 C#0/Db0
  18.35,  // 14 D0
  19.45,  // 15 D#0/Eb0
  20.60,  // 16 E0
  21.83,  // 17 F0
  23.12,  // 18 F#0/Gb0
  24.50,  // 19 G0
  25.96,  // 20 G#0/Ab0
  27.50,  // 21 A0
  29.14,  // 22 A#0/Bb0
  30.87,  // 23 B0
  32.70,  // 24 C1
  34.65,  // 25 C#1/Db1
  36.71,  // 26 D1
  38.89,  // 27 D#1/Eb1
  41.20,  // 28 E1
  43.65,  // 29 F1
  46.25,  // 30 F#1/Gb1
  49.00,  // 31 G1
  51.91,  // 32 G#1/Ab1
  55.00,  // 33 A1
  58.27,  // 34 A#1/Bb1
  61.74,  // 35 B1
  65.41,  // 36 C2
  69.30,  // 37 C#2/Db2
  73.42,  // 38 D2
  77.78,  // 39 D#2/Eb2
  82.41,  // 40 E2
  87.31,  // 41 F2
  92.50,  // 42 F#2/Gb2
  98.00,  // 43 G2
  103.83,  // 44 G#2/Ab2
  110.00,  // 45 A2
  116.54,  // 46 A#2/Bb2
  123.47,  // 47 B2
  130.81,  // 48 C3
  138.59,  // 49 C#3/Db3
  146.83,  // 50 D3
  155.56,  // 51 D#3/Eb3
  164.81,  // 52 E3
  174.61,  // 53 F3
  185.00,  // 54 F#3/Gb3
  196.00,  // 55 G3
  207.65,  // 56 G#3/Ab3
  220.00,  // 57 A3
  233.08,  // 58 A#3/Bb3
  246.94,  // 59 B3
  261.63,  // 60 C4
  277.18,  // 61 C#4/Db4
  293.66,  // 62 D4
  311.13,  // 63 D#4/Eb4
  329.63,  // 64 E4
  349.23,  // 65 F4
  369.99,  // 66 F#4/Gb4
  392.00,  // 67 G4
  415.30,  // 68 G#4/Ab4
  440.00,  // 69 A4
  466.16,  // 70 A#4/Bb4
  493.88,  // 71 B4
  523.25,  // 72 C5
  554.37,  // 73 C#5/Db5
  587.33,  // 74 D5
  622.25,  // 75 D#5/Eb5
  659.25,  // 76 E5
  698.46,  // 77 F5
  739.99,  // 78 F#5/Gb5
  783.99,  // 79 G5
  830.61,  // 80 G#5/Ab5
  880.00,  // 81 A5
  932.33,  // 82 A#5/Bb5
  987.77,  // 83 B5
  1046.50,  // 84 C6
  1108.73,  // 85 C#6/Db6
  1174.66,  // 86 D6
  1244.51,  // 87 D#6/Eb6
  1318.51,  // 88 E6
  1396.91,  // 89 F6
  1479.98,  // 90 F#6/Gb6
  1567.98,  // 91 G6
  1661.22,  // 92 G#6/Ab6
  1760.00,  // 93 A6
  1864.66,  // 94 A#6/Bb6
  1975.53,  // 95 B6
  2093.00,  // 96 C7
  2217.46,  // 97 C#7/Db7
  2349.32,  // 98 D7
  2489.02,  // 99 D#7/Eb7
  2637.02,  // 100 E7
  2793.83,  // 101 F7
  2959.96,  // 102 F#7/Gb7
  3135.96,  // 103 G7
  3322.44,  // 104 G#7/Ab7
  3520.00,  // 105 A7
  3729.31,  // 106 A#7/Bb7
  3951.07,  // 107 B7
  4186.01,  // 108 C8
  4434.92,  // 109 C#8/Db8
  4698.63,  // 110 D8
  4978.03,  // 111 D#8/Eb8
  5274.04,  // 112 E8
  5587.65,  // 113 F8
  5919.91,  // 114 F#8/Gb8
  6271.93,  // 115 G8
  6644.88,  // 116 G#8/Ab8
  7040.00,  // 117 A8
  7458.62,  // 118 A#8/Bb8
  7902.13,  // 119 B8
};

const char *NoteMap::midi_names[] =
{
  "??",  // 0
  "??",  // 1
  "??",  // 2
  "??",  // 3
  "??",  // 4
  "??",  // 5
  "??",  // 6
  "??",  // 7
  "??",  // 8
  "??",  // 9
  "??",  // 10
  "??",  // 11
  "C0",  // 12 16.35
  "C#0/Db0",  // 13 17.32
  "D0",  // 14 18.35
  "D#0/Eb0",  // 15 19.45
  "E0",  // 16 20.60
  "F0",  // 17 21.83
  "F#0/Gb0",  // 18 23.12
  "G0",  // 19 24.50
  "G#0/Ab0",  // 20 25.96
  "A0",  // 21 27.50
  "A#0/Bb0",  // 22 29.14
  "B0",  // 23 30.87
  "C1",  // 24 32.70
  "C#1/Db1",  // 25 34.65
  "D1",  // 26 36.71
  "D#1/Eb1",  // 27 38.89
  "E1",  // 28 41.20
  "F1",  // 29 43.65
  "F#1/Gb1",  // 30 46.25
  "G1",  // 31 49.00
  "G#1/Ab1",  // 32 51.91
  "A1",  // 33 55.00
  "A#1/Bb1",  // 34 58.27
  "B1",  // 35 61.74
  "C2",  // 36 65.41
  "C#2/Db2",  // 37 69.30
  "D2",  // 38 73.42
  "D#2/Eb2",  // 39 77.78
  "E2",  // 40 82.41
  "F2",  // 41 87.31
  "F#2/Gb2",  // 42 92.50
  "G2",  // 43 98.00
  "G#2/Ab2",  // 44 103.83
  "A2",  // 45 110.00
  "A#2/Bb2",  // 46 116.54
  "B2",  // 47 123.47
  "C3",  // 48 130.81
  "C#3/Db3",  // 49 138.59
  "D3",  // 50 146.83
  "D#3/Eb3",  // 51 155.56
  "E3",  // 52 164.81
  "F3",  // 53 174.61
  "F#3/Gb3",  // 54 185.00
  "G3",  // 55 196.00
  "G#3/Ab3",  // 56 207.65
  "A3",  // 57 220.00
  "A#3/Bb3",  // 58 233.08
  "B3",  // 59 246.94
  "C4",  // 60 261.63
  "C#4/Db4",  // 61 277.18
  "D4",  // 62 293.66
  "D#4/Eb4",  // 63 311.13
  "E4",  // 64 329.63
  "F4",  // 65 349.23
  "F#4/Gb4",  // 66 369.99
  "G4",  // 67 392.00
  "G#4/Ab4",  // 68 415.30
  "A4",  // 69 440.00
  "A#4/Bb4",  // 70 466.16
  "B4",  // 71 493.88
  "C5",  // 72 523.25
  "C#5/Db5",  // 73 554.37
  "D5",  // 74 587.33
  "D#5/Eb5",  // 75 622.25
  "E5",  // 76 659.25
  "F5",  // 77 698.46
  "F#5/Gb5",  // 78 739.99
  "G5",  // 79 783.99
  "G#5/Ab5",  // 80 830.61
  "A5",  // 81 880.00
  "A#5/Bb5",  // 82 932.33
  "B5",  // 83 987.77
  "C6",  // 84 1046.50
  "C#6/Db6",  // 85 1108.73
  "D6",  // 86 1174.66
  "D#6/Eb6",  // 87 1244.51
  "E6",  // 88 1318.51
  "F6",  // 89 1396.91
  "F#6/Gb6",  // 90 1479.98
  "G6",  // 91 1567.98
  "G#6/Ab6",  // 92 1661.22
  "A6",  // 93 1760.00
  "A#6/Bb6",  // 94 1864.66
  "B6",  // 95 1975.53
  "C7",  // 96 2093.00
  "C#7/Db7",  // 97 2217.46
  "D7",  // 98 2349.32
  "D#7/Eb7",  // 99 2489.02
  "E7",  // 100 2637.02
  "F7",  // 101 2793.83
  "F#7/Gb7",  // 102 2959.96
  "G7",  // 103 3135.96
  "G#7/Ab7",  // 104 3322.44
  "A7",  // 105 3520.00
  "A#7/Bb7",  // 106 3729.31
  "B7",  // 107 3951.07
  "C8",  // 108 4186.01
  "C#8/Db8",  // 109 4434.92
  "D8",  // 110 4698.63
  "D#8/Eb8",  // 111 4978.03
  "E8",  // 112 5274.04
  "F8",  // 113 5587.65
  "F#8/Gb8",  // 114 5919.91
  "G8",  // 115 6271.93
  "G#8/Ab8",  // 116 6644.88
  "A8",  // 117 7040.00
  "A#8/Bb8",  // 118 7458.62
  "B8",  // 119 7902.13
};

NoteMap::NoteMap(int sample_rate)
{
  int i,n,k;

  memset(frequency_to_midi, 0, sizeof(frequency_to_midi));

  for (i = 12; i < 119; i++)
  {
    int start = (int)midi_notes[i];
    int end = (int)midi_notes[i + 1];
    int half = start + ((end - start) / 2);

    //printf("%d) %d %d\n", i, start, end);

    for (n = start; n < half; n++)
    {
      frequency_to_midi[n] = i;
    }

    for (n = half; n < end; n++)
    {
      frequency_to_midi[n] = i + 1;
    }
  }

  freq_scale = (FLOAT)(22050) / (FLOAT)sample_rate;
  freq_scale = freq_scale * (FLOAT)sample_rate / SAMPLES;
  //freq_scale = 0.5;
printf("freq_scale=%f\n", freq_scale);

  for (k = 0; k < 8192; k++)
  {
    int freq = (FLOAT)k * freq_scale;
    int note = k < 8192 ? get_note(freq) : 0;

    dct_to_note[k] = note;
  }
}

void NoteMap::dct_to_midi(FLOAT *dcts, uint8_t *midi_notes, int count)
{
  int volume,note;
  //int freq;
  int k;

  memset(midi_notes, 0, 128);

  for (k = 0; k < count; k++)
  {
    volume = (int)fabs(dcts[k]);

    if (volume > VOLUME_THRESHOLD)
    {
#if 0
      freq = (FLOAT)k * freq_scale;
      note = k < 8192 ? get_note(freq) : 0;
#endif

      note = dct_to_note[k];

      if (midi_notes[note] < volume) { midi_notes[note] = volume; }
      //printf("%d - %s  volume=%d (%d,%f)\n", note, get_name(note), volume, k, dcts[k]);
    }
  }
}

void NoteMap::dct_to_midi(int *dcts, uint8_t *midi_notes, int count)
{
  int volume,note;
  //int freq;
  int k;

  memset(midi_notes, 0, 128);

  for (k = 0; k < count; k++)
  {
    volume = dcts[k];
    volume = (volume < 0) ? -volume : volume;

//printf("%d) %d [%x]   %x\n", k , volume, volume, VOLUME_THRESHOLD_INT);
    if (volume > VOLUME_THRESHOLD_INT)
    {
#if 0
      freq = (k * (44100 / 2)) / SAMPLES;
      note = k < 8192 ? get_note(freq) : 0;
#endif

      note = dct_to_note[k];

      volume = volume >> 15;
      volume = (volume > 255) ? 255 : volume;

      if (midi_notes[note] < volume) { midi_notes[note] = volume; }
    }
  }
}

void NoteMap::print_notes(uint8_t *midi_notes)
{
  int n;

  for (n = 0; n < 128; n++)
  {
    if (midi_notes[n] == 0) { continue; }

    printf("%d - %s  volume=%d\n", n, get_name(n), midi_notes[n]);
  }

}

