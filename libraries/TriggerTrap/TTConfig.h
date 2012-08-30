/************************************************************************************
*
* Name : Triggertrap TTConfig.h
* Author : Noah Shibley / NoMi Design
* Haje Jan Kamps / Triggertrap Ltd
* Date : July 10th 2011
* Version : 0.3.35
* Notes : Configuration File for Triggertrap
*
* Code for Triggertrap device (TTv1) and Triggertrap Shield (TT-k)
*
* This file is part of Triggertrap. See Triggertrap.com for more information.
*
* Triggertrap is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Triggertrap is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Triggertrap. If not, see <http://www.gnu.org/licenses/>.
*
***********************************************************************************/

#ifndef TT_CONFIG_H
#define TT_CONFIG_H

/*** CHOOSE YOUR VERSION OF THE TRIGGERTRAP ***************************************
*
* There are two different versions of the Triggertrap. If you are developing for
* the Arduino-compatible Shield version, leave the #define statement below alone.
*
* If you wish to install this code on the Triggertrap v1 (TTv1), you need to comment
* out the line below. In doing so, the code will be configured for the TTv1.
*
* If you don't know which device you have, you probably shouldn't be installing
* any code on it ;)
*
***********************************************************************************/

 #define TT_SHIELD


/*** SERIAL DEBUGGING ***************************************************************
 * 	
 * 	If you need debugging information over the Serial interface, uncomment the below 
 *  #Define. Keep in mind that serial debugging makes the Triggertrap slower, so 
 *  remember to turn it off when you are done!
 * 
 ***********************************************************************************/

 //#define SERIAL_DEBUG 


/*** HARDWARE QA TESTING ***********************************************************
 * 	
 * 	If you need to do some QA testing to verify quickly and easily that all sensors
 *  are working, and all buttons are working, uncomment below. 
 *  
 * 
 ***********************************************************************************/

// #define HARDWARE_TEST


/*** CHOOSE CAMERAS TO TRIGGER OVER INFRA-RED ***************************************
 * 	
 * 	The Triggertrap supports camera triggering over IR, but the IR protocol is very
 *  slow indeed. By default, the Triggertrap will transmit all the IR codes, one 
 *  after another. If your camera is the last one that is transmitted, that means a 
 *  delay of approximately 1 second. 
 * 
 *  If there are cameras you know you will never use, you can safely 'comment them 
 *  out' by adding two slashes at the beginning of the line. Cameras that are 
 * 'commented out' will be skipped by the Triggertrap - this makes triggering faster.
 * 
 ***********************************************************************************/

 #define CANON   
 #define FUJI
 #define NIKON	
 #define OLYMPUS
 #define PENTAX
 #define SONYCAM
 #define WLDC100

/*** FOCUS, SHUTTER, AND SLEEP SETTINGS ********************************************
 * 	
 * 	The Triggertrap lets you choose how long you want to send the 'trigger' or
 * 'focus' signals to your camera for. Some cameras need the shutter signal to be
 *  sent for a little bit longer than others. We've found that 150 ms is safe for 
 *  all cameras we tested with - but you can reduce this if needed. 
 * 
 * 	The 'focus' signal is sent before the 'shutter' signal. The camera will only try
 *  to focus as long as the Focus signal is sent, so if your camera takes 1 second to 
 *  focus, you may have to increase this to 1000 ms (i.e. 1 second). 
 * 
 *  The 'sleep' mode is set based on how long it takes between your last user input
 *  and the Triggertrap going into sleep mode. 
 * 
 ***********************************************************************************/
 	
 const int DEFAULT_SHUTTER_TIME = 150;   	// duration of shutter signal, in ms. Default: 150 
 const int DEFAULT_FOCUS_TIME = 500;     	// duration of focus signal, in ms. Default: 500 
 const unsigned int UI_SLEEP_MS = 30000; 	// time before device goes to sleep, in ms. Default: 30000
 


/*** ADVANCED SETTINGS **************************************************************
 * 	
 * 	Below, we're defining the pins we are using etc. In normal use, you shouldn't 
 *  have to change any of these, so it's probably a good idea to leave them alone
 *  unless you know what you're doing! 
 * 
 ***********************************************************************************/

    #define FIRMWARE_VERSION "0.3.35"       //The current firmware version of this TT


 #ifdef TT_SHIELD
	const byte START_BUTTON = 7;			// D7  = Digital in - Start button
	const byte FOCUS_TRIGGER_PIN = 9;		// D13 = Digital out - Camera Trigger A
    const byte SHUTTER_TRIGGER_PIN = 8;		// D12 = Digital out - Camera Trigger B
	const byte LASER_SENSOR = A3; 			// A3  = Laser sensor
	const byte AMBIENT_LIGHT_SENSOR = A3;	// A3  = Ambient light sensor
	const byte SOUND_IN = A5;				// A5  = Sound sensor
	const byte AUX = A0;  					// A0  = Auxiliary port
 #else //Normal TT
	const byte POWER_UI = PORTB6;			// A3  = Digital out - UI power mosfet switch
	const byte KEY_PAD_LEDS = PORTB7;		// D4  = Digital out - LED on keypad
	const byte START_BUTTON = 2;			// D2  = Digital in - Start button 
	const byte KEY_CHANGE = 3;				// D3  = Digital in - key change interrupt for touch ic
	const byte FOCUS_TRIGGER_PIN = 13;		// D13 = Digital out - Camera Trigger A
    const byte SHUTTER_TRIGGER_PIN = 12;	// D12 = Digital out - Camera Trigger B
	const byte LASER_SENSOR = A6; 			// A6  = Laser Sensor
	const byte AMBIENT_LIGHT_SENSOR = A0;	// A0  = Ambient light sensor
	const byte SOUND_IN = A7;				// A7  = Sound sensor
	const byte AUX = A2;  					// A2  = Auxiliary port
 #endif


 const byte NUM_OF_SENSORS = 6; 			//the number of sensor triggers



#endif //TT_CONFIG_H
