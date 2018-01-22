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
		songIndex = self.params[0] * 256 + self.params[1]
		songContentToSend = 'Index out of range'
		if songIndex < len(songs):
			song = songs[songIndex]
			songContent = getNotes(musicDir + '/' + song)
			startIndex = self.params[2] * 256 + self.params[3]
			endIndex = self.params[4] * 256 + self.params[5]

			songContentToSend = songContent[startIndex:endIndex]
		print('Received notes request:', self.params, songContentToSend)
		if songContentToSend != 'Index out of range':
			sock.send('N')
			l = len(songContentToSend)*2+3
			print(l)
			x = l.to_bytes(2, byteorder='big')
			sock.send(bytes([x[0]]))
			sock.send(bytes([x[1]]))
			
			for i in songContentToSend:
				note = i[0]
				byte = (ord(note[0]) - ord('A')) * 2
				print(note);
				if note[1] == 'S' :
					print("byteA: ", byte, "type: ", type(byte))
					byte = byte + 1
					byte *= 16;
					byte += (ord(note[2]) - ord('0'));
				else :
					print("byteB: ", byte, "type: ", type(byte))
					byte *= 16;
					byte += (ord(note[1]) - ord('0'));
				print("byteC: ", byte, "type: ", type(byte))
				x = byte.to_bytes(1, byteorder='big')
				print(type(x[0]))
				#print(chr(byte), ':', chr(int(i[1])))
				sock.send(bytes([x[0]]))
				sock.send(chr(ord(i[1]) - ord('0')))
  			
