/*hydroSHIELD.h - Library for controlling an open source hydroponic shield.
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
//LCD PINS
#define L_pin 9
#define D7_pin 10
#define D6_pin 11
#define D5_pin 12
#define D4_pin 13
#define E_pin A4
#define RS_pin A5

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
  void pump();
  boolean getLEVEL();
  //void *function*(); place code here
};





#endif //end of the allowance for accidental double include, place all code before this line
