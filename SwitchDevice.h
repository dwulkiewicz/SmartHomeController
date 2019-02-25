#pragma once

#include "Types.h"

class SwitchDevice
{
public:
	SwitchDevice();
	~SwitchDevice();
	bool setState(bool state);
  void invertState();
	bool switchOn();
	bool switchOff();
	bool getState();
private:
	bool state;
};
