#pragma once

#include <Arduino.h>

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
