/************************************************************************************
 * 	
 * 	Name    : Triggertrap auxiliary.h                         
 * 	Author  : Noah Shibley / NoMi Design                         
 * 	Date    : July 10th 2011                                    
 * 	Version : 0.1                                              
 * 	Notes   : The aux sensor port. Sub class of Trigger, inherits all Trigger functions           
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

#ifndef AUXILIARY_H
#define AUXILIARY_H


#include "utility/trigger.h"
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"    
#else
#include "WProgram.h"
#endif

class Aux : public Trigger{

public:


	Aux();

/***********************************************************
 * 
 * sensorLevel
 *
 * return the current analog value from the aux sensor. Range 0-255
 * 
 ***********************************************************/
	int sensorLevel();

/***********************************************************
 * 
 * trigger
 *
 * trigger the camera, if the sensor value has changed, the shutter will activate
 * 
 ***********************************************************/
	boolean trigger();
	
/***********************************************************
 * 
 * setThreshold
 *
 * set the aux threshold value
 * 
 ***********************************************************/	
	void setThreshold(int threshold){ setOption(2,threshold); } 
		
/***********************************************************
 * 
 * To change the behavior of these functions for the aux sensor,uncomment 
 * the function prototypes below and edit new functions in auxiliary.cpp
 * 
 ***********************************************************/

  /*
	boolean rise();
   	*/


  /*
	boolean fall();
   	*/

  /*
	boolean change();
   	*/

/***********************************************************
 * 
 * getModeMenu
 *
 * return the string value of Mode Menu for this sensor from the flash memory
 * 
 ***********************************************************/
	void getModeMenu(char buffer[]);

/***********************************************************
 * 
 * getActiveMessage
 *
 * get the current sensors LCD message to print during trap Active mode.
 * 
 ***********************************************************/	
	void getActiveMessage(char buffer[]);

private:



};

#endif

