#include "timelapse.h"

//mode menu title
const prog_char timeMenu[] PROGMEM= {"TimeLaps"};

 //option Menu titles
const prog_char delta[] PROGMEM="delta";
const prog_char startdelay[] PROGMEM="delay";
const prog_char numShots[] PROGMEM = "#shots";

const prog_char * timeSelectMenu[] PROGMEM  = 	   //options menu order
{   
delta,
startdelay,
numShots,
};

//option order should match timeSelectMenu order
const int TIME_DELTA = 0;
const int TIME_DELAY = 1; 
const int TIME_NUMSHOTS = 2;

/***********************************************************
 * 
 * TimeLapse
 *
 *  
 * 
 ***********************************************************/
TimeLapse::TimeLapse(){
	
	    triggerState_ = false; //off

		setOption(TIME_DELTA,0);    //set time delta to 0
		setOption(TIME_DELAY,0);    //set time delay to 0
		setOption(TIME_NUMSHOTS,0); //set #shots to 0
		delayCount = 0; 
		select_ = 0;  //set 
//		Sleep sl;
//		sleep = &sl;
	  	
	
}

boolean TimeLapse::delayFirstShot()
{	
	if(shotCounter_ < 1) //first shot happens RIGHT away on start button press, unless delayed
	{
			if(delayCount == 0) { delayCount = millis(); } //set this only the first time through the loop
			shutterReady = true; //if delay is up, take the first shot, 
			shutter(true); //set delay to true, we want delay
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
boolean TimeLapse::trigger()
{
	//delay first shot
	//take new shot at each interval
	//sleep if in battery mode
	//stop if past shotcount
	int remainTime;
	//don't allow zero time delta. Will crash the device
	if(option(TIME_DELTA) == 0) { incOption(TIME_DELTA,1); }
   

	if(delayFirstShot() == true)
	{
		return true;
	}

	remainTime = countDown(); //get the remaining time to next shot

	shutter(false); //delay to false, delay is only for 1st shot
			
	 if(digitalRead(0) == HIGH || digitalRead(1) == HIGH) //USB
	 {
	
	 }
	 else
  	 {
		if(shutterStateA_ == false && shutterStateB_ == false) //don't sleep if shutter is triggered
		{
			sleepNow(remainTime); //sleep now if there is time left to sleep
		}	
	  }
			
			
	  if (remainTime < 1) 
	  {
			//times up,take a shot
			delayCount = millis(); //start counting till delay is up

			startBttnTime = delayCount; //don't call millis twice, just use delayCount, reset startButtnTime
			timelapseCountDown = startBttnTime/1000;
			shutterReady = true;
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
int TimeLapse::countDown()
{
	
	 if(digitalRead(0) == HIGH || digitalRead(1) == HIGH) //USB connected
	 {
		if(shotCounter_ < 1)
		{
			startBttnTime = millis(); //reset startBttnTime, so interval starts here
		}
		timelapseCountDown = millis()/1000;
	 }	
	 else
	 {
	 
		if(shotCounter_ < 1)
		{
			startBttnTime = millis(); //reset startBttnTime, so interval starts here
			timelapseCountDown = startBttnTime/1000; //set it the same, for use with sleep watchdog, reset if not using sleep
		}
	}	
	

	
      int elapsedTime = timelapseCountDown - startBttnTime/1000;

	  int remainTime = option(TIME_DELTA) - elapsedTime;
	

	return remainTime;

}

/***********************************************************
 * 
 * SleepNow
 *
 *  
 * 
 ***********************************************************/
void TimeLapse::sleepNow(int remainTime)
{
//	int remainTime = option(TIME_DELTA) - elapsedTime;
	
	Sleep sleep;
	
	sleep.pwrSaveMode();
	//sleep.addInterrupt0(FALLING); //startButton press down to cancel sleep
	
	if(remainTime > 8)
	{
		#ifdef SERIAL_DEBUG
		Serial.println("sleeping for: 8 sec");   
		delay(100);
		#endif
		timelapseCountDown = timelapseCountDown + 8;	
		sleep.addWatchDog(9); //8sec
		sleep.sleepNow(); //go to sleep
		return;
	}
	else if(remainTime > 4)
	{
		#ifdef SERIAL_DEBUG
		Serial.println("sleeping for: 4 sec");
		delay(100);
		#endif
	//	timelapseCountDown = timelapseCountDown + millis()/1000; 
	    timelapseCountDown = timelapseCountDown + 4;
		sleep.addWatchDog(8); //4sec
		 sleep.sleepNow(); //go to sleep
		 return;
	}
	else if(remainTime > 2)
	{
	   	#ifdef SERIAL_DEBUG
		Serial.println("sleeping for: 2 sec");
		delay(100);
		#endif	
	   timelapseCountDown = timelapseCountDown + 2;
		sleep.addWatchDog(7); //2sec
		 sleep.sleepNow(); //go to sleep
		return;
	}
	else if(remainTime > 1)
	{
		#ifdef SERIAL_DEBUG
		Serial.println("sleeping for: 1 sec");
		delay(100);
		#endif
//		timelapseCountDown = timelapseCountDown + millis()/1000; 
		timelapseCountDown = timelapseCountDown + 1;
		sleep.addWatchDog(6); //1sec
		 sleep.sleepNow(); //go to sleep
		return;
	}
	else //if 1 sec left
	{
		timelapseCountDown = timelapseCountDown + 1; 
		
	}
	
	//less then 1sec, don't sleep
	
	  
}

/***********************************************************
 * 
 * decSetting
 *
 *  
 * 
 ***********************************************************/
void TimeLapse::decSetting(char buffer[],int dec)
{

	
		switch (select_)
	    {
	     case TIME_DELAY:
	 	  decOption(TIME_DELAY, 54000,dec); //max secs
		   formatTimeString(option(TIME_DELAY),buffer); //format and save string in buffer
	      break;
	    case TIME_DELTA:
	      decOption(TIME_DELTA, 54000,dec);
	 	  formatTimeString(option(TIME_DELTA),buffer);
	      break;
	    case TIME_NUMSHOTS:
	      decOption(TIME_NUMSHOTS, 50000,dec); 
		  utoa (option(TIME_NUMSHOTS),buffer,10);
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
void TimeLapse::incSetting(char buffer[],int inc)
{
	
	
		switch (select_)
	    {
	     case TIME_DELAY:
	 	   incOption(TIME_DELAY, 54000,inc); //max secs
		   formatTimeString(option(TIME_DELAY),buffer); 
	      break;
	    case TIME_DELTA:
	      incOption(TIME_DELTA, 54000,inc);
	 	  formatTimeString(option(TIME_DELTA),buffer);
	      break;
	    case TIME_NUMSHOTS:
	      incOption(TIME_NUMSHOTS, 50000,inc); 
		  utoa (option(TIME_NUMSHOTS),buffer,10);
	      break;
	    default: 
	      break;
	    }
		
}

/***********************************************************
 * 
 * getModeMenu
 *
 *  
 * 
 ***********************************************************/
void TimeLapse::getModeMenu(char buffer[])
{
	 //reads the timeMenu title from flash
	 strcpy_P(buffer, timeMenu); 
	
}

/***********************************************************
 * 
 * getSelectMenu
 *
 *  
 * 
 ***********************************************************/
void TimeLapse::getSelectMenu(char buffer[])
{
	 //reads the timeSelectMenu options from flash memory
	 strcpy_P(buffer, (const prog_char *)pgm_read_word(&(timeSelectMenu[select_])));
}

/***********************************************************
 * 
 * getActiveMessage
 *
 * get the current sensors LCD message to print during trap Active mode.
 * 
 ***********************************************************/
void TimeLapse::getActiveMessage(char buffer[])
{
	buffer[0] = 0;
	
	itoa (countDown(),buffer,10);
	
	
}