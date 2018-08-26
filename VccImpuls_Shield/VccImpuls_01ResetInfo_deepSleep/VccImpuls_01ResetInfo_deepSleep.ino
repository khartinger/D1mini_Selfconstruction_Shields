//_____VccImpuls_01ResetInfo_deepSleep.ino____180822-180826_____
// If the power supply is connected to the VccImpuls-Shield (K1)
// a short impulse on S1 starts the D1 mini. 
// As reason=6 (power on), LED shines green for 3 seconds.
// * If D5=1 the D1mini turns off the power supply (D6=0).
// * If D5=0 the D1mini goes to deepSleep (turns also power off).
// --> System is turned off, start with a new S1 impulse.
//--------------------------------------------------------------
// If button S1 stays pressed and
// * if D5=0 the D1mini goes to deepSleep for 1 second, wakes
//   up (if pin D0 is connected to RST!) and the LED shines
//   red for 3 seconds (reason=5 reset, deep sleep).
//   Then D1mini goes to sleep again and so on.
// * if D5=1 the D1mini tries to turn off the power supply
//   (D6=0). Because S1 does the power supply, loop() is 
//   done and the LED blinks yellow until S1 is released.
//--------------------------------------------------------------
// If power supply is connected to D1mini (by USB) the LED
// shines green for 3secs and then blinks yellow.
// Hardware: WeMos D1 mini
//           VccImpuls-Shield (do-it-yourself construction)
// Note: For wake up CONNECT D0 to RST !!! (K3)

#include <Arduino.h>
extern "C" {
 #include <user_interface.h>
}
#define PIN_IN          D5
#define PIN_RELAIS      D6
#define PIN_LED_GREEN   D7
#define PIN_LED_RED     D8

void setup() {
 int reason;
 //-----setup pins----------------------------------------------
 pinMode(PIN_RELAIS, OUTPUT);          // pin D6 output (relais)
 digitalWrite(PIN_RELAIS, HIGH);       // relais on (hold power)
 pinMode(PIN_IN, INPUT);               // pin D5 is input
 pinMode(PIN_LED_GREEN, OUTPUT);       // pin D7 is output (led)
 pinMode(PIN_LED_RED, OUTPUT);         // pin D8 is output (led)
 pinMode(D0, WAKEUP_PULLUP);           // connect D0->RST !!!
 //-----set led depending on power on reason--------------------
 reason=(ESP.getResetInfoPtr())->reason;
 switch(reason)
 {
  case REASON_DEEP_SLEEP_AWAKE:        // wakeup after deepSleep
   digitalWrite(PIN_LED_GREEN, LOW);   // LED
   digitalWrite(PIN_LED_RED, HIGH);    // red
   break;
  case REASON_EXT_SYS_RST:             // wakeup after deepSleep
   digitalWrite(PIN_LED_GREEN, HIGH);  // LED
   digitalWrite(PIN_LED_RED, LOW);     // green
   break;
  default:
   digitalWrite(PIN_LED_GREEN, HIGH);  // LED
   digitalWrite(PIN_LED_RED, HIGH);    // yellow (red+green)
 }
 delay(3000);
 //-----react to input pin D5-----------------------------------
 if(digitalRead(PIN_IN)>0)             // D5=1: turn power off
  digitalWrite(PIN_RELAIS, LOW);       // relais off
 else                                  // D5=0: deep sleep
  ESP.deepSleep(1 * 1000000L);         // start deep sleep (us)
 delay(200);                           // just to be sure 
}

//-----loop = blinking led--------------------------------------
void loop()
{
 digitalWrite(PIN_LED_GREEN, LOW);     // LED
 digitalWrite(PIN_LED_RED, LOW);       // off
 delay(500);                           // wait 0,5s
 digitalWrite(PIN_LED_GREEN, HIGH);    // LED
 digitalWrite(PIN_LED_RED, HIGH);      // yellow (red+green)
 delay(500);                           // wait 0,5s
}
