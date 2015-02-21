
OSTYPE=$(shell uname -s)
ifeq ($(OSTYPE),Linux)
ALSA=-lasound
endif
CFLAGS=-Wall -O3 -g
#CFLAGS=-Wall -O3 -mcpu=arm1176jzf-s -mfpu=vfp -mfloat-abi=hard -ffast-math
LDFLAGS=-lm $(ALSA)
CPP=g++

OBJECTS=AudioDevice.o AudioInput.o DCT.o Midi.o MidiDevice.o MidiFile.o NoteMap.o WAV.o 

default: $(OBJECTS)
	$(CPP) -o test_wav test_wav.cxx \
	   $(OBJECTS) \
	   $(CFLAGS) $(LDFLAGS)

int: $(OBJECTS)
	$(CPP) -o test_wav_int test_wav_int.cxx \
	   $(OBJECTS) \
	   $(CFLAGS) $(LDFLAGS)

%.o: %.cxx %.h float.h
	$(CPP) -c $*.cxx $(CFLAGS)

clean:
	@rm -f test_wav *.o
	@echo "Clean!"


