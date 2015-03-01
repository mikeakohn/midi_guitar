/**
 *  midi_guitar
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2015 by Michael Kohn
 *
 */


#ifndef _DCT_H
#define _DCT_H

#include "float.h"

//#define SAMPLES 44100
//#define SAMPLE_RATE 22050
#define SAMPLE_RATE 8192
#define SAMPLES 8192
#define DCT_LEN 3151
#define VOLUME_THRESHOLD 2
#define VOLUME_THRESHOLD_INT (4 << INT_PRECISION)

class DCT
{
public:
  DCT();
  ~DCT() { }

  void compute_dct_ii(FLOAT *buffer, FLOAT *dct, int N);
  void init_cos_lookup();
  void init_cos_lookup_int();
  void compute_dct_ii_cos_lookup(FLOAT *buffer, FLOAT *dct, int N);
  void compute_dct_ii_cos_lookup(int *buffer, int *dct, int N);

private:
  FLOAT *cos_lookup;
  int *cos_lookup_int;
  FLOAT w0,wk;
  int w0_int,wk_int;
  FLOAT pi_2_n_1[SAMPLES];
};

#endif

