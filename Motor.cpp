#include "Motor.h"

void Motor::setup(int pwm, int in1, int in2)
{
	this->pwm = pwm;
	input1 = in1;
	input2 = in2;
	analogWrite(pwm, 255); // configures the speed of the motor
}

void Motor::configureSpeed(int speed)
{
	analogWrite(pwm, speed);
}

// Rotates the motor for a total of 5 seconds
// TODO. 
//	- might need to fix the direction of rotation
//	- might need to fix the duration of rotation
void Motor::closeShutters()
{
	if (shuttersOpen)
	{
		digitalWrite(input1, LOW);
		digitalWrite(input2, HIGH);
		shuttersOpen = false;
		delay(5000);
		brake();
	}
}

// Rotates the motor for a total of 5 seconds
void Motor::openShutters()
{
	if (!shuttersOpen)
	{
		digitalWrite(input1, HIGH);
		digitalWrite(input2, LOW);
		shuttersOpen = true;
		delay(5000);
		brake();
	}
}

void Motor::setShuttersAsOpen(bool state)
{
	shuttersOpen = state;
}

void Motor::brake()
{
	digitalWrite(input1, HIGH);
	digitalWrite(input2, HIGH);
}