#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H
#include "Arduino.h"
#include "ValidationHelper.h"

class TemperatureSensor
{
public:
	// Functions
	bool setup(int pin); // sets the pin number for the sensor
	bool changePinNumber(int pin);
	double collectSingleReading(); // gets a single reading
	double collectAvgReading(); // gets an average of 20 readings spaced 1ms apart

private:
	// Attributes
	int tempPin = -1; // the pin to be used for the temperature sensor
	double singleReading;
	double avgReading;
	ValidationHelper validationHelper;
	// Functions
	double convertReadingToUsable(double reading);
};

#endif