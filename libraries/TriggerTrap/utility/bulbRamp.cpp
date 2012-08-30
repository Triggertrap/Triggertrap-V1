/************************************************************************************
 * 	
 * 	Name    : Triggertrap BulbRamp.cpp                         
 * 	Author  : Noah Shibley / NoMi Design                         
 * 	Date    : July 10th 2011                                    
 * 	Version : 0.1                                              
 * 	Notes   : BulbRamp interval trigger          
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

#include "bulbRamp.h"

//mode menu title
const char PROGMEM bulbMenu[]= {"BulbRamp"};

 //option Menu titles
const char PROGMEM delta[]="interval";
const char PROGMEM startdelay[]="delay1st";
const char PROGMEM duration[] = "duration";
const char PROGMEM startExposure[] = "start";
const char PROGMEM endExposure[] = "end";

const char PROGMEM * const bulbOptionMenu[]  = 	   //options menu order
{   
delta,
startdelay,
duration,
startExposure,
endExposure,
};

//option order should match timeSelectMenu order
const int BULB_DELTA = 0;
const int BULB_DELAY = 1; 
const int BULB_DURATION = 2;
const int BULB_STARTEXP = 3;
const int BULB_ENDEXP = 4;

/***********************************************************
 * 
 * TimeLapse
 *
 *  
 * 
 ***********************************************************/
BulbRamp::BulbRamp(){
	
		maxOptionMenu = 5;
	    triggerState_ = false; //off
		abortTrigger = false; 
		setOption(BULB_DELTA,0);    //set time delta to 0
		setOption(BULB_DELAY,0);    //set time delay to 0
		setOption(BULB_DURATION,0); //set #shots to 0
		setOption(BULB_STARTEXP,33); //set #shots to 0
		setOption(BULB_ENDEXP,1000); //set #shots to 0
		
		delayCount = 0; 
		select_ = 0;  //set 
		focusPulseTime_ = DEFAULT_FOCUS_TIME;
		shutterPulseTime_ = DEFAULT_SHUTTER_TIME;
		focusArmed = false; //focus on/off default
		shutterArmed = true; //shutter on/off default
		IRShutter_ = false;  //IR on/off default
		startExposure = 33;
		endExposure = 1000;
		longExposure = 1000; 
		fractionIndex = 0;
		msValue = 0; 
		shotCounter_ = 0; 
	  	
	
}

/***********************************************************
 * 
 * delayFirstShot
 *
 *  
 * 
 ***********************************************************/
boolean BulbRamp::delayFirstShot()
{	
	if(shotCounter_ < 1) //first shot happens RIGHT away on start button press, unless delayed
	{
			if(delayCount == 0) { delayCount = millis(); } //set this only the first time through the loop
			focusReady = true; 
			shutterReady = true; //if delay is up, take the first shot, 
			IRReady = true; 
			shutter(true,false); //set delay to true, we want delay, set delay time unit false to seconds
			return true; 
	}
	else
	{
			return false; 
	}
}

/***********************************************************
 * 
 * trigger
 *
 *  
 * 
 ***********************************************************/
