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

#ifndef AUDIO_H
#define AUDIO_H

#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

class Audio
{
public:
  Audio();
  ~Audio();

  void init();
  void uninit();
  //float get_value();
  int16_t get_value();

private:
  bool calibrate(adc_unit_t unit, adc_atten_t atten);

  const char *TAG;
  adc_oneshot_unit_handle_t adc_handle;
  adc_cali_handle_t calibrate_handle;
  adc_channel_t channel;

};

#endif

