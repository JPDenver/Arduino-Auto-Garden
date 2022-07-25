#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "DHT.h"
#include <IRremote.h>
#include <Stepper.h>   

#define DHTPIN A2
#define DHTTYPE DHT11
#define STEPS 100  

DHT dht(DHTPIN, DHTTYPE);
Stepper stepper(STEPS, A8, A7, A6, A5); 

int DI = 12;
int RW = 11;
int DB[] = {3, 4, 5, 6, 7, 8, 9, 10};// use array to select pin for bus
int Enable = 2;
//dht sensor
String fahrenheit;
String humidity;
//moisture sensore
String moisture;
String moisture1;
char needswater;
char needswater1;
//light sensor
int light;
//button
const int BUTTON_PIN = A4; // Arduino pin connected to button's pin
int lastButtonState;    // the previous state of button
int currentButtonState; // the current state of button
//IrRemote
const byte IR_RECEIVE_PIN = A9;
int remoteinput;
int previous = 0; 

//define LCD writing
void LcdCommandWrite(int value) {
  // define all pins
  int i = 0;
  for (i = DB[0]; i <= DI; i++) // assign value for bus
  {
    digitalWrite(i, value & 01); // for 1602 LCD, it uses D7-D0( not D0-D7) for signal identification; here, it’s used for signal inversion.
    value >>= 1;
  }
  digitalWrite(Enable, LOW);
  delayMicroseconds(1);
  digitalWrite(Enable, HIGH);
  delayMicroseconds(1);  // wait for 1ms
  digitalWrite(Enable, LOW);
  delayMicroseconds(1);  // wait for 1ms
}

void LcdDataWrite(int value) {
  // initialize all pins
  int i = 0;
  digitalWrite(DI, HIGH);
  digitalWrite(RW, LOW);
  for (i = DB[0]; i <= DB[7]; i++) {
    digitalWrite(i, value & 01);
    value >>= 1;
  }
  digitalWrite(Enable, LOW);
  delayMicroseconds(1);
  digitalWrite(Enable, HIGH);
  delayMicroseconds(1);
  digitalWrite(Enable, LOW);
  delayMicroseconds(1);  // wait for 1ms
}

//Full Setup
void setup (void) {
  int i = 0;
  for (i = Enable; i <= DI; i++) {
    pinMode(i, OUTPUT);
    Serial.begin(57600);
    dht.begin();
    pinMode(BUTTON_PIN, INPUT_PULLUP); // set arduino pin to input pull-up mode
    currentButtonState = digitalRead(BUTTON_PIN);
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
    stepper.setSpeed(180);
  }

//THESE DELAYS WERE SLOWING DOWN THE STEPPER MOTOR
//  delay(100);
  // initialize LCD after a brief pause
  // for LCD control
  LcdCommandWrite(0x38);  // select as 8-bit interface, 2-line display, 5x7 character size
//  delay(64);
  LcdCommandWrite(0x38);  // select as 8-bit interface, 2-line display, 5x7 character size
//  delay(50);
  LcdCommandWrite(0x38);  // select as 8-bit interface, 2-line display, 5x7 character size
//  delay(20);
  LcdCommandWrite(0x06);  // set input mode
  // auto-increment, no display of shifting
//  delay(20);
  LcdCommandWrite(0x0E);  // display setup
  // turn on the monitor, cursor on, no flickering
//  delay(20);
  LcdCommandWrite(0x01);  // clear the scree, cursor position returns to 0
//  delay(100);
  LcdCommandWrite(0x80);  //  display setup
  //  turn on the monitor, cursor on, no flickering

//  delay(20);
}

