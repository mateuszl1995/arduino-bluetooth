from enum import Enum

musicDir = 'melodies'

class CommState(Enum):
	NONE = 1
	SONGS_REQUEST = 2
	NOTES_REQUEST = 3