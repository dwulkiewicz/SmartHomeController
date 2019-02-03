
#include "EventDispatcher.h"
#include "LightsControler.h"
#include "DisplayControler.h"
#include "NetworkControler.h"
#include "SensorsHelper.h"

EventDispatcher::EventDispatcher(){  
}
//----------------------------------------------------------------------------------------
void EventDispatcher::onRefreshIndoorTemperature(float value){
 	displayControler.onRefreshIndoorTemperature(value); 
  heatingControler.onRefreshIndoorTemp(value);  
}
//----------------------------------------------------------------------------------------
void EventDispatcher::onRefreshIndoorPreasure(float value){
  displayControler.onRefreshIndoorPreasure(value); 
}
//----------------------------------------------------------------------------------------
void EventDispatcher::onRefreshIndoorHumidity(float value){
  displayControler.onRefresIndoorHumidity(value);
}
//----------------------------------------------------------------------------------------
void EventDispatcher::onHeatingStatusChange(uint8_t status) {
   displayControler.refreshHeatingStatus(status);
}
//----------------------------------------------------------------------------------------
void EventDispatcher::onHeatingPeriodChange(uint8_t period) {
	displayControler.refreshHeatingPeriod(period);
}
//----------------------------------------------------------------------------------------
void EventDispatcher::onHeatingRequiredTempChange(float value) {
	displayControler.refreshHeatingRequiredTemp(value);
}
//----------------------------------------------------------------------------------------
void EventDispatcher::onHeatingConfigurationChange(void){
	heatingControler.onConfigurationChange();  
}  
//----------------------------------------------------------------------------------------
void EventDispatcher::onRefreshDateTime(const DateTime& dateTime){
	displayControler.onRefreshDateTime(dateTime);
	heatingControler.onRefreshDateTime(dateTime);
}
//----------------------------------------------------------------------------------------
void EventDispatcher::onSwitchChange(uint8_t switchId, uint8_t switchState) {
	networkControler.onSwitchChanged(switchId, switchState);
	displayControler.onSwitchChanged(switchId, switchState);
	lightsControler.onSwitchStateChanged(switchId, switchState);
}
//----------------------------------------------------------------------------------------
void EventDispatcher::onRefreshOutdoorTemperature(float value) {
  displayControler.onRefreshOutdoorTemperature(value);
}
//----------------------------------------------------------------------------------------
void EventDispatcher::onRefreshOutdoorHumidity(float value) {
  displayControler.onRefreshOutdoorHumidity(value);
}
//----------------------------------------------------------------------------------------
void EventDispatcher::onRefreshOutdoorPressure(float value) {
  displayControler.onRefreshOutdoorPreasure(value);
}
//----------------------------------------------------------------------------------------
void EventDispatcher::onWiFiStatusChange(uint8_t status){
  displayControler.refreshWiFiStatus(status);  
}
//----------------------------------------------------------------------------------------
void EventDispatcher::onMQTTStatusChange(int status){
	//TODO: zmienić na status
	displayControler.refreshMQTTStatus(status);    
}
//----------------------------------------------------------------------------------------
void EventDispatcher::onLightValueChange(uint8_t idx, uint8_t value) {
	lightsControler.onLightValueChanged(idx, value);
}
EventDispatcher eventDispatcher;