boolean BulbRamp::trigger()
{

	unsigned long remainTime;
	//don't allow zero time delta. Will crash the device
	if(option(BULB_DELTA) == 0) { incOption(BULB_DELTA,1); }
	
	//----------------Duration
	//if you set a duration limit, stop take pictures when it is reached
	if(elapsedDuration >= option(BULB_DURATION)) 
	{
		if(focusActive == false && shutterActive == false) //don't abort if shutter is triggered
		{ 
			elapsedDuration = 0; //clear this for next time
		 	abortTrigger = true; return false;
		}
   }

    remainTime = countDown(); //get the remaining time to next shot

    //calulate the shutterPulseTime. 
	int steps = option(BULB_DURATION) / option(BULB_DELTA);
    int stepSize = ( option(BULB_ENDEXP) - option(BULB_STARTEXP) ) / steps;
	shutterPulseTime_ = option(BULB_STARTEXP) + (stepSize * shotCounter_); 
	
	#ifdef SERIAL_DEBUG
	Serial.print("pulseTime:");
	Serial.println(shutterPulseTime_);
	#endif

    //-------------Delay
	if(delayFirstShot() == true)
	{
		return true;
	}
	

	shutter(false,false); //trigger shutter if shutterReady = true, set delay to false, false time unit seconds
			
	
	//----------Sleep		
	if(batteryPower() == true)
	{
		if(focusActive == false && shutterActive == false) //don't sleep if shutter is triggered
		{
			sleep_->pwrDownMode(); //deepest sleep
			sleep_->sleepDelay(abortTrigger,remainTime,shotCounter_);
			remainTime = 0;
		
		}	
	}
			
		
	 //---------- Shutter	
	  if ((long) remainTime < 0 ) {remainTime = 0;} //check for unsigned underflow, by converting to signed	
	  
	  if (remainTime <= 0) 
	  {
			//times up,take a shot
			delayCount = millis(); //start counting till delay is up
			startBttnTime = delayCount; //don't call millis twice, just use delayCount, reset startButtnTime
			focusReady = true; 
			shutterReady = true; //set shutter ready, will activate shutter next time through loop
			IRReady = true; 
			return shutterReady;
	  }
	  else
	  {
		return false; 
	  }



	
}

/***********************************************************
 * 
 * countDown
 *
 *  
 * 
 ***********************************************************/
unsigned long BulbRamp::countDown()
{

	if(shotCounter_ < 1)
	{
		startBttnTime = millis(); //reset startBttnTime, so interval starts here
		startDuration = startBttnTime/1000; //for duration limit
	}
	  unsigned long currentTime = millis();

      //for duration time limit
	   elapsedDuration = (currentTime/1000) - startDuration;
	
	  //for delta time limit		
      unsigned long elapsedTime = currentTime - startBttnTime;
	  unsigned long deltaTime =	(unsigned long) option(BULB_DELTA);
	  deltaTime = deltaTime*1000;
	  unsigned long remainTime = deltaTime - elapsedTime;
	
	return remainTime;

}

/***********************************************************
 * 
 * countDownInt
 *
 *  
 * 
 ***********************************************************/
unsigned int BulbRamp::countDownInt()
{   
	unsigned long longCount = countDown()/1000;
	unsigned int countInt = (unsigned int) longCount;
	return countInt;
}

/***********************************************************
 * 
 * batteryPower
 *
 *  
 * 
 ***********************************************************/
boolean BulbRamp::batteryPower()
{
	if(digitalRead(0) == HIGH && digitalRead(1) == HIGH) //USB connected
	{
		return false; //USB
	}
	else
	{
		return true; //battery
	}
}

/***********************************************************
 * 
 * decSetting
 *
 *  
 * 
 ***********************************************************/
