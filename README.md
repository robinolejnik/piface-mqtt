piface-mqtt
===========
This software is an interface between the PiFace-addon-board for RaspberryPi and a MQTT-Broker.
It publishes changes of the PiFace-inputs to MQTT. Likewise the PiFace-outputs are controllable via MQTT.
In this first version the software connects only to a broker on localhost.

### How to use ###

Input changes are published to topic `piface/input/<nr. of input (0-7)>`. Payload can be 0 or 1.
The payload is 1 when the input is low(button pressed) or 0 if it is open(high).
The Software subscribes to `piface/output/+`. The output channel can be 0-7. If you publish to `piface/output/0` with payload 1 output0 on the PiFace will turned on. Payload 0 will turn it off.
