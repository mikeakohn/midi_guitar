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

#include "HID.h"

HID::HID()
{
}

HID::~HID()
{
}

void init()
{
  #define TUSB_DESC_TOTAL_LEN (TUD_CONFIG_DESC_LEN + CFG_TUD_HID * TUD_HID_DESC_LEN)

  static const uint8_t hid_configuration_descriptor[] =
  {
    // Configuration number, interface count, string index, total length,
    // attribute, power in mA.
    TUD_CONFIG_DESCRIPTOR(1, 1, 0, TUSB_DESC_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),

    // Interface number, string index, boot protocol, report descriptor len,
    // EP In address, size & polling interval.
    TUD_HID_DESCRIPTOR(0, 4, false, sizeof(hid_report_descriptor), 0x81, 16, 10),
  };
}

void send_char(char value)
{
}

// Invoked when received GET HID REPORT DESCRIPTOR request.
// Application return pointer to descriptor, whose contents must exist
// long enough for transfer to complete.
uint8_t const *tud_hid_descriptor_report_cb(uint8_t instance)
{
  // Use only one interface and one HID report descriptor, so instance
  // parameter can be ignored.
  return hid_report_descriptor;
}

// Invoked when received GET_REPORT control request.
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request.
uint16_t tud_hid_get_report_cb(
  uint8_t instance,
  uint8_t report_id,
  hid_report_type_t report_type,
  uint8_t *buffer,
  uint16_t reqlen)
{
  (void)instance;
  (void)report_id;
  (void)report_type;
  (void)buffer;
  (void)reqlen;

  return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 ).
void tud_hid_set_report_cb(
  uint8_t instance,
  uint8_t report_id,
  hid_report_type_t report_type,
  uint8_t const *buffer,
  uint16_t bufsize)
{
}

const char *HID::hid_string_descriptor[5] =
{
  (char[]){0x09, 0x04},    // 0: is supported language is English (0x0409)
  "TinyUSB",               // 1: Manufacturer
  "TinyUSB Device",        // 2: Product
  "123456",                // 3: Serials, should use chip ID
  "Example HID interface", // 4: HID
};

