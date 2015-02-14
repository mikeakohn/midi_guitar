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
}

void DCT::compute_dct_ii(FLOAT *buffer, FLOAT *dct, int N)
{
  FLOAT sum;
  int n,k;
  FLOAT w,w0,wk;
  FLOAT n_times_2 = (FLOAT)2 * (FLOAT)N;
  FLOAT k_div_n_times_2;

  w0 = sqrt(1.0 / (FLOAT)N);
  wk = sqrt(2.0 / (FLOAT)N);

  for (k = 0; k < DCT_LEN; k++)
  {
    sum = 0;

    k_div_n_times_2 = (FLOAT)k / n_times_2;

    for (n = 0; n < N; n++)
    {
      //sum += buffer[n] * cos(M_PI * ((FLOAT)2 * (FLOAT)n + (FLOAT)1) * (FLOAT)k / ((FLOAT)2 * (FLOAT)N));
      sum += buffer[n] * cos(pi_2_n_1[n] * k_div_n_times_2);
    }

    w = (k == 0) ? w0 : wk;

    dct[k] = w * sum;
  }
}
