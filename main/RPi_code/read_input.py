import serial

ser = serial.Serial('/dev/ttyUSB0', 9600)

while True:
	ser.flushInput()
	inputRead = ser.read(7)

	# Parse the data
	moduleID = inputRead[0]
	temp = inputRead[1:6]

	# Print out the input
	print "Temp: %s" % temp
