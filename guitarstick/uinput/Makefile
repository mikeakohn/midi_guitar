
CFLAGS=-O3 -Wall -g

default:
	g++ -c InputDev.cpp $(CFLAGS)
	g++ -c Serial.cpp $(CFLAGS)
	g++ -o joydev joydev.cpp InputDev.o Serial.o $(CFLAGS)

clean:
	@rm -f *.o joydev
	@echo "Clean!"

