/************************************************************************************
 * 	
 * 	Name    : Triggertrap light.cpp                         
 * 	Author  : Noah Shibley / NoMi Design                         
 * 	Date    : July 10th 2011                                    
 * 	Version : 0.1                                              
 * 	Notes   : The ambient light sensor. Sub class of Sensor, inherits all Sensor functions           
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

#include "light.h"


int const LIGHT_RISE_MODE = 0;
int const LIGHT_FALL_MODE = 1;
int const LIGHT_CHANGE_MODE = 2;

//Menu Sensor Strings

   //Mode Menu Listing
   const char PROGMEM lightMenu[]= {"Light"};

  Light::Light() 
  {
	maxOptionMenu = 3;
    triggerState_ = false; //off
	abortTrigger = false; 
	setOption(TRIG_TYPE,0);
	setOption(TRIG_THRESHOLD,0);
	setOption(TRIG_DELAY,0);
	select_ = 0; 
    sensorPin_ = AMBIENT_LIGHT_SENSOR;
	focusPulseTime_ = DEFAULT_FOCUS_TIME;
	shutterPulseTime_ = DEFAULT_SHUTTER_TIME;
	focusArmed = false; //focus on/off default
	shutterArmed = true; //shutter on/off default
	IRShutter_ = false;  //IR on/off default
	
};
 
  

  int Light::sensorLevel()
  {
    sensorLevel_ = analogRead(sensorPin_) >> 2;
	return sensorLevel_;
  }

  boolean Light::trigger()
  {
    boolean lightStatus = false;

	shutter(true); 
	

    switch (option(TRIG_TYPE))
    {
    case LIGHT_RISE_MODE:

      lightStatus = rise();
      break;
    case LIGHT_FALL_MODE:

      lightStatus = fall();
      break;
    case LIGHT_CHANGE_MODE:

      lightStatus = change();
      break;
    default: //no default option
      break;
    }

	if(lightStatus == true)
    {
		delayCount = millis(); //start counting till delay is up
		focusReady = true; 
		shutterReady = true;
		IRReady = true; 
		return lightStatus;
	}	
	else
	{
    	 return lightStatus;
	}
   
  }

  //To change the behavior of these functions for the light sensor, edit here
  //Or add a new function here, to customize light sensor
  /*
	boolean Light::high()
   	{
   	
   	}
   	*/

  /*
	boolean Light::low()
   	{
   	
   	}
   	*/
  /*
	boolean Light::change()
   	{
   	
   	}
   	*/
	void Light::getModeMenu(char buffer[])
  {
	 strcpy_P(buffer, lightMenu); 

  }

/***********************************************************
 * 
 * getActiveMessage
 *
 * get the current sensors LCD message to print during trap Active mode.
 * 
 ***********************************************************/
void Light::getActiveMessage(char buffer[])
{
	buffer[0] = 0;
	
	itoa (sensorLevel(),buffer,10);
	
	
}
