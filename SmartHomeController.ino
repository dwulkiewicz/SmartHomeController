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
#include "EventDispatcher.h"
#include "LightsControler.h"
#include "DisplayControler.h"
#include "NetworkControler.h"
#include "RtcControler.h"
#include "SensorsHelper.h"

//#include <RTClib.h>
//RTC_DS1307 rtc;

/*
  SemaphoreHandle_t xMutex;
  //-----------------------------------------------------------------------------------------
  void TaskTempSensorLoop(void * pvParameters) {
	Serial.printf("TaskTempSensorLoop() running on core %d\r\n", xPortGetCoreID());
	while (true) {
		//Serial.print("TaskTempSensorLoop in....\r\n");
		xSemaphoreTake(xMutex, portMAX_DELAY);
		//Serial.print("TaskTempSensorLoop in\r\n");
		sensorsHelper.loop();
		//Serial.print("TaskTempSensorLoop out\r\n");
		xSemaphoreGive(xMutex);
		vTaskDelay(pdMS_TO_TICKS(TASK_INDOOR_TEMP_SENSOR_LOOP));
	}
  }
  //-----------------------------------------------------------------------------------------
  void TaskTimeLoop(void * pvParameters) {
	Serial.printf("TaskTimeLoop() running on core %d\r\n", xPortGetCoreID());
	while (true) {
		//Serial.print("TaskTimeLoop in....\r\n");
		xSemaphoreTake(xMutex, portMAX_DELAY);
		//Serial.print("TaskTimeLoop in\r\n");


		//rtcControler.loop();


		//sensorsHelper.loop();
		//Serial.print("TaskTimeLoop out\r\n");
		xSemaphoreGive(xMutex);
		vTaskDelay(pdMS_TO_TICKS(TASK_DATATIME_LOOP));
	}
  }
  //-----------------------------------------------------------------------------------------
  void TaskLightsControlerLoop(void * pvParameters) {
	Serial.printf("TaskLightsControlerLoop() running on core %d\r\n", xPortGetCoreID());
	while (true) {
		xSemaphoreTake(xMutex, portMAX_DELAY);
		lightsControler.loop();
		xSemaphoreGive(xMutex);
		vTaskDelay(pdMS_TO_TICKS(TASK_LIGHTS_LOOP));
	}
  }
  //-----------------------------------------------------------------------------------------
  void TaskNextionLoop(void * pvParameters) {
	Serial.printf("TaskNextionLoop() running on core %d\r\n", xPortGetCoreID());
	while (true) {
		xSemaphoreTake(xMutex, portMAX_DELAY);
		displayControler.loop();
		xSemaphoreGive(xMutex);
		vTaskDelay(pdMS_TO_TICKS(TASK_NEXTION_LOOP));
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
*/
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  pinMode(GPIO_BUZZER, OUTPUT);

  digitalWrite(GPIO_BUZZER, HIGH);
  vTaskDelay(pdMS_TO_TICKS(20));
  digitalWrite(GPIO_BUZZER, LOW);

  Wire.begin(I2C_SDA, I2C_SCL);
  Wire1.begin(I2C_SDA_1, I2C_SCL_1);

  vTaskDelay(pdMS_TO_TICKS(1000));

  byte error, address;
  int nDevices;

  Serial.println("Scanning Wire...");
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

  Serial.println("Scanning Wire1...");
  nDevices = 0;
  for (address = 1; address < 127; address++) {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire1.beginTransmission(address);
    error = Wire1.endTransmission();

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


  /*

  	if (!rtc.begin()) {
  		Serial.println("RtcControler::init() Couldn't find RTC");
  	}

  	if (!rtc.isrunning()) {
  		Serial.println("RtcControler::init() RTC is NOT running!");
  		// following line sets the RTC to the date & time this sketch was compiled
  		//rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  		// This line sets the RTC with an explicit date & time, for example to set
  		// January 21, 2014 at 3am you would call:
  		// rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  	}

  	while (true) {
  		DateTime dateTime = rtc.now();

  		Serial.print(dateTime.year(), DEC);
  		Serial.print('.');
  		Serial.print(dateTime.month(), DEC);
  		Serial.print('.');
  		Serial.print(dateTime.day(), DEC);
  		//Serial.print(" (");
  		//Serial.print(dayOfWeekName(dateTime.dayOfTheWeek()));
  		//Serial.print(") ");
  		Serial.print(dateTime.hour(), DEC);
  		Serial.print(':');
  		Serial.print(dateTime.minute(), DEC);
  		Serial.print(':');
  		Serial.print(dateTime.second(), DEC);
  		Serial.println();

  		delay(1000);


  	}
  */

  configuration.init();
  rtcControler.init();
  sensorsHelper.init();
  displayControler.init();
  lightsControler.init();

  heatingControler.init(rtcControler.now(), sensorsHelper.getTemperature());

  /* create Mutex */
  //xMutex = xSemaphoreCreateMutex();
  //xTaskCreatePinnedToCore(TaskTimeLoop, "TaskTimeLoop", 4096, NULL, 1, NULL, CORE_1);
  //xTaskCreatePinnedToCore(TaskTempSensorLoop, "TaskTempSensorLoop", 10000, NULL, 1, NULL, CORE_2);
  //xTaskCreatePinnedToCore(TaskNextionLoop, "TaskNextionLoop", 10000, NULL, 1, NULL, CORE_2);
  //xTaskCreatePinnedToCore(TaskLightsControlerLoop, "TaskLightsControlerLoop", 10000, NULL, 1, NULL, CORE_2);

  networkControler.init();
  //xTaskCreatePinnedToCore(TaskNetworkControlerLoop, "TaskNetworkControlerLoop", 10000, NULL, 1, NULL, CORE_1);

  heatingControler.onConfigurationChange();
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------


uint32_t loop_load_avg = 0;
uint32_t loop_last_time = 0;
uint32_t loop_last_light = 0;
uint32_t loop_last_sensor = 0;
uint32_t loop_last_display = 0;
uint32_t loop_last_statistic = 0;

uint32_t max_load = 0;
uint32_t max_loop_net = 0;
uint32_t max_loop_time = 0;
uint32_t max_loop_light = 0;
uint32_t max_loop_sensor = 0;
uint32_t max_loop_display = 0;


bool sonOffSwitch01 = false;

void loop() {
  uint32_t loop_start = millis();
  networkControler.loop();
  max_loop_net = MAX(millis() - loop_start, max_loop_net);

  if (millis() - loop_last_display > TASK_NEXTION_LOOP) {
    loop_last_display = millis();
    displayControler.loop();
    max_loop_display = MAX(millis() - loop_last_display, max_loop_display);
  }

  if (millis() - loop_last_time > TASK_DATATIME_LOOP) {
    loop_last_time = millis();
    rtcControler.loop();
    max_loop_display = MAX(millis() - loop_last_time, max_loop_time);
  }

  if (millis() - loop_last_light > TASK_LIGHTS_LOOP) {
    loop_last_light = millis();
    lightsControler.loop();
    max_loop_light = MAX(millis() - loop_last_light, max_loop_light);
  }

  if (millis() - loop_last_sensor > TASK_INDOOR_TEMP_SENSOR_LOOP) {
    loop_last_sensor = millis();
    sensorsHelper.loop();
    max_loop_sensor = MAX(millis() - loop_last_sensor, max_loop_sensor);
  }

  max_load = MAX(millis() - loop_start, max_load);

  if (millis() - loop_last_statistic > 4000) {

    //eventDispatcher.onScreenTouchSwitch(SWITCH_SONOFF_1_IDX, sonOffSwitch01);
    //sonOffSwitch01 = !sonOffSwitch01;

    loop_last_statistic = millis();
    logger.log(debug, "max_loop: %d, max_loop_net: %d, max_loop_time: %d, max_loop_light: %d, max_loop_sensor: %d, max_loop_display: %d\r\n", max_load, max_loop_net, max_loop_time, max_loop_light, max_loop_sensor, max_loop_display);

    max_load = 0;
    max_loop_net = 0;
    max_loop_time = 0;
    max_loop_light = 0;
    max_loop_sensor = 0;
    max_loop_display = 0;
  }
}
