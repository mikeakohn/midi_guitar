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
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>
#include <time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/gptimer.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_log.h"
#include "esp_task_wdt.h"
#include "sdkconfig.h"

//#include "Audio.h"
#include "DCT.h"
#include "Notes.h"
#include "Processor.h"
#include "UART.h"

static const char *TAG = "guitarstick";

static void run()
{
  Processor processor;

  processor.init();

  ESP_LOGI(TAG, "Initializing UART...");

  UART uart;

  uart.init();
  uart.send_char('A');

  ESP_LOGI(TAG, "Initialized and ready to process...");

#if 0
  float *data = processor.get_data();
  const int sample_rate = processor.get_sample_rate();
  const int sample_count = processor.get_sample_count();
  const int threshold = processor.get_threshold();
#endif

  //DCT dct;
  //dct.init(sample_rate, sample_count);

  Notes notes;
  int last_midi = 0;
  int last_sent = 0;

  int center_count = 0;

  while (true)
  {
    usleep(10000);

    int freq = processor.get_freq();
    int midi = notes.get_midi(freq);

    if (midi == 0)
    {
      center_count += 1;

      if (center_count < 20) { continue; }
    }
      else
    {
      center_count = 0;
    }

#if 0
    if (freq != 0)
    {
      ESP_LOGI(TAG, "freq=%d %s %d",
        freq,
        notes.get_name(freq),
        midi);
    }
#endif

    if (last_midi == midi)
    {
      if (last_sent != midi)
      {
        switch (midi)
        {
          case 0:
            //ESP_LOGI(TAG, "CENTER");
            uart.send_char(0xff);
            uart.send_char(63);
            uart.send_char(63);
            uart.send_char(0x0);
            break;
          case 70:
            ESP_LOGI(TAG, "UP");
            uart.send_char(0xff);
            uart.send_char(63);
            uart.send_char(0);
            uart.send_char(0x0);
            break;
          case 67:
            ESP_LOGI(TAG, "RIGHT");
            uart.send_char(0xff);
            uart.send_char(126);
            uart.send_char(63);
            uart.send_char(0x0);
            break;
          case 65:
            ESP_LOGI(TAG, "LEFT");
            uart.send_char(0xff);
            uart.send_char(0);
            uart.send_char(63);
            uart.send_char(0x0);
            break;
          case 61:
            ESP_LOGI(TAG, "DOWN");
            uart.send_char(0xff);
            uart.send_char(63);
            uart.send_char(126);
            uart.send_char(0x0);
            break;
          default:
            ESP_LOGI(TAG, "- %d", midi);
            uart.send_char(0xff);
            uart.send_char(63);
            uart.send_char(63);
            uart.send_char(0x0);
            break;
        }

        last_sent = midi;
      }
    }

    last_midi = midi;

#if 0
    if (processor.has_data() == false) { continue; }

    // Tuned down 1/2 step.
    // 392 = G  (3rd string 13th fret) right.
    // 349 = F  (3rd string 11th fret) left.
    // 466 = Bb (2rd string 12th fret) up.
    // 278 = Db (4rd string 12th fret) down.
    // 698 = F  (1st string 14th fret) fire.

    int ptr = processor.get_ptr();

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

ESP_LOGI(TAG, "%d %d %d %d %d",
  value392,
  value349,
  value466,
  value278,
  value698);

    if (p != 0)
    {
      uart.send_char(p);
    }

    processor.done_with_data();
#endif
  }
}

static void show_chip_info()
{
  // Print chip information.
  esp_chip_info_t chip_info;
  uint32_t flash_size;

  esp_chip_info(&chip_info);

  ESP_LOGI(
    TAG, "This is %s chip with %d CPU core(s), WiFi%s%s, ",
    CONFIG_IDF_TARGET,
    chip_info.cores,
   (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
   (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

  int major_rev = chip_info.revision / 100;
  int minor_rev = chip_info.revision % 100;
  ESP_LOGI(TAG, "silicon revision v%d.%d, ", major_rev, minor_rev);

  if (esp_flash_get_size(NULL, &flash_size) != ESP_OK)
  {
    ESP_LOGI(TAG, "Get flash size failed");
    return;
  }

  ESP_LOGI(
    TAG, "%luMB %s flash", flash_size / (1024 * 1024),
    (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

  ESP_LOGI(TAG, "Minimum free heap size: %ld bytes",
    esp_get_minimum_free_heap_size());
}

#ifdef __cplusplus
extern "C"
{
#endif

void app_main(void)
{
  show_chip_info();

  for (int i = 4; i >= 0; i--)
  {
    ESP_LOGI(TAG, "Restarting in %d seconds...", i);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }

  // Turn off watchdog.
  esp_task_wdt_deinit();

  run();

  ESP_LOGI(TAG, "Restarting now.");
  fflush(stdout);

  esp_restart();
}

#ifdef __cplusplus
}
#endif

