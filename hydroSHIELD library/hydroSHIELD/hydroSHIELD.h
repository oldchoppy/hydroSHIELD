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

#ifndef hydroSHIELD_h //allows for an accidental double include somewhere in the program
#define hydroSHIELD_h //ditto

#include <Arduino.h>

//DIGITAL PINS
//------------
#define temp_pin 2 //temperature sensor input pin
#define ONE_WIRE_BUS temp_pin //pin number
#define pump_pin 3 //pump output pin
#define water_sol_pin 4 //water solenoid output pin
#define button_right_pin 6 //right panel button
#define button_left_pin 5 //left panel button
//LCD PINS
#define L_pin 9
const int d7=10, d6=11, d5=12, d4=13, en=A4, rs=A5;
//SENSOR ENABLE PIN
#define sensor_e 8 //pin to enable tds, ph and soil moisture sensors

//ANALOG PINS
//------------
#define TDS_pin A0 //total dissolved solids sensor input pin
#define PH_pin A1 //PH sensor input pin
#define TFS_pin A2 //Tank Float Switch input pin
#define Soil_pin A3 //soil moisture pin

//hydroSHIELD class code
class hydroSHIELD
{
  public:
  void init();
  float getTEMP();
  int getSOILM();
  void pump(boolean state);
  boolean getLEVEL();
  void setWATER(boolean state);
  void enableSENSOR(boolean state);
  boolean getBUTTON_LEFT();
  boolean getBUTTON_RIGHT();
  float getPH();
  float getTDS();
  //void *function*(); place code here
};





#endif //end of the allowance for accidental double include, place all code before this line
