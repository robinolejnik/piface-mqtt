all: piface-mqtt.c
	gcc -L/usr/local/lib/ -lpiface-1.0 -lmosquitto -o piface-mqtt piface-mqtt.c
