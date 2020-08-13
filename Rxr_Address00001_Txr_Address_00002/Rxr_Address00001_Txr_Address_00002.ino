//Code for Receiver
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
const byte address1[6] = "00001";
const byte address2[6] = "00002";
float threshold = 2.5;
float reading = 0;
float command = 0;

void setup() {
  while (!Serial);
    Serial.begin(9600);
  
  radio.begin();

  radio.setRetries(15,15);
  radio.openWritingPipe(address2); //Setting the address where we will send the data
  radio.setPALevel(RF24_PA_LOW);
}


void loop() {
  radio.openReadingPipe(0, address1);   //Setting the address at which we will receive the data
  radio.startListening();              //This sets the module as receiver
  if (radio.available()) {
    Serial.println("Radio available");
      char text[32] = {0};                 //Saving the incoming data
      radio.read(&text, sizeof(text));    //Reading the data
      Serial.println(text);
      reading = atof(text);
        if(reading > threshold){
          command = 0.0;  //1 for open
          Serial.println("    value > threshold");
        }else{
          command = 1.0; //0 for close
          Serial.println("    value < threshold");
        }

      digitalWrite(2, HIGH);
      delay(20);
      digitalWrite(2, LOW); 
      
      //send command----------------------------------------------------------------------------
      dtostrf(command,4,4,text);

      radio.write(&text, sizeof(text));                  //Sending the message to receiver
      radio.stopListening();
      radio.write(&text, sizeof(text));   
      //end sending command----------------------------------------------------------------------
      }

      //to detect that the loop is still alive in receiver
      digitalWrite(3, HIGH);   
      delay(100); 
      digitalWrite(3, LOW);    
 

}
