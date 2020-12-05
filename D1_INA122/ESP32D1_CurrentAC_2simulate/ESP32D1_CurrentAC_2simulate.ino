//_____ESP32D1_CurrentAC_2simulate.cpp________201203-201203_____
// Demo program for class CurrentAC:
// * set reference points (x- and y-array)
// * print reference points
// * read an analog value every second
// * calculate current for simulated analog input values
// Results are printed to Serial (115200 Baud).
// Hardware:
// (1) WeMos D1 mini OR ESP32 D1 mini
//
// Created by Karl Hartinger, December 03, 2020
// Updates:
// 2020-12-03 New
// Released into the public domiac.
#include "src/currentAC/D1_class_CurrentAC.h"
//#define ESP32D1                   2       // ESP32 D1mini
//CurrentAC iac(ESP32D1);                   // ESP32 D1mini, 12Bit
CurrentAC iac;                              // D1mini, 10bit ADC

//_______SETUP__________________________________________________
void setup() {
 //-----set some parameters-------------------------------------
 float fAin;                                // result voltage
 int   iAin=500;                            // test value
 int iTest;                                 // test value
 Serial.begin(115200); Serial.println("\nsetup(): --Start--");
 //-----simulate input for iAin=500-----------------------------
 int   xRef1[]={  0, 4095};                 // x = ain
 float yRef1[]={0.0, 3.2};                  // y = voltage
 iac.setRefPoints(2, xRef1, yRef1);         // new ref values
 Serial.println("\nReference points: "+iac.getsRefPoints());
 fAin=iac.getCurrent(iAin);                 // calc result
 Serial.print(iAin); Serial.print(" = ");   // show
 Serial.print(fAin,4);                      // result
 Serial.println(" (should be 3.2/1023*500 = 1.5640 or 3.2/4095*500 = 0,3907)");
 //-----use 2 reference points with "y-offset" +1V--------------
 int   xRef2[]={  0, 1023};                 // x = ain
 float yRef2[]={1.0, 4.2};                  // y = voltage
 iac.setRefPoints(2, xRef2, yRef2);         // new ref values
 Serial.println("\nReference points: "+iac.getsRefPoints());
 fAin=iac.getCurrent(iAin);                 // calc result
 Serial.print(iAin); Serial.print(" = ");   // show
 Serial.print(fAin,4);                      // result
 Serial.println(" (should be 3.2/1023*500+1 = 2.5640)");

 //-----use 5 reference points with support points array--------
 // x-offset: ain=0 until v=0.2V, ain=1001 3.2V reached
 int   xRef[]={0,1,  2,1002,1023};          // x = ain
 float yRef[]={0,0,0.2, 3.2, 3.2};          // y = voltage
 iac.setRefPoints(5, xRef, yRef);           // new ref values
 Serial.println("\nReference points: "+iac.getsRefPoints());
 iTest=0;
 fAin=iac.getCurrent(iTest);                // calc result
 Serial.print(iTest); Serial.print(" = ");  // show
 Serial.print(fAin,4);                      // result
 Serial.println(" (should be 0.0000 - range 1)");
 iTest=1;
 fAin=iac.getCurrent(iTest);                // calc result
 Serial.print(iTest); Serial.print(" = ");  // show
 Serial.print(fAin,4);                      // result
 Serial.println(" (should be 0.0000 - range 1|2)");
 iTest=2;
 fAin=iac.getCurrent(iTest);                // calc result
 Serial.print(iTest); Serial.print(" = ");  // show
 Serial.print(fAin,4);                      // result
 Serial.println(" (should be 0.2000 - range 2|3)");
 fAin=iac.getCurrent(iAin);                 // calc result
 Serial.print(iAin); Serial.print(" = ");   // show
 Serial.print(fAin,4);                      // result
 Serial.println(" (should be (3.2-0.2)/(1002-2)*500+(0.2-0.006) = 1.6940 - range 3)");
 iTest=1000;
 fAin=iac.getCurrent(iTest);                // calc result
 Serial.print(iTest); Serial.print(" = ");  // show
 Serial.print(fAin,4);                      // result
 Serial.println(" (should be 3.2-2*0.003 = 3.1940 - range 3)");
 iTest=1002;
 fAin=iac.getCurrent(iTest);                // calc result
 Serial.print(iTest); Serial.print(" = ");  // show
 Serial.print(fAin,4);                      // result
 Serial.println(" (should be 3.2000 - range 3|4)");
 iTest=1010;
 fAin=iac.getCurrent(iTest);                // calc result
 Serial.print(iTest); Serial.print(" = ");  // show
 Serial.print(fAin,4);                      // result
 Serial.println(" (should be 3.2000 - range 4)");
 iTest=2000;
 fAin=iac.getCurrent(iTest);                // calc result
 Serial.print(iTest); Serial.print(" = ");  // show
 Serial.print(fAin,4);                      // result
 Serial.println(" (should be 3.2000 - range 4+)");

 //-----set reference parameters to default---------------------
 iac.setRefPoints();                        // default values
 Serial.println("\nReference points set to default: "+iac.getsRefPoints());
 Serial.print("Current limit for \"on\": ");//
 Serial.print(iac.getCurrentOn(),3);        //
 Serial.println(" A");                      //
 Serial.println("-----setup() finished---------------------\n");
}

//_______LOOP___________________________________________________
void loop() {
 delay(1000);                               // wait
}
