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

  w0 = SQRT(1.0 / (FLOAT)SAMPLES);
  wk = SQRT(2.0 / (FLOAT)SAMPLES);
}

void DCT::compute_dct_ii(FLOAT *buffer, FLOAT *dct, int N)
{
  FLOAT sum;
  int n,k;
  FLOAT w;
  FLOAT N_times_2 = (FLOAT)N * (FLOAT)2;
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
  FLOAT N_times_2 = (FLOAT)SAMPLES * (FLOAT)2;
  FLOAT k_div_N_times_2;

  cos_lookup = (FLOAT *)malloc(SAMPLES * DCT_LEN * sizeof(FLOAT));

  for (k = 0; k < DCT_LEN; k++)
  {
    int lookup_start = k * DCT_LEN;
    k_div_N_times_2 = (FLOAT)k / N_times_2;

    for (n = 0; n < SAMPLES; n++)
    {
      cos_lookup[lookup_start + n] = COS(pi_2_n_1[n] * k_div_N_times_2);
    }
  }
}

void DCT::compute_dct_ii_cos_lookup(FLOAT *buffer, FLOAT *dct, int N)
{
  FLOAT sum;
  int n,k;
  FLOAT w;

  for (k = 0; k < DCT_LEN; k++)
  {
    sum = 0;

    int lookup_start = k * DCT_LEN;

    for (n = 0; n < N; n++)
    {
      sum += buffer[n] * cos_lookup[lookup_start + n];
    }

    w = (k == 0) ? w0 : wk;

    dct[k] = w * sum;
  }
}


