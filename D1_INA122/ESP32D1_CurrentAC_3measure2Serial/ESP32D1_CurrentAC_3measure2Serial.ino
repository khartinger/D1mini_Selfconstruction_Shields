//_____ESP32D1_CurrentAC_3measure2Serial.cpp__201203-201203_____
// This demo program measures an alternating current using a 
// selfmade shield D1_INA122_V3 (with precision instrumentation
// amplifier INA122P) and a current sensor ASM010 every second.
// Results are printed to Serial (115200 Baud).
// Additionally the current state (on or off) and a change of
// the state are sent to Serial (115200Baud).
// Hardware:
// (1) WeMos D1 mini or ESP32 D1mini
// (2) Selfmade shield D1_INA122_V3 181108 (Analog In)
// (3) Current transformer (ASM-010)
// (4) Any AC-current source
//
// Created by Karl Hartinger, December 03, 2020
// Updates:
// 2020-12-03 New
// Released into the public domiac.
#include "src/currentAC/D1_class_CurrentAC.h"
CurrentAC iac;                              // D1mini, 10bit ADC
//#define ESP32D1                   2       // ESP32 D1mini
//CurrentAC iac(ESP32D1);                   // ESP32 D1mini, 12Bit

//_______SETUP__________________________________________________
void setup() {
 //-----set some parameters-------------------------------------
 Serial.begin(115200); Serial.println("\nsetup(): --Start--");
 //iac.setx4max(960);
 Serial.println("Reference points: "+iac.getsRefPoints());
 Serial.print("Current limit for \"on\": ");//
 Serial.print(iac.getCurrentOn(),3);        //
 Serial.println(" A");                      //
 Serial.println("-----setup() finished---------------------\n");
}

//_______LOOP___________________________________________________
void loop() {
 float  fAin=iac.getCurrent();              // float value
 int    iAin=iac.getLastAin();              // ADC code
 Serial.print(iAin); Serial.print(" = ");   // 0..1023 (7..995)
 Serial.print(fAin,3);                      // 3 decimals
 Serial.print(" A");                        // unit
 if(iac.isOn()) Serial.print(" on");        //
           else Serial.print(" off");       //
 if(iac.isChange()) Serial.print(" CHANGED!"); // 
 Serial.println();                          // next line
 delay(1000);                               // wait
}
