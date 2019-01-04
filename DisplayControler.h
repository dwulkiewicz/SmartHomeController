#ifndef __DisplayControler_H__
#define __DisplayControler_H__

#include <Arduino.h>
#include <Nextion.h>
#include "Configuration.h"

class DisplayControler
{
public:
	DisplayControler(Configuration* configuration);
public:
	void init();
	void loop();
private:
	Configuration* configuration;
public: //todo przenieść do private, obudować 
  NexPicture* picWiFiStatus;
};

extern NexText tIndoorTemp1;
extern NexText tIndoorTemp2;
extern NexText tDayOfMonth;
extern NexText tDayOfWeek;
extern NexText tTime1;
extern NexText tTime2;





#endif /* #ifndef __DisplayControler_H__ */
