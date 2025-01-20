/**
 *  GuitarStick
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: https://www.mikekohn.net/
 * License: BSD
 *
 * Copyright 2024 by Michael Kohn
 *
 * https://www.mikekohn.net/micro/guitarstick.php
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "Notes.h"

Notes::Notes()
{
  int length;

  length = sizeof(const char **) * MAX_FREQUENCY;
  frequency_to_name = (const char **)malloc(length);
  memset(frequency_to_name, 0, length);

  length = sizeof(int) * MAX_FREQUENCY;
  frequency_to_midi = (int *)malloc(length);
  memset(frequency_to_midi, 0, length);

  int n;
  const float percent = 0.22;

  for (n = 0; values[n].frequency != 0; n++)
  {
    int frequency = values[n].frequency;

    if (frequency >= MAX_FREQUENCY) { continue; }

    frequency_to_name[n] = values[n].name; 
    frequency_to_midi[n] = values[n].midi; 

    if (n != 0 && values[n + 1].name != NULL)
    {
      int low = values[n - 1].frequency;
      int high = values[n + 1].frequency;

      int distance_0 = frequency - low;
      int distance_1 = high - frequency;

      int delta_0 = (float)distance_0 * percent;
      int delta_1 = (float)distance_1 * percent;

      if (delta_0 > distance_0 / 2) { delta_0 = distance_0 / 2; }
      if (delta_1 > distance_1 / 2) { delta_1 = distance_1 / 2; }

      int x0 = frequency - delta_0;
      int x1 = frequency + delta_1;

      int r;
      for (r = x0; r <= x1; r++)
      {
        if (r >= MAX_FREQUENCY) { continue; }

        frequency_to_name[r] = values[n].name;
        frequency_to_midi[r] = values[n].midi;
      }
    }
  }
}

Notes::~Notes()
{
  free(frequency_to_name);
  free(frequency_to_midi);
}

#if 0
const char *Notes::get_name(int frequency)
{
  int n;

  for (n = 0; notes[n].frequency != 0; n++)
  {
    if (notes[n].frequency == frequency) { return notes[n].name; }
  }

  return NULL;
}

int Notes::get_midi(int frequency)
{
  int n;

  for (n = 0; values[n].frequency != 0; n++)
  {
    if (values[n].frequency == frequency) { return values[n].midi; }
  }

  return 0;
}
#endif

