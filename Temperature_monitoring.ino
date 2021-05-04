/*
   Name: Temperature_Monitoring.ino
   Descrption: Will be responsible for accessing all the
   data from the sensors and sharing it via I2C
   Developers: Maitri Trivdei, Harsh Nayal, Jatin Vira
*/

//Inclusion of all necessary header files
#include <OneWire.h>
#include <DallasTemperature.h>
#include <AHT20.h>
#include <Wire.h>
#include <Adafruit_MAX31865.h>

#define ONE_WIRE_BUS A0 //Pin selection for One Wire Bus

//Constants associated with PT1000
#define RREF      4300.0
#define RNOMINAL  1000.0

// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(ONE_WIRE_BUS);

// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);

//Declaration of variables associated with AHT20
int deviceCount = 0;
float T1, T2, humi, temp , aht_val;
int humidity , return_value ;

//Class declerations
AHT20 AHT;
Adafruit_MAX31865 thermo1 = Adafruit_MAX31865(10, 11, 12, 27);
Adafruit_MAX31865 thermo2 = Adafruit_MAX31865(36, 11, 12, 27);

//Declaration of variables associated with PT1000 and Max31865
uint16_t rtd1, rtd2 = 0;
float ratio1, ratio2, pt1, pt2 , R1, R2;
float final_temp, Pt1000_avg;

void setup(void)
{
  sensors.begin();
  AHT.begin();

  Serial.begin(115200);

  // locate devices on the bus
  deviceCount = sensors.getDeviceCount();

  thermo1.begin(MAX31865_2WIRE);
  thermo2.begin(MAX31865_2WIRE);

}

void loop(void)
{
  sensors.requestTemperatures();
  T1 = sensors.getTempCByIndex(0);
  T2 = sensors.getTempCByIndex(1);
  delay(1000);

  return_value = AHT.getSensor(&humi, &temp);
  if (return_value)    // GET DATA OK
  {
    humidity = humi * 100;
    aht_val = temp;
  }
  delay(1000);

  rtd1 = thermo1.readRTD();
  rtd2 = thermo2.readRTD();
  ratio1 = rtd1;
  ratio2 = rtd2;
  ratio1 /= 32768;
  ratio2 /= 32768;
  R1 = RREF * ratio1;
  R2 = RREF * ratio2;
  pt1 = thermo1.temperature(RNOMINAL, RREF);
  pt2 = thermo2.temperature(RNOMINAL, RREF);
  delay(1000);

  if (T1 == T2 && T2 == pt1 && pt1 == pt2)
  {
    final_temp = (T1 + T2 + pt1 + pt2) / 4;
  }
  else
  {
    Pt1000_avg = (pt1 + pt2) / 2 ;
  }
  delay(1000);

}
