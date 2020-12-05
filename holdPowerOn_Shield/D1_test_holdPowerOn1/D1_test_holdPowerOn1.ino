//_____D1_test_holdPowerOn1.ino______________200614-200614_____
// Hardware test for the selfmade prototype shield "holdPowerOn"
// (power on by a pulse and self locking relay IM01 at D6): 
// * When SET-button S1 is pressed, relay attracts and D1 mini
//   starts. D6 is set to 1 to self lock the relay.
//   The LED shines alternating red, orange and green.
// * Press button D5 to turn power off (sets D6 to 0).   
// * Power is turned off after 20secs anyway (to save energy :)
// Hardware: 
// (1) WeMos D1 mini
// (2) holdPowerOn shield IP01
// Important: supply voltage by holdPowerOn shield! (NOT by USB!)
// Created by Karl Hartinger, June 14, 2020.
// Released into the public domain.

//-----sensors, actors------------------------------------------
#define  PIN_TURN_OFF           D5
#define  PIN_RELAY              D6
#define  PIN_LED_GREEN          D7
#define  PIN_LED_RED            D8
#define  MILLIS_TIME2RUN     20000

//-----"state machine"------------------------------------------
unsigned long millisOff;

//_____turn power off___________________________________________
void turnPowerOff(bool success_)
{
 //-----turn power off------------------------------------------
 digitalWrite(PIN_RELAY, 0);
 digitalWrite(PIN_LED_GREEN,0); digitalWrite(PIN_LED_RED,1);
 delay(500);                           // wait for relay off
 //-----power should be turned off now-------------------------
 for(int i=1; i<20; i++)
 {
  delay(125); digitalWrite(PIN_LED_RED, 1);
  delay(125); digitalWrite(PIN_LED_RED, 0);
 }
 //-----if power is still not turned off yet: go to sleep-------
 digitalWrite(PIN_RELAY, 0);           // just for safety
 while(1) ESP.deepSleep(60 * 1000000L);
 delay(2000);                          // just for safety
}

//_____setup i/o pins and Serial________________________________
void setup() 
{
 pinMode(PIN_RELAY, OUTPUT);
 digitalWrite(PIN_RELAY, 1);
 pinMode(PIN_TURN_OFF, INPUT);
 pinMode(PIN_LED_GREEN, OUTPUT);
 pinMode(PIN_LED_RED, OUTPUT);
 millisOff=MILLIS_TIME2RUN+millis();   // time to turn power off
 {Serial.begin(115200); Serial.println();}
}

int i=0;
//_____main loop________________________________________________
void loop() 
{
 //-----LED-----------------------------------------------------
 if(i==0)  { digitalWrite(PIN_LED_GREEN,0); digitalWrite(PIN_LED_RED,1); }
 if(i==10) { digitalWrite(PIN_LED_GREEN,1); digitalWrite(PIN_LED_RED,1); }
 if(i==20) { digitalWrite(PIN_LED_GREEN,1); digitalWrite(PIN_LED_RED,0); }
 i++; if(i>30) i=0;
 //-----D5 pressed?---------------------------------------------
 if(digitalRead(PIN_TURN_OFF)==0) {
  turnPowerOff(true);
 }
 //-----timeout?------------------------------------------------
 if(millis()>millisOff) {
  turnPowerOff(false);                 // timeout
 }
 delay(100);                           // ain lasts 38ms
}
