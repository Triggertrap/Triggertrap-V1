/************************************************************************************
 * 	
 * 	Name    : Triggertrap Sound.h                         
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

#ifndef SOUND_H
#define SOUND_H

//#include "globals.h"
#include "trigger.h"
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"    // for digitalRead, digitalWrite, pinMode, delayMicroseconds
#else
#include "WProgram.h"
#endif

class Sound : public Trigger{

public:


	Sound();

	int sensorLevel();
	
	boolean trigger();
	
  //to change the behavior of the following functions for the mic, edit here. 
  //the current functional behavior is in the parent sensor class.
  //Or add a new function here, to customize mic sensor

  /*
	boolean change();
   	*/

	void getOptionMenu(char buffer[]);
	
	void getModeMenu(char buffer[]);
	
	void getActiveMessage(char buffer[]);

private:



};

#endif

