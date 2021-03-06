#include <SPI.h>  
#include "RF24.h" 
/////////////////////////Right motor
const int RFpin=5;//A-A
const int RRpin=6;//A-B
int rightBrake;
///////////////////////////////////

/////////////////////////Left motor
const int LFpin=9;//B-A
const int LRpin=3;//B-B
int leftBrake;
////////////////////////////////////
int throttle;
int FRneutral;
int STneutral;
/////////////////////////////////////// radio
RF24 myRadio (7, 8); //CE CS
byte addresses[][6] = {"0"};
int data[3];
///////////////////////////////////////

/////////////////////////joystick
int motorJoy;
int steeringJoy;
int spinButton;
int slop=5;
void setup() {
	//////////////////////////////////
	 pinMode(RFpin, OUTPUT);
  pinMode(RRpin, OUTPUT);
   pinMode(LFpin, OUTPUT);
  pinMode(LRpin, OUTPUT);
////////////////////////////////////////////
  Serial.begin(115200);
  ///////////////////////////////////////
    myRadio.begin(); 
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MIN);
  myRadio.setDataRate( RF24_250KBPS ) ; 
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
   delay(3000);
   ////////////////////////////////////calibration
   rad();
   FRneutral=data[1];
   STneutral=data[0];
  /////////////////////////////////////////
}
void loop() {
rad();
turn();
throttle = map(motorJoy, FRneutral, 1021, 70, 230);
 if (motorJoy>FRneutral+slop)
 {
    fwd();
    Serial.print("fwd:");
    Serial.println(motorJoy);
  }
   if (motorJoy<FRneutral-slop){

    rev();
    Serial.print("backward: ");
    Serial.println(motorJoy);
  }/////servo
 if ((motorJoy>FRneutral-slop)&&(motorJoy<FRneutral+slop))
  {
    Serial.println("STOP: ");
   stop();

  }
  if (spinButton==0){
  	spin();
  }

}
///////////////////////////functions
void rad(){
  if ( myRadio.available()) 
  {
    while (myRadio.available())
    {
      myRadio.read( &data, sizeof(data));
      Serial.println("got it!");
      motorJoy=data[1];
steeringJoy=data[0];
spinButton=data[2];
Serial.print("spin: ");
Serial.println(spinButton);
//delay(1000);

    }

}
}
void turn(){
	if (steeringJoy>STneutral+slop)///turn right
  {
  	leftBrake=0;
  	rightBrake=map(steeringJoy,STneutral,1021,0,throttle*.75);
    
  }
  if (steeringJoy<STneutral-slop)///////turn left
  {
  	rightBrake=0;
    leftBrake=map(steeringJoy,STneutral,0,0,throttle*.75);
  }
}
void fwd(){
	analogWrite(LRpin,0);
	analogWrite(RRpin,0);
	analogWrite(RFpin,throttle - rightBrake);
	analogWrite(LFpin,throttle - leftBrake);
}
void rev(){
	analogWrite(RFpin,0);
	analogWrite(LFpin,0);
	analogWrite(RRpin,throttle - rightBrake);
	analogWrite(LRpin,throttle - leftBrake);
}
void stop(){
	analogWrite(LRpin,0);
	analogWrite(LFpin,0);
	analogWrite(RFpin,0);
	analogWrite(RRpin,0);

}
void spin(){
	analogWrite(LRpin,0);
	analogWrite(LFpin,130);
	analogWrite(RRpin,130);
	analogWrite(RFpin,0);
}