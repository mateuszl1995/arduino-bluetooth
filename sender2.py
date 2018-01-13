#!/usr/bin/env python

from tkinter import *
import bluetooth
import os
from os.path import isfile, join
import json


port = 1
musicDir = 'melodies'

def findDevice(name):
	nearby_devices = bluetooth.discover_devices()
	for i in nearby_devices:
		if bluetooth.lookup_name( i ) == name:
			return i;

def main(port):
	sock = bluetooth.BluetoothSocket( bluetooth.RFCOMM )
	sock.connect((findDevice('HC-05'), port))
	print('Connected')
	communication(sock)
	sock.close()


def communication(sock):

	data = sock.recv(1024);
	print(data)
	if data == 'S':
		dataToSend = prepareDataForSend(musicDir+'/kotek_na_plotek.json')
		sock.send(dataToSend)

def getFilesInDir(path):
	return [f for f in os.listdir(path) if isfile(join(path, f))]

def getJsonFileContent(fileName):
	return json.load(open(fileName))

def prepareDataForSend(name):
	data = getJsonFileContent(name)
	dataToSend = data['name'] + ';' + ','.join(str(int(e['note'][1])* 7 + ord(e['note'][0]) - ord('A')) + ':'+ str(e['duration']) for e in data['notes'])
	print(dataToSend)
	return dataToSend;

main(port)