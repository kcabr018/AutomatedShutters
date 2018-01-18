#include "LightSensor.h"

bool LightSensor::setup(int pin)
{
	changePinNumber(pin);
}

bool LightSensor::changePinNumber(int pin)
{
	if (!ValidationHelper::validateSetup(pin))
	{
		ValidationHelper::printMessage("Invalid light sensor pin number!");
		return false;
	}

	// If the pin is valid, then set the variable and return true
	lightPin = pin;
	return true;
}

double LightSensor::collectSingleReading()
{
	
}

double LightSensor::collectAvgReading()
{
	avgReading = 0;
	for (int i = 0; i < 20; i++)
	{
		avgReading += collectSingleReading();
	}
	return avgReading / 20;
}