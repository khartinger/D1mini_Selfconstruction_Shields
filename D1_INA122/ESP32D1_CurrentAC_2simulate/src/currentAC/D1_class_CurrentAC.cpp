//_____D1_class_CurrentAC.cpp_________________201203-201203_____
// The class CurrentAC is a class for D1 mini or ESP32 D1mini
// for measuring AC current using a selfmade shield D1_INA122_V3
// (with precision instrumentation amplifier INA122P) and a 
// current sensor ASM010.
// The AC-current generates a INA122P-output voltage that is
// measured by Analog In. This value (0..1023|4095) is converted
// to an ampere value using arrays with support points (because
// the function iac=f(uin) usually is not linear).
// Hardware:
// (1) WeMos D1 mini or ESP32 D1mini
// (2) Selfmade shield D1_INA122_V3 181108 (Analog In)
// (3) Current transformer (ASM-010)
//
// Created by Karl Hartinger, December 03, 2020.
// Updates:
// 2020-12-03 New
// Released into the public domain.

#include "D1_class_CurrentAC.h"

// *************************************************************
//    constructor & co
// *************************************************************

//_____constructor (default board D1 mini, A0, default ion)_____
CurrentAC::CurrentAC() {
 int   boardType=1;                    // 1=D1 mini
 int   pin=A0;                         // analog in @ D1 mini
 float ion_A=CURRENTAC_A_ON;           // 10W @ 230V
 setup(boardType, ion_A, pin);
}

//_____constructor (default board D1 mini, A0)__________________
CurrentAC::CurrentAC(float ion_A) {
 int   boardType=1;                    // 1=D1 mini
 int   pin=A0;                         // analog in @ D1 mini
 setup(boardType, ion_A, pin);
}

//_____constructor (default analog in pin, on current)__________
CurrentAC::CurrentAC(int boardType) {
 int   pin=A0;                         // analog in @ D1 mini
 if(boardType==2) pin=CURRENTAC_PIN_AIN_ESP; // 2=ESP32 D1 mini
 float ion_A=CURRENTAC_A_ON;           // 10W @ 230V
 setup(boardType, ion_A, pin);
}

//_____constructor (default analog in pin)______________________
CurrentAC::CurrentAC(int boardType, float ion_A) {
 int   pin=A0;                         // analog in @ D1 mini
 if(boardType==2) pin=CURRENTAC_PIN_AIN_ESP; // 2=ESP32 D1 mini
 setup(boardType, ion_A, pin);
}

//_____constructor (no default values)__________________________
CurrentAC::CurrentAC(int boardType, float ion_A, int pin) {
 setup(boardType, ion_A, pin);
}

//_____set properties to default values_________________________
void CurrentAC::setup(int boardType, float ion_A, int pin)
{
 a_=0;                                 // start: 0 Ampere
 aOn_=ion_A;                           // limit current "on"
 ainLast_=0;                           // start ain=0 (ADC)
 bLastOn_=false;                       // current off 
 num50HzPeriods_=CURRENTAC_50HZ_PERIODS;//# measure 50Hz periods
 //------hardware specific values-------------------------------
 if(boardType==2)
 {//.....2=ESP32 D1 mini........................................
  iBoard_=boardType;                   // 2=ESP32D1
  ainPin_=pin;                         // several pins allowed 
  ainMax_=CURRENTAC_ADC_MAX12;         // 12bit ADC
  x4maxDefault_=x4maxESP32D1_;         // x-value for max (2A)
 }
 else
 {//.....default board: Wemos D1 mini...........................
  iBoard_=CURRENTAC_BOARD_DEFAULT;     // 1=D1mini
  ainPin_=CURRENTAC_PIN_AIN_D1;        // A0=17 default D1mini
  ainMax_=CURRENTAC_ADC_MAX10;         // 10bit ADC
  x4maxDefault_=x4maxD1mini_;          // x-value for max (2A)
 }
 //------support points (limits)--------------------------------
 x4max_=x4maxDefault_;
 // set numPoints_, xPoints_, yPoints_
 setRefPoints();                       // use default values
 //------additional properties----------------------------------
 //measuring();                          // measure current
}

// *************************************************************
//    get measurement values
// *************************************************************

//_____measure and calculate current in A_______________________
// change: ainLast_
// return: current in Ampere
float CurrentAC::getCurrent()
{
 ainLast_=measure();
 return getCurrent(ainLast_);
}

//_____dac value of last measured current_______________________
// das not force a measurement!
int CurrentAC::getLastAin() { return ainLast_; }

//_____measure current and return true, if current is on________
// return true: current is on (>aOn_)
bool CurrentAC::isOn() {
 ainLast_=measure();
 return(getCurrent(ainLast_)>aOn_);
}

//_____measure current and return true, if current is off_______
// return true: current is off (<=aOn_)
bool CurrentAC::isOff() { return(!isOn()); }

//_____return true, if current on|off changed since last call___
bool CurrentAC::isChange() {
 bool bNow=isOn();
 if(bLastOn_!=bNow)
 {
  bLastOn_=bNow;
  return true;
 }
 return false;
}

//_____return current limit for "on"____________________________
float CurrentAC::getCurrentOn() { return aOn_; }

// *************************************************************
//    set parameter
// *************************************************************

