from tkinter import *
import bluetooth
from os import walk
from os.path import isfile, join

port = 1

def findDevice(name):
	nearby_devices = bluetooth.discover_devices()
	for i in nearby_devices:
		if bluetooth.lookup_name( i ) == name:
			return i;

def main(port):
	port = 1
	sock = bluetooth.BluetoothSocket( bluetooth.RFCOMM )
	sock.connect((findDevice('HC-05'), port))
	print('Connected')
	communication(sock)
	sock.close()


def communication(sock):
	while 1==1:
		data = sock.recv(1024);
		print(str(data.decode("UTF-8")));
		sock.send(data)
def getFilesInDir(path):
	return [f for f in listdir(path) if isfile(join(path, f))]

print(getFilesInDir('melodies'))
