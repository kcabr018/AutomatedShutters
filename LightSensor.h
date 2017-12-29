#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H
#include "Arduino.h"
#include "ValidationHelper.h"

class LightSensor
{
public:
	bool setup(int pin); // sets the pin number for the sensor
	bool changePinNumber(int pin);
	double collectSingleReading();
	double collectAvgReading();

private:
	// Attributes
	int lightPin = -1;
	double singleReading;
	double avgReading;
	ValidationHelper validationHelper;
};

#endif 
