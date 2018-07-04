/*  hydroSHIELD source code
    Copyright (C) 2018  David Stanger

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <hydroSHIELD.h> //include the library, this automatically includes OneWire and DallasTemperature. Be sure to install these libraries prior to running.
#include <LiquidCrystal.h>


hydroSHIELD hs; //create the instance
float tempC; //variable for temperature in Celcius
int soilm_setpoint = 1; // change the soil moisture setpoint to control how much water is pumped. This number is between 1-100 in percent  (ie. setpoint=1 means pump when lower than 1%)
int time_s; //variable for keeping time in seconds
int time_s_prev;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
boolean idle; //flag for idle state
int powersave_threshold=10; //number of idle seconds before activating powersave mode
int time_diff;
int idle_threshold=1;

void setup() {
  //lcd.print("hello, world!");
  hs.init(); //function to setup the hydroponic shield configuration
  delay(1000);
  lcd.clear();
}

void loop() {
  time_diff=time_s-time_s_prev;

  //-----------------
  //Water Refill Mode
  //-----------------
  if(hs.getBUTTON_RIGHT()==0&&idle==false&&time_diff>=idle_threshold){//if the right button is pressed while not idling
    lcd.print("filling...");//print the status on the screen
    while(hs.getLEVEL()==0){//wait for level sensor to trigger
      hs.setWATER(HIGH);//energize solenoid
    }
    hs.setWATER(LOW);//de-energize solenoid after tank is full
    lcd.clear();//clear lcd
  }
  //----------------
  //POWERSAVING MODE
  //----------------
  time_s = millis() / 1000; // seconds the program has been running
  if (time_diff>powersave_threshold){//time since last button activity is greater than the threshold
  powersave_mode(HIGH);//turn powersave mode on
  idle=true;
  }
  if(hs.getBUTTON_LEFT()==0||hs.getBUTTON_RIGHT()==0){ //if either button is pressed, turn off power saving mode
    powersave_mode(LOW); //turn off power saving mode
    idle=false; //reset idle time flag
    if(time_diff>powersave_threshold){
    time_s_prev=time_s; //save the time last idle flag was reset
    }
  }
  //------------------
  lcd.setCursor(0, 1);
  //delay(2500);
  //delay(500);
  //digitalWrite(4,HIGH);
  //hs.setWATER(HIGH);
  //tempC = hs.getTEMP();
  //hs.enableSENSOR(HIGH);
  //hs.setWATER(LOW);
  //digitalWrite(4,LOW);
  //boolean level = hs.getLEVEL();
  //Serial.println("water level is: " + String(level));
  int soilm = hs.getSOILM();
  if (soilm < 1) {
    hs.pump(HIGH);
  }
  else {
    hs.pump(LOW);
  }
  Serial.println("PH is " + String(hs.getPH()));
  //hs.getTDS();
}


//-------------------
//POWER SAVE FUNCTION
//-------------------
void powersave_mode(boolean state) {
  if (state) {
    hs.setLCDBACKLIGHT(LOW); //turn off the LCD screen backlight
    lcd.noDisplay(); //turn off the LCD display but keep the info current
    hs.enableSENSOR(LOW); //turn off the sensor enable pin (disables PH,TDS, and soil moisture sensors)
  }
  else {
    hs.setLCDBACKLIGHT(HIGH); //turn on the backlight
    lcd.display(); //turn on the display
    //sensor enable is not turned on because a polling function will ensure it is turned on before reading values
  }
}

