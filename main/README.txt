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
			back on in the morning when the currentHour hits the hourOn time. If this option
			is not enabled, then the system will remain off when the system should normally 
			start. 
			- this will be necessary in cases when the owner will be gone for days at a time
				They would not want to manually turn their system off, and have it 
				automatically come back ON the next morning, if they're not home.

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

Xbee Config and Info
	- Communication using Xbee requires that all devices attempting to communicate be on the same
		Personal Area Network (PAN)
 	- Each device will have its own 16 bit address
	- Xbee allows for broadcasting data to all hosts in the PAN or sending it to a single host
	- In this system, there will be a default PAN for all hosts
	- A starting address will begin with the RPi Xbee and increment for each of the remaining modules
	- Data transmission:
		* The RPi is the only device which will be allowed to broadcast data in the form of time
			and power messages
		* Arduino modules will not broadcast data on the PAN
		* Arduino modules will specify the RPi address as the destination of their transmission 
	- XBee supply voltage can only be between 2.1-3.6V
		* need a logic level converter to convert the voltage supply and serial input from the 
			Arduino
		* The XBee explorer serves as a llc
	- The XBee explorer is like a post office. If a person (Arduino) has a letter to send out
		or transmit, then it must send that letter to the post office. The post office will 
		receive that letter as input and then get ready to send it out to its destination. In
		this example, the DIN pin of the XBee explorer must be for receiving data from the
		Arduino. The DOUT pin of the XBee explorer will not be for sending out data wirelessly 
		since if this was the case, then there would be no need for a cable to connect to this
		pin. The DOUT pin is for sending data that has wirelessly arrived to the XBee and
		sending it to the Arduino destination. 
	- Connect the 5V pin on the XBee explorer to the 5V pin on the Arduino
	- Connect the DOUT pin on the XBee explorer to the Rx pin on the Arduino
		* Data transmitted out of the wired DOUT of the XBee explorer will be for the Arduino to
			receive 
	- Connect the DIN pin on the XBee explorer to the Tx pin on the Arduino
		* Data transmitted out of the Arduino will be going into the XBee explorer so that it may
		be sent out wirelessly

RPi data transmission
	- The RPi will need to change the destination address setting dynamically using command mode (AT
		mode)
		1. Enter AT mode by transmitting the character sequence '+++' across the serial
			connection. After 3 seconds, the module will be in command mode. 
		2. Enter 'ATDL xxxx' to change to a new destination address xxxx
		3. Enter 'ATWR' to save these changes
		4. Enter 'ATCN' to exit command mode
		* the system should now be able to transmit to the new destination address