void BulbRamp::decSetting(char buffer[],int dec)
{

	
		switch (select_)
	    {
	     case BULB_DELAY:
	 	  decOption(BULB_DELAY, 54000,dec); //max secs
		  if(option(BULB_DELAY) == 0) //delay 0 is infinity 
		  {
				buffer[0] = 0;
				strcat(buffer,"Off");
				strcat(buffer,"\0");
		  }
		  else
		  {	
		   		formatTimeString(option(BULB_DELAY),buffer); //format and save string in buffer
		  }
	      break;
	    case BULB_DELTA:
	      decOption(BULB_DELTA, 54000,dec);
		  setIntervalLimits();
		  if(option(BULB_DELTA) == 0) //delta 0 is none 
		  {
				buffer[0] = 0;
				strcat(buffer,"-");
				strcat(buffer,"\0");
		  }
		  else
		  {
	 	  	formatTimeString(option(BULB_DELTA),buffer);
      	  }
	      break;
	    case BULB_DURATION:
	      decOption(BULB_DURATION, 54000,dec);
	      setIntervalLimits();
	 	  if(option(BULB_DURATION) == 0) //duration 0 is none 
		  {
				buffer[0] = 0;
				strcat(buffer,"-");
				strcat(buffer,"\0");
		  }
		  else
		  {
	 	  	formatTimeString(option(BULB_DURATION),buffer);
      	  }	
		  
	      break;
		  case BULB_STARTEXP:
		  incDecFraction(BULB_STARTEXP,false,dec,buffer);
		  setIntervalLimits();
	      break;
		  case BULB_ENDEXP:
		  incDecFraction(BULB_ENDEXP, false,dec,buffer);
		  setIntervalLimits();
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
void BulbRamp::incSetting(char buffer[],int inc)
{
	
	
		switch (select_)
	    {
	     case BULB_DELAY:
	 	  incOption(BULB_DELAY, 54000,inc); //max time in secs
		  if(option(BULB_DELAY) == 0) //delay 0 is infinity 
		  {
				buffer[0] = 0;
				strcat(buffer,"Off");
				strcat(buffer,"\0");
		  }
		  else
		  {	
		   		formatTimeString(option(BULB_DELAY),buffer); 
		  }
	      break;
	    case BULB_DELTA:
	      incOption(BULB_DELTA, 54000,inc); //max time in secs
		  setIntervalLimits();
		  if(option(BULB_DELTA) == 0) //delay 0 is none
		  {
				buffer[0] = 0;
				strcat(buffer,"-");
				strcat(buffer,"\0");
		  }
		  else
		  { 
	 	  	formatTimeString(option(BULB_DELTA),buffer);
	      }
		  break;
	     case BULB_DURATION:
	      incOption(BULB_DURATION, 54000,inc);
	 	  setIntervalLimits();
	 	  if(option(BULB_DURATION) == 0) //duration 0 is none 
		  {
				buffer[0] = 0;
				strcat(buffer,"-");
				strcat(buffer,"\0");
		  }
		  else
		  {
	 	  	formatTimeString(option(BULB_DURATION),buffer);
      	  }
	      break;
		  case BULB_STARTEXP:
	 	  incDecFraction(BULB_STARTEXP,true,inc,buffer);
		  setIntervalLimits();	
	      break;
		  case BULB_ENDEXP:
	 	  incDecFraction(BULB_ENDEXP,true,inc,buffer);
		  setIntervalLimits();
	      break;
	    default: 
	      break;
	    }
		
}
/***********************************************************
 * 
 * incDecFraction
 *
 *  
 * 
 ***********************************************************/
void BulbRamp::incDecFraction(int optionConst, boolean incDec, int incDecAmt, char buffer[]){
	
		//	1/30; 1/20; 1/15; 1/10; 1/8; 1/6; 1/4; 1/3; 1/2; 1; 3/2; 2/1; 5/2; 3/1; 7/2;
		   const byte shutterSpeedDenoms[] = {30,20,15,10,8,6,4,3,2}; //funky intervals, lets keep as an array
		   byte numOfDenom = 9; //array size
		   int sign = 1;
		   if(!incDec) sign = -1; //flip from add or subtract
		   int incFrac = 1 * sign;
		   int incHalf = 500 * sign;
		   int incSec = 1000 * sign; 

		   
		   if(option(optionConst) == 0) setOption(optionConst,33);	//just in case
	
		if(incDecAmt > 0) //if you are not inc/dec you are just refreshing the screen
		{
		
		  if( option(optionConst) < 1000)
		  {
			
			for(int i=0;i<numOfDenom; ++i) //match the fraction
			{ 
		
				if(option(optionConst) == (unsigned int) 1000/shutterSpeedDenoms[i])
				{
					fractionIndex = i; 
					fractionIndex = fractionIndex + incFrac; 
					break;
				}
			}
		
			if(fractionIndex < 0) fractionIndex = 0; //too low
			msValue = (unsigned) 1000/shutterSpeedDenoms[fractionIndex];
			if(fractionIndex >= numOfDenom) // too high, now in secs
			{ 
				msValue = option(optionConst) + incHalf;
			}
			
		  }
		  else if(option(optionConst) < 10000 ) //1-10sec
		  {
			    
				msValue = option(optionConst) + incHalf;
				if(msValue < 1000) //too low now
				{
					 fractionIndex = 8; //in case you now have a fraction
				     msValue = (unsigned) 1000/shutterSpeedDenoms[fractionIndex];
				}
		  }		
		  else if(option(optionConst) <= 60000)//10-60sec
		  {
				msValue = option(optionConst) + incSec;
				if(msValue > 60000)	msValue = 60000; //limit 60sec
		  }
		  
		
		
		  setOption(optionConst,msValue);
		}	
	
		
		fractionToString(optionConst, shutterSpeedDenoms[fractionIndex],buffer);
		
}

/***********************************************************
 * 
 * setIntervalLimits
 *
 *  
 * 
 ***********************************************************/
void BulbRamp::setIntervalLimits()
{
		  //set the short and long exposure for limiting
		  if(option(BULB_STARTEXP) <= option(BULB_ENDEXP))
		  {
			longExposure = option(BULB_ENDEXP);
		  }
		  else //end is shorter then start
		  {
			longExposure = option(BULB_STARTEXP);
		  }
	
		 if(option(BULB_DELTA)*1000 < longExposure) //seconds to milliseconds
		{
			if(longExposure < 1000) longExposure = 1000; //dont allow delta less then 1 second
			int deltaLimit = longExposure/1000;
			setOption(BULB_DELTA,deltaLimit);
		 }
		
		 if(option(BULB_DELTA) > option(BULB_DURATION)) //never allow delta greater then duration
		 {
			setOption(BULB_DURATION,option(BULB_DELTA));
		 } 
		 
}

/***********************************************************
 * 
 * fractionToString
 *
 *  
 * 
 ***********************************************************/
void BulbRamp::fractionToString(int optionConst, int fraction, char buffer[])
{
	  //output to result to buffer
	  char tempBuffer[5];
	  buffer[0] = 0;
	
	 if(option(optionConst) < 1000)		
	  {					
		strcat(buffer,"1/");
		utoa(fraction,tempBuffer,10);
		strcat(buffer,tempBuffer);
		strcat(buffer,"sec");	
		strcat(buffer,'\0');
	 }	
	 else if(option(optionConst) <= 60000) 
	 {
		float secf = (float) option(optionConst)/1000; //set from EndExposure or Start
		dtostrf(secf, 3, 1, tempBuffer); //avr lib float to str
		strcat(buffer,tempBuffer);
		strcat(buffer,"sec");	
		strcat(buffer,'\0');
	 }
}


/***********************************************************
 * 
 * getModeMenu
 *
 *  
 * 
 ***********************************************************/
void BulbRamp::getModeMenu(char buffer[])
{
	 //reads the timeMenu title from flash
	 strcpy_P(buffer, bulbMenu); 
	
}

/***********************************************************
 * 
 * getOptionMenu
 *
 *  
 * 
 ***********************************************************/
void BulbRamp::getOptionMenu(char buffer[])
{
	 //reads the timeSelectMenu options from flash memory
	 strcpy_P(buffer, (const char PROGMEM *)pgm_read_word(&(bulbOptionMenu[select_])));
}

/***********************************************************
 * 
 * getActiveMessage
 *
 * get the current sensors LCD message to print during trap Active mode.
 * 
 ***********************************************************/
void BulbRamp::getActiveMessage(char buffer[])
{
	//buffer[0] = 0;
	
	//itoa (countDownInt(),buffer,10);
	utoa((unsigned int) shutterPulseTime_ ,buffer,10);
	
	
}
