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

#ifndef UART_H
#define UART_H

#include <stdint.h>

#include "driver/uart.h"

class UART
{
public:
  UART();
  ~UART();

  void init();
  void send_char(uint8_t value);
  int read_data(uint8_t *buffer, int length);

private:
  const char *TAG;
  const uart_port_t uart_port;
  const int buffer_size;

};

#endif

