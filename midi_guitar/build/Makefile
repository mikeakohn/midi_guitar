
OSTYPE=$(shell uname -s)
ifeq ($(OSTYPE),Linux)
ALSA=-lasound
OTHEROBJ= \
  AudioDevice.o \
  AudioInput.o \
  Display.o
else
OTHER_OBJECTS=
endif
SDL=$(shell sdl-config --cflags --libs)
CFLAGS=-Wall -O3 -g
#CFLAGS=-Wall -O3 -mcpu=arm1176jzf-s -mfpu=vfp -mfloat-abi=hard -ffast-math
LDFLAGS=-lm $(ALSA) $(SDL)
CPP=g++
VPATH=../src

OBJECTS= \
  DCT.o \
  Midi.o \
  MidiDevice.o \
  MidiFile.o \
  NoteMap.o \
  WAV.o 

default: $(OBJECTS) $(OTHER_OBJECTS)
	$(CPP) -o ../midi_guitar ../src/midi_guitar.cxx \
	   $(OBJECTS) $(OTHER_OBJECTS) \
	   $(CFLAGS) $(LDFLAGS)

int: $(OBJECTS) $(OTHER_OBJECTS)
	$(CPP) -o ../midi_guitar_int ../src/midi_guitar_int.cxx \
	   $(OBJECTS) $(OTHER_OBJECTS) \
	   $(CFLAGS) $(LDFLAGS)

%.o: %.cxx %.h float.h
	$(CPP) -c $< -o $*.o $(CFLAGS)

clean:
	@rm -f midi_guitar midi_guitar_int *.o
	@echo "Clean!"

