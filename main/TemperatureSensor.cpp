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
	// singleReading = analogRead(tempPin);
	// singleReading = convertReadingToUsable(singleReading);
	double tempF;

	int reading = analogRead(4);
	Serial.println(reading);

	double voltage = reading * 4.53; // 4.53 is the voltage being supplied to the sensor
	voltage /= 1024; // 1024 is the 2^10 which is the precision of the ADC using 10 bits
	Serial.print(voltage);
	Serial.println(" V");

	double tempC = (voltage - 0.5) * 100; // 25C is 0.75 V with an offset of 0.5 V
	tempF = (tempC * 9.0/5.0) + 32;

	return tempF;
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
double TemperatureSensor::convertReadingToUsable(double reading)
{
	// double voltage = reading * 5;
	// voltage /= 1024.0;

	// double tempC = (voltage - 0.5) * 100;
	// double tempF = (tempC * 9.0 / 5.0) + 32;

	double tempC = reading * 3.3;
	tempC /= 1024;
	Serial.print(" - ");
	Serial.print(tempC); Serial.println(" volts");
	tempC = (tempC - 0.5) * 100;

	double tempF = (tempC * 9.0 / 5.0) + 32;

	return tempC;
}

