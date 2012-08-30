/************************************************************************************
 * 	
 * 	Name    : AtTouch                         
 * 	Author  : Noah Shibley, NoMi Design  nomidesign.net                        
 * 	Date    : July 18th 2011                                    
 * 	Version : 0.1                                              
 * 	Notes   : Arduino Library for use with the AT42QT1070 Q-touch chip via i2c                       
 * 
 * 	Copyright (c) 2011 NoMi Design All right reserved.
 * 
 * 	This file is part of AtTouch.
 * 
 * 		    AtTouch is free software: you can redistribute it and/or modify
 * 		    it under the terms of the GNU General Public License as published by
 * 		    the Free Software Foundation, either version 3 of the License, or
 * 		    (at your option) any later version.
 * 
 * 		    AtTouch is distributed in the hope that it will be useful,
 * 		    but WITHOUT ANY WARRANTY; without even the implied warranty of
 * 		    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * 		    GNU General Public License for more details.
 * 
 * 		    You should have received a copy of the GNU General Public License
 * 		    along with AtTouch.  If not, see <http://www.gnu.org/licenses/>.
 * 
 ***********************************************************************************/


#include "AtTouch.h"

AtTouch* AtTouch::pAtTouch = 0; 

/***********************************************************
 * 
 * AtTouch
 * 
 ***********************************************************/
AtTouch::AtTouch()
{
  pAtTouch = this;	//the ptr points to this object
 
}

/***********************************************************
 * 
 * begin
 * 
 ***********************************************************/
void AtTouch::begin(int interruptPin,boolean disableAutoCal){

  _changePin = (byte) interruptPin;
  _interruptVal = (byte) interruptPin-2;  //arduino interupt values 0=pin2, 1=pin3
	
  keyHit = false; 
  holdDown_ = false;
  startTime = 0;
  holdRefreshInterval = 1; //1 for 100ms 1000ms/100 = 10 so 100ms/100 = 1
	

  I2c.begin(); 
  I2c.pullup(0);	
  // reset device by writing non-zero value to register 0x39  
  I2c.write(0x1B, 0x39, 0xFF);
  delay(100); // wait for device to restart
  I2c.begin(); // re-open the i2c after device has restarted
  I2c.pullup(0);
  if(disableAutoCal == true)
  {
	delay(100);
	I2c.write(0x1B, 0x37, 0);  
  }
  pinMode(_changePin, INPUT);
  attachInterrupt(_interruptVal,bttnPressISR,FALLING);  //setup the key change interrupt, call bttnpress on interrupt

}

/***********************************************************
 * 
 * hit
 * 
 ***********************************************************/
boolean AtTouch::hit()
{
  
  return keyHit;
}  

/***********************************************************
 * 
 * getStartTime
 * 
 ***********************************************************/
unsigned long AtTouch::getStartTime()
{
	return startTime;	
}

/***********************************************************
 * 
 * setRefreshSpeed
 * 
 ***********************************************************/
void AtTouch::setRefreshSpeed(int intervalMilSec)
{
	holdRefreshInterval = intervalMilSec/100;
}

/***********************************************************
 * 
 * update
 * 
 ***********************************************************/
void AtTouch::update()
{
	if (keyHit == true)
	{
		activeKey_ = readActiveKey();
		
	}
	
}
/***********************************************************
 * 
 * getKey
 * 
 ***********************************************************/
int AtTouch::getKey()
{
	return (int) activeKey_;
}

/***********************************************************
 * 
 * readActiveKey
 * 
 ***********************************************************/
int AtTouch::readActiveKey()
{
  
  
  int keyAddress = readActiveAddress();
 
  
  int keyValue = 0; //start at one, for key0
  for(uint8_t i = keyAddress ;i>0;i = i>>1) //shift until you hit 1, thats which bit place
  {
   keyValue++; //increment keyValue to find bit place
  }
 	activeKey_ = (byte) keyValue; //save the keyValue for later
	if(keyValue == 9 || keyValue == 0) { holdDown_ = false; } //9 is key up, so stop press and hold timer
  
  return keyValue;
  
  
}

/***********************************************************
 * 
 * readActiveAddress
 * 
 ***********************************************************/
int AtTouch::readActiveAddress(){

  if(keyHit == false) //not active return right away
  {
    return -1;
  }
  else
  {

    int readstatus = 0;
    int bttnNum = 0;

    // to clear change int we must read both status bytes 02 and 03
    I2c.read(0x1B, 0x02, 1); 
    readstatus = I2c.receive();

	I2c.read(0x1B, 0x03, 1); 
    bttnNum = I2c.receive();

    keyHit = false;
    if (readstatus == 1) // seems to trigger int twice for each press... this filters the second one...
    {
      
      if(bttnNum == 0)
      {
	    #ifdef SERIAL_DEBUG
        Serial.println("read error");
		#endif
      }  
      return bttnNum;
    }

  }



}

/***********************************************************
 * 
 * hold
 * 
 ***********************************************************/
boolean AtTouch::hold()
{
  if(holdDown_ == true)
  {
	  unsigned long elapsedTime = millis() - startTime;

	  if (elapsedTime >= 1100) //hold interval time has passed
	  {
		
		unsigned int holdRefreshTime = millis()/100 - updateTime;
		
		if(holdRefreshTime >= holdRefreshInterval) //restrict update 100ms refresh
		{
			updateTime = millis()/100; //divide by 100, lose some precision, 100ms minimum
			return true;
		}
		else //don't update too fast
		{
			return false; 
		}
		
		 
	  }
	  else //haven't held down long enough
	  {
		return false;
	  }
   }
   else //not holding down at all
   {
	    return false; 
   }	
}


/***********************************************************
 * 
 * bttnPressISR
 * 
 ***********************************************************/
void bttnPressISR()
{
	AtTouch::pAtTouch->keyHit = true;
	AtTouch::pAtTouch->holdDown_ = true;
	AtTouch::pAtTouch->startTime = millis(); //start hold time,TODO figure out how to do this without reading a function
}



