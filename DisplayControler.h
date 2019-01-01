#ifndef __DisplayControler_H__
#define __DisplayControler_H__

#include <Arduino.h>
#include <Nextion.h>
#include "Configuration.h"

class DisplayControler
{
public:
	DisplayControler(Configuration* configuration);
	void setup();
	void loop();
private:
	Configuration* configuration;


public: //todo przenieść do private, obudować 
  NexPicture* picWiFiStatus;
};

#endif /* #ifndef __DisplayControler_H__ */
