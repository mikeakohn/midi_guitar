#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "WAV.h"

WAV::WAV(FILE *in) : read_count(0)
{
  this->in = in;
}

int WAV::read_headers()
{
  parse_header();
  parse_fmt_chunk();
  parse_data_chunk();

  return 0;
}

int WAV::read_data(FLOAT *samples, int count)
{
  int16_t sample;
  int ch;
  int n;

  //length = (fmt_chunk.bytes_per_sample == 2) ? data_length / 2 : data_length;
  //length = (length > count) ? count : length;

//printf("read_count=%d length=%d data_length=%d\n", read_count, length, data_length);

  if (fmt_chunk.bits_per_sample == 8)
  {
    read_count += count;
    if (read_count > data_length) { return -1; }

    for (n = 0; n < count; n++)
    {
      ch = getc(in);
      sample = ch - 128;
    }
  }
    else
  if (fmt_chunk.bits_per_sample == 16)
  {
    read_count += count * 2;
    if (read_count > data_length) { return -1; }

    for (n = 0; n < count; n++)
    {
      sample = read_int16();
      sample = (sample < -32767) ? -32767 : sample;
      samples[n] = (FLOAT)sample / 32767.0;
    }
  }
    else
  {
    printf("Unknown wav format.\n");
    return 0;
  }

  return 0;
}

int WAV::read_chars(char *s, int n)
{
  int t,ch;

  for (t = 0; t < n; t++)
  {
    ch = getc(in);
    if (ch == EOF) return -1;
    s[t] = ch;
  }
  s[t] = 0;

  return 0;
}

uint32_t WAV::read_int32()
{
unsigned int t;

  t = getc(in);
  t = (getc(in) << 8) | t;
  t = (getc(in) << 16) | t;
  t = (getc(in) << 24) | t;

  return t;
}

uint16_t WAV::read_int16()
{
  uint16_t t;

  t = getc(in);
  t = (getc(in) << 8) | t;

  return t;
}


int WAV::parse_header()
{
  int length;
  char fourcc[5];
  char riff_type[5];

  read_chars(fourcc, 4);
  if (strncmp(fourcc,"RIFF", 4) != 0)
  {
    printf("This file is not a RIFF/WAV\n");
    return -1;
  }

  length = read_int32();
  read_chars(riff_type, 4);

  printf("RIFF Header\n");
  printf("----------------------------\n");
  printf("          Length: %d\n", length);
  printf("            Type: %s\n", riff_type);
  printf("----------------------------\n");

  return 0;
}

int WAV::parse_fmt_chunk()
{
  char chunk_type[5];
  int length;

  read_chars(chunk_type, 4);
  length = read_int32();
  fmt_chunk.format_type = read_int16();
  fmt_chunk.channel_numbers = read_int16();
  fmt_chunk.sample_rate = read_int32();
  fmt_chunk.bytes_per_second = read_int32();
  fmt_chunk.bytes_per_sample = read_int16();
  fmt_chunk.bits_per_sample = read_int16();

  printf("FMT Chunk\n");
  printf("----------------------------\n");
  printf("      Chunk Type: %s\n", chunk_type);
  printf("          Length: %d\n", length);
  printf("     Format Type: ");
  if (fmt_chunk.format_type == 0)
  { printf("Mono\n"); }
    else
  if (fmt_chunk.format_type == 1)
  { printf("Stereo\n"); }
    else
  { printf("unknown\n"); }

  printf(" Channel Numbers: %d\n", fmt_chunk.channel_numbers);
  printf("     Sample Rate: %d\n", fmt_chunk.sample_rate);
  printf("Bytes Per Second: %d\n", fmt_chunk.bytes_per_second);
  printf("Bytes Per Sample: ");
  if (fmt_chunk.bytes_per_sample == 1)
  { printf("8 bit mono (%d)\n", fmt_chunk.bytes_per_sample); }
    else
  if (fmt_chunk.bytes_per_sample == 2)
  { printf("8 bit stereo or 16 bit mono (%d)\n", fmt_chunk.bytes_per_sample); }
    else
  if (fmt_chunk.bytes_per_sample == 4)
  { printf("16 bit stereo (%d)\n",fmt_chunk.bytes_per_sample); }
    else
  { printf("?\n"); }

  printf(" Bits Per Sample: %d\n", fmt_chunk.bits_per_sample);
  printf("----------------------------\n");

  return 0;
}

int WAV::parse_data_chunk()
{
  char riff_type[5];

  read_chars(riff_type, 4);
  data_length = read_int32();

  printf("Data Header\n");
  printf("----------------------------\n");
  printf("          Length: %d\n", data_length);
  printf("            Type: %s\n", riff_type);
  printf("----------------------------\n");

  if (strncmp(riff_type, "data", 4) != 0)
  {
    return -1;
  }

  return 0;
}

