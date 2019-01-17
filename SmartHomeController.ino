/************************************************************************/
/*                                                                      */
/*              Project:  SmartHomeControler							*/
/*              Hardware: ESP32, Nextion 7.0, DS1307, BME280            */
/*                                                                      */
/*              Author: Dariusz Wulkiewicz                              */
/*                      d.wulkiewicz@gmail.com                          */
/*                                                                      */
/*              Date: 01.2019                                           */
/************************************************************************/
#include <Arduino.h>
#include <math.h>
#include <ArduinoOTA.h>
#include <Nextion.h>
#include <Wire.h> //I2C
#include <uRTCLib.h>

#include "Constants.h"
#include "Configuration.h"
#include "LightsControler.h"
#include "DisplayControler.h"
#include "NetworkControler.h"
#include "SensorsHelper.h"
#include "RtcHelper.h"


//-----------------------------------------------------------------------------------------

Configuration configuration;
DisplayControler displayControler(&configuration);
NetworkControler networkControler(&configuration);

SemaphoreHandle_t xMutex;
//-----------------------------------------------------------------------------------------
void TaskTempSensorLoop(void * pvParameters) {
	Serial.printf("TaskTempSensorLoop() running on core %d\r\n", xPortGetCoreID());
	while (true) {
		xSemaphoreTake(xMutex, portMAX_DELAY);
		displayControler.refreshIndoorTemperature();
		xSemaphoreGive(xMutex);
		vTaskDelay(pdMS_TO_TICKS(4000));
	}
}
//-----------------------------------------------------------------------------------------
void TaskTimeLoop(void * pvParameters) {
	Serial.printf("TaskTimeLoop() running on core %d\r\n", xPortGetCoreID());
	while (true) {
		xSemaphoreTake(xMutex, portMAX_DELAY);
		displayControler.refreshTime();
		xSemaphoreGive(xMutex);
		vTaskDelay(pdMS_TO_TICKS(500));
	}
}
//-----------------------------------------------------------------------------------------
void TaskLightsControlerLoop(void * pvParameters) {
	Serial.printf("TaskLightsControlerLoop() running on core %d\r\n", xPortGetCoreID());
	while (true) {
		
		vTaskDelay(pdMS_TO_TICKS(100));
	}
}

//-----------------------------------------------------------------------------------------
void TaskNextionLoop(void * pvParameters) {
	Serial.printf("TaskNextionLoop() running on core %d\r\n", xPortGetCoreID());
	while (true) {
		displayControler.loop();
		vTaskDelay(10);
	}
}
//-----------------------------------------------------------------------------------------
void TaskOTALoop(void * pvParameters) {
	Serial.printf("TaskOTALoop() running on core %d\r\n", xPortGetCoreID());
	while (true) {
		ArduinoOTA.handle();
		vTaskDelay(10);
	}
}

//-----------------------------------------------------------------------------------------
void TaskNetworkControlerLoop(void * pvParameters) {
	Serial.printf("TaskOTALoop() running on core %d\r\n", xPortGetCoreID());
	while (true) {
		networkControler.loop();
		vTaskDelay(10);
	}
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
void setup() {
	Serial.begin(115200);

	lightsControler.init();

	pinMode(GPIO_BUZZER, OUTPUT);

	digitalWrite(GPIO_BUZZER, HIGH);
	delay(50);
	digitalWrite(GPIO_BUZZER, LOW);

	Wire.begin(I2C_SDA, I2C_SCL);

	byte error, address;
	int nDevices;

	Serial.println("Scanning...");
	nDevices = 0;
	for (address = 1; address < 127; address++) {
		// The i2c_scanner uses the return value of
		// the Write.endTransmisstion to see if
		// a device did acknowledge to the address.
		Wire.beginTransmission(address);
		error = Wire.endTransmission();

		if (error == 0) {
			Serial.print("I2C device found at address 0x");
			if (address < 16)
				Serial.print("0");
			Serial.print(address, HEX);
			Serial.println("  !");

			nDevices++;
		}
		else if (error == 4) {
			Serial.print("Unknown error at address 0x");
			if (address < 16)
				Serial.print("0");
			Serial.println(address, HEX);
		}
	}
	if (nDevices == 0)
		Serial.println("No I2C devices found\n");
	else
		Serial.println("done\n");

	configuration.init();
	SensorsHelper::init();
	displayControler.init();
	networkControler.init();

	/* create Mutex */
	xMutex = xSemaphoreCreateMutex();
	xTaskCreatePinnedToCore(TaskOTALoop, "TaskOTALoop", 4096, NULL, 1, NULL, CORE_1);
	xTaskCreatePinnedToCore(TaskNextionLoop, "TaskNextionLoop", 4096, NULL, 1, NULL, CORE_2);
	xTaskCreatePinnedToCore(TaskTimeLoop, "TaskTimeLoop", 4096, NULL, 1, NULL, CORE_1);
	xTaskCreatePinnedToCore(TaskTempSensorLoop, "TaskTempSensorLoop", 4096, NULL, 1, NULL, CORE_1);
	xTaskCreatePinnedToCore(TaskNetworkControlerLoop, "TaskNetworkControlerLoop", 4096, NULL, 1, NULL, CORE_2);
	xTaskCreatePinnedToCore(TaskLightsControlerLoop, "TaskLightsControlerLoop", 4096, NULL, 1, NULL, CORE_2);

	ArduinoOTA
		.onStart([]() {
		String type;
		if (ArduinoOTA.getCommand() == U_FLASH)
			type = "sketch";
		else // U_SPIFFS
			type = "filesystem";

		// NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
		Serial.println("Start updating " + type);
	})
		.onEnd([]() {
		Serial.println("\nEnd");
	})
		.onProgress([](unsigned int progress, unsigned int total) {
		Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
	})
		.onError([](ota_error_t error) {
		Serial.printf("Error[%u]: ", error);
		if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
		else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
		else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
		else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
		else if (error == OTA_END_ERROR) Serial.println("End Failed");
	});
	ArduinoOTA.setHostname((const char *)NetworkControler::getHostName().c_str());
	ArduinoOTA.begin();
}
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
void loop() {
	//nope, do nothing here
	vTaskDelay(portMAX_DELAY);
}
