
#include "RGBdriver.h"
#include "Constants.h"
#include "Configuration.h"
#include "LightsControler.h"


RGBdriver Driver(GPIO_CLK, GPIO_DIO);

LightsControler::LightsControler(){
}

LightsControler::~LightsControler(){
}

void LightsControler::init() {
	pinMode(GPIO_SW_1, INPUT);
	pinMode(GPIO_SW_2, INPUT);
	pinMode(GPIO_SW_3, INPUT);
	pinMode(GPIO_SW_4, INPUT);

	Driver.begin(); // begin
	Driver.SetColor(0, 0, 0); //Blue. first node data
	Driver.SetColor(0, 0, 0); //Blue. second node data      
	Driver.end();

	//TODO:
	//wczyta� konfiguracj�

}

void LightsControler::loop() {
	//TODO:
	//doda� zdarzenie wywo�ywane po zmianie stanu


	if (digitalRead(GPIO_SW_1) || digitalRead(GPIO_SW_2) || digitalRead(GPIO_SW_3) || digitalRead(GPIO_SW_4)) {
		Driver.begin(); // begin
		Driver.SetColor(255, 255, 255); //Blue. first node data
		Driver.SetColor(255, 255, 255); //Blue. second node data      
		Driver.end();
	}
	else
	{
		Driver.begin(); // begin
		Driver.SetColor(0, 0, 0); //Blue. first node data
		Driver.SetColor(0, 0, 0); //Blue. second node data      
		Driver.end();
	}
}

LightsControler lightsControler;
