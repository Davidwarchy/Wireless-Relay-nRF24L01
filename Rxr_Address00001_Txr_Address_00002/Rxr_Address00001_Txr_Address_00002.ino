//Code for Receiver
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <avr/wdt.h>

//keypad variables
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'.','0','#'}
};
byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {10, 9, 6}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
LiquidCrystal_I2C lcd(0x27, 16, 2);

//radio variables & other general ones
RF24 radio(7, 8); // CE, CSN
const byte address1[6] = "00001";
const byte address2[6] = "00002";
float threshold = 2.5;
float reading = 0;
float command = 0;

void display_norm(){
  char x[] = "THX:";
  char y[] = "RDG:";
  char instruction[] = "# VARIES THX";
  
  lcd.clear();
  lcd.print(x);
  lcd.print(threshold);
  lcd.print(" ");
  lcd.print(y);
  lcd.print(reading);
  lcd.setCursor(0,1);
  lcd.print(instruction);
}
void setup() {
  wdt_enable(WDTO_8S);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  
  while (!Serial);
    Serial.begin(9600);
  
  lcd.init();
    
  radio.begin();
  radio.setRetries(15,15);
  radio.openWritingPipe(address2); //Setting the address where we will send the data
  radio.setPALevel(RF24_PA_LOW);

  display_norm();
}

void loop() {
  //keypad and lcd logic
  //press any key or '#' to begin recording
  char key = keypad.getKey();
  char string[32];
  int index = 0;
  memset(string,0,sizeof(string));
  if(key){
          //_______________lcd code
          lcd.clear();
          lcd.print("NEW THX:");
          //_______________lcd code
    do{
      key = keypad.getKey();
      if(key){      
        if(key == '#'){
          Serial.print('\n');
          break;
        }else{
          Serial.print(key);
          lcd.print(key);
          string[index] = key;
          index++;
        }
      }
    } while(1);
    //after break
    Serial.print("String is equal to ");
    Serial.println(string);
    float _threshold = atof(string);
    if(_threshold!=0){
      if((_threshold > 0)&&(_threshold<5.00)){
        threshold = _threshold;
      }else{
        lcd.clear();
        lcd.print("BTW 0 - 5.00");
      }
    }
    display_norm();
    
  }
  
  radio.openReadingPipe(0, address1);   //Setting the address at which we will receive the data
  radio.startListening();              //This sets the module as receiver
  if (radio.available()) {
    Serial.println("Radio available");
      char text[10] = {0};                 //Saving the incoming data
      radio.read(&text, sizeof(text));    //Reading the data
      reading = atof(text);
      Serial.println("Reading :");
      Serial.println(reading);
      display_norm();
        if(reading > threshold){
          command = 0.0;  //0 for close
        }else{
          command = 1.0; //1 for open
        }
      //Pin to indicate that this radio is receiving
      digitalWrite(A0, HIGH);   
      delay(100);
      digitalWrite(A0, LOW); 
      
      //send command----------------------------------------------------------------------------
      dtostrf(command,4,4,text);

      radio.write(&text, sizeof(text));                  //Sending the message to receiver
      radio.stopListening();
      radio.write(&text, sizeof(text));   
      //end sending command----------------------------------------------------------------------
      }
      Serial.print("Threshold: ");
      Serial.println(threshold);
      
      //to detect that the loop is still alive in receiver
      digitalWrite(A1, HIGH);   
      delay(100);
      digitalWrite(A1, LOW); 
      wdt_reset();
}
