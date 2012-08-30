/************************************************************************************
 * 	
 * 	Name    : Triggertrap auxiliary.cpp                         
 * 	Author  : Noah Shibley / NoMi Design                         
 * 	Date    : July 10th 2011                                    
 * 	Version : 0.1                                              
 * 	Notes   : The aux sensor port. Sub class of Sensor, inherits all Sensor functions           
 * 
 * 			  Code for Triggertrap device (TTv1) and Triggertrap Shield (TT-k)                      
 * 
 * 	Copyright (c) 2012 NoMi Design (http://n0m1.com) All right reserved.
 * 
 * 	This file is part of Triggertrap. See Triggertrap.com for more information.
 *
 *  For more information about the Auxiliary port on the TTv1 and TT-k devices,
 *   including information about the voltages and types of signal the Aux mode
 *   expects in order to be able to function correctly,  please refer to the 
 *   Aux mode in the Triggertrap user manual. 
 *   URL: http://triggertrap.com/user-manual/#AuxMode
 *
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

#include "auxiliary.h"

/************************************************************************************
 * There are three different 'modes' for the auxiliary sensor. 
 * 'on rise' triggers whenever the signal goes up through the threshold level. 
 * 'on fall' triggers whenever the signal goes down through the threshold level. 
 * 'on change' triggers whenever the signal crosses the threshold level
 *  (so basically, 'on change' is both 'on rise' and 'on fall')
 *
 * These modes are defined below. 
 ***********************************************************************************/

//menu listing values
int const AUX_RISE_MODE = 0;
int const AUX_FALL_MODE = 1;
int const AUX_CHANGE_MODE = 2;

//Mode Menu Listing, saved in flash mem
const char PROGMEM auxMenu[] = {"Aux"};

 //Option Menu titles, saved in flash mem
const char PROGMEM trigType[] ="type";
const char PROGMEM trigDelay[] ="delay";
const char PROGMEM trigThreshold[] = "threshold";


  Aux::Aux() 
  {
	//initialize all class variables
	maxOptionMenu = 3;
    triggerState_ = false; //off
	abortTrigger = false; 
	setOption(TRIG_TYPE,0);
	setOption(TRIG_THRESHOLD,0);
	setOption(TRIG_DELAY,0);
	select_ = 0; 
    sensorPin_ = AUX;
	focusPulseTime_ = DEFAULT_FOCUS_TIME;
	shutterPulseTime_ = DEFAULT_SHUTTER_TIME;
	focusArmed = false; //focus on/off default
	shutterArmed = true; //shutter on/off default
	IRShutter_ = false;  //IR on/off default

};
 
  
/***********************************************************
 * 
 * sensorLevel
 *
 * return the current analog value from the aux sensor. Range 0-255
 * 
 ***********************************************************/
  int Aux::sensorLevel()
  {
    sensorLevel_ = analogRead(sensorPin_) >> 2; //shift analog input to a range of 0-255
	return sensorLevel_;
  }


/***********************************************************
 * 
 * trigger
 *
 * trigger the camera, if the sensor value has changed, the shutter will activate
 * 
 ***********************************************************/
 boolean Aux::trigger()
 {
    boolean auxStatus = false;

	shutter(true); 


    switch (option(TRIG_TYPE))
    {
    	case AUX_RISE_MODE:

        	auxStatus = rise();

        break;
        case AUX_FALL_MODE:

      		auxStatus = fall();

      	break;
    	case AUX_CHANGE_MODE:

      		auxStatus = change();

      	break;
    	default: //no default option
      	break;
    }


     if(auxStatus == true) //set all values ready to activate the shutter
    {
		delayCount = millis(); //start counting till delay is up
		focusReady = true; 
		shutterReady = true;
		IRReady = true; 
		return auxStatus;
	}	
	else
	{
    	return auxStatus;
	}
}


/***********************************************************
 * 
 * To change the behavior of these functions for the aux sensor, edit
 * or add a new function here to customize aux sensor. By editing these functions 
 * below you are overriding the default function behavior in Aux's parent class. 
 * 
 ***********************************************************/

  /*
	boolean Aux::rise()
   	{
   	
   	}
   	*/

  /*
	boolean Aux::fall()
   	{
   	
   	}
   	*/
  /*
	boolean Aux::change()
   	{
   	
   	}
   	*/

/***********************************************************
 * 
 * getModeMenu
 *
 * return the string value of Mode Menu for this sensor from the flash memory
 * 
 ***********************************************************/
	void Aux::getModeMenu(char buffer[])
  {
	 strcpy_P(buffer, auxMenu); 

  }

/***********************************************************
 * 
 * getActiveMessage
 *
 * get the current sensors LCD message to print during trap Active mode.
 * 
 ***********************************************************/
void Aux::getActiveMessage(char buffer[])
{
	buffer[0] = 0;
	
	itoa (sensorLevel(),buffer,10);
	
	
}
