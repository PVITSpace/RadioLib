#define RADIOLIB_GODMODE
#include <RadioLib.h>

SX1268 lora = new Module(10, 2, 3, 9);

LoRaWANNode node(&lora, &EU868);

void setup() {
  Serial.begin(9600);
  Serial.println(node._band->uplinkDefault[0].freqStart);
}

void loop() {
  // put your main code here, to run repeatedly:

}
