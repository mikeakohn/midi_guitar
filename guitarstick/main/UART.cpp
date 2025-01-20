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
#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_log.h"

#include "UART.h"

UART::UART() :
  TAG("UART"),
  uart_port(UART_NUM_1),
  buffer_size(1024)
{
}

UART::~UART()
{
}

void UART::init()
{
  // Configure parameters of an UART driver,
  // communication pins and install the driver.
  uart_config_t uart_config = { };

  uart_config.baud_rate  = 9600;
  uart_config.data_bits  = UART_DATA_8_BITS;
  uart_config.parity     = UART_PARITY_DISABLE;
  uart_config.stop_bits  = UART_STOP_BITS_1;
  uart_config.flow_ctrl  = UART_HW_FLOWCTRL_DISABLE;
  uart_config.source_clk = UART_SCLK_DEFAULT;

  int intr_alloc_flags = 0;

#if CONFIG_UART_ISR_IN_IRAM
    intr_alloc_flags = ESP_INTR_FLAG_IRAM;
#endif

    ESP_ERROR_CHECK(
      uart_driver_install(
        uart_port,
        buffer_size * 2,
        0,
        0,
        NULL,
        intr_alloc_flags));

    ESP_ERROR_CHECK(uart_param_config(uart_port, &uart_config));

    // TX: GPIO 6.
    // RX: GPIO 7.
    ESP_ERROR_CHECK(
      uart_set_pin(
        uart_port,
        6,
        7,
        UART_PIN_NO_CHANGE,
        UART_PIN_NO_CHANGE));
}

void UART::send_char(uint8_t value)
{
  uart_write_bytes(uart_port, (const char *)&value, 1);
}

int UART::read_data(uint8_t *buffer, int length)
{
  return uart_read_bytes(uart_port, buffer, length, 20 / portTICK_PERIOD_MS);
}

