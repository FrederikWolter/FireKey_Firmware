/*********************************************************************
* FIREKEY - PROJECT
* 
* Code snippet to test turining off the RX/TX LEDs.
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
