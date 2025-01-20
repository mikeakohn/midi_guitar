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

#ifndef HID_H
#define HID_H

#include <stdint.h>

class HID
{
public:
  HID();
  ~HID();

  void init();
  void send_char(char value);

private:
  static uint8_t const *tud_hid_descriptor_report_cb(uint8_t instance);

  static uint16_t tud_hid_get_report_cb(
    uint8_t instance,
    uint8_t report_id,
    hid_report_type_t report_type,
    uint8_t *buffer,
    uint16_t reqlen);

  static void tud_hid_set_report_cb(
    uint8_t instance,
    uint8_t report_id,
    hid_report_type_t report_type,
    uint8_t const *buffer,
    uint16_t bufsize)

  static const char *hid_string_descriptor[5];
};

#endif

