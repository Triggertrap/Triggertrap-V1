/************************************************************************************
 * 	
 * 	Name    : Triggertrap TTUI.h                        
 * 	Author  : Noah Shibley / NoMi Design                       
 * 	Date    : July 10th 2011                                    
 * 	Version : 0.1                                              
 * 	Notes   :  All the interface stuff for trigger trap, buttons and screen menus                 
 * 
 * 			Code for Triggertrap device (TTv1) and Triggertrap Shield (TT-k)                      
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

#ifndef TTUI_H
#define TTUI_H


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

const byte START_MESSAGE = 0; //startup screen
const byte MODE_MENU= 1; //mode menu 
const byte OPTION_MENU = 2;//option menu
const byte UP_MENU = 3; //up menu
const byte DOWN_MENU = 4;  //down menu
const byte SETTING_MENU = 5; //system menu

const byte SYSTEM_NUM_OPTIONS = 4; //total number of system menu options

extern "C" void startDownHandler(void) __attribute__ ((signal));  //ISR function for interrupt 
extern "C" void startUpHandler(void) __attribute__ ((signal));  //ISR function for interrupt 
extern "C" void sleepHandler(void) __attribute__ ((signal));  //ISR function for interrupt 

#ifdef TT_SHIELD
class TTUI: public LiquidCrystal_SR_LCD3 {
#else
class TTUI: public LiquidCrystal {
#endif

public:


TTUI();

friend void startDownHandler(void); //make the ISR a friend of this class
friend void startUpHandler(void); //make the ISR a friend of this class

/***********************************************************
 * 
 * begin
 *
 * pass every Trigger object to the UI
 * 
 ***********************************************************/
void setup(Trigger *triggers[]);

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
 * uiPowerOff
 *
 * Turn off power to the keys and the LCD screen
 * 
 ***********************************************************/	
	void uiPowerOff();
	
/***********************************************************
 * 
 * batteryPower
 *
 * returns true if the device is running on battery power
 * 
 ***********************************************************/	
boolean batteryPower();
		

private:
	
	  volatile boolean trapActive_;	//trigger was activated on start button press
	  byte currentTrigger;   //which trigger (mode) is active
	  byte activeMenu; //which menu is being displayed? Mode, Option, Inc, Dec
	  byte incSystemOption; //increment system option index value
	  boolean systemSetting; //value of current system setting. on/off
	  
	  byte systemMenu; //index value of the system menu.

	  int lcdContrast; //lcd contrast value 0-255
	
	  static TTUI* pTTUI; //ptr to TTUI class for the ISR

	  #ifdef TT_SHIELD
		TTShield touch;
	  #else
	  	AtTouch touch;  //touch sensor library object
	  #endif
	
	  Trigger *triggers[NUM_OF_SENSORS]; //array of trigger object pointers
	
	  
	  int activeRefreshTime; //time counter for refresh of LCD, during trigger active
	  unsigned long 	previousMillis_UIPower; //time counter for powering down the leds and screen
	  
	  volatile boolean startBttnHold; //is the button held down or not? 
	  volatile unsigned int holdBttnStart; //time in sec the bttn was first held down.
	  boolean state_UIPower;	//UI power on, or off flag
	  boolean onBatteryPower; //using battery power or usb?
	  volatile unsigned long prevIntTime;  //debounce
 
/***********************************************************
 * 
 * systemCheck
 *
 *
 * 
 ***********************************************************/
void systemCheck();

/***********************************************************
 * 
 * updateLCD
 *
 * handles updating the LCD display every 300ms
 * 
 ***********************************************************/
void updateLCD();

/***********************************************************
 * 
 * bttnMode
 *
 * increments the Mode to the next trigger object
 * 
 ***********************************************************/
	void bttnMode();  
	
/***********************************************************
 * 
 * bttnSelect
 *
 * increments the Option menu to the next select option
 * 
 ***********************************************************/
	void bttnOption();

/***********************************************************
 * 
 * bttnUp
 *
 * increments the selected option value
 * 
 ***********************************************************/
	void bttnUp(boolean hold);

/***********************************************************
 * 
 * bttnDown
 *
 * decrements the selected option value
 * 
 ***********************************************************/
	void bttnDown(boolean hold);
 
/***********************************************************
 * 
 * uiPowerTimeOut
 *
 * Turn off power to the keys and the LCD screen, after a certain time
 * 
 ***********************************************************/
void uiPowerTimeOut();

/***********************************************************
 * 
 * getSystemModeMenu
 *
 * fills the char buffer with the text to be displayed by the system mode menu
 * 
 ***********************************************************/
void getSystemModeMenu(char buffer[]);

/***********************************************************
 * 
 * getSystemOptionMenu
 *
 * fills the char buffer with the text to be displayed by the system option menu
 * 
 ***********************************************************/
void getSystemOptionMenu(char buffer[]);

/***********************************************************
 * 
 * setSystemSettingMenu
 *
 * fills the char buffer with the text to be displayed by 
 *  the system setting values, and sets the values
 * 
 ***********************************************************/
void setSystemSettingMenu(char buffer[],int change);
                      
/***********************************************************
 * 
 * printMode
 *
 * print Mode Menu Setting to LCD Display
 * 
 ***********************************************************/
void printMode(int row);

/***********************************************************
 * 
 * printSelect
 *
 * print Option Menu Setting to LCD Display
 * 
 ***********************************************************/
void printSelect(int row);

/***********************************************************
 * 
 * printInc
 *
 * print Inc Menu Setting to LCD Display
 * 
 ***********************************************************/
void printInc(int row, int incVal);

/***********************************************************
 * 
 * printDec
 *
 * print Dec Menu Setting to LCD Display
 * 
 ***********************************************************/
void printDec(int row, int decVal);

/***********************************************************
 * 
 * initStart
 *
 * This is called by the start button ISR. 
 * 
 ***********************************************************/
void initStart(unsigned long startTime);

/***********************************************************
 * 
 * resetCheck
 *
 * check to see if the start button is being held down, for eeprom reset 
 * 
 ***********************************************************/
void resetCheck();

};

#endif 


