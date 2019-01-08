/************************************************************************/
/*                                                                      */
/*              ESP32 WiFi Lora - DS1307 Nextion                  */
/*              Hardware: ESP32 (Heltec Wifi-Lora-32)                   */
/*                                                                      */
/*              Author: Dariusz Wulkiewicz                              */
/*                      d.wulkiewicz@gmail.com                          */
/*                                                                      */
/*              Date: 12.2018                                           */
/************************************************************************/
#include <Arduino.h>
#include <math.h>
#include <ArduinoOTA.h>
#include <Nextion.h>
#include <Wire.h> //I2C
#include <uRTCLib.h>

#include "Constants.h"
#include "Configuration.h"
#include "DisplayControler.h"
#include "NetworkControler.h"
#include "SensorsHelper.h"
#include "RtcHelper.h"

//-----------------------------------------------------------------------------------------

Configuration configuration;
DisplayControler displayControler(&configuration);
NetworkControler networkControler(&configuration, &displayControler);

//-----------------------------------------------------------------------------------------
uint8_t lastTemp1 = 255;
uint8_t lastTemp2 = 255;
void TaskTempSensorLoop(void * pvParameters) {
  Serial.printf("TaskTempSensorLoop() running on core %d\r\n", xPortGetCoreID());
  while (true) {
	SensorsHelper::startMeasure();
    delay(1000);
    /*Indoor temperature*/
	  uint16_t t = SensorsHelper::getTemperature();
    uint8_t t1 = t / 10;
    uint8_t t2 = t % 10;         
    if (lastTemp1 != t1){
      if (tIndoorTemp1.setText(String(t1).c_str()))
        lastTemp1 = t1;
    }  
    if (lastTemp2 != t2){
      if (tIndoorTemp2.setText(String(t2).c_str()))
        lastTemp2 = t2;
    }   
  }
}
//-----------------------------------------------------------------------------------------


void TaskTimeLoop(void * pvParameters) {
  Serial.printf("TaskTimeLoop() running on core %d\r\n", xPortGetCoreID());
  uint8_t lastMinute = 99;
  uint8_t lastHour = 99;
  uint8_t lastDay = 99;
  uint8_t lastMonth = 99;
  uint8_t lastDayOfWeek = 99;
  uint8_t minute = 99;
  uint8_t hour = 99;
  uint8_t day = 99;
  uint8_t month = 99;
  uint8_t dayOfWeek = 99;
  char buf[5];

  while (true) {
    //Serial.printf("TaskTimeLoop() running on core %d\r\n", xPortGetCoreID());
    rtc.refresh();
	month = rtc.month();
	day = rtc.day();
	dayOfWeek = rtc.dayOfWeek();
	hour = rtc.hour();
	minute = rtc.minute();

	/*Hour*/
	if (lastHour != hour) {
		if (tTime1.setText(String(hour / 10).c_str()) && tTime2.setText(String(hour % 10).c_str()))
			lastHour = hour;
	}
	/*Minute*/
	if (lastMinute != minute) {
		if (tTime3.setText(String(minute / 10).c_str()) && tTime4.setText(String(minute % 10).c_str()))
			lastMinute = minute;
	}
  	/*Month*/
    if (lastMonth != month){
      if(pMonth.setPic(DisplayControler::monthPic(month)))
        lastMonth = month;
    }
  	/*Day of month*/
  	if (lastDay != day) {
		if(pDayOfMonth1.setPic(DisplayControler::dayOfMonthPic(day/10)) && pDayOfMonth2.setPic(DisplayControler::dayOfMonthPic(day%10)))
  		  lastDay = day;
  	} 
    /*Day of week*/
  	if (lastDayOfWeek != dayOfWeek) {
  		if(pDayOfWeek.setPic(DisplayControler::dayOfWeekPic(dayOfWeek)))
  		  lastDayOfWeek = dayOfWeek;
  	}

  delay(500);
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
  delay(2000);

  pinMode(BUILT_LED, OUTPUT);
  Wire.begin(I2C_SDA, I2C_SCL);

  Serial.printf("setup() running on core %d\r\n", xPortGetCoreID());

  configuration.init();
  SensorsHelper::init();
  displayControler.init();
  networkControler.init();

  xTaskCreatePinnedToCore(TaskOTALoop, "TaskOTALoop", 4096, NULL, 1, NULL, CORE_1);
  xTaskCreatePinnedToCore(TaskNextionLoop, "TaskNextionLoop", 4096, NULL, 1, NULL, CORE_2);
  xTaskCreatePinnedToCore(TaskTimeLoop, "TaskTimeLoop", 4096, NULL, 1, NULL, CORE_1);
  xTaskCreatePinnedToCore(TaskTempSensorLoop, "TaskTempSensorLoop", 4096, NULL, 1, NULL, CORE_1);
  xTaskCreatePinnedToCore(TaskNetworkControlerLoop, "TaskNetworkControlerLoop", 4096, NULL, 1, NULL, CORE_1);

  
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
