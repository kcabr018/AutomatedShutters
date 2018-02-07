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
	return convertReadingToUsable(singleReading);
}

double TemperatureSensor::collectAvgReading()
{
	avgReading = 0;

	for (int i = 0; i < 20; i++)
	{
		avgReading += collectSingleReading();
	}
	return avgReading / 20;
}

// Converts the analog reading from the temp sensor into degrees Fahrenheit
double TemperatureSensor::convertReadingToUsable(int reading)
{
	double voltage = reading * 4.53; // 4.53 is the voltage being supplied to the sensor
	voltage /= 1024; // 1024 is the 2^10 which is the precision of the ADC using 10 bits

	double tempC = (voltage - 0.5) * 100; // 25C is 0.75 V with an offset of 0.5 V
	double tempF = (tempC * 9.0/5.0) + 32;

	return tempF;
}

