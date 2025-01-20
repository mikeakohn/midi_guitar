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

#include <math.h>

#include "FreqCalc.h"

FreqCalc::FreqCalc() :
  threshold   { 400   },
  sample_rate { 20000 }
{
}

FreqCalc::~FreqCalc()
{
}

int FreqCalc::get_frequency(
  const int16_t *data,
  int sample_count,
  int ptr)
{
  int n;
  int wave = 0;
  int change_count = 0;
  int start = -1;
  int last = -1;

  for (n = 0; n < sample_count; n++)
  {
    if (data[n] > threshold)
    {
      if (wave == -1)
      {
        change_count += 1;
        last = n;
      }

      wave = 1;
      if (start == -1) { start = n; }
    }
      else
    if (data[n] < -threshold)
    {
      if (wave == 1)
      {
        wave = -1;
      }
    }
  }

  if (start == -1 || last == -1) { return 0; }

//printf("%d %d %d\n", sample_rate, last, start);
//printf("%d\n", sample_rate / (last - start));
//printf("%d\n", sample_rate / sample_count);

  int frequency = change_count * sample_rate / (last - start);

  return frequency;
}

