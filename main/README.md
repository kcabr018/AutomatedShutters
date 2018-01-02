MESSAGES AND PROTOCOLS

2 types of messages:
	- power: can include power on/off messages or autoTurnOn messages
		* power on/off messages will tell the Arduino module to either start or stop its
		operation
			- format: p0<1 digit int for power state>
			- the 'p' specifies that this is a power message
			- the '0' specifies that this specific power message type is the power on/off 
			message
			- the <1 digit int for power state> will either be '0' or '1'. '0' for turning
			the system OFF or ON respectively
			- ex. 'p00' - this will turn the system OFF
			
		* autoTurnOn messages will tell the Arduino to enable the ability for it to turn itself
		back on in the morning when the currentHour hits the hourOn time. If this option is not
		enabled, then the system will remain off when the system should normally start. 
			- this will be necessary in cases when the owner will be gone for days at a time.
			They would not want to manually turn their system off, and have it automatically
			come back ON the next morning, if they're not home.

			- format: p1<1 digit int for autoTurnOn state>
			- the 'p' specifies that this is a power message
			- the '1' specifies that this specific power message type is the autoTurnOn
			message 
			- the <1 digit int for autoTurnOn state> will either be '0' or '1' for disabling
			or enabling the autoTurnOn feature respectively 
			- ex. 'p10' - this will turn off the autoTurnOn feature which means the system
			will not automatically power back on in the morning

	- time: can include sync or configuration of hourOn and hourOff messages
		* sync messages will be used to configure the internal clock on the Arduino
			- the Time.h library takes in the epoch time (seconds since Jan 1, 1970) and 
			converts this into the current time

			- format: t0<10 digit epoch time>
			- the 't' specifies that this is a time message
			- the '0' specifies that this time message is a sync
			- the <10 digit epoch time> will be the epoch time.
			- ex. 't01514764800' - this sets the epoch time to be 1514764800 which will
			convert to Jan 1, 2018 00:00:00

		* hourOn configuration messages are used to set the time at which the system should turn
		itself on in the morning
			- format: t1<2 digit hourOn>
			- the 't' specifies that this is a time message
			- the '1' specifies that this time message is for configuring hourOn
			- the <2 digit hourOn> should be a value between 00 and 23
			- ex. 't107' - this tells the system to set the hourOn to be 7AM
		
		* hourOff configuration messages are used to set the time at which the system should
		turn itself off in the evening
			- format: t2<2 digit hourOff>
			- the 't' specifies that this is a time message
			- the '2' specifies that this time message is for configuring hourOff
			- the <2 digit hourOff> should be a value between 00 and 23
			- ex. 't220' - this tells the system to set the hourOff to be 8PM

CONFIGURATIONS
# XBEE PANID and rules for communicating between modules will go here
