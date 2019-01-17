
#include <Arduino.h>
#include "Configuration.h"
#include "DisplayControler.h"

#ifndef __NetworkControler_H__
#define __NetworkControler_H__


class NetworkControler
{
public:
	NetworkControler(Configuration* configuration);
	void init();
  void loop();
  static String getHostName();
private:
	Configuration* configuration;
private:
  void reconnect(); 
  static void mqttCallback(char* topic, byte* payload, unsigned int length);  
};

#endif /* #ifndef __NetworkControler_H__ */
