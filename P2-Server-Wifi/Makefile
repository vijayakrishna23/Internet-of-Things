
DEVICE_NAME		?= first_soldier

all: firmware.bin

# generate a WAV file of a 440Hz tone 
wave.wav:
	python3 wav_generator.py -sr 16000  -f 440  -d 1.0  --wav wave.wav

# generate a CPP Header file given a WAV file
wave.h: wave.wav
	python3 wav_to_header.py -wav elephant.wav -cpp wave.h

# builds the Argon firmeware given the CPP files
firmware.bin: p2.cpp wave.h 
	particle compile argon p2.cpp wave.h --saveTo firmware.bin

# flashes the Argon device over the web
web: 
	particle flash ${DEVICE_NAME} firmware.bin 

# flashes the Argon device over USB
serial: firmware.bin
	particle flash --serial firmware.bin

# run a test suite on the python codes
tests:
	make -C wave_test all

.PHONY: clean
clean:
	rm -rf wave.wav wave.h  firmware.bin
