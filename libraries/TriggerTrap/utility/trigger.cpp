/************************************************************************************
 * 	
 * 	Name    : Triggertrap trigger.cpp                        
 * 	Author  : Noah Shibley / NoMi Design                       
 * 	Date    : July 10th 2011                                    
 * 	Version : 0.1                                              
 * 	Notes   :  The base class for all sensors. 
 *			   DO NOT EDIT - EDITING WILL CHANGE ALL SENSORS BEHAVIOR
 *			   ,inherit instead to a sub class and change there.            
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

#include "trigger.h"

//default active status message
const prog_char activeMessage[] PROGMEM= {"Armed"};

 //Option Menu default
const prog_char trigThreshold[] PROGMEM = "threshld";
const prog_char trigDelay[] PROGMEM="delay";
const prog_char trigType[] PROGMEM="type";

const prog_char * selectMenu[] PROGMEM  = 	   //options menu
{   
trigThreshold,	
trigDelay,
trigType,
};


 //Value setting titles for values that are not just numbers (ex. type option)
const prog_char onRise[] PROGMEM="On Rise";
const prog_char onFall[] PROGMEM="On Fall";	
const prog_char onChange[] PROGMEM="On Chnge";


const prog_char * settingMenu[] PROGMEM  = 	   // select menu options
{   
onRise,onFall,onChange
};


/***********************************************************
 * 
 * setSelect
 *
 *  
 *   
 ***********************************************************/
  void Trigger::setSelect(int select)
  {
    select_ = select; 

  }
/***********************************************************
 * 
 * select
 *
 * 
 *   
 ***********************************************************/
  const int Trigger::select()
  {
    return select_;
  }

/***********************************************************
 * 
 * incSelect
 *
 * 
 * 
 ***********************************************************/
  void Trigger::incSelect()
  {
	select_++;
	select_ = select_ % maxOptionMenu;  
  }

/***********************************************************
 * 
 * option
 *
 * 
 * 
 ***********************************************************/
const unsigned int Trigger::option(int menuOption)
{
	return optionValues[menuOption];
}

/***********************************************************
 * 
 * setOption
 *
 * 
 * 
 ***********************************************************/
void Trigger::setOption(int menuOption,unsigned int value)
{
	optionValues[menuOption] = value;
}

/***********************************************************
 * 
 * incOption
 *
 * 
 * 
 ***********************************************************/
void Trigger::incOption(int menuOption, unsigned int maxValue, int inc)
{
	unsigned int mOpt = optionValues[menuOption];
	mOpt=mOpt+inc;
	if(mOpt > maxValue) { mOpt = 0; } //limits

	optionValues[menuOption] = mOpt;
}

/***********************************************************
 * 
 * decOption
 *
 * 
 * 
 ***********************************************************/
void Trigger::decOption(int menuOption, unsigned int maxValue, int dec)
{
	unsigned int mOpt = optionValues[menuOption];
	mOpt=mOpt-dec;
	if(mOpt > maxValue) { mOpt = maxValue; }  //limits, unsigned so < 0 is 65535

	optionValues[menuOption] = mOpt;
}

/***********************************************************
 * 
 * setTriggerPulseTime
 *
 * 
 * 
 ***********************************************************/
void Trigger::setTriggerPulseTime(unsigned long focusPulseTime, unsigned long shutterPulseTime) 
{
	focusPulseTime_ = focusPulseTime;
	shutterPulseTime_ = shutterPulseTime;
}

/***********************************************************
 * 
 * trigger
 *
 * pure virtual, must override in sub class!
 * 
 
 boolean Trigger::trigger()
 {


 }
***********************************************************/


/***********************************************************
 * 
 * shutter
 *
 * 
 * 
 ***********************************************************/
