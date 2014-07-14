#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpiface-1.0/pfio.h> 
#include <mosquitto.h>

char *host = "localhost";
char *on = "1";
char *off = "0";

void my_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message) {
	unsigned char channel;
	sscanf(message->topic, "piface/output/%u", &channel);
	if(channel<8 && message->payloadlen==1) {
		if(strncmp(message->payload, on, 1)==0)
			pfio_digital_write(channel, 1);
		if(strncmp(message->payload, off, 1)==0)
			pfio_digital_write(channel, 0);
	}
}

int main(int argc, char*argv[]) {
	
	struct mosquitto *mosq = NULL;
	char *buffer = malloc(16);
	unsigned char i;
	unsigned char input_status, input_status_old;
	
	pfio_init();
	mosquitto_lib_init();
	mosq = mosquitto_new(NULL, true, NULL);
	if(!mosq) {
		fprintf(stderr, "MQTT Error: Out of memory.\n");
		return 1;
	}
	mosquitto_message_callback_set(mosq, my_message_callback);
	if(mosquitto_connect_async(mosq, host, 1883, 60)) {
		fprintf(stderr, "MQTT Error: Unable to connect.\n");
		return 1;
	}
	mosquitto_subscribe(mosq, NULL, "piface/output/+", 1);
	mosquitto_loop_start(mosq);

	while(1) {
		input_status = pfio_read_input();
		for(i=0;i<8;i++) {
			if((input_status_old^input_status) & (1<<i)) {
				sprintf(buffer, "piface/input/%u", i);
				if(input_status&(1<<i))
					mosquitto_publish(mosq, NULL, buffer, 2, on, 1, 0);
				else
					mosquitto_publish(mosq, NULL, buffer, 2, off, 1, 0);
			}
		}
		input_status_old = input_status;
	}
}