//_____set current limit for "on"_______________________________
// e.g. for P=10W: I=P/U=10/230=0,0435A
void CurrentAC::setCurrentOn(float onAmpere)
{
 if(onAmpere<0) return;
 aOn_=onAmpere;
}

//_____set ADC value for maximal current________________________
bool CurrentAC::setx4max(int x4max)
{
 if(x4max<=0) return false;
 x4max_=x4max;
 return true;
}

//_____set number of 50Hz periods to measure____________________
// duration of measurement: periods*20ms
void CurrentAC::setNumberOf50HzPeriods(int periods)
{
  if(periods<1 ||periods>1000) return;
  num50HzPeriods_=periods;
}

// *************************************************************
//    support points to calculate current
// *************************************************************

//_____set reference points for given board_____________________
// change: numPoints_, xPoints_, yPoints_
// return: true=OK, false=error
bool CurrentAC::setRefPoints()
{
 if(iBoard_==CURRENTAC_BOARD_ESP32D1)
 {
  numPoints_=numPointsESP32D1_;
  for(int i=0; i<numPoints_; i++)
  {
   xPoints_[i]=xPointsESP32D1_[i];
   yPoints_[i]=yPointsESP32D1_[i];
  }
  return true;
 }
 numPoints_=numPointsD1mini_;
 for(int i=0; i<numPoints_; i++)
 {
  xPoints_[i]=xPointsD1mini_[i];
  yPoints_[i]=yPointsD1mini_[i];
 }
 return true;
}

//_____set suppert points (minimum 2, max CURRENTAC_POINTS_MAX)_
// change: numPoints_, xPoints_, yPoints_
// return: true=OK, false=error
bool CurrentAC::setRefPoints(int num, int xValues[], float yValues[])
{
 //------check number of support points-------------------------
 if(num<2 || num>CURRENTAC_POINTS_MAX) return false;
 //------check x-limits-----------------------------------------
 int xOld=xValues[0];
 if(xOld<0 || xOld>ainMax_) return false;
 //------check for increasing x-values--------------------------
 for(int i=1; i<num; i++)
 {
  if(xValues[i]<=xOld) return false;
  xOld=xValues[i];
 }
 //-----values ok, accept them----------------------------------
 numPoints_=num;
 for(int i=0; i<numPoints_; i++)
 {
  xPoints_[i]=xValues[i];
  yPoints_[i]=yValues[i];
 }
 return true;
}

//_____return reference points as String________________________
// return: reference points as String
String CurrentAC::getsRefPoints()
{
 char   s0[18];
 String s1;
 int    decimals=3;
 for(int i=0; i<numPoints_; i++)
 {
  sprintf(s0,"(%04d/",xPoints_[i]);
  s1+=String(s0);
  s1+=String(dtostrf(yPoints_[i],decimals+2,decimals,s0));
  s1+=") ";
 }
 return s1;
}

// *************************************************************
//    some more methods
// *************************************************************

//_____calculate current in Ampere for given ADC-value__________
// called by getCurrent()
// changes: ainLast_
// return : current (float)
float CurrentAC::getCurrent(int ain_code)
{
 int     i=0;                          // counter
 int     i1;                           // start index
 float   f1;
 double  x1, y1, x2, y2;
 double  ain_k, ain_d;
 if(ain_code<0) { return 0; }
 //if(ain_code>ainMax_) { return yPoints_[numPoints_-1];}
 long ain1=ain_code;
 if(x4max_!=0) ain1=ain1*x4maxDefault_/x4max_;
 ain_code=ain1;
 int imax=numPoints_-1;
 for(i=0; i<imax; i++)
 {
  if(ain_code < xPoints_[i])
  {
   if(i==0) i1=i;
   else i1=i-1;
   break;
  }
 }
 if(i>=imax) i1=numPoints_-2;
 x1=xPoints_[i1];
 y1=yPoints_[i1];
 x2=xPoints_[i1+1];
 y2=yPoints_[i1+1];
 if(x2==x1) 
 {
   f1=y1;
   ainLast_=x1;
 }
 else
 {
  ain_k=(y2-y1)/(x2-x1);
  ain_d=(x2*y1-x1*y2)/(x2-x1);
  f1=ain_d + ain_k*ain_code;
  ainLast_=ain_code;
 }
 //if(x4max_!=0) f1=f1*x4maxDefault_/x4max_;
 return f1;
}

//_____measure current (result: ADC value)______________________
// uses   : num50HzPeriods_
// changes: -
// return : voltage average of 2 periods (40ms @ 50Hz)
int CurrentAC::measure()
{
 long totalsum=0;                      // total sum
 int  ain1;                            // help: one input value
 //------measuring: "warm up"-----------------------------------
 ain1=analogRead(ainPin_);
 delay(1);
 //------measuring some periods---------------------------------
 for(int p=0; p<num50HzPeriods_; p++)
 {//-----measure one period-------------------------------------
  for(int p1=0; p1<20; p1++)
  {
   ain1=analogRead(ainPin_);
   totalsum=totalsum+ain1;
   delay(1);
  }
 }
 //------calc total average value-------------------------------
 int totalavg=totalsum/(num50HzPeriods_*20);
 return(totalavg);
}
