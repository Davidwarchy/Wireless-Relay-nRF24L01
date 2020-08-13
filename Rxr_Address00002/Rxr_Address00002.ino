//Code for Receiver
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00002";

void setup() {
  while (!Serial);
    Serial.begin(9600);
  
  radio.begin();
  radio.openReadingPipe(0, address);   //Setting the address at which we will receive the data
  radio.startListening();              //This sets the module as receiver
}

void loop() {
  if (radio.available()) {
    Serial.println("Radio available");
      char text[32] = {0};                 //Saving the incoming data
      radio.read(&text, sizeof(text));    //Reading the data
      Serial.println(text);
      
      digitalWrite(2, HIGH);
      delay(20);
      digitalWrite(2, LOW);
      
      }
      //to detect that the loop is still alive in receiver
      digitalWrite(3, HIGH);   
      delay(1000); 
      digitalWrite(3, LOW); 
}
