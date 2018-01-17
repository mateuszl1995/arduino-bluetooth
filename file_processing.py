import os
from os.path import isfile, join
import json
def getFilesInDir(path):
	return [f for f in os.listdir(path) if isfile(join(path, f))]

def getJsonFileContent(fileName):
	return json.load(open(fileName))

def prepareDataForSend(name):
	data = getJsonFileContent(name)
	dataToSend = data['name'] + ';' + ','.join(e['note'] + ':' + str(e['duration']) for e in data['notes'])
	print(dataToSend)
	return dataToSend;

def getSongName(name):
	data = getJsonFileContent(name)
	return data['name'];

def getNotes(name):
	data = getJsonFileContent(name)
	return [[e['note'], str(e['duration'])] for e in data['notes']]