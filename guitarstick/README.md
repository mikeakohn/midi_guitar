
GuitarStick
===========

This is a proof of concept that allows playing old video games
with a guitar.

This software takes input from a guitar into an ADC of an
ESP32 microcontroller which converts the sound samples into MIDI
notes. The MIDI notes are pushed over UART serial into a PC
which translates the MIDI notes into joystick presses.

The flow of the data looks like this:

         Guitar
           |
           |
         ESP32
           |
           |
      UART to USB 
           |
           |
    uinput/joydev.cpp
           |
           |
      Linux Kernel
           |
           |
         MAME

The uinput/joydev.cpp is just a simple program that listens to
/dev/ttyUSB0 and translates the MIDI notes to joystick directions
and pushes commands into the kernel using the uinput interface.
This allows a userland program to be a joystick driver without
having to create a kernel module.

To test to make sure the data from the guitar into the Linux kernel
looks correct, there is a program test_joystick/test_joystick.c
which reads from /dev/input/js0 and dumps the output as text to
the screen.

Guitar To MIDI
--------------

This started out doing DCT, which ended up being a little too slow.
Instead the code analyzes the digital data to count how many times
the signal goes above 0, blow zero, then back in a certain amount of
time to figure out the frequency.

Unlike DCT, this method cannot separate out a digital signal that has
multiple frequencies.

Building
--------

To install the devkit:

    https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/get-started/linux-macos-setup.html#get-started-linux-macos-first-steps

To set up the environment:

    . $HOME/esp/esp-idf/export.sh

There is a Makefile that has all the idf.py commands documented.

