#define BTN_RED 3
#define BTN_BLUE 2
#define RXPIN 10
#define TXPIN 11
#define AMPLIFIER_IN 5  // wzmacniacz - wejście IN
#define SCREEN_LIGHT 4
#define MELODY_SIZE 250
#define BUFFER_SIZE 510
#define MAX_TIMEOUT 1000

#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <QueueArray.h>


LiquidCrystal lcd(8, 9, 6, 7, 12, 13); // wyświetlacz
SoftwareSerial bluetooth(RXPIN, TXPIN);

enum State {OFF, MENU, LIST, SONG};
State state, old_state;
short option, options;

class note {
public:
  note(char _freq, byte _timing) {freq = _freq; timing = _timing; }
  char freq;  // example: E6 (E na 4 bitach i 6 na 4 bitach)
  byte timing;   // example: 8 (ósemka), 4 (ćwierćnuta)
};
note to_note(const char * str, int timing); // we should move this function to server to increase free memory
void require_songs(short first, short last);
void require_notes(short song_index, short first_note, short last_note);
void play(note n);
void process_buffer();

short song_id, note_id, song_count, note_count;

QueueArray <note> notes;
QueueArray <char*> songs;

char buffer[BUFFER_SIZE];
char request_type;
short received_bytes;
short frame_size;

unsigned long last_time_note_played, now;

void setup()
{
  Serial.begin(9600);
  bluetooth.begin(9600);
  lcd.begin(16, 2);
  pinMode(AMPLIFIER_IN, OUTPUT);
  pinMode(BTN_RED, INPUT_PULLUP);
  pinMode(BTN_BLUE, INPUT_PULLUP);
  pinMode(SCREEN_LIGHT, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(BTN_RED), interrupt_red, FALLING);
  attachInterrupt(digitalPinToInterrupt(BTN_BLUE), interrupt_blue, FALLING);

  state = MENU;
  option = 0, options = 2;
  song_id = play_index = song_count = note_id = note_count = 0;
  refresh_display();
  require_songs(song_id, song_id + 10);
}

void loop()
{
  int n = bluetooth.available();
  while (n--) {
    char x = bluetooth.read();
    if (received_bytes > 0 || x == 'P' || x == 'N')
      buffer[received_bytes++] = x;
  }
  if (received_bytes >= 3)
    frame_size = ((short) buffer[1]) << 8 + (short) buffer[2];
  if (received_bytes > 3 && received_bytes == frame_size)  {// before receive 3 bytes, we don't know what is frame_size and we shouldn't process buffer
    process_buffer();
    received_bytes = 0;
  }

  if (notes.count() > 0) {
      now = millis();
      if (now > last_time_note_played + (1300 / notes.front().timing) ) {// 1300, not 1000, +30 % delay to distinguish two notes
        play(notes.pop());
        note_id++;
        last_time_note_played = now;
      }
      
  }
}

void interrupt_red()
{
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 250) {
    if (state == OFF) {
      state = MENU;
    }
    else if (state == MENU) {
      if (option == 0)
        state = LIST;
        if (song_id + 5 > last_song)
          require_songs(last_song + 1, song_id + 9);
      else
        state = OFF;
    }
    else if (state == LIST) {
      state = SONG;
    }
    else if (state == SONG) {
      state = MENU;
    }
    option = 0;
    refresh_display();
  }
  last_interrupt_time = interrupt_time;
  
}

void interrupt_blue()
{
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 250) {
    //Serial.println("blue interrupt 2");
    song_id = (song_id+1) % song_count;
    if (song_id + 5 > last_song)
          require_songs(last_song + 1, song_id + 9);
    refresh_display();
  }
  last_interrupt_time = interrupt_time;
}

void disp(const char * first_line, const char * second_line, bool arrow = true) {
  lcd.clear();
  lcd.setCursor(0, 0);
  if (arrow) {
    lcd.print("-> ");
    lcd.setCursor(3, 0);
  }
  lcd.print(first_line);
  lcd.setCursor(0, 1);
  lcd.print(second_line);
}


void display_menu() {
  static const char * play = "Play";
  static const char * turn_off = "Turn off";
  if (option == 0)
    disp(play, turn_off);
   else
    disp(turn_off, play);
    
  require_songs(song_id, song_id + 10);
}

void display_list() {
  if (option == 0)
    disp("-> Song 1", "Song 2");
   else
    disp("Song 1", "-> Song 2");
}

void display_song() {
  disp("Title", "Timer", false);
}

void refresh_display() {
  if (state != OFF)
    lcd.clear();
  
  if (state == MENU)
    display_menu();
  else if (state == LIST)
    display_list();
  else if (state == SONG)
    display_song();
    
  if (state == OFF) {
    digitalWrite(SCREEN_LIGHT, LOW);
    lcd.clear();
  }
  else
    digitalWrite(SCREEN_LIGHT, HIGH);
}


note to_note(const char * str, byte timing) {
  note n;
  n.freq = (str[0] - 'A') * 2;
  if (str[1] == 'S') {
    n.freq++;
    n.freq <<= 4;
    n.freq += (str[2] - '0');
  } else {
    n.freq <<= 4;
    n.freq += (str[1] - '0');
  }
  n.timing = timing;
}

short freq_of_8[] = {7040, 7458, 7902, -1, 4186, 4435, 4699, 4978, 5274, -1, 5588, 5920, 6272, 6644}; // freqs of: A8, AS8, B8, BS8, ..., G8, GS8 (value -1 if not exists) 
void play(note n) {
  short freq = freq_of_8[n.freq >> 4] >> (8 - (n.freq && 0x0F));  // get frequency for octave 8 and fit frequency to octave = (n.freq & 0x0F)
  tone(AMPLIFIER_IN, freq, 1000 / n.timing); // 1000 ms
  // delay(1300 / n.timing); // delay + 30% to distinguish two notes
}

void bluetooth_write(short n) {
  char x1 = n >> 8;
  char x2 = n & 0xff;
  bluetooth.write(x1);
  bluetooth.write(x2);
  Serial.write(x1);
  Serial.write(x2);
}

void require_songs(short first, short last) {
  bluetooth.write('P');
  bluetooth_write(first);
  bluetooth_write(last);
  request_type = 'P';
  received_bytes = 0;
  frame_size = 3;
}

void require_notes(short song_index, short first_note, short last_note) {
  bluetooth.write('N');
  bluetooth_write(song_index);
  bluetooth_write(first_note);
  bluetooth_write(last_note);
  request_type = 'N';
  received_bytes = 0;
  frame_size = 3;
}

void process_buffer() {
  if (buffer[0] == 'P') {
    short song_count = ((short) buffer[3]) << 8 + (short) buffer[4];
    for (int i = 5; i < frame_size; ) {
      char * song_info = new char[21];
      strncpy(song_info, &buffer[i], 20);
      songs.my_push(song_info);
      i += strlen(song_info);
    }
  }
  if (request_type == 'N') {
    for (int i = 3; i < frame_size; i+=2) {
      note n(buffer[i], buffer[i+1]);
      notes.my_push(n);
    }
  }
}

/*
 * if(bluetooth.available()){
    k = bluetooth.read();
    Serial.println(k);
    //k = bluetooth.read();
  } else {
    //bluetooth.write("he2llo");
  }
 */




