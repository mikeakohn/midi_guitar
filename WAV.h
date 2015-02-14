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

#ifndef _WAV_H
#define _WAV_H

#include <stdint.h>

#include "float.h"

class WAV
{
public:
  WAV(FILE *in);
  ~WAV() { }
  int read_headers();
  int read_data(FLOAT *samples, int count);
  int get_sample_rate() { return fmt_chunk.sample_rate; }

private:
  FILE *in;
  int read_count;
  int data_length;

  struct fmt_chunk_t
  {
    uint16_t format_type;
    uint16_t channel_numbers;
    uint32_t sample_rate;
    uint32_t bytes_per_second;
    uint16_t bytes_per_sample;
    uint16_t bits_per_sample;
  } fmt_chunk;

  int read_chars(char *s, int n);
  uint32_t read_int32();
  uint16_t read_int16();

  int parse_header();
  int parse_fmt_chunk();
  int parse_data_chunk();
};

#endif