void Trigger::shutter(boolean delayActive,boolean delayUnitMs)
{
    //shutter & focus ready
    //wait for focus OR shutter delay (depending on which is active)
    //fire focus, when focus is complete or almost complete (return a value from resetFocus)
    //fire shutter
    //reset both

	
	resetFocus(false);
	resetShutter(false); //make the shutter close after 50ms delay
	
	   //ready, but need to wait for delay timer, unless delayActive is false, then skip the delay
	if(delayActive == false || delayFire(delayUnitMs, option(TRIG_DELAY)) == true) 
	{
		focusFire();
		
		//fire shutter when focus is almost complete.
		if(focusArmed == false || delayFire(true, (focusPulseTime_- shutterPulseTime_)) == true)
		{
			shutterFire();

			if(shutterArmed == false || shutterActive == false) //wait for shutter and focus to finish before you fire IR. or shutter is off
			{	
				if(focusActive == false) //make sure focus isn't active either
				{
					IRFire();
				}	
			}
			
		}
		
	}
	
}

/***********************************************************
 * 
 * focusFire
 *
 * 
 * 
 ***********************************************************/
void Trigger::focusFire()
{
		if(focusArmed == false) //focus
		{
			focusPulseTime_ = shutterPulseTime_;
		}
		else
		{
			focusPulseTime_ = DEFAULT_FOCUS_TIME; 
		}
	
	//	if(focusArmed == true) //focus
	//	{
			if(focusReady == true) //trigger() set this to ready
			{
				focusReady = false; //turn off, focus only once.
			
				digitalWrite(FOCUS_TRIGGER_PIN,LOW); //trigger camera
				focusDelay = millis();
				PORTB |= (1<<PORTB7);		    //digitalWrite(KEY_PAD_LEDS,HIGH); turn on keypad LEDs
				focusActive = true;
			
				if(shutterArmed == false && IRShutter_ == false) //only update shotCounter if nothing else is
				{
					shotCounter_++;
				}
			
				#ifdef SERIAL_DEBUG
				Serial.println("Focus");
				#endif
			}
	//	}
	//	else
	//	{
	//		focusReady = false; //turn it off if its not armed. 
	//	}
}

/***********************************************************
 * 
 * shutterFire
 *
 * 
 * 
 ***********************************************************/
void Trigger::shutterFire()
{
	
			if(shutterArmed == true) //shutter
			{
				if(shutterReady == true) //trigger() set this to ready
				{
				
					shutterReady = false; 
				
					digitalWrite(SHUTTER_TRIGGER_PIN,LOW);
					shutterDelay = millis(); 
					PORTB |= (1<<PORTB7);		    //digitalWrite(KEY_PAD_LEDS,HIGH); turn on keypad LEDs
					shutterActive = true;
			 
					shotCounter_++; 

					#ifdef SERIAL_DEBUG
					Serial.println("Shutter");
					#endif
				}	
			}
			else
			{
				shutterReady = false; //turn it off if its not armed
			}
}
/***********************************************************
 * 
 * IRFire
 *
 * 
 * 
 ***********************************************************/
void Trigger::IRFire()
{
		if(IRShutter_ == true)
		{
			if(IRReady == true)
			{
				IRReady = false; 
				#ifdef SERIAL_DEBUG
				Serial.println("IR");
				#endif
				
				PORTB |= (1<<PORTB7);		    //digitalWrite(KEY_PAD_LEDS,HIGH); turn on keypad LEDs
				IRTransmit();
				PORTB &= ~ (1<<PORTB7);        //digitalWrite(KEY_PAD_LEDS,LOW); //turn off led
				
				if(shutterArmed == false) //only increment shotcounter if shutter is not doing it
				{
					shotCounter_++; 
				}
			
			
			}	
		}
		else
		{
			IRReady = false; 
		}
}

/***********************************************************
 * 
 * resetFocus
 *
 * 
 * 
 ***********************************************************/
void Trigger::resetFocus(boolean resetNow)
{
	
  //reset trigger low after small delay
  if(focusActive == true)
  {
	 focusReady = false; //don't take a picture again until its done taking a picture already!
	
	 if(resetNow == true || (millis() - focusDelay > focusPulseTime_)) 
	 {
	   
		#ifdef SERIAL_DEBUG
		Serial.println("clear focus");
		#endif
	
		digitalWrite(FOCUS_TRIGGER_PIN,HIGH);
		focusDelay = millis();  // save the last time you took a photo
		focusActive = false;
		PORTB &= ~ (1<<PORTB7);        //digitalWrite(KEY_PAD_LEDS,LOW); //turn off led

	 }	
  }		
}

