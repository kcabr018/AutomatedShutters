#ifndef VALIDATION_HELPER_H
#define VALIDATION_HELPER_H
#include "Arduino.h"

class ValidationHelper
{
public:
	// Functions
	static void printMessage(String message);
	static bool validateSetup(int pin);

};

#endif
