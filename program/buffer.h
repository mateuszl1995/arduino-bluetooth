class Buffer {
public:
  char buffer[BUFFER_SIZE];
  short received_bytes;
  short frame_size;

  void process_buffer() {
    Serial.println("process buffer");
    Serial.print("type = "); Serial.println(buffer[0]);
    if (buffer[0] == 'P') {
      song_count = ((short) buffer[3]) * 256 + (short) buffer[4];
      for (int i = 5; i < frame_size; ) {
        char * song_info = new char[26];
        strncpy(song_info, &buffer[i], 25);
        songs.my_push(song_info);
        Serial.println(song_info);
        i += strlen(song_info) + 1;
        Serial.print("i = "); Serial.println(i);
      }
      screen.refresh(state);
    }
    if (buffer[0] == 'N') {
      note_count = ((short) buffer[3]) * 256 + (short) buffer[4];
      for (int i = 5; i < frame_size; i+=2) {
        Note n(buffer[i], buffer[i+1]);
        notes.my_push(n);
      }
    }
    init();
  }

  void init() {
    received_bytes = 0;
    frame_size = 3;
  }

  void push(char x) {
    if (received_bytes == 0 && x != 'P' && x != 'N')
      return;    
    buffer[received_bytes++] = x;
    if (received_bytes == 3)
      frame_size = ((short) buffer[1]) * 256 + (short) buffer[2];
    if (received_bytes == frame_size)
      process_buffer();
  }
  short count() {
    return received_bytes;
  }
};
