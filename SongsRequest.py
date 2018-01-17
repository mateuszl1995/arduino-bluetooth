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
		startIndex = self.params[0] * 255 + self.params[1]
		endIndex = self.params[2] * 255 + self.params[3]
		songsToSend = [getSongName(musicDir + '/' + i) for i in songs[startIndex:endIndex]]
		print('Received songs request:', self.params, songsToSend)
