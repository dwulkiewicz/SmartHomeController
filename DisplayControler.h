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
public:
  static uint8_t dayOfWeekPic(uint8_t dayOfWeek);
  static uint8_t monthPic(uint8_t month);  
};

extern NexText tIndoorTemp1;
extern NexText tIndoorTemp2;
extern NexText tDayOfMonth;
extern NexPicture pDayOfWeek;
extern NexPicture pMonth;

extern NexText tTime1;
extern NexText tTime2;
extern NexText tTime3;
extern NexText tTime4;




#endif /* #ifndef __DisplayControler_H__ */
