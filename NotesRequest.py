from constants import *
from file_processing import *

class NotesRequest:

	commStartMark = 78 # ASCII N
	paramsCount = 6

	def __init__(self):
		self.params = []

	def processInput(self, input, sock):
		for i, elem in enumerate(input):
			if self.assignValue(elem):
				self.sendData(sock)
				self.clear()
				return i, CommState.NONE
		return len(input), CommState.NOTES_REQUEST

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
		songIndex = self.params[0] * 255 + self.params[1]
		songContentToSend = 'Index out of range'
		if songIndex < len(songs):
			song = songs[songIndex]
			songContent = getNotes(musicDir + '/' + song)
			startIndex = self.params[2] * 255 + self.params[3]
			endIndex = self.params[4] * 255 + self.params[5]

			songContentToSend = songContent[startIndex:endIndex]
		print('Received notes request:', self.params, songContentToSend)