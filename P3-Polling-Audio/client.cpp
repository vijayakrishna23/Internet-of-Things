/*********************************************
* Created by: Tejesh Kalicharan, Nimmagadda
* tnimmag@iu.edu id:2000432930
* Date: 7th Feb, 2019
*********************************************/

#include "application.h"

// FIXME
IPAddress remoteIP(18,218,51,247);
uint32_t remote_port = 44444;

UDP udp;

float val[16000];

int led = D7;

int read_pin = A4;

void setup() {

  Serial.begin(9600);

  // wait up to 10 seconds for USB host to connect
  // requires firmware >= 0.5.3
  //waitFor(Serial.isConnected, 10000);
  Serial.printf("Connected\n");

  //Needs to be here for UDP to work correctly
  udp.begin(8888);
  pinMode(led, OUTPUT);
}

void loop(){

	Serial.printf("Sending Data\n"); 
	
  	if( WAVE_SAMPLE_RATE != 16000)
		Serial.printf("Warning:  Incorrect sampling rate!\n");
	int i = 0;
	for(i=0; i<16000; i++)
	{
		val[i] = ((analogRead(read_pin)*3.3)/4095);
		delayMicroseconds(50);
	}

	
	//FIXME:  your code here! 

	int rxError = 0;
	char message[128];
	
	udp.sendPacket((byte*)val, sizeof(val), remoteIP, remote_port);

	int count = udp.receivePacket((byte*)message, 127);
	if (count >= 0 && count < 128) {
	  message[count] = 0;
	  rxError = 0;
	} else if (count < -1) {
	  rxError = count;
  // need to re-initialize on error
	  udp.begin(8888);
	}
	if (!rxError) {
	  Serial.printf (message);
	}	
	// Checks for "Yes440Hz" and turns D7 led 'on' for 15 secs. 
	if(message[0] == 'Y')
	{
		digitalWrite(led, HIGH);
		delay(15000);
		digitalWrite(led,LOW);
		delay(15000);
	}
	delay(100); //0.1 second delay

}
