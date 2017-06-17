/*
 * BB-420 Rev 2D Interface
 * 17 July 2017
 * 
 * 
 * 
 */

#include <Wire.h>
#include "RTClib.h"
#include "libs/mybutton.h"
#include <LCD.h>
#include <LiquidCrystal_I2C.h> // includes the LiquidCrystal Library
LiquidCrystal_I2C  lcd(0x27,2,1,0,4,5,6,7); // 0x27 is the I2C bus address for an unmodified backpack
RTC_DS1307 RTC;


#define MODE_AUTO      0
#define MODE_MANUAL    1
#define MODE_SET_SPEED 2
#define MODE_SET_DELAY 3
#define MODE_SET_TIME  4
#define MODE_SET_DATE  5
#define MODE_SET_SCHED 6

String modeTxt[] = {"Auto","Manual","Speed","Delay","Time","Date","Sched."};


int xpos = 0;
int speedpps = 100;
int mode = 4;
unsigned long limitDelay = 30;



// Setup buttons
Button butA(2);
Button butB(3);
Button butC(4);
Button butD(5);

void setup() {
  // Setup LCD Module
  lcd.begin(16,2); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display
  createChars();
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(1);
  lcd.noCursor();

  // Setup Real Time Clock Module
  Wire.begin();
  RTC.begin();
  if (! RTC.isrunning()) {
    //Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  
  //Serial.begin(9600);
  
  
  
}

void loop() {
  if(butA.updateButton()){
    mode ++;
    if(mode > 6){
      mode = 0; 
    }
  }
  if(butB.updateButton()){
    if(mode == MODE_MANUAL){
      xpos --;
    }
    if(mode == MODE_SET_SPEED){
      speedpps --;
    }
    if(mode == MODE_SET_DELAY){
      limitDelay --;
    }
  }
  if(butC.updateButton()){
    if(mode == MODE_MANUAL){
      xpos ++;
    }
    if(mode == MODE_SET_SPEED){
      speedpps ++;
    }
    if(mode == MODE_SET_DELAY){
      limitDelay ++;
    }
  }
  butD.updateButton();
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(modeTxt[mode]);
  /*
  lcd.print(" ");
  lcd.print(butA.state()==LOW ? "A" : " ");
  lcd.print(butB.state()==LOW ? "B" : " ");
  lcd.print(butC.state()==LOW ? "C" : " ");
  lcd.print(butD.state()==LOW ? "D" : " ");
  */
  //lcd.setCursor(0,1);
  
  if(mode == MODE_AUTO ||mode == MODE_MANUAL){
    lcd.print(" X: ");
    lcd.print(xpos);
  }
  if(mode == MODE_SET_SPEED){
    lcd.print(": ");
    lcd.print(speedpps);
  }
  if(mode == MODE_SET_DELAY){
    lcd.print(": ");
    lcd.print(limitDelay);
  }
  if(mode == MODE_SET_TIME){
    lcd.print(": ");
    printTime();
  }
  if(mode == MODE_SET_DATE){
    lcd.print(": ");
    printDate();
  }
  //Serial.println(butA.state());
  delay(10);
}

void printTime(){
  DateTime now = RTC.now();
  lcd.print(padNum(now.hour()));
  lcd.print(':');
  lcd.print(padNum(now.minute()));
  lcd.print(':');
  lcd.print(padNum(now.second()));
}

void printDate(){
  DateTime now = RTC.now();
  //lcd.print(now.year(), DEC);
  //lcd.print('/');
  lcd.print(now.month(), DEC);
  lcd.print('/');
  lcd.print(now.day(), DEC);
  lcd.print(' ');
  lcd.print(dayString(now.dayOfWeek()));
}

String dayString(int dayNumber)
{
  String dayArray[] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
  return dayArray[dayNumber];
}

String padNum(int num){
  String numString = String(num);
  if(num<10){
    numString = "0" + numString;
  }
  return numString;
}

// Utility to create some custom characters for the LCD
void createChars(){
  byte c_up[8] = {
    B00100,
    B01110,
    B10101,
    B00100,
    B00100,
    B00100,
    B00100,
  };
  byte c_down[8] = {
    B00100,
    B00100,
    B00100,
    B00100,
    B10101,
    B01110,
    B00100,
  };
  byte c_left[8] = {
    B00000,
    B00100,
    B01000,
    B11111,
    B01000,
    B00100,
    B00000,
  };
  byte c_right[8] = {
    B00000,
    B00100,
    B00010,
    B11111,
    B00010,
    B00100,
    B00000,
  };
  byte c_bulb_on[8] = {
    B01110,
    B10001,
    B10001,
    B10001,
    B01110,
    B01010,
    B01110,
  };
  byte c_bulb_off[8] = {
    B01110,
    B11111,
    B11111,
    B11111,
    B01110,
    B01010,
    B01110,
  };
  lcd.createChar(0, c_up);
  lcd.createChar(2, c_down);
  lcd.createChar(1, c_left);
  lcd.createChar(3, c_right);
  lcd.createChar(4, c_bulb_on);
  lcd.createChar(5, c_bulb_off);
}

