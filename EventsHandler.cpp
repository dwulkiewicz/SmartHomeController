
#include "EventsHandler.h"
#include "LightsControler.h"
#include "DisplayControler.h"
#include "NetworkControler.h"
#include "SensorsHelper.h"

EventsHandler::EventsHandler(){  
}
//----------------------------------------------------------------------------------------
void EventsHandler::onRefreshIndoorTemperature(float value){
 	displayControler.onRefreshIndoorTemperature(value); 
  heatingControler.onRefreshIndoorTemp(value);  
}
//----------------------------------------------------------------------------------------
void EventsHandler::onRefreshIndoorPreasure(float value){
  displayControler.onRefreshIndoorPreasure(value); 
}
//----------------------------------------------------------------------------------------
void EventsHandler::onRefreshIndoorHumidity(float value){
  displayControler.onRefresIndoorHumidity(value);
}
//----------------------------------------------------------------------------------------
void EventsHandler::onHeatingStatusChange(uint8_t status) {
   displayControler.refreshHeatingStatus(status);
}
//----------------------------------------------------------------------------------------
void EventsHandler::onHeatingConfigurationChange(void){
	heatingControler.onConfigurationChange();  
}  
//----------------------------------------------------------------------------------------
void EventsHandler::onRefreshDateTime(const DateTime& dateTime){
	displayControler.onRefreshDateTime(dateTime);
	heatingControler.onRefreshDateTime(dateTime);
}
//----------------------------------------------------------------------------------------
void EventsHandler::onSwitchChange(uint8_t switchId, uint8_t switchState) {
	networkControler.onSwitchChanged(switchId, switchState);
	displayControler.onSwitchChanged(switchId, switchState);
	lightsControler.onSwitchChanged(switchId, switchState);
}
//----------------------------------------------------------------------------------------
void EventsHandler::onRefreshOutdoorTemperature(float value) {
  displayControler.onRefreshOutdoorTemperature(value);
}
//----------------------------------------------------------------------------------------
void EventsHandler::onRefreshOutdoorHumidity(float value) {
  displayControler.onRefreshOutdoorHumidity(value);
}
//----------------------------------------------------------------------------------------
void EventsHandler::onRefreshOutdoorPressure(float value) {
  displayControler.onRefreshOutdoorPreasure(value);
}
//----------------------------------------------------------------------------------------
void EventsHandler::onWiFiStatusChange(uint8_t status){
  displayControler.showWiFiStatus(status);  
}
//----------------------------------------------------------------------------------------
void EventsHandler::onMQTTStatusChange(int status){
  //TODO: zmienić na status
  displayControler.showMQTTStatus(status);    
}


EventsHandler eventsHandler;