/***********************************************************
 * 
 * resetShutter
 *
 * 
 * 
 ***********************************************************/
void Trigger::resetShutter(boolean resetNow)
{
	

  //reset trigger low after small delay
  if(shutterActive == true)
  {

	  shutterReady = false; //don't take a picture again until its done taking a picture already!
		#ifdef SERIAL_DEBUG
		Serial.println(millis() - shutterDelay);
		Serial.println (shutterPulseTime_);
		#endif
	 if(resetNow == true || (millis() - shutterDelay > shutterPulseTime_)) 
	 {
	   	
		#ifdef SERIAL_DEBUG
		Serial.println("clear shutter");
		#endif
		
		digitalWrite(SHUTTER_TRIGGER_PIN,HIGH);
		shutterDelay = millis(); // save the last time you took a photo
		shutterActive = false; 
		PORTB &= ~ (1<<PORTB7);        //digitalWrite(KEY_PAD_LEDS,LOW); //turn off led
	 }	
  }
}


/***********************************************************
 * 
 * delayFire
 *
 * 
 * 
 ***********************************************************/
boolean Trigger::delayFire(boolean delayUnitMs, int expireTime)

{	int elapsed;
	
	if(shutterReady == true || focusReady == true || IRReady == true) //trigger() set this to ready
	{
		
		
		if(delayUnitMs)
		{
				unsigned long currentTime = millis();
				unsigned long delaySec = delayCount;
				elapsed = (int) (currentTime - delaySec);
		}
		else
		{
			int currentTime = millis()/1000;
			int delaySec = delayCount/1000;
			elapsed = currentTime - delaySec;
		}
	
		if(elapsed > expireTime) 
		{
	
			return true; //delays up, fire
		}
		else
		{
			return false; //times not up, wait some more
		}
	}
	else
	{
		return false; //shutter or focus is not ready
	}	
}


/***********************************************************
 * 
 * rise
 *
 * if trigger just went high, trigger the camera. (return true)
 * 
 ***********************************************************/
