
CFLAGS=-Wall -g
#CFLAGS=-Wall -O3 -mcpu=arm1176jzf-s -mfpu=vfp -mfloat-abi=hard -ffast-math
LDFLAGS=-lm -lasound

default:
	g++ -c AudioDevice.cxx $(CFLAGS)
	g++ -c AudioInput.cxx $(CFLAGS)
	g++ -c WAV.cxx $(CFLAGS)
	g++ -c Midi.cxx $(CFLAGS)
	g++ -c MidiDevice.cxx $(CFLAGS)
	g++ -c MidiFile.cxx $(CFLAGS)
	g++ -c NoteMap.cxx $(CFLAGS)
	g++ -c DCT.cxx $(CFLAGS)
	g++ -o test_wav test_wav.cxx AudioDevice.o AudioInput.o DCT.o \
	       Midi.o MidiDevice.o MidiFile.o NoteMap.o WAV.o \
	   $(CFLAGS) $(LDFLAGS)

clean:
	@rm -f test_wav *.o
	@echo "Clean!"


