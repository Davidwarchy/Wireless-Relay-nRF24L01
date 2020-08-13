//Code for Transmitter
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN         
const byte address[6] = "00001";     //Byte of array representing the address. This is the address where we will send the data. This should be same on the receiving side.

void setup() {
radio.begin();                  //Starting the Wireless communication
radio.setRetries(15,15);
radio.openWritingPipe(address); //Setting the address where we will send the data
radio.stopListening();          //This sets the module as transmitter
radio.setPALevel(RF24_PA_LOW);
}

void loop(){
const char text[] = "who!who!nettigo!is great";
radio.write(&text, sizeof(text));                  //Sending the message to receiver
digitalWrite(2,HIGH);
delay(20);
digitalWrite(2,LOW);

delay(1000);
}
