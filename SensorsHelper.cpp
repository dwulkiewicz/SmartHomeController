
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "SensorsHelper.h"
#include "Constants.h"


#define ONE_WIRE_BUS DS18B20_PIN


// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// arrays to hold device address
DeviceAddress insideThermometer;

Adafruit_BME280 bme; // I2C

SensorsHelper::SensorsHelper()
{
}

SensorsHelper::~SensorsHelper()
{
}

//----------------------------------------------------------------------------------------
// function to print a device address
void printDS18B20Address(DeviceAddress deviceAddress)
{
	for (uint8_t i = 0; i < 8; i++)
	{
		if (deviceAddress[i] < 16) Serial.print("0");
		Serial.print(deviceAddress[i], HEX);
	}
}
//----------------------------------------------------------------------------------------
void SensorsHelper::init() {


  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  bool status = bme.begin(I2C_ADDRESS_BME280);
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
  }

/*  
	// locate devices on the bus
	Serial.print("Locating DS18B20 devices...");
	sensors.begin();
	Serial.print("Found ");
	Serial.print(sensors.getDeviceCount() + 1, DEC);
	Serial.println(" devices.");

	// report parasite power requirements
	Serial.print("Parasite power is: ");
	if (sensors.isParasitePowerMode())
		Serial.println("ON");
	else
		Serial.println("OFF");

	if (!sensors.getAddress(insideThermometer, 0))
	{
		Serial.println("Unable to find address for Device 0");
	}
	// show the addresses we found on the bus
	Serial.print("Device 0 Address: ");
	printDS18B20Address(insideThermometer);
	Serial.println();

	// set the resolution to 9 bit (Each Dallas/Maxim device is capable of several different resolutions)
	sensors.setResolution(insideThermometer, 12);

	Serial.print("Device 0 Resolution: ");
	Serial.print(sensors.getResolution(insideThermometer), DEC);
	Serial.println();

	sensors.setWaitForConversion(false);
	Serial.println("DONE");
*/ 
}
//----------------------------------------------------------------------------------------
void SensorsHelper::startMeasure() {
//	sensors.requestTemperatures(); // Send the command to get temperatures
}

//----------------------------------------------------------------------------------------
uint16_t SensorsHelper::getTemperature(){
    float t = bme.readTemperature();
    Serial.printf("SensorsHelper::getTemperature() -> %f°C\r\n",t);   
    return round(t*10.0);    
}
//----------------------------------------------------------------------------------------
uint16_t SensorsHelper::getHumidity(){
    float h = bme.readHumidity();
    Serial.printf("SensorsHelper::getHumidity() -> %f%%\r\n", h);   
    return round(h);
}
//----------------------------------------------------------------------------------------
uint16_t SensorsHelper::getPreasure(){
    float p = bme.readPressure();
    Serial.printf("SensorsHelper::getPreasure() -> %fhPa\r\n", p);   
    return round(p);
}
