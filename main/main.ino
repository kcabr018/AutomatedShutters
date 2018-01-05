#include <Time.h>
#include <TimeLib.h>
#include "Motor.h"
#include "LightSensor.h"
#include "TemperatureSensor.h"

// Single character message tags
#define TIME_HEADER	't' // Header tag for time-related messages (sync, hourOn/hourOff changing)
#define POWER_HEADER	'p' // Header tag for system power actions
#define FORMAT_HEADER	'f' // Header tag for setting the date format message (long/short) 
#define MAX_MSG_LENGTH	12  // The maximum allowable chars in a single transmission
#define TIME_REQUEST	65

const int deviceId = 1; // This device's ID
bool isDaytime;
bool poweredOn;		// the power state of the system
bool autoTurnOn;	// whether the system should resume operation in the morning time after having 
			// been manually shut off
double tempReading; 	// a variable to store the temp readings obtained from the TemperatureSensor
			// class
double lightReading; 	// a variable to store the light readings obtained from the LightSensor class
double mediumTemp = 87; // the average temperature to compare input temp readings against
int hourOff, hourOn; 	// the hours in which the system should be turned on and off. Use 24hr format
int currentHour, currentMinute; // The hour and minutes of the current time. Used to compare against
				// the off and on times. 
char serialInput[MAX_MSG_LENGTH];
TemperatureSensor temperature;
LightSensor light;
Motor motor;

void setup()
{
	Serial.begin(9600);
	temperature.setup(0);
	/*light.setup(1);*/
	isDaytime = true;
	poweredOn = true;
	autoTurnOn = true;
	hourOff = 20;
	hourOn = 7;
}

void loop()
{
	resetValues();
	while (Serial.available())
	{
		Serial.print("Current hour: "); //TESTER
		Serial.println(currentHour); //TESTER
		Serial.print("Current minute: "); //TESTER
		Serial.println(currentMinute); //TESTER

		char input = Serial.read();
		serialInput[0] = input;
		processMessage(); // call the function to process the serial input
	}
	if (timeStatus() != timeNotSet)
	{
		//digitalClockDisplay();
	}

	//	// Set the power of the system based on whether it's day or night
	//	if (currentHour == hourOn && currentMinute == 0 && !poweredOn && autoTurnOn)
	//	{
	//		poweredOn = true;
	//		isDaytime = true;
	//	}
	//
	//	/**********************************************
	//	* Check input queue for messages from the RPi * 
	//	***********************************************/
	//	// messages can contain:
	//	//	- commands to power on/off the  system
	//	//	- the current location and time
	//	//	- any updates to the hourOff or hourOn settings
	// 
	//	if (poweredOn) 
	//	{
	//		/***********************
	//		* Collect the readings *
	//		************************/
	//		tempReading = temperature.collectAvgReading();
	//		/*lightReading = light.collectAvgReading();*/	// To be incorporated later
	//
	//		/*********************************************
	//		* Change the state of the shutters if needed *
	//		**********************************************/
	//		if (tempReading < mediumTemp && isDaytime) // if it's cold and daytime, open the shutters
	//		{
	//			motor.openShutters();
	//		}
	//		else // every other instance should try to close the shutters
	//		{
	//			motor.closeShutters();
	//		}
	//
	//		/****************************************************************
	//		* Send data to centralized database, to be stored and displayed *
	//		*****************************************************************/
	//		// Can send to a file on a RPi or other system and then periodically move all this data to a sql db
	//		// Can create a website to display this information
	//
	//
	//		/*******************************************************
	//		* Turn off the system when currentHour reaches hourOff * 
	//		********************************************************/
	//		if (currentHour == hourOff && currentMinute == 0)
	//		{
	//			poweredOn = false;
	//			isDaytime = false;
	//		}
	//	}

	delay(10000); // wait 1 minute before repeating
}

void digitalClockDisplay()
{
	// digital clock display of the time
	Serial.print(hour());
	printDigits(minute());
	printDigits(second());
	Serial.print(" ");
	Serial.print(day());
	Serial.print(" ");
	Serial.print(month());
	Serial.print(" ");
	Serial.print(year());
	Serial.println();
}

// Process the serial input and perform the correct action
void processMessage()
{
	switch (serialInput[0])
	{
	case 'p':
		processPowerMessage();
		break;
	case 't':
		processTimeMessage();
		break;
	default:
		Serial.println("Unable to interpret message type");
		break;
	}
	Serial.print("serialInput: ");
	Serial.println(serialInput);
}

