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
#include <Arduino.h>
#include <hydroSHIELD.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//OneWire library Setup
OneWire oneWire(temp_pin); //onewire setup instance
DallasTemperature sensors(&oneWire); //pass one Wire reference to DallasTemperature library
DeviceAddress insideThermometer; //array for device address


//---------------------
//Initialize the shield
//---------------------
void hydroSHIELD::init(){
	Serial.begin(9600); //begin serial connection
	//--------------------------
	//Temp Sensor Initialization
	//--------------------------
	OneWire oneWire(temp_pin); //onewire setup instance
	//DallasTemperature sensors(&oneWire); //pass one Wire reference to DallasTemperature library
	//DeviceAddress insideThermometer; //array for device address
	Serial.println("Temp Sensor Library Loaded...");
	Serial.print("Locating Temp Sensors...");
	sensors.begin();// begin communication with all sensors on the bus
	Serial.print("Found ");
	Serial.println(sensors.getDeviceCount(), DEC); //number of found devices on the bus
	if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0");// Search for devices on the bus and assign based on an index.	
	sensors.setResolution(insideThermometer, 10);//Set resolution (between 9-12 bit) and remember higher resolution takes longer
  	Serial.print("Device 0 Resolution: ");
  	Serial.print(sensors.getResolution(insideThermometer), DEC); //display resolution for a sanity check
  	Serial.println();
	//-------------------
	//Pump Initialization
	//-------------------
	pinMode(pump_pin,OUTPUT);
	digitalWrite(pump_pin,LOW);		
}

//--------------------------------------
//Get temperature as a value in Celcius
//--------------------------------------
float hydroSHIELD::getTEMP(){
	sensors.requestTemperatures();//request temperatures from all devices
	float tempC = sensors.getTempCByIndex(0); //temperature in Celcius of first sensor in the array
	Serial.print("Temp C: ");
	Serial.print(tempC);
	Serial.print(" Temp F: ");
	Serial.println(DallasTemperature::toFahrenheit(tempC)); // Converts tempC to Fahrenheit	
	return tempC;
}

//---------------------------------
//Get Soil Moisture as an integer percentage
//---------------------------------
int hydroSHIELD::getSOILM(){
	int soilm=analogRead(Soil_pin); //read soil sensor
	soilm=map(soilm,250,1023,100,0); //map sensor input to a percentage from 0 to 100
	Serial.println("Soil Moisture="+String(soilm)+"%");
	return soilm;
}

//--------
//Use Pump
//--------
void hydroSHIELD::pump(){
	digitalWrite(pump_pin,HIGH);
}

//------------------------------
//Get tank level as a true/false
//------------------------------
boolean hydroSHIELD::getLEVEL(){
	if(digitalRead(TFS_pin)){ //if float switch is closed
	boolean level=0; //water level is low
	return level;
	}
	else{ //if float switch is open
	boolean level=1; //water level is high
	return level;
	}
}

