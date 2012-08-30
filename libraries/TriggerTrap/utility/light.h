/************************************************************************************
 * 	
 * 	Name    : Triggertrap light.h                         
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

#ifndef LIGHT_H
#define LIGHT_H

//#include "globals.h"
#include "trigger.h"
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"    // for digitalRead, digitalWrite, pinMode, delayMicroseconds
#else
#include "WProgram.h"
#endif

class Light : public Trigger{

public:


	Light();

	int sensorLevel();

	boolean trigger();
	
	void setThreshold(int threshold){ setOption(2,threshold); } //TODO change 2 to something more clear, define
		
  //To change the behavior of these functions for the light sensor, edit here
  //Or add a new function here, to customize light sensor
  /*
	boolean start();
   	*/

  /*
	boolean getStop();
   	*/
  /*
	boolean change();
   	*/


	void getModeMenu(char buffer[]);
	
	void getActiveMessage(char buffer[]);

private:



};

#endif

