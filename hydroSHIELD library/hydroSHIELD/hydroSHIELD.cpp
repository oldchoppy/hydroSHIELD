#include <Arduino.h>
#include <hydroSHIELD.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>
//OneWire library Setup
OneWire oneWire(temp_pin); //onewire setup instance
DallasTemperature sensors(&oneWire); //pass one Wire reference to DallasTemperature library
DeviceAddress insideThermometer; //array for device address

//Global Variables
//----------------
int counter;

//---------------------
//Initialize the shield
//---------------------
void hydroSHIELD::init(){
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);
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
	Serial.println(sensors.getDeviceCount(), DEC); //number of found deviceson the bus
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
	//------------------
	//LCD initialization
	//------------------
	byte ul[8] = { //upper left part of shield graphic
  	B00111,
 	B00100,
  	B00100,
  	B00100,
  	B00100,
  	B00010,
	B00001,
	};
	byte ur[8] = { //upper right part of shield graphic
  	B11100,
 	B00100,
  	B00100,
  	B00100,
  	B00100,
  	B01000,
  	B10000,
	};
	byte um[8] = { //upper middle part of shield graphic
  	B11111,
	B00000,
 	B00100,
  	B01010,
  	B10001,
  	B01110,
  	B00000,
	B11111,
	};
	lcd.createChar(0,ul); //library reference to create custom graphic
	lcd.createChar(1,ur); //library reference to create custom graphic
	lcd.createChar(2,um); //library reference to create custom graphic
	lcd.begin(16,4); //configure lcd columns, rows
	lcd.setCursor(0,3); //set cursor to bottom line for text
	lcd.print("  hydroSHIELD"); //title
	lcd.setCursor(6,1); //set cursor to middle for icon
	lcd.write(byte(0)); //draw icon 1/3
	lcd.write(byte(2)); //draw icon 2/3
	lcd.write(byte(1)); //draw icon 3/3
	pinMode(L_pin,OUTPUT); //configure the LCD screen backlight
     setLCDBACKLIGHT(HIGH); //turn on LCD screen backlight
	//-----------------------------
	//Water Solenoid Initialization
	//-----------------------------
	pinMode(water_sol_pin,OUTPUT);	
	//----------------------
	//Sensor Enable PIN Init
	//----------------------
	enableSENSOR(HIGH);
	//----------------------
	//Panel Buttons Init
	//----------------------
	pinMode(button_right_pin, INPUT);
	digitalWrite(button_right_pin, HIGH);
	pinMode(button_left_pin, INPUT);
	digitalWrite(button_left_pin, HIGH);	
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
void hydroSHIELD::pump(boolean state){
	if(state){
	digitalWrite(pump_pin,HIGH);
	}
	else{
	digitalWrite(pump_pin,LOW);
	}
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

//----------------------------
//Set water solenoid ON or OFF
//----------------------------
void hydroSHIELD::setWATER(boolean state){
	if(state){
	digitalWrite(water_sol_pin,HIGH);
	}
	else{
	digitalWrite(water_sol_pin,LOW);
	}
}

//-------------------------------------
//Enable TDS, PH, SOIL MOISTURE SENSORS
//-------------------------------------
void hydroSHIELD::enableSENSOR(boolean state){
	if(state){
	digitalWrite(sensor_e,HIGH);
	}
	else{
	digitalWrite(sensor_e,LOW);
	}
}
//-------------------------------------
//Read Panel Button Inputs
//-------------------------------------
boolean hydroSHIELD::getBUTTON_LEFT(){
	return digitalRead(button_left_pin);
}
boolean hydroSHIELD::getBUTTON_RIGHT(){
	return digitalRead(button_right_pin);
}
//-------------------------------------
//Read PH from Sensor
//-------------------------------------
/*This code has been adapted from DFROBOT.COM:
 # This sample codes is for testing the pH meter V1.0.
 # Editor : YouYou
 # Date   : 2013.10.12
 # Ver    : 0.1
 # Product: pH meter
 # SKU    : SEN0161
*/
float hydroSHIELD::getPH(){
int buf[10],temp;
float b;
unsigned long int avgValue;

  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(PH_pin);
    delay(10);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++) {                     //take the average value of 6 center sample
  avgValue+=buf[i];
  }
  float phValue=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
  phValue=3.5*phValue;                      //convert the millivolt into pH value
  return phValue; 
}
//----------------------------
//TDS METER FUNCTIONS
//----------------------------
float hydroSHIELD::getTDS(){
/***************************************************
 DFRobot Gravity: Analog TDS Sensor / Meter For Arduino
 <https://www.dfrobot.com/wiki/index.php/Gravity:_Analog_TDS_Sensor_/_Meter_For_Arduino_SKU:_SEN0244>
 
 Created 2017-8-22
 By Jason <jason.ling@dfrobot.com@dfrobot.com>
 
 GNU Lesser General Public License.
 See <http://www.gnu.org/licenses/> for details.
 All above must be included in any redistribution
 
 /***********Notice and Trouble shooting***************
 1. This code is tested on Arduino Uno and Leonardo with Arduino IDE 1.0.5 r2 and 1.8.2.
 2. More details, please click this link: <https://www.dfrobot.com/wiki/index.php/Gravity:_Analog_TDS_Sensor_/_Meter_For_Arduino_SKU:_SEN0244>
 ****************************************************/

#define TdsSensorPin TDS_pin
#define VREF 5.0      // analog reference voltage(Volt) of the ADC
#define SCOUNT  30           // sum of sample point
int analogBuffer[SCOUNT];    // store the analog value in the array, read from ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0,copyIndex = 0;
float averageVoltage = 0,tdsValue = 0,temperature=getTEMP();
float bTemp=analogRead(TDS_pin);
	for(int i=1;i<=SCOUNT;i++){
	bTemp=bTemp+analogRead(TDS_pin);
}

      averageVoltage = bTemp * (float)VREF/SCOUNT/ 1024.0; // read the analog value more stable by the median filtering algorithm, and convert to voltage value
      float compensationCoefficient=1.0+0.02*(temperature-25.0);    //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
      float compensationVolatge=averageVoltage/compensationCoefficient;  //temperature compensation
      tdsValue=(133.42*compensationVolatge*compensationVolatge*compensationVolatge - 255.86*compensationVolatge*compensationVolatge + 857.39*compensationVolatge)*0.5; //convert voltage value to tds value
      //Serial.print("voltage:");
      //Serial.print(averageVoltage,2);
      //Serial.print("V   ");
      Serial.print("TDS Value:");
      Serial.print(tdsValue,0);
      Serial.println("ppm");
}
//------------------------------
//LCD BACKLIGHT FUNCTION
//------------------------------
void hydroSHIELD::setLCDBACKLIGHT(boolean state){
	if (state)
	digitalWrite(L_pin,HIGH);
	else
	digitalWrite(L_pin,LOW);
}
