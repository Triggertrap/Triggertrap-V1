/************************************************************************************
 * 	
 *      Name    : Triggertrap Arduino Sketch                         
 *      Author  : Noah Shibley   / NoMi Design - http://n0m1.com
 *              : Michael Grant  / NoMi Design - http://n0m1.com
 *              : Haje Jan Kamps / Triggertrap - http://triggertrap.com                        
 *      Updated : July 16, 2012                                    
 *      Version : 0.3.36                                              
 * 	Notes   : Triggertrap code for Triggertrap Device and Triggertrap Shield for Arduino. 
 *                For more information see Triggertrap.com and github.com/triggertrap
 * 
 *                Code for Triggertrap device (TTv1) and Triggertrap Shield (TT-k)                      
 * 
 *      Copyright (c) 2012 NoMi Design (http://n0m1.com) All right reserved.
 * 
 *      This file is part of Triggertrap. See Triggertrap.com for more information.
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

#include <I2C.h>
#include <PinChangeInt.h>
#include <PinChangeIntConfig.h>
#include <LCD.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_SR_LCD3.h>
#include <AtTouch.h>
#include <TTSleep.h>
#include <TriggerTrap.h>

const int LASER_MODE = 0;
const int SOUND_MODE = 1;
const int LIGHT_MODE = 2;
const int TIMELAPSE_MODE = 3; 
const int AUX_MODE = 4;


TriggerTrap tt;

/***********************************************************
 * 
 * Setup
 * 
 ***********************************************************/
void setup() {   

  tt.setup();

}

/***********************************************************
 * 
 * loop
 * 
 ***********************************************************/
void loop() {

  tt.update();
  
  if(tt.trigger())
  {
    
     if(tt.triggerMode() == SOUND_MODE)
     {
       //Serial.println("sound trigger");
     }
     else if(tt.triggerMode() == LASER_MODE)
     {
       //Serial.println("laser trigger");
     }  
     else if(tt.triggerMode() == LIGHT_MODE)
     {
       //Serial.println("light trigger");
     }  
     else if(tt.triggerMode() == TIMELAPSE_MODE)
     {
       //Serial.println("timelapse trigger");
     }   
     else if(tt.triggerMode() == AUX_MODE)
     {
       //Serial.println("aux trigger");
     }    
    
  }

}









