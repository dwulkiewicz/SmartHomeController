/************************************************************************/
/*                                                                      */
/*              Project:  SmartHomeControler							              */
/*              Hardware: ESP32, Nextion 7.0, DS1307, BME280            */
/*                                                                      */
/*              Author: Dariusz Wulkiewicz                              */
/*                      d.wulkiewicz@gmail.com                          */
/*                                                                      */
/*              Date: 01.2019                                           */
/************************************************************************/
#include <Arduino.h>
#include <math.h>
#include <Nextion.h>
#include <Wire.h> //I2C

#include "Constants.h"
#include "Configuration.h"
#include "EventsHandler.h"
#include "LightsControler.h"
#include "DisplayControler.h"
#include "NetworkControler.h"
#include "RtcControler.h"
#include "SensorsHelper.h"

SemaphoreHandle_t xMutex;
//-----------------------------------------------------------------------------------------
void TaskTempSensorLoop(void * pvParameters) {
	Serial.printf("TaskTempSensorLoop() running on core %d\r\n", xPortGetCoreID());
	while (true) {
		xSemaphoreTake(xMutex, portMAX_DELAY);
		eventsHandler.onRefreshIndoorTemperature();
		xSemaphoreGive(xMutex);
		vTaskDelay(pdMS_TO_TICKS(TASK_INDOOR_TEMP_SENSOR_LOOP));
	}
}
//-----------------------------------------------------------------------------------------
void TaskTimeLoop(void * pvParameters) {
	Serial.printf("TaskTimeLoop() running on core %d\r\n", xPortGetCoreID());
	while (true) {
		xSemaphoreTake(xMutex, portMAX_DELAY);
		rtcControler.loop();
		xSemaphoreGive(xMutex);
		vTaskDelay(pdMS_TO_TICKS(TASK_DATATIME_LOOP));
	}
}
//-----------------------------------------------------------------------------------------
void TaskLightsControlerLoop(void * pvParameters) {
	Serial.printf("TaskLightsControlerLoop() running on core %d\r\n", xPortGetCoreID());
	while (true) {
		lightsControler.loop();
		vTaskDelay(pdMS_TO_TICKS(100));
	}
}
//-----------------------------------------------------------------------------------------
void TaskNextionLoop(void * pvParameters) {
	Serial.printf("TaskNextionLoop() running on core %d\r\n", xPortGetCoreID());
	while (true) {
		displayControler.loop();
		vTaskDelay(pdMS_TO_TICKS(100));
	}
}
//-----------------------------------------------------------------------------------------
void TaskNetworkControlerLoop(void * pvParameters) {
	Serial.printf("TaskNetworkControlerLoop() running on core %d\r\n", xPortGetCoreID());
	while (true) {
		networkControler.loop();
		vTaskDelay(1);
	}
}
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
void setup() {
	Serial.begin(115200);

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
	lightsControler.init();
	heatingControler.init();

	/* create Mutex */
	xMutex = xSemaphoreCreateMutex();
	xTaskCreatePinnedToCore(TaskNextionLoop, "TaskNextionLoop", 4096, NULL, 1, NULL, CORE_1);
	xTaskCreatePinnedToCore(TaskTimeLoop, "TaskTimeLoop", 4096, NULL, 1, NULL, CORE_1);
	xTaskCreatePinnedToCore(TaskTempSensorLoop, "TaskTempSensorLoop", 4096, NULL, 1, NULL, CORE_1);
	xTaskCreatePinnedToCore(TaskNetworkControlerLoop, "TaskNetworkControlerLoop", 4096, NULL, 1, NULL, CORE_2);
	xTaskCreatePinnedToCore(TaskLightsControlerLoop, "TaskLightsControlerLoop", 4096, NULL, 1, NULL, CORE_1);
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
void loop() {
	//nope, do nothing here
	vTaskDelay(portMAX_DELAY);
}
