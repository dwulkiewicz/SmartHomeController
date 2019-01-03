
#include <Arduino.h>
#include "Configuration.h"
#include "DisplayControler.h"

#ifndef __NetworkControler_H__
#define __NetworkControler_H__


class NetworkControler
{
public:
	NetworkControler(Configuration* configuration,DisplayControler* displayControler);
	void init();
  void loop();
private:
	Configuration* configuration;
  DisplayControler* displayControler;
  String hostname;  
private:
  void reconnect(); 
  static void mqttCallback(char* topic, byte* payload, unsigned int length);  
  void setSwitch(uint8_t item, String value);  
};

#endif /* #ifndef __NetworkControler_H__ */
