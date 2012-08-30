/************************************************************************************
 * 	
 * 	Name    : Triggertrap HWTest.cpp                       
 * 	Author  : Noah Shibley / NoMi Design                         
 * 	Date    : July 10th 2011                                    
 * 	Version : 0.1                                              
 * 	Notes   : HardWare Test Mode for factory QA.                 
 * 
 * 			  Code for Triggertrap device (TTv1) and Triggertrap Shield (TT-k)                      
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

// To enable the hardware test mode, uncomment the appropriate line in TTConfig.h


#include <HWTest.h>

//for the ISR
HWTest* HWTest::pTTUI = 0; 

#ifdef TT_SHIELD
HWTest::HWTest():
LiquidCrystal_SR_LCD3(11, 10, 12 ,2) 
#else
HWTest::HWTest():
LiquidCrystal(A3,4,5, 6, 7, 8, 9) 
#endif
{

  pTTUI = this;	//the ptr points to this object
  trapActive_ = false; 
}

/***********************************************************
 * 
 * begin
 * 
 ***********************************************************/
void HWTest::setup(Trigger& laser, Trigger& sound, Trigger& light,Trigger& timeLapse, Trigger& aux)
{

#ifdef SERIAL_DEBUG
  Serial.begin(9600);
#endif

#ifndef SERIAL_DEBUG
  // USB Serial port configure as input, disable pullup
  pinMode (0, INPUT);
  pinMode (1, INPUT);
  digitalWrite(0, LOW);
  digitalWrite(1, LOW);
#endif

  analogReference(INTERNAL);

  //set triggers into array
  triggers[0] = &laser;
  triggers[1] = &sound;
  triggers[2] = &light;
  triggers[3] = &timeLapse;
  triggers[4] = &aux;

  testState = 0;

  //configure start button
  pinMode(START_BUTTON, INPUT);       // Start Button
  digitalWrite(START_BUTTON, HIGH);   // turn on pullup resistor for Start button
#ifdef TT_SHIELD
  PCintPort::attachInterrupt(START_BUTTON,startTestHandler,FALLING);  
#else
  attachInterrupt(0,startTestHandler,FALLING); //trigger ISR function on start button press.
#endif

  //Shutter and Focus pins set to output
  pinMode(FOCUS_TRIGGER_PIN, OUTPUT);
  pinMode(SHUTTER_TRIGGER_PIN, OUTPUT);
  digitalWrite(FOCUS_TRIGGER_PIN, HIGH); // is off
  digitalWrite(SHUTTER_TRIGGER_PIN, HIGH); // is off

  //set UI Power
#ifndef TT_SHIELD //if TT is normal not Shield
  DDRB |= (1<<PORTB7);   //pinMode(KEY_PAD_LEDS, OUTPUT);      // LED on UI
  DDRB |= (1<<PORTB6);   //pinMode(POWER_UI,OUTPUT);
#endif	
  state_UIPower = false; 
  uiPowerOn(); //turn on power at startup

#ifdef TT_SHIELD
  touch.begin();
#else
  touch.begin(KEY_CHANGE);  //init touch UI with key change interrupt
#endif //TT_SHIELD endif

  //this class inherits from LCD, so call lcd functions as part of this class
#ifndef TT_SHIELD //if TT is normal not Shield
  //LCD Stuff
  TCCR1B = TCCR1B & 0b11111000 | 0x01; //-- sets the pwm base

  pinMode (10, OUTPUT); // lcd contrast output 

  if(batteryPower() == true)
  {
    long lcdContrast = analogRead(A1);
    lcdContrast = 175 - ((lcdContrast * 5)  / 32);
    analogWrite (10, lcdContrast);

  }
  else
  {
    byte lcdContrast = 25; 
    analogWrite (10, lcdContrast);

  }
#endif // END TT if

  begin(8, 2);
  // Print a message to the LCD.
  print("TT ");
  setCursor(0,1);
  print("QA TEST");


#ifdef SERIAL_DEBUG
  Serial.print("TT QA TEST v");
  Serial.println(FIRMWARE_VERSION);
#endif

  interrupts(); //make sure interrupts are on
}

/***********************************************************
 * 
 * checkBttnStatus
 * 
 ***********************************************************/
