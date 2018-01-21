#!/usr/bin/env python

from tkinter import *
import bluetooth

from communication import *

port = 1



def main(port):
	sock = bluetooth.BluetoothSocket( bluetooth.RFCOMM )
	sock.connect((findDevice('HC-05'), port))
	print('Connected')
	while 1==1:
		communication(sock)
	sock.close()

main(port)
		
#processInput([123, 23, 21, 80, 0, 0,0,1,3,2,12,78, 0] , 'sd')
#processInput([0,0,0,0,2], 'sd')
#processInput([78], 'sd')
#processInput([0,20,0,30,0,40], 'sd')