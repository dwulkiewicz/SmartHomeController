
#include "EventsHandler.h"
#include "LightsControler.h"
#include "DisplayControler.h"
#include "NetworkControler.h"

EventsHandler::EventsHandler()
{  
}
//----------------------------------------------------------------------------------------
void EventsHandler::onRefreshIndoorTemperature(){
    displayControler.refreshIndoorTemperature();  
}
//----------------------------------------------------------------------------------------
void EventsHandler::onRefreshDateTime(){
    displayControler.refreshTime();    
}
//----------------------------------------------------------------------------------------
void EventsHandler::onSwitchChange(uint8_t switchId, uint8_t switchState) {
	networkControler.onSwitchChanged(switchId, switchState);
	displayControler.onSwitchChanged(switchId, switchState);
	lightsControler.onSwitchChanged(switchId, switchState);
}

EventsHandler eventsHandler;
