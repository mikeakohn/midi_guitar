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

#ifndef FREQ_CALC_H
#define FREQ_CALC_H

#include <stdint.h>

class FreqCalc
{
public:
  FreqCalc();
  ~FreqCalc();

  void set_threshold(int value)   { threshold   = value; }
  void set_sample_rate(int value) { sample_rate = value; }
  //void set_time_ms(int value)     { time_ms     = value; }

  int get_frequency(
    const int16_t *data,
    int sample_count,
    int ptr);

private:
  int threshold;
  int sample_rate;
  //int time_ms;

};

#endif