void processPowerMessage()
{
	readInput(2, 1); // Read the remaining bytes of the message

	if (serialInput[1] == '0') // '0' is for turning the power on/off
	{
		Serial.println("Running the poweredOn case"); //TESTER
		int newPowerState = convertCharToInt('0', serialInput[2]);

		if (newPowerState == 1)
			poweredOn = true;
		else if (newPowerState == 0)
			poweredOn = false;

		Serial.print("Changing the power state to be: ");
		Serial.println(newPowerState);
	}
	else if (serialInput[1] == '1') // '1' is for enabling/disabling autoTurnOn
	{
		int newAutoTurnOn = convertCharToInt('0', serialInput[2]);
		autoTurnOn = newAutoTurnOn;

		Serial.print("autoTurnOn changed to: "); //TESTER
		Serial.println(autoTurnOn); //TESTER
	}
}

/***** 
* Processes the time message to distinguish between a sync message or hourOff/hourOn update
* - 't0<10 digit int> for sync messages'
* - 't1<2 digit int> for setting hourOn
*		- ex. t108 to set hourOn to be 8AM
* - 't2<2 digit int> for setting hourOff
*		- ex. t219 for setting hourOff to be 7PM
*****/
void processTimeMessage()
{
	char messageType = (char)Serial.read();
	serialInput[1] = messageType;

	switch (messageType)
	{
	case '0':
		Serial.println("Found a 0 in the time message"); //TESTER
		processSyncMessage();
		break;
	case '1':
		processHourChange('1');
		break;
	case '2':
		processHourChange('2');
		break;
	default:
		Serial.println("Invalid time message type");
		break;
	}
}

// A time message with option 1 or 2 will bring us to this function.
// These options are for changing the hourOn or hourOff times
void processHourChange(char type)
{
	if (type == '1')
	{
		readInput(2, 2);
		hourOn = convertCharToInt(serialInput[2], serialInput[3]);
		Serial.print("New hourOn: "); //TESTER
		Serial.println(hourOn); //TESTER
	}
	else if (type == '2')
	{
		readInput(2, 2);
		hourOff = convertCharToInt(serialInput[2], serialInput[3]);
		Serial.print("New hourOff: "); //TESTER
		Serial.println(hourOff); //TESTER
	}
}

// Reads the next 'size' bytes of serial input and stores it in the global variable 'serailInput'
String readInput(int size, int startingIndex)
{
	for (int i = startingIndex; i < startingIndex + size; i++)
	{
		serialInput[i] = (char)Serial.read();
	}
}

void printDigits(int digits)
{
	// utility function for digital clock display: prints preceding colon and leading 0
	Serial.print(":");
	if (digits < 10)
		Serial.print('0');
	Serial.print(digits);
}

/*****
* A sync message is one defines the real time clock for the Arduino. It uses the epoch
* time to get the actual time. Epoch time is the number of seconds that have elapsed since
* Jan 1, 1970. This message type is composed of 12 bytes:
* - <byte 0: message type><byte 1: message action><bytes 2-11: the number of seconds>
* - ex. t01483254000
*	- will set the clock to be Jan 1, 2017 00:00:000
*****/
void processSyncMessage()
{
	Serial.print("Read sync-time message: "); //TESTER
	Serial.println(serialInput); //TESTER

	unsigned long pctime;
	const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013

	pctime = Serial.parseInt();
	if (pctime >= DEFAULT_TIME) // check the integer is a valid time (greater than Jan 1 2013)
	{
		Serial.print("The PC time is: "); //TESTER
		Serial.println(pctime);
		setTime(pctime); // Sync Arduino clock to the time received on the serial port
	}

	currentHour = hour();
	currentMinute = minute();
}

time_t requestSync()
{
	Serial.write(TIME_REQUEST);
	return 0; // the time will be sent later in response to serial mesg
}

// Clears the values of certain variables for each loop instance
void resetValues()
{
	for (int i = 0; i < MAX_MSG_LENGTH; i++)
		serialInput[i] = '\0';
}

// Converts from char to int by using the decimal value of the char
int convertCharToInt(char tens, char ones)
{
	int ten = (int)tens - (48 - 9*((int)tens - 48));
	int one = (int)ones - 48;
	return ten + one;
}
