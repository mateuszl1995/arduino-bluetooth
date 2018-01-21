from constants import *
from file_processing import *

class SongsRequest:

	commStartMark = 80 # ASCII P
	paramsCount = 4

	def __init__(self):
		self.params = []

	def processInput(self, input, sock):
		for i, elem in enumerate(input):
			if self.assignValue(elem):
				self.sendData(sock)
				self.clear()
				return i, CommState.NONE
		return len(input), CommState.SONGS_REQUEST

	def assignValue(self, i):
		self.params.append(i);
		if len(self.params) >= self.paramsCount:
			return True
		else:
			return False

	def clear(self):
		self.params = []

	def sendData(self, sock):
		songs = getFilesInDir(musicDir)
		startIndex = self.params[0] * 256 + self.params[1]
		endIndex = self.params[2] * 256 + self.params[3]
		songsToSend = [getSongName(musicDir + '/' + i) for i in songs[startIndex:endIndex]]
		print('Received songs request:', self.params, songsToSend)
		
		sock.send('P')
		size = 5
		for i in songsToSend:
			size += (len(i) + 1)
		print(size)
		x = size.to_bytes(2, byteorder='big')
		sock.send(bytes([x[0]]))
		sock.send(bytes([x[1]]))
		
		l = len(songs)
		print(l)
		x = l.to_bytes(2, byteorder='big')
		sock.send(bytes([x[0]]))
		sock.send(bytes([x[1]]))
		
		for i in songsToSend:
			sock.send(bytes(i, "UTF-8"))
			sock.send(chr(0))
			#for c in songsToSend[i]:
				
		
		
		