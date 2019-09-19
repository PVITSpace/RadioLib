/*
   RadioLib SIM800 Send SMS Example
*/

// include the library
#include <RadioLib.h>

// SIM800 has the following connections:
// TX pin: 9
// RX pin: 8
SIM800 gsm = new Module(9, 8);

// or using RadioShield
// https://github.com/jgromes/RadioShield
//SIM800 gsm = RadioShield.ModuleA;

void setup() {
  Serial.begin(9600);

  // initialize SIM800 with default settings
  Serial.print(F("[SIM800] Initializing ... "));
  // baudrate:  9600 baud
  int state = gsm.begin(9600);
  if (state == ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }
}

void loop() {
  // send SMS to number 0123456789
  Serial.print(F("[SIM800] Sending SMS ... "));
  int state = gsm.sendSMS("774313955", "Hello World!");
  if (state == ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
  }

  // wait 10 seconds before sending again
  //delay(10000);
  while(true);
}
