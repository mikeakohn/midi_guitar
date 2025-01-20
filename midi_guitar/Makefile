
default:
	@+make -C build

int:
	@+make -C build int

%.o: %.cxx %.h float.h
	$(CPP) -c $*.cxx $(CFLAGS)

clean:
	@rm -f midi_guitar midi_guitar_int build/*.o
	@echo "Clean!"

