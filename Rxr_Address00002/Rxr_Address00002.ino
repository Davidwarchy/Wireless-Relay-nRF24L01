//Code for Receiver
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00002";
int command = 0;
char state[32] = {0};

//servo setup
Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards
int pos = 0;    // variable to store the servo position

void setup() {
  while (!Serial);
    Serial.begin(9600);
  
  radio.begin();
  radio.openReadingPipe(0, address);   //Setting the address at which we will receive the data
  radio.startListening();              //This sets the module as receiver

  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  strcpy(state,"closed"); //assumed state of tap is 
}
void open_tap(){
  if(strcmp(state,"closed")==0){
    myservo.write(90);
    strcpy(state,"open");
  }
}
void close_tap(){
  if(strcmp(state,"open")==0){
    myservo.write(1);
    strcpy(state,"closed");
  }
}

void loop() {
  if (radio.available()) {
    Serial.println("Radio available");
      char text[32] = {0};                 //Saving the incoming data
      radio.read(&text, sizeof(text));    //Reading the data
      command = atoi(text);
      Serial.print("command received is : ");
      Serial.println(command);
      
      digitalWrite(2, HIGH);
      delay(20);
      digitalWrite(2, LOW);
      
      }
      //turn on or off
        if( command == 0 ){
          close_tap();
          
        }else if( command == 1){
          open_tap();
        } else {
          Serial.print("    command is : ");
          Serial.println(command);
        }
      //to detect that the loop is still alive in receiver
      digitalWrite(3, HIGH);   
      delay(1000); 
      digitalWrite(3, LOW); 
}
