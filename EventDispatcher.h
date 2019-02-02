#ifndef __EventsHandler_H__
#define __EventsHandler_H__

#include <Arduino.h>
#include "RtcControler.h"
#include "HeatingControler.h"

class EventDispatcher
{
public:
	EventDispatcher();
public:
  //DateTime
  void onRefreshDateTime(const DateTime& dateTime);
  //Switches
  void onSwitchChange(uint8_t switchId, uint8_t switchState); 
  //Heating
  void onHeatingStatusChange(uint8_t status);
  void onHeatingPeriodChange(uint8_t period);
  void onHeatingRequiredTempChange(float value);
  void onHeatingConfigurationChange(void);  
  //Indoor
  void onRefreshIndoorTemperature(float value);
  void onRefreshIndoorPreasure(float value);
  void onRefreshIndoorHumidity(float value);  
  //Outdoor
  void onRefreshOutdoorTemperature(float value);
  void onRefreshOutdoorHumidity(float value);
  void onRefreshOutdoorPressure(float value);
  //Network connectiom
  void onWiFiStatusChange(uint8_t status);
  void onMQTTStatusChange(int status);     
  //Lights
  void onLightValueChange(uint8_t idx, uint8_t value);

};
extern EventDispatcher eventDispatcher;
#endif /* #ifndef __EventsHandler_H__ */
