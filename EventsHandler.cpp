
#include "EventsHandler.h"
#include "LightsControler.h"
#include "DisplayControler.h"
#include "NetworkControler.h"
#include "SensorsHelper.h"

EventsHandler::EventsHandler()
{  
}
//----------------------------------------------------------------------------------------
void EventsHandler::onRefreshIndoorTemperature(){
    //TODO: dodać klasę SensorControler 
	
	displayControler.refreshIndoorTemperature(); 

	float indoorTemp = SensorsHelper::getTemperature() / 10;

	heatingControler.onRefreshIndoorTemp(indoorTemp);
}
//----------------------------------------------------------------------------------------
void EventsHandler::onRefreshDateTime(const TDateTime& dateTime){
  displayControler.onRefreshDateTime(dateTime);
	heatingControler.onRefreshDateTime(dateTime);
}
//----------------------------------------------------------------------------------------
void EventsHandler::onSwitchChange(uint8_t switchId, uint8_t switchState) {
	networkControler.onSwitchChanged(switchId, switchState);
	displayControler.onSwitchChanged(switchId, switchState);
	lightsControler.onSwitchChanged(switchId, switchState);
}

EventsHandler eventsHandler;
