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

#ifndef DCT_H
#define DCT_H

#include <stdint.h>

class DCT
{
public:
  DCT();
  ~DCT();

  void init(int sample_rate, int sample_count);

  int calc(
    const float *data,
    int sample_count,
    int sample_rate,
    int ptr,
    int freq)
  {
    return calc_full(data, sample_count, sample_rate, ptr, freq);
  }

private:
  int calc_full(
    const float *data,
    int sample_count,
    int sample_rate,
    int ptr,
    int freq);

  void init_cos_lookup_float(int sample_rate, int sample_count);
  void init_cos_lookup_int(int sample_rate, int sample_count);

  float *cos_lookup_float;
  int *cos_lookup_int;

  const int int_precision;
  const int dct_len;
};

#endif

