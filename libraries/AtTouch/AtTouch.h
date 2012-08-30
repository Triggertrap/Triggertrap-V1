/************************************************************************************
 * 	
 * 	Name    : AtTouch                         
 * 	Author  : Noah Shibley, Michael Grant NoMi Design  nomidesign.net                        
 * 	Date    : July 18th 2011                                    
 * 	Version : 0.1                                              
 * 	Notes   : Arduino Library for use with the AT42QT1070 Q-touch chip via i2c                       
 * 
 * 	Copyright (c) 2011 NoMi Design All right reserved.
 * 
 * 	This file is part of AtTouch.
 * 
 * 		    AtTouch is free software: you can redistribute it and/or modify
 * 		    it under the terms of the GNU General Public License as published by
 * 		    the Free Software Foundation, either version 3 of the License, or
 * 		    (at your option) any later version.
 * 
 * 		    AtTouch is distributed in the hope that it will be useful,
 * 		    but WITHOUT ANY WARRANTY; without even the implied warranty of
 * 		    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * 		    GNU General Public License for more details.
 * 
 * 		    You should have received a copy of the GNU General Public License
 * 		    along with AtTouch.  If not, see <http://www.gnu.org/licenses/>.
 * 
 ***********************************************************************************/


#ifndef AtTouch_H
#define AtTouch_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"    // for digitalRead, digitalWrite, pinMode, delayMicroseconds
#else
#include "WProgram.h"
#endif
#include <I2C.h>

//#define SERIAL_DEBUG  //do you want status messages printed to serial? 


extern "C" void bttnPressISR(void) __attribute__ ((signal)); 


class AtTouch {

public:
  friend void bttnPressISR(void); //make friend so bttnPressISR can access private var keyhit	
 
  AtTouch();
  
/***********************************************************
 * 
 * begin
 *
 * init the AtTouch class
 *   
 ***********************************************************/
  void begin(int interruptPin,boolean disableAutoCal=true);

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

private:

  int readActiveAddress();	
	
  byte _changePin;
  byte _interruptVal;
  volatile boolean keyHit;
  static AtTouch* pAtTouch; //ptr to AtTouch class for the ISR
  byte activeKey_; //the current key num pressed
  volatile boolean holdDown_; //is the button held down, or just pressed
  volatile unsigned long startTime; //the press and hold startTime
  unsigned int updateTime; //for setting the pace of hold updates
  unsigned int holdRefreshInterval; 

};

#endif

