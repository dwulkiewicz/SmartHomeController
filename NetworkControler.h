#ifndef __NetworkControler_H__
#define __NetworkControler_H__

#include <Arduino.h>

class NetworkControler
{
public:
	NetworkControler();
	void init();
  void loop();
  static String getHostName();
  void onSwitchChanged(uint8_t switchId, uint8_t switchState);
private:
  void initWiFi();
  void initMQTT();
  void initOTA();
  bool reconnect(); 
  static void mqttCallback(char* topic, byte* payload, unsigned int length);  
  static String statusToString(int status);
};
extern NetworkControler networkControler;
#endif /* #ifndef __NetworkControler_H__ */
