
from SongsRequest import SongsRequest
from NotesRequest import NotesRequest
from constants import *
from tkinter import *
import bluetooth

state = CommState.NONE;
songsRequest = SongsRequest()
notesRequest = NotesRequest();

def findDevice(name):
	nearby_devices = bluetooth.discover_devices()
	for i in nearby_devices:
		if bluetooth.lookup_name( i ) == name:
			return i

def communication(sock):
	data = sock.recv(1024);
	print(type(data))
	input = [int(i) for i in data] 
	#print(data.length)
	print(input)
	print("--------")
	processInput(input, sock)

def searchForStartingPoint(input):
	global songsRequest
	global notesRequest
	for i, elem in enumerate(input):
		if elem == songsRequest.commStartMark:
			return i, CommState.SONGS_REQUEST;
		if elem == notesRequest.commStartMark:
			return i, CommState.NOTES_REQUEST;
	return -1, CommState.NONE

def processInput(input, sock):
	global state
	global songsRequest
	global notesRequest
	while(input):
		fullInput = input[:]
		if state == CommState.NONE:
			i, state = searchForStartingPoint(input)
			if i != -1:
				input = input[i + 1:]
		if state == CommState.SONGS_REQUEST:
			i, state = songsRequest.processInput(input, sock)
			input = input[i + 1:]
		if state == CommState.NOTES_REQUEST:
			i, state = notesRequest.processInput(input, sock)
			input = input[i + 1:]
		if input == fullInput: #no change
			break