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
#define SAMPLES 8192
#define DCT_LEN 3151
#define VOLUME_THRESHOLD 2

class DCT
{
public:
  DCT();
  ~DCT() { }

  void compute_dct_ii(FLOAT *buffer, FLOAT *dct, int N);
  void init_cos_lookup();
  void compute_dct_ii_cos_lookup(FLOAT *buffer, FLOAT *dct, int N);

private:
  FLOAT *cos_lookup;
  FLOAT w0,wk;
  FLOAT pi_2_n_1[SAMPLES];
};

#endif

