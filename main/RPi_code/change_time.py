import serial
import sys

ser = serial.Serial('/dev/ttyUSB0', 9600)
time = int(sys.argv[1]);

if time < 10:
	ser.write('t10%d' % time);
else:
	ser.write('t1%d' % time);
ser.close()
