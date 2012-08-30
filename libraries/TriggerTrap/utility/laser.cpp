/************************************************************************************
 * 	
 * 	Name    : Triggertrap laser.cpp
 * 	Author  : Noah Shibley / NoMi Design                         
 * 	Date    : July 10th 2011                                    
 * 	Version : 0.1                                              
 * 	Notes   : The laser break beam sensor. Sub class of Sensor, inherits all Sensor functions           
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

#include "laser.h"


int const MAKE_LASER_MODE = 0;
int const BREAK_LASER_MODE = 1;
int const CHANGE_LASER_MODE = 2;

   //Menu Sensor Strings

   //Mode Menu Listing
   const char PROGMEM laserMenu[]= {"Laser"};

	 //Option Menu default
	const char PROGMEM laserType[] = "type";
	const char PROGMEM laserDelay[]="delay";

	const char PROGMEM * const laserOptionMenu[]  = 	   //options menu
	{   
	laserType,	
	laserDelay,
	};


 const char PROGMEM onMake[]="On Make";
 const char PROGMEM onBreak[]="On Break";	
 const char PROGMEM onChange[]="On Chnge";


const char PROGMEM * const laserSettingMenu[]  = 	   // setting menu options
{   
	onMake,onBreak,onChange
};

const int LASER_TYPE = 0;
const int LASER_DELAY = 1;
const int LASER_THRESHOLD = 2; 

  Laser::Laser() 
  {
	maxOptionMenu = 2; 
    triggerState_ = false; //off
	abortTrigger = false; 
	setOption(LASER_TYPE,0);
	setOption(LASER_DELAY,0);
	setOption(LASER_THRESHOLD,250);
	select_ = 0; 
    sensorPin_ = LASER_SENSOR;
	focusPulseTime_ = DEFAULT_FOCUS_TIME;
	shutterPulseTime_ = DEFAULT_SHUTTER_TIME;
	focusArmed = false; //focus on/off default
	shutterArmed = true; //shutter on/off default
	IRShutter_ = false;  //IR on/off default
	
  }

  int Laser::sensorLevel()
  {
	  
	 // analogReference(DEFAULT);
	  sensorLevel_ = analogRead(sensorPin_) >> 2;
	 // analogReference(INTERNAL);
	  return sensorLevel_;
  }

  boolean Laser::trigger()
  {
	
	
	setOption(LASER_THRESHOLD,250); //just set laser to always be 128. Its really the safest value
   
 	boolean laserStatus = false;

	shutter(true); 


    switch (option(LASER_TYPE))
    {
    case MAKE_LASER_MODE:

      laserStatus = rise();
      break;
    case BREAK_LASER_MODE:

      laserStatus = fall();
      break;
    case CHANGE_LASER_MODE:

      laserStatus = change();
      break;
    default: //no default option
      break;
    }

    if(laserStatus == true)
    {
		delayCount = millis(); //start counting till delay is up
		focusReady = true; 
		shutterReady = true;
		IRReady = true; 
		return laserStatus;
	}	
	else
	{
    	return laserStatus;
	}

  }

  //to change the behavior of these functions for laser, edit here
  //Or add a new function here, to customize laser sensor
  
boolean Laser::rise()
{
boolean changed = change();

if(changed == true) //trigger changed state
{
if(sensorLevel_ > option(LASER_THRESHOLD)) //sesnor data being received, any value above threshold
{
return true;

}
else //sensor, 0 value
{
return false;
}
}
else //sensor didn't change state, still high or low...
{
return false;
}

}

boolean Laser::fall()
{
boolean changed = change();

if(changed == true) //sensor changed state
{
if(sensorLevel_ <= option(LASER_THRESHOLD)) //sensor low, stopped
{
return true;

}
else //sensor recieving data
{
return false;
}
}
else //sensor didn't change state
{
return false;
}

}
  
  
  
  
  
boolean Laser::change()
{
boolean state = triggerState_;
int threshold = option(LASER_THRESHOLD); //get setting option 2 threshold

//analogReference(DEFAULT);
sensorLevel_ = analogRead(sensorPin_) >> 2; //shift 1024 to 255
//analogReference(INTERNAL);

//state is high, or state is low, depending on change above or below threshold
if(sensorLevel_ > threshold) //true above
{
state = true;

}
else if(sensorLevel_ <= threshold) //false below
{
state = false;
}

//trigger either on or off, not measuring soundValue
if(state != triggerState_)
{
triggerState_ = state;
return true; //changed its status above or below threshold

}
else //nothing changing here
{
return false;
}
}


/***********************************************************
 * 
 * decSetting
 *
 * 
 * 
 ***********************************************************/
void Laser::decSetting(char buffer[], int dec)
{

	
		switch (select_)
	    {
	     case LASER_TYPE:
	 	  decOption(LASER_TYPE, 2,dec);
		  getSettingMenu(buffer); 
	      break;
	    case LASER_DELAY:
	      decOption(LASER_DELAY, 9999,dec);
	      if(option(LASER_DELAY) == 0) //delay 0 is off
		  {
				buffer[0] = 0;
				strcat(buffer,"Off");
				strcat(buffer,"\0");
		  }
		  else
		  {
	      	formatMSString(option(LASER_DELAY),buffer);
 	  	  }
	      break;
	    default: 
	      break;
	    }

}

/***********************************************************
 * 
 * incSetting
 *
 * 
 * 
 ***********************************************************/
void Laser::incSetting(char buffer[], int inc)
{
	
	
		switch (select_)
	    {
	     case LASER_TYPE:
	 	  incOption(LASER_TYPE, 2,inc);
		  getSettingMenu(buffer); 
	      break;
	    case LASER_DELAY:
	      incOption(LASER_DELAY, 9999,inc);
		  if(option(LASER_DELAY) == 0) //delay 0 is off
		  {
				buffer[0] = 0;
				strcat(buffer,"Off");
				strcat(buffer,"\0");
		  }
		  else
		  {
		  	formatMSString(option(LASER_DELAY),buffer);
	 	  }
	      break;
	    default: 
	      break;
	    }
		
}


  

void Laser::getModeMenu(char buffer[])
{

	strcpy_P(buffer, laserMenu); 
	
}

/***********************************************************
 * 
 * getOptionMenu
 *
 *  
 * 
 ***********************************************************/
void Laser::getOptionMenu(char buffer[])
{
	 //reads the timeSelectMenu options from flash memory
	 strcpy_P(buffer, (const char PROGMEM *)pgm_read_word(&(laserOptionMenu[select_])));
}

void Laser::getSettingMenu(char buffer[])
{
	strcpy_P(buffer, (const char PROGMEM *)pgm_read_word(&(laserSettingMenu[option(LASER_TYPE)]))); 
}

/***********************************************************
 * 
 * getActiveMessage
 *
 * get the current sensors LCD message to print during trap Active mode.
 * 
 ***********************************************************/
void Laser::getActiveMessage(char buffer[])
{
	buffer[0] = 0;
	
	itoa (sensorLevel(),buffer,10);
	
	
}


