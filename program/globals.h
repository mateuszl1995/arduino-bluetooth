enum State {OFF, MENU, LIST, SONG};
State state, old_state;

short song_id, note_id, song_count, note_count;

QueueArray <Note> notes;
QueueArray <char*> songs;

short option;

unsigned long next_note_time, now, start_song;

