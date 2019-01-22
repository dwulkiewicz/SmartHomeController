#ifndef __EventsHandler_H__
#define __EventsHandler_H__

#include <Arduino.h>
#include "RtcControler.h"
#include "HeatingControler.h"

class EventsHandler
{
public:
	EventsHandler();
public:
  void onRefreshIndoorTemperature();
  void onRefreshDateTime(const TDateTime& dateTime);
  void onSwitchChange(uint8_t switchId, uint8_t switchState);
};
extern EventsHandler eventsHandler;
#endif /* #ifndef __EventsHandler_H__ */
