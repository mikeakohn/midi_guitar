
CFLAGS=-Wall -O3
#CFLAGS=-Wall -O3 -mcpu=arm1176jzf-s -mfpu=vfp -mfloat-abi=hard -ffast-math
LDFLAGS=-lm

default:
	g++ -c AudioDevice.cxx $(CFLAGS)
	g++ -c AudioInput.cxx $(CFLAGS)
	g++ -c WAV.cxx $(CFLAGS)
	g++ -c MidiFile.cxx $(CFLAGS)
	g++ -c MidiMap.cxx $(CFLAGS)
	g++ -c DCT.cxx $(CFLAGS)
	g++ -o test_wav test_wav.cxx AudioDevice.o AudioInput.o DCT.o \
	       MidiFile.o MidiMap.o WAV.o \
	   $(CFLAGS) $(LDFLAGS)

clean:
	@rm -f test_wav *.o
	@echo "Clean!"


