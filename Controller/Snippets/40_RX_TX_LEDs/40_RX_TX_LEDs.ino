/*********************************************************************
* FIREKEY - PROJECT
* 
* Code snippet to test turining off the RX/TX LEDs.
* 
* See:
* - https://arduino.stackexchange.com/questions/63446/arduino-pro-micro-switch-off-leds
* - https://github.com/arduino/ArduinoCore-avr/blob/master/variants/micro/pins_arduino.h (does not work)
*
* Requried libs:
* - None
*********************************************************************/

void setup() {
  pinMode(LED_BUILTIN_TX,INPUT);
  pinMode(LED_BUILTIN_RX,INPUT);

  Serial.begin(9600);
}

void loop() {
  Serial.println("I am alive");
  delay(500);
}