void HWTest::update()
{
  byte result;
  char printBuffer[9];


  switch(testState)
  {		
  case 0:  //MODE------------------------------------
    updateLCD("Touch","MODE");
    result = keyDown();

    if(result == MODE_BTTN)
    {
      testState++;
    }

    break;
  case 1:  //OPTION------------------------------------
    updateLCD("Touch","OPTION");
    result = keyDown();

    if(result == SELECT_BTTN)
    {
      testState++;
    }

    break;
  case 2:  //DOWN------------------------------------
    updateLCD("Touch","DOWN");
    result = keyDown();

    if(result == DOWN_BTTN)
    {
      testState++;
    }

    break;
  case 3: //UP------------------------------------
    updateLCD("Touch","UP");
    result = keyDown();

    if(result == UP_BTTN)
    {
      testState++;
    }

    break;
  case 4:  //Start------------------------------------
    updateLCD("Press","Start");
    if(startPressed == true)
    {
      startPressed = false;
      testState++;
    }

    break;
  case 5:  //LED------------------------------------
    updateLCD("LED","On");
    PORTB |= (1<<PORTB7); //digitalWrite(KEY_PAD_LEDS,HIGH); turn on keypad LEDs

    if(startPressed == true)
    {
      PORTB &= ~ (1<<PORTB7);  //digitalWrite(KEY_PAD_LEDS,LOW); //turn off led
      startPressed = false;
      testState++;
    }

    break;
  case 6:  //Light Sensor------------------------------------
    triggers[2]->getActiveMessage(printBuffer);
    updateLCD("Light",printBuffer);

	if(triggers[2]->sensorLevel() < 200)
	{
		minThresh = true;
	}
	
	if(minThresh == false && triggers[2]->sensorLevel() > 200)
	{
		updateLCD("Too","Bright");
		delay(1000);
	}
	
    if(minThresh == true && triggers[2]->sensorLevel() > 250)
    {
      minThresh = false;
      testState++;
    }

    break;
  case 7: //Sound Sensor------------------------------------					
    triggers[1]->getActiveMessage(printBuffer);
    updateLCD("Sound",printBuffer);

    if(triggers[1]->sensorLevel() <= 200)
	{
		minThresh = true;
	}
	
	if(minThresh == false && triggers[1]->sensorLevel() > 200)
	{
		updateLCD("Too","Loud");
		delay(1000);
	}
	
    if(minThresh == true && triggers[1]->sensorLevel() > 250)
    {
      minThresh = false;
      testState++;
    }

    break;
  case 8:  //Laser Sensor------------------------------------

    triggers[0]->getActiveMessage(printBuffer);
    updateLCD("Laser",printBuffer);

    if(triggers[0]->sensorLevel() <= 200)
	{
		minThresh = true;
	}

	if(minThresh == false && triggers[0]->sensorLevel() > 200)
	{
		updateLCD("Too","Bright");
		delay(1000);
	}

    if(minThresh == true && triggers[0]->sensorLevel() > 250)
    {
      minThresh = false;
      testState++;
    }

    break;
  case 9:  //Aux Sensor------------------------------------

    triggers[4]->getActiveMessage(printBuffer);
    updateLCD("Aux",printBuffer);

	 if(triggers[4]->sensorLevel() == 0)
	 {	
      	testState++;
	 }
   
    break;
  case 10: //Test IR------------------------------------

    updateLCD("Test","IR");
    triggers[0]->IRTransmit();

    if(startPressed == true)
    {
      startPressed = false;
      testState++;
    }
    break;

  case 11:  //Test Focus------------------------------------

    updateLCD("Test","Focus");
    digitalWrite(FOCUS_TRIGGER_PIN,LOW); //trigger camera

    if(startPressed == true)
    {
      digitalWrite(FOCUS_TRIGGER_PIN,HIGH);
      startPressed = false;
      testState++;
    }

    break;
  case 12:  //Test Shutter------------------------------------

    updateLCD("Test","Shutter");
    digitalWrite(SHUTTER_TRIGGER_PIN,LOW);

    if(startPressed == true)
    {
      digitalWrite(SHUTTER_TRIGGER_PIN,HIGH);
      startPressed = false;
      testState++;
    }
    break;	
  case 13:  //Done------------------------------------

    updateLCD("done","");

    if(startPressed == true)
    {	
	  startPressed = false;
      testState=0;
    }
    break;

  default: //no default option, just here for compiler
    break;
  }


}

void HWTest::updateLCD(char line1[],char line2[])
{
  //only update LCD every 300ms
  int now = millis()/100; //100 ms 
  int elapsed = now - activeRefreshTime;

  if(elapsed > 3)  //300ms
  {
    activeRefreshTime = now;
    clear();
    print(line1);
    setCursor(0,1);
    print(line2); 
  }
}

int HWTest::keyDown()
{

  int hitKeyVal = 0;

  if(touch.hit() == true) //just press key pad
  { 

    hitKeyVal = touch.readActiveKey(); //read which key was hit
  } 

  return hitKeyVal;



}

/***********************************************************
 * 	   
 * uiPowerOn
 *  
 ***********************************************************/
void HWTest::uiPowerOn()
{
  if(state_UIPower == false) //if ui power off
  {
    state_UIPower = true;     


#ifdef TT_SHIELD
    touch.begin();
#else //Normal TT
    PORTB &= ~ (1<<PORTB6);    // enable Vsw power to touch IC and LCD
    delay (150);             // wait for power to stabilize and hw to start
    touch.begin(KEY_CHANGE);         //re init touch keys
#endif //ifdef TT_SHIELD


#ifdef SERIAL_DEBUG
    Serial.println("UI On");
#endif
  }
}

/***********************************************************
 * 
 * batteryPower
 * 
 ***********************************************************/
boolean HWTest::batteryPower()  
{

  if(digitalRead(0) == HIGH || digitalRead(1) == HIGH) //USB connected
  {
    onBatteryPower = false; 
    return false;
  }
  else
  {
    onBatteryPower = true; 
    return true;
  }

}


/***********************************************************
 * 	   
 * startDownHandler ISR
 *  
 ***********************************************************/
void startTestHandler(void)
{

  HWTest::pTTUI->startPressed = true;

}







