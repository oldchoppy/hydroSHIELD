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

#include <hydroSHIELD.h>

hydroSHIELD hs;
float tempC;
int soilm_setpoint=1;// change the soil moisture setpoint to control how much water is pumped. This number is in percent (ie. pump when lower than 1%).

void setup() {
  // set up the LCD's number of columns and rows:
  //lcd.begin(16, 4);
  // Print a message to the LCD.
  //lcd.print("hello, world!");
  pinMode(9,OUTPUT);
  digitalWrite(9,HIGH);
  hs.init();
  delay(1000);
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
 //digitalWrite(8,HIGH);
  //lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  //lcd.print(millis() / 1000);
  //delay(2500);
  //digitalWrite(8,LOW);
  //delay(500);
  //digitalWrite(4,HIGH);
  //hs.setWATER(HIGH);
  //tempC=hs.getTEMP();
  //Serial.println(tempC);
  hs.enableSENSOR(HIGH);
  delay(1000);
  //hs.setWATER(LOW);
  //digitalWrite(4,LOW);
  boolean level=hs.getLEVEL();
  Serial.println("water level is: "+String(level));
  int soilm=hs.getSOILM();
  if (soilm<1){
    hs.pump(HIGH);
  }
  else{
    hs.pump(LOW);
  }
  Serial.println(hs.getBUTTON_LEFT());
  Serial.println(hs.getBUTTON_RIGHT());
  Serial.println("PH is "+String(hs.getPH()));
  hs.getTDS();
}

