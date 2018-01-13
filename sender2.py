from tkinter import *
import bluetooth

port = 1
nearby_devices = bluetooth.discover_devices()
for i in nearby_devices:
	if bluetooth.lookup_name( i ) == 'HC-05':
		bd_addr = i

sock = bluetooth.BluetoothSocket( bluetooth.RFCOMM )
sock.connect((bd_addr, port))

print('Connected');

while 1==1:
	data = sock.recv(1024);
	print(str(data.decode("UTF-8")));
	sock.send(data)

sock.close()