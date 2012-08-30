/************************************************************************************
 * 	
 * 	Name    : Triggertrap HWTest.h                       
 * 	Author  : Noah Shibley / NoMi Design                       
 * 	Date    : July 10th 2011                                    
 * 	Version : 0.1                                              
 * 	Notes   : HardWare Test Mode for factory QA.                 
 * 
 * 			  Code for Triggertrap device (TTv1) and Triggertrap Shield (TT-k)                      
 * 
 * 	Copyright (c) 2011 NoMi Design (http://n0m1.com) All right reserved.
 * 
 * 	This file is part of Triggertrap. See Triggertrap.com for more information.
 * 
 * 		    Triggertrap is free software: you can redistribute it and/or modify
 * 		    it under the terms of the GNU General Public License as published by
 * 		    the Free Software Foundation, either version 3 of the License, or
 * 		    (at your option) any later version.
 * 
 * 		    Triggertrap is distributed in the hope that it will be useful,
 * 		    but WITHOUT ANY WARRANTY; without even the implied warranty of
 * 		    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * 		    GNU General Public License for more details.
 * 
 * 		    You should have received a copy of the GNU General Public License
 * 		    along with Triggertrap.  If not, see <http://www.gnu.org/licenses/>.
 * 
 ***********************************************************************************/

// To enable the hardware test mode, uncomment the appropriate line in TTConfig.h

#ifndef HWTEST_H
#define HWTEST_H


#include <avr/sleep.h>

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"    // for digitalRead, digitalWrite, pinMode, delayMicroseconds
#else
#include "WProgram.h"
#endif

#include <TTConfig.h>
  #include <LCD.h>
#ifdef TT_SHIELD
  #include <LiquidCrystal_SR_LCD3.h>
  #include <PinChangeInt.h>
  #include <PinChangeIntConfig.h>
  #include "utility/TTShield.h"
#else
  #include <LiquidCrystal.h>
  #include <AtTouch.h>
#endif
#include <utility/trigger.h>

const byte MODE_BTTN = 2; //mode bttn address
const byte SELECT_BTTN = 3;//select bttn address
const byte UP_BTTN = 6; //up bttn address
const byte DOWN_BTTN = 5;  //down bttn address

extern "C" void startTestHandler(void) __attribute__ ((signal));  //ISR function for interrupt 

#ifdef TT_SHIELD
class HWTest: public LiquidCrystal_SR_LCD3 {
#else
class HWTest: public LiquidCrystal {
#endif

public:


HWTest();

friend void startTestHandler(void); //make the ISR a friend of this class

/***********************************************************
 * 
 * begin
 *
 * pass every Trigger object to the UI
 * 
 ***********************************************************/
void setup(Trigger& laser, Trigger& sound, Trigger& light, Trigger& timeLapse, Trigger& aux );

/***********************************************************
 * 
 * update
 *
 * does all UI stuff necessary each loop
 * 
 ***********************************************************/
void update();

/***********************************************************
 * 
 * trigger
 *
 * returns which trigger object has been selected by the UI (mode button)
 * 
 ***********************************************************/
int trigger(){ return (int) currentTrigger; }

/***********************************************************
 * 
 * trapActive
 *
 * returns the status of whether or not the start button has been pressed
 * 
 ***********************************************************/
boolean trapActive() { return trapActive_; }

/***********************************************************
 * 
 * uiPowerOn
 *
 * Turn on power to the keys and the LCD screen
 * 
 ***********************************************************/	
	void uiPowerOn();
	
/***********************************************************
 * 
 * batteryPower
 *
 * returns true if the device is running on battery power
 * 
 ***********************************************************/	
boolean batteryPower();

/***********************************************************
 * 
 * keyDown
 *
 * returns key number hit
 * 
 ***********************************************************/
int keyDown();

/***********************************************************
 * 
 * updateLCD
 *
 * print to LCD and update every 300ms
 * 
 ***********************************************************/
void updateLCD(char line1[],char line2[]);	
		

private:
	
	  int testState;
	  boolean minThresh;
	  int currentTrigger;
	  boolean trapActive_;
	  boolean state_UIPower;	//UI power on, or off flag
	  boolean onBatteryPower;	
	  unsigned long activeRefreshTime; //used for lcd refresh
	

	
	
	  static HWTest* pTTUI; //ptr to TTUI class for the ISR

	  #ifdef TT_SHIELD
		TTShield touch;
	  #else
	  	AtTouch touch;  //touch sensor library object
	  #endif
	
	  Trigger *triggers[5]; //array of trigger object pointers
	  
	  volatile boolean startPressed; //is the button held down or not? 

 
};

#endif 


