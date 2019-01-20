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
  bool reconnect(); 
  static void mqttCallback(char* topic, byte* payload, unsigned int length);  
  static String statusToString(int status,String& statusStr);
};
extern NetworkControler networkControler;
#endif /* #ifndef __NetworkControler_H__ */
