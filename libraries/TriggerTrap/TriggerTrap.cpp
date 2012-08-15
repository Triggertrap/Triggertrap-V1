/************************************************************************************
 * 	
 * 	Name    : Triggertrap TriggerTrap.cpp                        
 * 	Author  : Noah Shibley / NoMi Design                         
 * 	Date    : July 10th 2011                                    
 * 	Version : 0.1                                              
 * 
 * 			  Code for Triggertrap device (TTv1) and Triggertrap Shield (TT-k)                      
 * 
 * 	Copyright (c) 2012 NoMi Design (http://n0m1.com) All right reserved.
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

#include "TriggerTrap.h"


TriggerTrap::TriggerTrap()
{
	
}

void TriggerTrap::setup()
{
	//pass in sleep object for timelapse and bulbramp sleep
	timeLapse.begin(sleep);
	bulbRamp.begin(sleep);

	triggers[0] = &laser;
	triggers[1] = &mic;
	triggers[2] = &light;
	triggers[3] = &timeLapse;
	triggers[4] = &aux;
	triggers[5] = &bulbRamp;
	
	tui.setup(triggers);
}

void TriggerTrap::update()
{
	 tui.update(); //update the UI
}

boolean TriggerTrap::trigger()
{	
	boolean triggerStatus = false;
	 
  	if(tui.trapActive()) //if start button pressed = true
    {
    	//call trigger function of active sensor object, held in triggers array
    	triggerStatus = triggers[tui.trigger()]->trigger(); 
	}
	
	return triggerStatus;
	
}

int TriggerTrap::triggerMode()
{
	return tui.trigger();	
}



