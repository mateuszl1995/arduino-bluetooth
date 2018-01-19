from enum import Enum

musicDir = 'melodies'

class CommState(Enum):
	NONE = 1
	SONGS_REQUEST = 2
	NOTES_REQUEST = 3

notesMapping = {
	'A' : 0,
	'AS' : 1,
	'B' : 2,
	'C' : 3,
	'CS' : 4,
	'D' : 5,
	'DS' : 6,
	'E' : 7,
	'F' : 8,
	'FS' : 9,
	'G' : 10,
	'GS' : 11,

}