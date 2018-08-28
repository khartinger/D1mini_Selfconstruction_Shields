//_____DIO578_01_Blink.ino____________________170131-180827_____
// Demo program for DIO578 selfconstruction shield:
// Red LED lights up for 1.5s, green LED 0.5s and so on.
// If button is pressed, both LEDs are on (=> yellow).
// Hardware: WeMos D1 mini
//           DIO578_Shield with 2 LEDs at D7 (green), D8 (red)
//                         and 1 Button at D5
// Created by Karl Hartinger, August 27, 2018
// Last modified: -
// Released into the public domain.

const int pinButton=D5;
const int pinLED7=D7;
const int pinLED8=D8;
const int statemax=20;
const int statedelay=100;
int   state=0;

void setup() {
 pinMode(pinButton, INPUT);
 pinMode(pinLED7, OUTPUT);
 pinMode(pinLED8, OUTPUT);
}

void loop() 
{
 int in1_;
 in1_= digitalRead(pinButton);
 if(in1_)
 {
  if(state<15)
  { //-----red LED lights up------------------------------------
   digitalWrite(pinLED8, 1);
   digitalWrite(pinLED7, 0);
  }
  else
  { //-----green LED lights up----------------------------------
   digitalWrite(pinLED7, 1);
   digitalWrite(pinLED8, 0);
  }
  state++;
  if(state>statemax) state=0;
 }
 else
  { //-----both LEDs light up, no change of state number--------
  digitalWrite(pinLED7, 1);
  digitalWrite(pinLED8, 1);
 }
 delay(statedelay);
}
