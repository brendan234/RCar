#include <SPI.h>  
#include "RF24.h"
////////////////////////////////////radio
RF24 myRadio (7, 8);
byte addresses[][6] = {"0"};
///////////////////////////////////// joystick pins
const int xPin=A6;
const int yPin=A7;
const int buttonPin=4;
//////////////////////////////////// joystick values
int xVal;
int yVal;
int spinButton;
int data[3]; 
////////////////////////////////////
void setup() {
  // put your setup code here, to run once:
/////////////////////////////////radio
	Serial.begin(115200);
  myRadio.begin();  
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MIN);
  myRadio.setDataRate( RF24_250KBPS ) ; 
  myRadio.openWritingPipe( addresses[0]);
  delay(1000);
  /////////////////////////////////joystick
  pinMode(xPin,INPUT); 
  pinMode(yPin,INPUT);
  pinMode(buttonPin,INPUT_PULLUP);
  /////////////////////////////////

}

void loop() {
  // put your main code here, to run repeatedly:
	
joystick();
rad();

}
void joystick(){
	xVal=analogRead(xPin);
	yVal=analogRead(yPin);
	spinButton=digitalRead(buttonPin);
	data[0]=xVal;
	data[1]=yVal;
	data[2]=spinButton;

}
void rad(){
	myRadio.write(&data, sizeof(data));
	//myRadio.write(yVal,sizeof(yVal)); 
	Serial.print("xVal: ");
	Serial.println(xVal);
	Serial.print("yVal: ");
	Serial.println(yVal);
	Serial.print("Spin Button: ");
	Serial.println(spinButton);


}
