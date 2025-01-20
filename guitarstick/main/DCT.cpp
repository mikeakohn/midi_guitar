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

#include "DCT.h"

DCT::DCT() :
  cos_lookup_float { nullptr },
  cos_lookup_int   { nullptr },
  int_precision    { 14      },
  dct_len          { 700     }
{
}

DCT::~DCT()
{
  if (cos_lookup_float != nullptr) { free(cos_lookup_float); }
  if (cos_lookup_int   != nullptr) { free(cos_lookup_int);   }
}

void DCT::init(int sample_rate, int sample_count)
{
  //init_cos_lookup_float(sample_rate, sample_count);
  //init_cos_lookup_int(sample_rate, sample_count);
}

int DCT::calc_full(
  const float *data,
  int sample_count,
  int sample_rate,
  int ptr,
  int freq)
{
  const float k = (float)(freq * 2);
  const float N = (float)sample_rate;
  const float N2 = N * 2;
  double sum = 0;

  for (int n = 0; n < sample_count; n++)
  {
    const float xn = data[ptr++];
    if (ptr >= sample_count) { ptr = 0; }

    //sum += xn * cos(M_PI * (2.0 * (float)n + 1.0) * k / (2.0 * N));
    sum += xn * cos(M_PI * (2.0 * (float)n + 1.0) * k / N2);
    //sum += xn;
  }

  if (sum < 0) { sum =- sum; }

  return sum;
}

void DCT::init_cos_lookup_float(int sample_rate, int sample_count)
{ 
  int k,n;
  //int lookup_n = 0; 
  //float N_times_2 = (float)sample_rate * (float)2;
  //float k_div_N_times_2;
  
  cos_lookup_float = (float *)malloc(sample_count * dct_len * sizeof(float));
  
  for (k = 0; k < dct_len; k++)
  { 
    //k_div_N_times_2 = (float)k / N_times_2;
    
    for (n = 0; n < sample_count; n++)
    { 
      //cos_lookup_float[lookup_n++] = cos(pi_2_n_1[n] * k_div_N_times_2);
    }
  }
}

void DCT::init_cos_lookup_int(int sample_rate, int sample_count)
{
  int k, n;
  //int lookup_n = 0;
  //float N_times_2 = (float)sample_rate * (float)2;
  //float k_div_N_times_2;

  cos_lookup_int = (int *)malloc(sample_count * dct_len * sizeof(int));

  for (k = 0; k < dct_len; k++)
  {
    //k_div_N_times_2 = (float)k / N_times_2;

    for (n = 0; n < sample_count; n++)
    {
      //cos_lookup_int[lookup_n++] =
      //  (cos(pi_2_n_1[n] * k_div_N_times_2) * (float)(1 << int_precision));
    }
  }
}

