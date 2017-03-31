#include <Arduino.h>
#include <Wire.h>
#include <datachan.h>

void setup() {
	delay(1000);

}

void loop() {

	digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);   
  //USB_main();

}
