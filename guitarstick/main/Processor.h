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

#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdint.h>

#include "driver/gptimer.h"

#include "Audio.h"
//#include "DCT.h"
//#include "UART.h"
#include "FreqCalc.h"

class Processor
{
public:
  Processor();
  ~Processor();

  void init();
  //void done_with_data()   { is_ready = false;    }
  //float *get_data()       { return data;         }
  int16_t *get_data()     { return data;         }
  bool has_data()         { return is_ready;     }
  int get_sample_rate()   { return sample_rate;  }
  int get_sample_count()  { return sample_count; }
  int get_threshold()     { return threshold;    }
  int get_ptr()           { return ptr;          }

  int get_freq()          { return freq;         }

  //int debug;

private:
  struct TimerUserData
  {
    int interrupt_count;
    int count_value;
    Processor *processor;
  };

  void init_gpio();
  void init_timer(int resolution, gptimer_alarm_cb_t timer_callback_func);

  void get_sample();

  static bool IRAM_ATTR timer_callback(
    gptimer_handle_t timer,
    const gptimer_alarm_event_data_t *edata,
    void *args);

  Audio audio;
  //DCT dct;
  //UART uart;
  FreqCalc freq_calc;

  //float *data;
  int16_t *data;
  bool is_ready;
  const int sample_rate;
  const int sample_count;
  const int threshold;
  int ptr;

  int freq;

  gptimer_handle_t gptimer;
  TimerUserData timer_user_data;
};

#endif

