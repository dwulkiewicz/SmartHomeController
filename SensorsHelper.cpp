
#include "SensorsHelper.h"
#include "Constants.h"
#include "EventDispatcher.h"

SensorsHelper::SensorsHelper(){
}
//----------------------------------------------------------------------------------------
void SensorsHelper::init() {
  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  bool status = bme.begin(I2C_ADDRESS_BME280, &Wire1);
  if (!status) {
    logger.log(error,"Could not find a valid BME280 sensor, check wiring!\r\n");
  }
}
//----------------------------------------------------------------------------------------
void SensorsHelper::loop(){
	eventDispatcher.onRefreshIndoorTemperature(getTemperature());
	eventDispatcher.onRefreshIndoorHumidity(getHumidity());
	eventDispatcher.onRefreshIndoorPreasure(getPreasure());
}
//----------------------------------------------------------------------------------------
float SensorsHelper::getTemperature(){
    float t = round(bme.readTemperature() * 10.0) / 10.0;
    //Serial.printf("SensorsHelper::getTemperature() -> %f°C\r\n",t);   
    return t;    
}
//----------------------------------------------------------------------------------------
float SensorsHelper::getHumidity(){
    float h = round(bme.readHumidity() * 10.0) / 10.0;
    //Serial.printf("SensorsHelper::getHumidity() -> %f%%\r\n", h);   
    return h;
}
//----------------------------------------------------------------------------------------
float SensorsHelper::getPreasure(){
    float p = round(bme.readPressure()/10.0)/10.0;
    //Serial.printf("SensorsHelper::getPreasure() -> %fhPa\r\n", p);   
    return p;
}

SensorsHelper sensorsHelper;
