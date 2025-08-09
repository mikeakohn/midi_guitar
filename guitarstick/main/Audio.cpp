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

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
//#include "soc/soc_caps.h"
#include "esp_log.h"

#include "Audio.h"

Audio::Audio() :
  TAG              { "Audio" },
  adc_handle       { nullptr },
  calibrate_handle { nullptr },
  channel          { ADC_CHANNEL_3 }
{
}

Audio::~Audio()
{
}

void Audio::init()
{
  // Init.
  adc_oneshot_unit_init_cfg_t init_config = { };
  init_config.unit_id = ADC_UNIT_1,

  //ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &adc_handle));
  adc_oneshot_new_unit(&init_config, &adc_handle);

  // Config.
  adc_oneshot_chan_cfg_t config = { };
  config.bitwidth = ADC_BITWIDTH_DEFAULT;
  config.atten = ADC_ATTEN_DB_12;

  ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_handle, channel, &config));

  // Calibrate reference voltage.
  calibrate(ADC_UNIT_1, ADC_ATTEN_DB_12);

  ESP_LOGI(TAG, "Audio::init() done.");
}

void Audio::uninit()
{
  ESP_ERROR_CHECK(adc_oneshot_del_unit(adc_handle));

  // This is odd. In the calibrate() function (from the ESP32 docs) it
  // seems both curve and line could be supported on the same chip.
  // This function has "elif".
#if ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
  ESP_LOGI(TAG, "Deregister %s calibration scheme Curve Fitting");
  ESP_ERROR_CHECK(adc_cali_delete_scheme_curve_fitting(calibrate_handle));
#elif ADC_CALI_SCHEME_LINE_FITTING_SUPPORTED
  ESP_LOGI(TAG, "Deregister %s calibration scheme Line Fitting");
  ESP_ERROR_CHECK(adc_cali_delete_scheme_line_fitting(calibrate_handle));
#endif
}

int16_t Audio::get_value()
{
  int adc_value;
  int voltage;

  ESP_ERROR_CHECK(adc_oneshot_read(adc_handle, channel, &adc_value));

  //ESP_LOGI(TAG, "ADC Channel[%d] Raw Data: %d", ADC_UNIT_1 + 1, channel, adc_value);

  if (calibrate_handle != nullptr)
  {
    ESP_ERROR_CHECK(adc_cali_raw_to_voltage(calibrate_handle, adc_value, &voltage));

    //ESP_LOGI(TAG, "ADC%d Channel[%d] Cali Voltage: %d mV", ADC_UNIT_1 + 1, channel, voltage);
  }   

  // This circuit puts ~0.55mv on the ADC pin. It seems to sit closer to
  // 0.560mv so subtract 560.

  //float value = float(voltage - 560) / 560.0;
  //if (value < -1.0) { value = -1.0; }
  //if (value >  1.0) { value =  1.0; }

  int16_t value = voltage - 560;

  return value;
}

bool Audio::calibrate(adc_unit_t unit, adc_atten_t atten)
{
  esp_err_t ret = ESP_FAIL;
  bool calibrated = false;

#if ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
  if (!calibrated)
  {
    ESP_LOGI(TAG, "Calibration scheme version is Curve Fitting");

    adc_cali_curve_fitting_config_t cali_config =
    {
      .unit_id = unit,
      .chan = channel,
      .atten = atten,
      .bitwidth = ADC_BITWIDTH_DEFAULT,
    };

    ret = adc_cali_create_scheme_curve_fitting(&cali_config, &calibrate_handle);

    if (ret == ESP_OK)
    {
      calibrated = true;
    }
  }
#endif

#if ADC_CALI_SCHEME_LINE_FITTING_SUPPORTED
  if (!calibrated)
  {
    ESP_LOGI(TAG, "Calibration scheme version is Line Fitting");

    adc_cali_line_fitting_config_t cali_config = { };
    cali_config.unit_id = unit,
    cali_config.atten = atten,
    cali_config.bitwidth = ADC_BITWIDTH_DEFAULT,

    ret = adc_cali_create_scheme_line_fitting(&cali_config, &calibrate_handle);

    if (ret == ESP_OK)
    {
      calibrated = true;
    }
  }
#endif

  if (ret == ESP_OK)
  {
    ESP_LOGI(TAG, "Calibration Success");
  }
    else if (ret == ESP_ERR_NOT_SUPPORTED || !calibrated)
  {
    ESP_LOGW(TAG, "eFuse not burnt, skip software calibration");
  }
    else
  {
    ESP_LOGE(TAG, "Invalid arg or no memory");
  }

  return calibrated;
}