boolean Trigger::rise()
{
    boolean changed = change();

    if(changed == true) //trigger changed state
    {
      	if(sensorLevel_ > option(TRIG_THRESHOLD)) //sesnor data being received, any value above threshold 
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

/***********************************************************
 * 
 * fall
 *
 * if sensor just went low, trigger the camera. (return true)
 * 
 ***********************************************************/
   boolean Trigger::fall()
  {
    boolean changed = change();

    if(changed == true) //sensor changed state
    {
     	 if(sensorLevel_ <= option(TRIG_THRESHOLD)) //sensor low, stopped
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

/***********************************************************
 * 
 * change
 *
 * if sensor changed state (high/low), trigger the camera. (return true)
 * 
 ***********************************************************/
boolean Trigger::change()
{
    boolean state = triggerState_; 
	int threshold = option(TRIG_THRESHOLD); //get setting option 2 threshold

	sensorLevel_ = analogRead(sensorPin_) >> 2; //shift 1024 to 255
	
	
    //state is high, or state is low, depending on change above or below threshold
    if(sensorLevel_ > threshold)  //true above
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
void Trigger::decSetting(char buffer[], int dec)
{

	
		switch (select_)
	    {
	     case TRIG_TYPE:
	 	  decOption(TRIG_TYPE, 2,dec);
		  getSettingMenu(buffer); 
	      break;
	    case TRIG_DELAY:
	      decOption(TRIG_DELAY, 9999,dec);
	      if(option(TRIG_DELAY) == 0) //delay 0 is infinity 
		  {
				buffer[0] = 0;
				strcat(buffer,"Off");
				strcat(buffer,"\0");
		  }
		  else
		  {
	      	formatMSString(option(TRIG_DELAY),buffer);
 	  	  }
		  //itoa (option(TRIG_DELAY),buffer,10);
	      break;
	    case TRIG_THRESHOLD:
	      decOption(TRIG_THRESHOLD, 255,dec); 
		  formatThresholdString(option(TRIG_THRESHOLD),buffer);
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
void Trigger::incSetting(char buffer[], int inc)
{
	
	
		switch (select_)
	    {
	     case TRIG_TYPE:
	 	  incOption(TRIG_TYPE, 2,inc);
		  getSettingMenu(buffer); 
	      break;
	    case TRIG_DELAY:
	      incOption(TRIG_DELAY, 9999,inc);
		  if(option(TRIG_DELAY) == 0) //delay 0 is infinity 
		  {
				buffer[0] = 0;
				strcat(buffer,"Off");
				strcat(buffer,"\0");
		  }
		  else
		  {
		  	formatMSString(option(TRIG_DELAY),buffer);
	 	  }
		  //itoa (option(TRIG_DELAY),buffer,10);
	      break;
	    case TRIG_THRESHOLD:
	      incOption(TRIG_THRESHOLD, 255,inc); 
		  formatThresholdString(option(TRIG_THRESHOLD),buffer);
	      break;
	    default: 
	      break;
	    }
		
}

/***********************************************************
 * 
 * formatTimeString
 *
 * 
 * 
 ***********************************************************/
void Trigger::formatTimeString(unsigned int data, char buffer[])
{
	//setting the first char to 0 causes str functions to think 
	//of the buffer as empty, like a clear buffer.
	buffer[0] = 0;
	char tempBuffer[5];
	//transform delay seconds into min and add to tempbuffer
	utoa (data/60,tempBuffer,10);
	//add minute data to buffer
	strcat(buffer, tempBuffer);
	//add minute symbol to buffer
	strcat(buffer,":");
	//transform delay seconds into remainder seconds
	if(data%60 < 10) strcat(buffer,"0");
	utoa(data%60,tempBuffer,10);
	//add second data to buffer
	strcat(buffer,tempBuffer);
	//add second symbol to buffer
	strcat(buffer,'\0');
		
}

/***********************************************************
 * 
 * formatTimeStringLapse
 *
 * 
 * 
 ***********************************************************/
void Trigger::formatTimeStringLapse(unsigned int data, char buffer[])
{
	//setting the first char to 0 causes str functions to think 
	//of the buffer as empty, like a clear buffer.
	buffer[0] = 0;
	char tempBuffer[5];
	//transform delay seconds into min and add to tempbuffer
	utoa (data/600,tempBuffer,10);
	//add minute data to buffer
	strcat(buffer, tempBuffer);
	//add minute symbol to buffer
	strcat(buffer,":");
	//transform delay seconds into remainder seconds
	if(data%600 < 100) strcat(buffer,"0");
	if(data%600 < 10) strcat(buffer,"0");
	utoa(data%600,tempBuffer,10);
	//add second data to buffer
	strcat(buffer,tempBuffer);
	//add second symbol to buffer
	strcat(buffer,'\0');
		
}

/***********************************************************
 * 
 * formatMSString
 *
 * 
 * 
 ***********************************************************/
void Trigger::formatMSString(unsigned int data,char buffer[])
{
	//setting the first char to 0 causes str functions to think 
	//of the buffer as empty, like a clear buffer.
	buffer[0] = 0;
	char tempBuffer[5];
	//transform delay ms into seconds
	utoa (data/1000,tempBuffer,10);
	//add second data to buffer
	strcat(buffer, tempBuffer);
	//add second delimiter symbol to buffer
	strcat(buffer,".");
	//transform delay seconds into remainder seconds
	if(data%1000 < 100) strcat(buffer,"0");
	if(data%1000 < 10) strcat(buffer,"0");
	utoa(data%1000,tempBuffer,10);
	//add second data to buffer
	strcat(buffer,tempBuffer);
	strcat(buffer,"sec");
	//add second symbol to buffer
	strcat(buffer,'\0');
}

/***********************************************************
 * 
 * formatThresholdString
 *
 * 
 * 
 ***********************************************************/
void Trigger::formatThresholdString(unsigned int data, char buffer[])
{
	buffer[0] = 0;
	char tempBuffer[5];
	int level = sensorLevel();
	
	utoa(data,tempBuffer,10);
	
	strcat(buffer,tempBuffer);
	
	strcat(buffer,"|");
	
	itoa (level,tempBuffer,10);
	
	strcat(buffer,tempBuffer);
	
	strcat(buffer,'\0');
	
}


/***********************************************************
 * 
 * IRShutter
 *
 * 
 * 
 ***********************************************************/
void Trigger::IRTransmit()
{
	#ifdef OLYMPUS
		irsend.sendNEC(0x61DC807F,32); //RM-2 (olympus) is probably NEC protocol
	#endif
	
	#ifdef NIKON
		unsigned int MLL3Buffer[] = {2000,27830,390,1580,410,3580,400}; //shutter from: http://www.bigmike.it/ircontrol/
		for (int i = 0; i < 2; ++i) { //send twice
			irsend.sendRaw(MLL3Buffer,7,38); //ML-L3 (Nikon), suppose to be 38.4khz
			delay(63);
		}
	#endif
	
	#ifdef PENTAX
		unsigned int PentaxBuffer[] = {13000,3000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000}; //shutter
			irsend.sendRaw(PentaxBuffer,16,38); //found here, not sure which pentax, all? http://sourceforge.net/project/showfiles.php?group_id=131383
	#endif
	
	#ifdef WLDC100 
		//1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1
		unsigned int WLDC100Buffer[] = {9200,4400,612,1621,612,1621,612,1621,612,1621,612,1621,  //11111
										612,512,612,512,612,512,612,512,612,512,612,512,612,512,612,512, //000000000
										612,1621,612,1621,612,1621, //111
										599}; //trailing 1
		irsend.sendRaw(WLDC100Buffer,16,32);	
	#endif

 	#ifdef CANON
		unsigned int CanonBuffer[] = {490,7400,490}; //shutter
		irsend.sendRaw(CanonBuffer,4,33);
	#endif

	#ifdef SONYCAM
		for (int i = 0; i < 3; ++i) {
			irsend.sendSony(0xB4B8F,20); // or maybe 0x7478F both found here: http://sebastian.setz.name/arduino/my-libraries/multi-camera-ir-control#comment-110
			delay(10); // changed from 100 - 0.3.36
		}
	#endif
	
	#ifdef FUJI
	irsend.sendNEC(0xD30C817E,32); //Fuji NEC protocol
	#endif
}

/***********************************************************
 * 
 * getOptionMenu
 *
 * 
 * 
 ***********************************************************/
  void Trigger::getOptionMenu(char buffer[])
  {
	 strcpy_P(buffer, (const prog_char *)pgm_read_word(&(selectMenu[select_])));
  }

/***********************************************************
 * 
 * getSettingMenu
 *
 * 
 * 
 ***********************************************************/
  void Trigger::getSettingMenu(char buffer[])
  {
	strcpy_P(buffer, (const prog_char *)pgm_read_word(&(settingMenu[option(TRIG_TYPE)]))); 
  }

/***********************************************************
 * 
 * getModeMenu
 *
 * 
 * 
 *
 * 	void Trigger::getModeMenu(char buffer[])
 * {
 *	
 *		//	strcpy_P(buffer, (char*)pgm_read_word(&(menu))); 
 * }
 ***********************************************************/

/***********************************************************
 * 
 * getActiveMessage
 *
 * 
 * 
 ***********************************************************/
void Trigger::getActiveMessage(char buffer[])
{
	strcpy_P(buffer,activeMessage); //default active message
}

/***********************************************************
 * 
 * saveState
 *
 * 
 * 
 ***********************************************************/
void Trigger::saveState()
{
	#ifdef SERIAL_DEBUG
	Serial.println("saveState");
	#endif
	
	   //each trigger object will save to a seperate eeprom memory space
	    int objectMemoryOffset = triggerIndex*16;
	
	   eeprom_write(select_, optionSelect,objectMemoryOffset);
	   eeprom_write(optionValues[0], optionVal[0],objectMemoryOffset);
	   eeprom_write(optionValues[1], optionVal[1],objectMemoryOffset);
	   eeprom_write(optionValues[2], optionVal[2],objectMemoryOffset);
	   eeprom_write(optionValues[3], optionVal[3],objectMemoryOffset);
	   eeprom_write(optionValues[4], optionVal[4],objectMemoryOffset);
	 

}
/***********************************************************
 * 
 * restoreState
 *
 * 
 * 
 ***********************************************************/
void Trigger::restoreState()
{
	#ifdef SERIAL_DEBUG
	Serial.println("restoreState");
	#endif
	
	//each trigger object will save to a seperate eeprom memory space
	int objectMemoryOffset = triggerIndex*16;
	
	eeprom_read(select_, optionSelect,objectMemoryOffset);
	if(select_ > 5 || select_ < 0) select_ = 0;
	eeprom_read(optionValues[0], optionVal[0],objectMemoryOffset);
	eeprom_read(optionValues[1], optionVal[1],objectMemoryOffset);
	eeprom_read(optionValues[2], optionVal[2],objectMemoryOffset);
	eeprom_read(optionValues[3], optionVal[3],objectMemoryOffset);
	eeprom_read(optionValues[4], optionVal[4],objectMemoryOffset);

}

/***********************************************************
 * 
 * saveSystem
 *
 * 
 * 
 ***********************************************************/
void Trigger::saveSystem()
{
	//each trigger object will save to a seperate eeprom memory space
	int objectMemoryOffset = triggerIndex*16;
	
	  eeprom_write(focusArmed,cameraFocus,objectMemoryOffset);
	  eeprom_write(shutterArmed,cameraShutter,objectMemoryOffset);
	  eeprom_write(IRShutter_,cameraIR,objectMemoryOffset);
	  eeprom_write(TTContrast,LCDContrast,objectMemoryOffset);

}

/***********************************************************
 * 
 * restoreSystem
 *
 * 
 * 
 ***********************************************************/
void Trigger::restoreSystem()
{
	//each trigger object will save to a seperate eeprom memory space
	int objectMemoryOffset = triggerIndex*16;
	
	eeprom_read(focusArmed,cameraFocus,objectMemoryOffset);
	if(focusArmed == 255) focusArmed = false;  //check just in case. Eeprom could be blank.
	eeprom_read(shutterArmed,cameraShutter,objectMemoryOffset);
	if(shutterArmed == 255) shutterArmed = true; 
	eeprom_read(IRShutter_,cameraIR,objectMemoryOffset);
	if(IRShutter_ == 255) IRShutter_ = false;
	eeprom_read(TTContrast,LCDContrast,objectMemoryOffset);
	if(TTContrast == 255) TTContrast = 30;
}

/***********************************************************
 * 
 * initState
 *
 * 
 * 
 ***********************************************************/
void Trigger::initState()
{
	#ifdef SERIAL_DEBUG
	Serial.println("initState");
	#endif
	
	//each trigger object will save to a seperate eeprom memory space
	int objectMemoryOffset = triggerIndex*16;
	
	   eeprom_write(0, optionSelect,objectMemoryOffset);
	   eeprom_write(0, optionVal[0],objectMemoryOffset);
	   eeprom_write(0, optionVal[1],objectMemoryOffset);
	   eeprom_write(0, optionVal[2],objectMemoryOffset);
	   eeprom_write(0, optionVal[3],objectMemoryOffset);
	   eeprom_write(0, optionVal[4],objectMemoryOffset);
	   eeprom_write(false,cameraFocus,objectMemoryOffset);
	   eeprom_write(true,cameraShutter,objectMemoryOffset);
	   eeprom_write(false,cameraIR,objectMemoryOffset);
	   eeprom_write(30,LCDContrast,objectMemoryOffset);
}

/***********************************************************
 * 
 * start
 *
 * 
 * 
 ***********************************************************/
void Trigger::start(unsigned long startTime)
{ 
	
	shotCounter_ = 0; //reset shot count. 
	startBttnTime = startTime; 
	delayCount = 0; 
	abortTrigger = false; 
}
/***********************************************************
 * 
 * getStop
 *
 * 
 * 
 ***********************************************************/
boolean Trigger::getStop()
{
	return abortTrigger;  
}

