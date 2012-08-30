/************************************************************************************
 * 	
 * 	Name    : Triggertrap TTShield.h                        
 * 	Author  : Noah Shibley / NoMi Design                         
 * 	Date    : Dec 12th 2011                                    
 * 	Version : 0.1                                              
 * 	Notes   : 
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

#ifndef TTSHIELD_H
#define TTSHIELD_H


#include <PinChangeInt.h>
#include <PinChangeIntConfig.h>
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"    // for digitalRead, digitalWrite, pinMode, delayMicroseconds
#else
#include "WProgram.h"
#endif

extern "C" void bttnShieldISR(void) __attribute__ ((signal)); 

const byte MODE_BTTN_PIN = 6; //mode bttn 
const byte OPTION_BTTN_PIN = 5;//option bttn 
const byte UP_BTTN_PIN = 4; //up bttn 
const byte DOWN_BTTN_PIN = 3;  //down bttn 

class TTShield {

public:
	
 friend void bttnShieldISR(void); //make friend so bttnPressISR can access private var keyhit

TTShield();

/***********************************************************
 * 
 * begin
 *
 * init the AtTouch class
 *   
 ***********************************************************/
  void begin();

/***********************************************************
 * 
 * readActiveKey
 *
 * request the touch chip the value of the active key
 *   
 ***********************************************************/
  int readActiveKey(); 
  
/***********************************************************
 * 
 * hit
 *
 * return whether hit is true or false
 *   
 ***********************************************************/
  boolean hit();

/***********************************************************
 * 
 * setRefreshSpeed
 *
 * during press and hold, restrict how often per sec hold returns true
 *   
 ***********************************************************/
  void setRefreshSpeed(int intervalMilSec);

/***********************************************************
 * 
 * getStartTime
 *
 * get the time that the key was pressed
 *   
 ***********************************************************/
  unsigned long getStartTime();

/***********************************************************
 * 
 * getKey
 *
 * get the key number that was pressed
 *   
 ***********************************************************/
  int getKey();

/***********************************************************
 * 
 * update
 *
 * read the key value that was pressed
 *   
 ***********************************************************/
  void update();

/***********************************************************
 * 
 * hold
 *
 * return the hold status. 
 *   
 ***********************************************************/
  boolean hold();

/***********************************************************
 * 
 * clear
 *
 * clear (dettach) the interrupts for the buttons
 *   
 ***********************************************************/
  void clear();
 
  


private:

  static TTShield* pTTShield; //ptr to TTShield class for the ISR
  byte activeKey_; //the current key num pressed
  volatile boolean keyHit;
  volatile boolean holdDown_; //is the button held down, or just pressed
  volatile unsigned long startTime; //the press and hold startTime
  unsigned int updateTime; //for setting the pace of hold updates
  unsigned int holdRefreshInterval;

};

#endif







