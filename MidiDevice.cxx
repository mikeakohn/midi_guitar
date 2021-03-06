/**
 *  midi_guitar
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2015 by Michael Kohn
 *
 * MidiDevice.cxx: Module for writing to a MIDI device.
 *
 * Notes:
 *   aplay -l
 *   aonnect -o
 *   /etc/init.d/timidity start
 *   modprobe snd-virmidi
 *   aconnect 20:0 128:0
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "MidiDevice.h"

MidiDevice::MidiDevice(const char *device)
{
#ifdef ALSA
  status = snd_rawmidi_open(&read_handle, &write_handle, device, 0);

  if (status != 0)
  {
    printf("MidiDevice status=%s\n", snd_strerror(status));
  }
#endif
}

MidiDevice::~MidiDevice()
{
#ifdef ALSA
  snd_rawmidi_close(read_handle);
  snd_rawmidi_close(write_handle);
#endif
}

int MidiDevice::init()
{
  if (status < 0)
  {
    return -1;
  }

  return 0;
}

void MidiDevice::write_midi_note_on(Note *note)
{
#ifdef ALSA
  uint8_t command[3];

  command[0] = 0x90 + note->midi_channel;
  command[1] = note->pitch;
  command[2] = note->volume;

  if (snd_rawmidi_write(write_handle, command, 3) < 0)
  {
    printf("ERROR snd_rawmidi_write()\n");
  }
#endif
}

void MidiDevice::write_midi_note_off(Note *note)
{
#ifdef ALSA
  uint8_t command[3];

  command[0] = 0x80 + note->midi_channel;
  command[1] = note->pitch;
  command[2] = note->volume;

  if (snd_rawmidi_write(write_handle, command, 3) < 0)
  {
    printf("ERROR snd_rawmidi_write()\n");
  }
#endif
}

