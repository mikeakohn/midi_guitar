/**
 *  midi_guitar
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2015 by Michael Kohn
 *
 * DCT.cxx: Input is a list of samples, output is a set of DCT's.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <math.h>

//#include "FrequencyMap.h"
#include "DCT.h"

DCT::DCT()
{
  int n;
  for (n = 0; n < SAMPLES; n++)
  {
    pi_2_n_1[n] = M_PI * (2.0 * (FLOAT)n + 1);
  }

  w0 = SQRT(1.0 / (FLOAT)SAMPLES) * ((float)SAMPLE_RATE / (float)SAMPLES);
  wk = SQRT(2.0 / (FLOAT)SAMPLES) * ((float)SAMPLE_RATE / (float)SAMPLES);

  w0_int = w0 * (1 << INT_PRECISION) * ((float)SAMPLE_RATE / (float)SAMPLES);
  wk_int = wk * (1 << INT_PRECISION) * ((float)SAMPLE_RATE / (float)SAMPLES);
}

void DCT::compute_dct_ii(FLOAT *buffer, FLOAT *dct, int N)
{
  FLOAT sum;
  int n,k;
  FLOAT w;
  FLOAT N_times_2 = (FLOAT)SAMPLE_RATE * (FLOAT)2;
  FLOAT k_div_N_times_2;

  for (k = 0; k < DCT_LEN; k++)
  {
    sum = 0;

    k_div_N_times_2 = (FLOAT)k / N_times_2;

    for (n = 0; n < N; n++)
    {
      //sum += buffer[n] * COS(M_PI * ((FLOAT)2 * (FLOAT)n + (FLOAT)1) * (FLOAT)k / ((FLOAT)2 * (FLOAT)N));
      sum += buffer[n] * COS(pi_2_n_1[n] * k_div_N_times_2);
    }

    w = (k == 0) ? w0 : wk;

    dct[k] = w * sum;
  }
}

void DCT::init_cos_lookup()
{
  int k,n;
  int lookup_start = 0;
  FLOAT N_times_2 = (FLOAT)SAMPLE_RATE * (FLOAT)2;
  FLOAT k_div_N_times_2;

  cos_lookup = (FLOAT *)malloc(SAMPLES * DCT_LEN * sizeof(FLOAT));

  for (k = 0; k < DCT_LEN; k++)
  {
    k_div_N_times_2 = (FLOAT)k / N_times_2;

    for (n = 0; n < SAMPLES; n++)
    {
      cos_lookup[lookup_start + n] = COS(pi_2_n_1[n] * k_div_N_times_2);
    }

    lookup_start += SAMPLES;
  }
}

void DCT::init_cos_lookup_int()
{
  int k,n;
  int lookup_start = 0;
  FLOAT N_times_2 = (FLOAT)SAMPLE_RATE * (FLOAT)2;
  FLOAT k_div_N_times_2;

  cos_lookup_int = (int *)malloc(SAMPLES * DCT_LEN * sizeof(int));

  for (k = 0; k < DCT_LEN; k++)
  {
    k_div_N_times_2 = (FLOAT)k / N_times_2;

    for (n = 0; n < SAMPLES; n++)
    {
      cos_lookup_int[lookup_start + n] =
        (COS(pi_2_n_1[n] * k_div_N_times_2) * (float)(1 << INT_PRECISION));
    }

    lookup_start += SAMPLES;
  }
}

void DCT::compute_dct_ii_cos_lookup(FLOAT *buffer, FLOAT *dct, int N)
{
  int n,k;
  int lookup_start = 0;
  FLOAT sum;
  FLOAT w;

  for (k = 0; k < DCT_LEN; k++)
  {
    sum = 0;

    for (n = 0; n < N; n++)
    {
      sum += buffer[n] * cos_lookup[lookup_start + n];
    }

    w = (k == 0) ? w0 : wk;

    dct[k] = w * sum;

    lookup_start += SAMPLES;
  }
}

void DCT::compute_dct_ii_cos_lookup(int *buffer, int *dct, int N)
{
  int n,k;
  int lookup_start = 0;
  int sample;
  int sum;
  int w;

  for (k = 0; k < DCT_LEN; k++)
  {
    sum = 0;

    for (n = 0; n < N; n++)
    {
      if (INT_PRECISION == 15) { sample = buffer[n]; }
      else { sample = buffer[n] >> (15 - INT_PRECISION); }

//printf("%d  %d\n", sample, cos_lookup_int[lookup_start + n]);
      sum += (sample * cos_lookup_int[lookup_start + n]) >> INT_PRECISION;
    }

    w = (k == 0) ? w0_int : wk_int;

    dct[k] = (w * sum) >> INT_PRECISION;

    lookup_start += SAMPLES;
  }
}


