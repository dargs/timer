/* 
 dargs 20180325
 
 ardunio v1.6.4
 Tested on Uno board.

 Countdown clock to sync with artwork playing off brightsign player. To let visitors know when the artwork is due to start again. 
 On power up, timer=0
 loop counts down
 when digital input 2 = HIGH then clock = 1 hour.
 
 
 thanks to the post  http://forum.arduino.cc/index.php?topic=262478.0 
 @BulldogLowell @gvictor00 @robtillaart @Delta_G
 for the timer code !

 * https://playground.arduino.cc/Main/LedControl 
 * We use pins 12,11 and 10 on the Arduino for the SPI interface
 * Pin 12 is connected to the DATA IN-pin of the first MAX7221
 * Pin 11 is connected to the CLK-pin of the first MAX7221
 * Pin 10 is connected to the LOAD(/CS)-pin of the first MAX7221 	
 * There will only be a single MAX7221 attached to the arduino 
*/

#include "LedControl.h"
LedControl lc=LedControl(12,11,10,1); 
unsigned long startTime;
int min = 0;
int sec = 3600; // 3600 seconds in an hour. this is what the counter counts down from. 
int displayMin = 0;
int displaySec = 0;
int starttime = 0;

int triggerPin = 2;
int triggerState = 0;
int m1, m2,s1, s2; // the indivdual digit value for 4 digits

void setup() 
{
  Serial.begin(9600); // usb serial port debug
  lc.shutdown(0,false);   //wake up the MAX72XX from power-saving mode 
  lc.setIntensity(0,15);  //set a high brightness for the Leds
  pinMode(triggerPin, INPUT); // install 10k pull-up resistor so by default input is HIGH, until grounded then becomes LOW to reset.
}

void loop()  // Clock is always counting down.. if Pin 2 is LOW, then clock resets to 1 hour.
{
  triggerState = digitalRead(triggerPin);
  if (triggerState == LOW) {   
    sec = 3600; 
  } 
  
  if (millis() - startTime >= 1000)
    {
      sec--;
      startTime += 1000;
      displayMin = sec/60;
      displaySec = sec % 60;
      debug(displaySec,displayMin);
      displayTime(displaySec,displayMin);
    }
}

void displayTime(int seconds, int minutes)
{
  if (minutes < 10)
  {
  m2=0; //First minute LED d1splays zero 
  m1=minutes; //2nd digit of minutes
  }
  else
  {
  m2=minutes /10;
  m1=minutes % 10;
  }

  //Seconds are displayed by s1 and s2
  if (seconds < 10)
  {
  s2=0; 
  s1=seconds; 
  }
  else
  {
  s2=seconds/10;
  s1=seconds % 10;
  }
  
   // Send values to 7219 via SPI using LED Control library
    lc.setDigit(0,5,0,false);
    lc.setDigit(0,4,0,false);
    lc.setDigit(0,3,(byte)m2,false);
    lc.setDigit(0,2,(byte)m1,false);
    lc.setDigit(0,1,(byte)s2,false);
    lc.setDigit(0,0,(byte)s1,false);
}

void debug(int seconds, int minutes)
{
  if (seconds<0) Serial.print("negative");
  Serial.print(" Minutes: ");
  Serial.print(minutes);
  Serial.print(" Seconds: ");
  Serial.println(seconds);
}