void loop (void) {
  lastButtonState    = currentButtonState;      // save the last state
  currentButtonState = digitalRead(BUTTON_PIN); // read new state
  if(lastButtonState == HIGH && currentButtonState == LOW) {
    LcdCommandWrite(0x01);  // clear the scree, cursor position returns to 0
    delay(1000);
    LcdCommandWrite(0x80 + 1);
    delay(100);
    moisture = analogRead(0);
    // Start LCD write
    LcdDataWrite('P');
    LcdDataWrite('1');
    LcdDataWrite(' ');
    LcdDataWrite('M');
    LcdDataWrite('o');
    LcdDataWrite('i');
    LcdDataWrite('s');
    LcdDataWrite('t');
    LcdDataWrite('u');
    LcdDataWrite('r');
    LcdDataWrite('e');
    LcdDataWrite('=');
  
    LcdDataWrite(moisture[0]);
  
    LcdCommandWrite(0xc0 + 1);
  
    if (moisture[0] == '6'
        || moisture[0] == '7'
        || moisture[0] == '8'
        || moisture[0] == '9'
       ) {
      needswater = 'N';
      LcdDataWrite('D');
      LcdDataWrite('o');
      LcdDataWrite('e');
      LcdDataWrite('s');
      LcdDataWrite(' ');
      LcdDataWrite('N');
      LcdDataWrite('O');
      LcdDataWrite('T');
  
    }
    else {
      needswater = 'Y';
      LcdDataWrite('N');
      LcdDataWrite('e');
      LcdDataWrite('e');
      LcdDataWrite('e');
      LcdDataWrite('d');
      LcdDataWrite('s');
      LcdDataWrite(' ');
      LcdDataWrite('W');
      LcdDataWrite('a');
      LcdDataWrite('t');
      LcdDataWrite('e');
      LcdDataWrite('r');
    }
    delay(2000);
    //  LcdDataWrite('F');
    delay(500);
  
    LcdCommandWrite(0x01);  // clear the scree, cursor position returns to 0
    delay(500);
    LcdCommandWrite(0x80 + 1);
    delay(100);
    moisture1 = analogRead(1);
    // write in welcome message
    LcdDataWrite('P');
    LcdDataWrite('2');
    LcdDataWrite(' ');
    LcdDataWrite('M');
    LcdDataWrite('o');
    LcdDataWrite('i');
    LcdDataWrite('s');
    LcdDataWrite('t');
    LcdDataWrite('u');
    LcdDataWrite('r');
    LcdDataWrite('e');
    LcdDataWrite('=');
  
    LcdDataWrite(moisture1[0]);
  
    LcdCommandWrite(0xc0 + 1);
  
    if (moisture1[0] == '6'
        || moisture1[0] == '7'
        || moisture1[0] == '8'
        || moisture1[0] == '9'
       ) {
      needswater1 = 'N';
      LcdDataWrite('D');
      LcdDataWrite('o');
      LcdDataWrite('e');
      LcdDataWrite('s');
      LcdDataWrite(' ');
      LcdDataWrite('N');
      LcdDataWrite('O');
      LcdDataWrite('T');
  
    }
    else {
      needswater1 = 'Y';
      LcdDataWrite('N');
      LcdDataWrite('e');
      LcdDataWrite('e');
      LcdDataWrite('e');
      LcdDataWrite('d');
      LcdDataWrite('s');
      LcdDataWrite(' ');
      LcdDataWrite('W');
      LcdDataWrite('a');
      LcdDataWrite('t');
      LcdDataWrite('e');
      LcdDataWrite('r');
    }
    delay(1000);
    //  LcdDataWrite('F');
    delay(1000);
  
    LcdCommandWrite(0x01);  // clear the scree, cursor position returns to 0
    delay(1000);
    LcdCommandWrite(0x80 + 1);
    delay(100);
  
    fahrenheit = String(dht.readTemperature(true));
  
    LcdDataWrite('T');
    LcdDataWrite('e');
    LcdDataWrite('m');
    LcdDataWrite('p');
    LcdDataWrite('e');
    LcdDataWrite('r');
    LcdDataWrite('a');
    LcdDataWrite('t');
    LcdDataWrite('u');
    LcdDataWrite('r');
    LcdDataWrite('e');
    LcdDataWrite('=');
    LcdDataWrite(fahrenheit[0]);
    LcdDataWrite(fahrenheit[1]);
  
  //  Serial.println(fahrenheit);
  
    LcdCommandWrite(0xc0 + 1);
  
    LcdDataWrite('H');
    LcdDataWrite('u');
    LcdDataWrite('m');
    LcdDataWrite('i');
    LcdDataWrite('d');
    LcdDataWrite('i');
    LcdDataWrite('t');
    LcdDataWrite('y');
    LcdDataWrite('=');
  
    humidity = String(dht.readHumidity());
    LcdDataWrite(humidity[0]);
    LcdDataWrite(humidity[1]);
    delay(3000);
  
    //Reset LCD
    LcdCommandWrite(0x01);  // clear the scree, cursor position returns to 0
    delay(1000);
    LcdCommandWrite(0x80 + 1);
    delay(100);
  
    LcdDataWrite('L');
    LcdDataWrite('i');
    LcdDataWrite('g');
    LcdDataWrite('h');
    LcdDataWrite('t');
    LcdDataWrite(' ');
    LcdDataWrite('=');
    LcdDataWrite(' ');
  
  
    light = analogRead(A3);
    // We'll have a few threshholds, qualitatively determined
    if (light < 10) {
      Serial.println(" - Dark");
      LcdDataWrite('D');
      LcdDataWrite('a');
      LcdDataWrite('r');
      LcdDataWrite('k');
  
    } else if (light < 200) {
      Serial.println(" - Dim");
      LcdDataWrite('D');
      LcdDataWrite('i');
      LcdDataWrite('m');
  
    } else if (light < 500) {
      Serial.println(" - Light");
      LcdDataWrite('L');
      LcdDataWrite('i');
      LcdDataWrite('g');
      LcdDataWrite('h');
      LcdDataWrite('t');
    } else if (light < 800) {
      Serial.println(" - Bright");
      LcdDataWrite('B');
      LcdDataWrite('r');
      LcdDataWrite('i');
      LcdDataWrite('g');
      LcdDataWrite('h');
      LcdDataWrite('t');
      LcdDataWrite('!');
  
    } else {
      Serial.println(" - Very bright");
    }
    delay(3000);
  
    //Reset LCD
    LcdCommandWrite(0x01);  // clear the scree, cursor position returns to 0
    delay(1000);
    LcdCommandWrite(0x80);
    delay(100);
      //Display Summary
  
  //    LcdDataWrite('T');
  //    LcdDataWrite('M');
  //    LcdDataWrite('P');
  //    LcdDataWrite(':');
      LcdDataWrite(fahrenheit[0]);
      LcdDataWrite(fahrenheit[1]);
      LcdDataWrite('F');
  
  
      LcdDataWrite(' ');
      LcdDataWrite('1');
      LcdDataWrite('W');
      LcdDataWrite(':');
      LcdDataWrite(needswater);
  
      LcdDataWrite(' ');
      LcdDataWrite('2');
      LcdDataWrite('W');
      LcdDataWrite(':');
      LcdDataWrite(needswater1);
  

 }
  if (IrReceiver.decode())
   {

      Serial.println(IrReceiver.decodedIRData.command);
      remoteinput = IrReceiver.decodedIRData.command;

      IrReceiver.resume();
//      delay(500);
   }
  if (remoteinput == 22){
      Serial.println('1');
      int val = analogRead(0);      
      stepper.step(val - previous);     
      previous = val;
    }

}
