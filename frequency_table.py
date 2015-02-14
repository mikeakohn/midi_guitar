#!/usr/bin/env python

import sys

# C0  - 12
# C#0 - 13
# D   - 14
# D#  - 15
# E   - 16
# F   - 17
# F#  - 18
# G   - 19
# G#  - 20

# A0  - 21
# A#0 - 22
# B0  - 23

def midi_notes():
  print "double midi_notes[] ="
  print "{"

  for i in range(0, 12):
    print "  0,  // " + str(i)

  i = 12

  fp = open("frequency_table.txt", "rb")

  for line in fp:
    tokens = line.strip().split()
    if len(tokens) != 3: print "ERROR"; sys.exit(-1)
    print "  " + tokens[1] + ",  // " + str(i) + " " + tokens[0]
    i += 1

  print "};"
  print

  fp.close()

def midi_names():
  print "char *midi_names[] ="
  print "{"

  for i in range(0, 12):
    print "  \"??\",  // " + str(i)

  i = 12

  fp = open("frequency_table.txt", "rb")

  for line in fp:
    tokens = line.strip().split()
    if len(tokens) != 3: print "ERROR"; sys.exit(-1)
    print "  \"" + tokens[0] + "\",  // " + str(i) + " " + tokens[1]
    i += 1

  print "};"
  print

  fp.close()

# ----------------------------- fold here -----------------------------

midi_notes()
midi_names()


