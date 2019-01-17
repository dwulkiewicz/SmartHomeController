#pragma once

#include <Arduino.h>

#define SW_OFF 0
#define SW_ON 1

class OnOffDevice
{
public:
	OnOffDevice();
	~OnOffDevice();
	bool setState(bool state);
  void invertState();
	bool switchOn();
	bool switchOff();
	bool getState();
private:
	bool state;
};
