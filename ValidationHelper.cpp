#include "ValidationHelper.h"

void ValidationHelper::printMessage(String message)
{
	Serial.println(message);
}

bool ValidationHelper::validateSetup(int pin)
{
	if (pin >= 0)
		return true;
	else return false;
}