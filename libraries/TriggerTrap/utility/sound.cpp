/************************************************************************************
 * 	
 * 	Name    : Triggertrap Sound.cpp                         
 * 	Author  : Noah Shibley / NoMi Design                         
 * 	Date    : July 10th 2011                                    
 * 	Version : 0.1                                              
 * 	Notes   : The sound input mic. Sub class of Sensor, inherits all Sensor functions           
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

#include "sound.h"



//Menu Sensor Strings

   //Mode Menu Listing
   const char PROGMEM soundMenu[]= {"Sound"};
   
	 //Option Menu default
	const char PROGMEM soundThreshold[] = "threshld";
	const char PROGMEM soundDelay[]="delay";

	const char PROGMEM * const soundOptionMenu[]  = 	   //options menu
	{   
	soundThreshold,	
	soundDelay,
	};

  Sound::Sound() 
  {
	maxOptionMenu = 2;
    triggerState_ = false; //off
    abortTrigger = false; 
	setOption(TRIG_TYPE,2);
	setOption(TRIG_THRESHOLD,0);
	setOption(TRIG_DELAY,0);
	select_ = 1; 
    sensorPin_ = SOUND_IN;
	focusPulseTime_ = DEFAULT_FOCUS_TIME;
	shutterPulseTime_ = DEFAULT_SHUTTER_TIME;
	focusArmed = false; //focus on/off default
	shutterArmed = true; //shutter on/off default
	IRShutter_ = false;  //IR on/off default
	

 }

  int Sound::sensorLevel()
  {
   	sensorLevel_ = analogRead(sensorPin_) >> 2;
	return sensorLevel_;
  }

  boolean Sound::trigger()
  {
    boolean soundStatus = false;

	shutter(true);
 
    soundStatus = rise();
    
    if(soundStatus == true)
    {
		delayCount = millis(); //start counting till delay is up
		startBttnTime = delayCount; //don't call millis twice, just use delayCount, same value.
		focusReady = true; 
		shutterReady = true;
		IRReady = true; 
		return soundStatus;
	}	
	else
	{
    	return soundStatus;
	}
		
  }

  //to change the behavior of the following functions for the mic, edit here. 
  //the current functional behavior is in the base trigger class.
  //Or add a new function here, to customize mic sensor
  /*
	boolean Sound::change()
   	{
   	
   	}
   	*/

/***********************************************************
 * 
 * getOptionMenu
 *
 *  
 * 
 ***********************************************************/
void Sound::getOptionMenu(char buffer[])
{
	 //reads the timeSelectMenu options from flash memory
	 strcpy_P(buffer, (const char PROGMEM *)pgm_read_word(&(soundOptionMenu[select_])));
}

void Sound::getModeMenu(char buffer[])
{
	 strcpy_P(buffer, soundMenu); 
}

/***********************************************************
 * 
 * getActiveMessage
 *
 * get the current sensors LCD message to print during trap Active mode.
 * 
 ***********************************************************/
void Sound::getActiveMessage(char buffer[])
{
	buffer[0] = 0;
	
	itoa (sensorLevel(),buffer,10);
	
	
}



