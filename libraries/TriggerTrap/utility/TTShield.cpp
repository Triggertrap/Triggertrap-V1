/************************************************************************************
 * 	
 * 	Name    : Triggertrap TTShield.cpp                        
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

#include "TTShield.h"

TTShield* TTShield::pTTShield = 0; 

TTShield::TTShield()
{
	 pTTShield = this;	//the ptr points to this object
	
}

void TTShield::begin()
{
	
  keyHit = false; 
  holdDown_ = false;
  startTime = 0;
  holdRefreshInterval = 1; //1 for 100ms 1000ms/100 = 10 so 100ms/100 = 1

  pinMode(DOWN_BTTN_PIN, INPUT);
 digitalWrite(DOWN_BTTN_PIN,HIGH);
  PCintPort::attachInterrupt(DOWN_BTTN_PIN,bttnShieldISR,CHANGE);  //setup the key change interrupt, call bttnpress on interrupt	

  pinMode(UP_BTTN_PIN, INPUT);
  digitalWrite(UP_BTTN_PIN,HIGH);
  PCintPort::attachInterrupt(UP_BTTN_PIN ,bttnShieldISR,CHANGE);  //setup the key change interrupt, call bttnpress on interrupt

  pinMode(OPTION_BTTN_PIN, INPUT);
  digitalWrite(OPTION_BTTN_PIN,HIGH);
  PCintPort::attachInterrupt(OPTION_BTTN_PIN,bttnShieldISR,CHANGE);  //setup the key change interrupt, call bttnpress on interrupt

  pinMode(MODE_BTTN_PIN, INPUT);
  digitalWrite(MODE_BTTN_PIN,HIGH);
  PCintPort::attachInterrupt(MODE_BTTN_PIN,bttnShieldISR,CHANGE);  //setup the key change interrupt, call bttnpress on interrupt
}

/***********************************************************
 * 
 * hit
 * 
 ***********************************************************/
boolean TTShield::hit()
{
  
  return keyHit;
}

/***********************************************************
 * 
 * getStartTime
 * 
 ***********************************************************/
unsigned long TTShield::getStartTime()
{
	return startTime;	
}

/***********************************************************
 * 
 * setRefreshSpeed
 * 
 ***********************************************************/
void TTShield::setRefreshSpeed(int intervalMilSec)
{
	holdRefreshInterval = intervalMilSec/100;
}

/***********************************************************
 * 
 * update
 * 
 ***********************************************************/
void TTShield::update()
{
	if (keyHit == true)
	{
		activeKey_ = readActiveKey();
		
	}
	
}

/***********************************************************
 * 
 * getKey
 * 
 ***********************************************************/
int TTShield::getKey()
{
	return (int) activeKey_;
}

/***********************************************************
 * 
 * readActiveKey
 * 
 ***********************************************************/
int TTShield::readActiveKey()
{
  byte keyValue = 0;

  keyHit = false; //so it will only count 1 hit for each press
 
  if(PCintPort::arduinoPin == DOWN_BTTN_PIN)
  {
	keyValue = 5; //value used in TTUI matches values from ATTouch CapSense chip
  }
  else if(PCintPort::arduinoPin == UP_BTTN_PIN)
  {
	keyValue = 6;
  }
  else if(PCintPort::arduinoPin == OPTION_BTTN_PIN)
  {
	keyValue = 3;
  }
  else if(PCintPort::arduinoPin == MODE_BTTN_PIN)
  {
	keyValue = 2;
  }
  
  if(digitalRead(PCintPort::arduinoPin) == HIGH) //if the pin that just changed is now HIGH (not pressed)
  {
	keyValue = 9; //Nothing pressed
	
  }
  
  activeKey_ = (byte) keyValue; //save the keyValue for later
  if(keyValue == 9 || keyValue == 0) { holdDown_ = false; } //9 is key up, so stop press and hold timer
  
  return keyValue;
  
  
}

/***********************************************************
 * 
 * hold
 * 
 ***********************************************************/
boolean TTShield::hold()
{
  if(holdDown_ == true)
  {
	  unsigned long elapsedTime = millis() - startTime;

	  if (elapsedTime >= 1100) //hold interval time has passed
	  {
		
		unsigned int holdRefreshTime = millis()/100 - updateTime;
		
		if(holdRefreshTime >= holdRefreshInterval) //restrict update 100ms refresh
		{
			updateTime = millis()/100; //divide by 100, lose some precision, 100ms minimum
			return true;
		}
		else //don't update too fast
		{
			return false; 
		}
		
		 
	  }
	  else //haven't held down long enough
	  {
		return false;
	  }
   }
   else //not holding down at all
   {
	    return false; 
   }	
}

/***********************************************************
 * 
 * clear
 * 
 ***********************************************************/
void TTShield::clear()
{
	PCintPort::detachInterrupt(DOWN_BTTN_PIN);
	PCintPort::detachInterrupt(UP_BTTN_PIN);
	PCintPort::detachInterrupt(OPTION_BTTN_PIN);
	PCintPort::detachInterrupt(MODE_BTTN_PIN);
}


/***********************************************************
 * 
 * bttnShieldISR
 * 
 ***********************************************************/
void bttnShieldISR()
{
	TTShield::pTTShield->keyHit = true;
	TTShield::pTTShield->holdDown_ = true;
	TTShield::pTTShield->startTime = millis(); //start hold time,TODO figure out how to do this without reading a function
}	