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
}
//----------------------------------------------------------------------------------------
void SensorsHelper::startMeasure() {
	sensors.requestTemperatures(); // Send the command to get temperatures
}

//----------------------------------------------------------------------------------------
uint16_t SensorsHelper::getTemperature(){
	return round(sensors.getTempC(insideThermometer) * 10.0);
}

