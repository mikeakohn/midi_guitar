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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "driver/gptimer.h"
#include "esp_log.h"
#include "soc/gpio_reg.h"

#include "Processor.h"

Processor::Processor() :
  TAG          { "Processor" },
  //debug        { 0      },
  is_ready     { false  },
  sample_rate  { 20000  },
  sample_count { 6000   },
  threshold    { 400    },
  ptr          { 0      },
  freq         { 0      }
{
  //const int length = sample_count * sizeof(float);
  //data = (float *)malloc(length);

  const int length = sample_count * sizeof(int16_t);
  data = (int16_t *)malloc(length);

  if (data == NULL)
  {
    ESP_LOGI(TAG, "Could not allocate data %s:%d.", __FILE__, __LINE__);
  }

  memset(data, 0, length);
}

Processor::~Processor()
{
  free(data);
}

void Processor::init()
{
  init_gpio();

  audio.init();
  //dct.init();
  //uart.init();
  //uart.send_char('A');

  // Interrupt every 20,0000 times a second.
  init_timer(1000000 / sample_rate, timer_callback);
  ESP_ERROR_CHECK(gptimer_start(gptimer));

  ESP_LOGI(TAG, "Processor::init() complete");
}

void Processor::init_gpio()
{
  gpio_config_t io_conf = { };

  io_conf.intr_type = GPIO_INTR_DISABLE;
  io_conf.mode = GPIO_MODE_OUTPUT;
  io_conf.pin_bit_mask = (1ULL << GPIO_NUM_9);
  io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  io_conf.pull_up_en = GPIO_PULLUP_DISABLE;

  gpio_config(&io_conf);
}

void Processor::init_timer(
  int resolution,
  gptimer_alarm_cb_t timer_callback_func)
{
  gptimer_config_t timer_config = { };
  timer_config.clk_src = GPTIMER_CLK_SRC_DEFAULT;
  timer_config.direction = GPTIMER_COUNT_UP;
  timer_config.resolution_hz = 1000000;

  ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &gptimer));

  timer_user_data.interrupt_count = 0;
  timer_user_data.count_value = 0;
  timer_user_data.processor = this;

  gptimer_event_callbacks_t cbs =
  {
    .on_alarm = timer_callback_func,
  };

  gptimer_register_event_callbacks(gptimer, &cbs, &timer_user_data);
  gptimer_enable(gptimer);

  gptimer_alarm_config_t alarm_config = { };
  //.alarm_count = 1000000, // period = 1s
  alarm_config.alarm_count = resolution;
  alarm_config.flags.auto_reload_on_alarm = true;

  ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &alarm_config));
}

void Processor::get_sample()
{
  //float sample = audio.get_value();
  int16_t sample = audio.get_value();

  data[ptr++] = sample;
  if (ptr >= sample_count) { ptr = 0; }

  if (ptr == 0 || ptr == (sample_count / 2))
  {
    freq = freq_calc.get_frequency(data, sample_count, ptr);

#if 0
    is_ready = true;
    // Tuned down 1/2 step.
    // 392 = G  (3rd string 13th fret) right.
    // 349 = F  (3rd string 11th fret) left.
    // 466 = Bb (2rd string 12th fret) up.
    // 278 = Db (4rd string 12th fret) down.
    // 698 = F  (1st string 14th fret) fire.

    int value392 = dct.calc(data, sample_count, sample_rate, ptr, 392);
    int value349 = dct.calc(data, sample_count, sample_rate, ptr, 349);
    int value466 = dct.calc(data, sample_count, sample_rate, ptr, 466);
    int value278 = dct.calc(data, sample_count, sample_rate, ptr, 278);
    int value698 = dct.calc(data, sample_count, sample_rate, ptr, 698);

    int p = 0;

    if (value392 > threshold) { p = 'r'; }
    if (value349 > threshold) { p = 'l'; }
    if (value466 > threshold) { p = 'u'; }
    if (value278 > threshold) { p = 'd'; }
    if (value698 > threshold) { p = 'f'; }
#endif

#if 0

    if (value > threshold) { p = 1; }

    gpio_set_level(GPIO_NUM_9, p);
#endif

    //debug = value;
    //debug = p;
  }
}

bool IRAM_ATTR Processor::timer_callback(
  gptimer_handle_t timer,
  const gptimer_alarm_event_data_t *edata,
  void *args)
{
  TimerUserData *timer_user_data = (TimerUserData *)args;
  timer_user_data->interrupt_count++;
  timer_user_data->count_value = edata->count_value;

  //gpio_set_level(GPIO_NUM_9, timer_user_data->interrupt_count & 1);
  //gpio_set_level(GPIO_NUM_9, 0);

  Processor *processor = (Processor *)timer_user_data->processor;
  processor->get_sample();

  return pdFALSE;
}

