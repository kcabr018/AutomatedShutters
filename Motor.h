#ifndef MOTOR_H
#define MOTOR_H
#include "Arduino.h"

class Motor
{
public:
	// Functions
	void setup(int pwm, int in1, int in2);
	void openShutters(); // rotates the motor to open the shutters
	void closeShutters(); // rotates the motor to close the shutters
	void configureSpeed(int speed);

private:
	// Attributes
	bool shuttersOpen = false; // start the shutter's state as closed
	int pwm = -1; // The pulse-width modulation pin that will control the speed
	int input1 = -1; // Connects IN1 of the HBridge IC to digital pin 8 of the arduino
	int input2 = -1; // Connects IN2 of the HBridge IC to digital pin 0 of the arduino

	// Functions
	void setShuttersAsOpen(bool state); // sets the shutter state as open (true) or closed (false)
	void brake(); // stops the motor

protected:


};

#endif // !MOTOR_H
