#include "TemperatureSensor.h"

bool TemperatureSensor::setup(int pin)
{
	changePinNumber(pin);
}

bool TemperatureSensor::changePinNumber(int pin)
{
	if (!ValidationHelper::validateSetup(pin))
	{
		ValidationHelper::printMessage("INVALID TEMPERATURE PIN NUMBER!");
		return false;
	}

	tempPin = pin;
	//ValidationHelper::printMessage("Pin " + String(pin) + " configured as temp sensor pin"); // Tester
	return true;
}

double TemperatureSensor::collectSingleReading()
{
	singleReading = analogRead(tempPin);
	singleReading = convertReadingToUsable(singleReading);
	ValidationHelper::printMessage("Got a single reading of: " + String(singleReading, 2)); // Tester
	return singleReading;
}

double TemperatureSensor::collectAvgReading()
{
	avgReading = 0;

	for (int i = 0; i < 20; i++)
	{
		avgReading += collectSingleReading();
	}
	ValidationHelper::printMessage("Got an average reading of: " + String(avgReading/20, 2)); // Tester
	return avgReading / 20;
}

// Converts the analog reading from the temp sensor into degrees Fahrenheit
double TemperatureSensor::convertReadingToUsable(double reading)
{
	double voltage = reading * 5;
	voltage /= 1024.0;

	double tempC = (voltage - 0.5) * 100;
	double tempF = (tempC * 9.0 / 5.0) + 32;

	return tempF;
}

