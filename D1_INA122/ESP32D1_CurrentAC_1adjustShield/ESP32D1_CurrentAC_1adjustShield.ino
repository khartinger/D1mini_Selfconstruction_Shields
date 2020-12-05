//_____ESP32D1_CurrentAC_1adjustShield.cpp____201129-201129_____
// This program measures every two seconds
// * 40x an analog in value and sends it to Serial 
//   (comma separated, time lag 1ms),
// * counts the number of maximum values 
// * calculates the average and sends it to Serial (115200 Baud).
// You can copy the comma separated values into a file and open
//   it with open office calc to draw a diagram.
//
// Hardware: 
// (1) WeMos D1 mini oder ESP32 D1 mini
// (2) selfmade D1_INA122 shield (V3)
// (3) AC current source e.g. Märklin AC transformer plus 
//     electric load (resistor, lamp, ...)
// (4) ampere meter or multimeter (AC)
//
// Proposal for adjustment of the poti @ D1_INA122 shield:
// * Number of maximum values should be about 10 (maximum is 20)
//   or for e.g. imax=2A average 1000 (D1 mini) or 4000 (ESP32)
//  
// Created by Karl Hartinger, November 29, 2020.
// Changes:
// 2020-11-29 New
// Released into the public domain.

#include <Arduino.h>                   // A0
//#define ESP32D1               true
#ifdef ESP32D1
 #define AIN_PIN                36     // 36 = pin A0 @ ESP32 D1mini
 #define AIN_MAX              4095     // 12 bit ADC
#else
 #define AIN_PIN                A0     // A0 = 17 @ D1mini
 #define AIN_MAX              1023     // 10 bit ADC
#endif

//_______measure function_______________________________________
int measure()
{
 int  numPeriod=2;                     // number of 50Hz periods
 int  numMax[numPeriod];               // number of AIN_MAX values
 long totalsum=0;                      // total sum
 int  ain1;                            // help: one input value
 //------measuring: "warm up"-----------------------------------
 ain1=analogRead(AIN_PIN);
 delay(1);
 //------measuring some periods---------------------------------
 for(int p=0; p<numPeriod; p++)
 {//-----measure one period-------------------------------------
  numMax[p]=0;
  for(int p1=0; p1<20; p1++)
  {
   ain1=analogRead(AIN_PIN);
   Serial.print(ain1); Serial.print(";");
   totalsum=totalsum+ain1;
   if(ain1>=AIN_MAX) numMax[p]++;
   delay(1);
  }
 }
 Serial.println();
 //------calc total average value-------------------------------
 int totalavg=totalsum/(numPeriod*20);

 //------print result to Serial---------------------------------
 for(int i=0; i<numPeriod; i++)
 {
  Serial.print("Period "); Serial.print(i);
  Serial.print(": number of "); Serial.print(AIN_MAX);
  Serial.print("-values (maximum is 20) "); Serial.println(numMax[i]);
 }
 return(totalavg);
}

//_______measure function with details__________________________
int measure_delails()
{
 int  numPeriod=4;                     // number of 50Hz periods
 int  xmin[numPeriod];                 // minimum value / period
 int  xmax[numPeriod];                 // maximum value / period
 long xsum[numPeriod];                 // sum values /period
 int  numMax[numPeriod];               // number of AIN_MAX values
 long totalsum=0;                      // total sum
 int  ain1;                            // help: one input value
 //------measuring: "warm up"-----------------------------------
 ain1=analogRead(AIN_PIN);
 delay(1);
 //------measuring some periods---------------------------------
 for(int p=0; p<numPeriod; p++)
 {//-----measure one period-------------------------------------
  xsum[p]=0;
  numMax[p]=0;
  for(int p1=0; p1<20; p1++)
  {
   ain1=analogRead(AIN_PIN);
   Serial.print(ain1); Serial.print(";");
   if(p1==0)
   {//...first value is min and max as well.....................
    xmin[p]=ain1;
    xmax[p]=ain1;
   }
   else
   {//...new minimum or maximum?................................
    if(ain1<xmin[p]) xmin[p]=ain1;
    if(ain1>xmax[p]) xmax[p]=ain1;
   }
   //....number of AIN_MAX values, average......................
   xsum[p]=xsum[p]+ain1;
   if(ain1>=AIN_MAX) numMax[p]++;
   delay(1);
  }
  totalsum+=xsum[p];
 }
 Serial.println();
 //------calc total average value-------------------------------
 int totalavg=totalsum/(numPeriod*20);
 //------print result to Serial---------------------------------
 for(int i=0; i<numPeriod; i++)
 {
  Serial.print("Period "); Serial.print(i);
  Serial.print(": min "); Serial.print(xmin[i]);
  Serial.print(", max "); Serial.print(xmax[i]);
  Serial.print(", average "); Serial.print((xsum[i]/20));
  Serial.print(", number of "); Serial.print(AIN_MAX);
  Serial.print("-values "); Serial.println(numMax[i]);
 }
 return(totalavg);
}

//_______SETUP__________________________________________________
void setup() {
 Serial.begin(115200);Serial.println(); // init serial com
 pinMode(AIN_PIN, OUTPUT);              // set A0 pin high res
}

//_______LOOP___________________________________________________
void loop() {
 //int sensorValue = analogRead(pinAin); // analog in
 int sensorValue = measure();            // 40ms
 //------Convert Ain (0...1023|4096) to a voltage (0...3.2V):---
 float voltage = sensorValue * (3.2 / AIN_MAX);   // convert
 //------print out the value to serial--------------------------
 Serial.print(sensorValue);            // native analog in value
 Serial.print("  (=");                 // equal sign
 Serial.print(voltage,3);              // voltage value
 Serial.println("V)");                 // unit (voltage)
 delay(1960);                          // wait 2 (1,96) sec
}
