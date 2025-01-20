
default:
	idf.py build

program:
	idf.py -p /dev/ttyUSB0 flash

setup:
	idf.py set-target esp32c3
	idf.py menuconfig

clean:
	idf.py fullclean
	cd uinput && clean
	cd test_joystick && clean

