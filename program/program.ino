#define BTN_RED 3
#define BTN_BLUE 2
#define AMPLIFIER_IN 5  // wzmacniacz - wejście IN
#define SCREEN_LIGHT 4
#define RXPIN 10
#define TXPIN 11
#define MELODY_SIZE 250
#define BUFFER_SIZE 510
#define MAX_TIMEOUT 1000
#define SONGS_SIZE 2
#define NOTES_SIZE 200

#include <SimpleTimer.h>
#include <QueueArray.h>
#include "note.h"
#include "globals.h"
#include "screen.h"
Screen screen;
#include "buffer.h"
#include "bluetooth.h"


Buffer buffer;
Bluetooth bluetooth(buffer);
SimpleTimer timer;


void setup()
{
  Serial.begin(9600);
  hc05.begin(9600);
  lcd.begin(16, 2);
  pinMode(AMPLIFIER_IN, OUTPUT);
  pinMode(BTN_RED, INPUT_PULLUP);
  pinMode(BTN_BLUE, INPUT_PULLUP);
  pinMode(SCREEN_LIGHT, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(BTN_RED), interrupt_red, FALLING);
  attachInterrupt(digitalPinToInterrupt(BTN_BLUE), interrupt_blue, FALLING);

  state = MENU;
  song_id = song_count = note_id = note_count = 0;
  song_count = 1;
  option = 0;
  screen.refresh(state);

  Serial.println("start");
  
  timer.setInterval(1000, updateTimer);
}

void loop()
{
  bluetooth.read();
  
  if (notes.count() > 0) {
      now = millis();
      if (now > next_note_time ) {// 1300, not 1000, +30 % delay to distinguish two notes
        Note n = notes.pop();
        n.play();
        next_note_time = now + 1300 / n.timing;
        note_id++;
        Serial.print("Note: "); Serial.print(note_id); Serial.print("/"); Serial.println(note_count);
        if (note_id + notes.count() < note_count && notes.count() < NOTES_SIZE / 2) {
          bluetooth.require_notes(song_id, note_id + notes.count(), note_id + notes.count() + NOTES_SIZE/2);
        }
      }  
      if (state == SONG && notes.count() == 0) {
        state = MENU;
        screen.refresh(state);
      }   
  }
  timer.run();
  
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
      if (option == 0) {
        if (songs.count() == 0) {
          bluetooth.require_songs(0, SONGS_SIZE);
        }
        state = LIST;        
      }
      else {
        state = OFF;
        while(notes.count() > 0)
          notes.pop();
      }
    }
    else if (state == LIST) {
      start_song = millis();
      note_id = 0;
      updateTimer();
      state = SONG;
      while (notes.count() > 0)
        notes.pop();
      bluetooth.require_notes(song_id, 0, NOTES_SIZE);
    }
    else if (state == SONG) {
      state = MENU;
    }
    screen.refresh(state);
  }
  last_interrupt_time = interrupt_time;
  
}

void interrupt_blue()
{
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 250) {
    if (state == MENU)
      option = (option + 1) % 2;
    else if (state == LIST) {
      char * song = songs.pop();
      delete song;
      song_id = (song_id + 1) % song_count;
      short required_song = (song_id + songs.count()) % song_count;
      bluetooth.require_songs(required_song, required_song+1);
    }
    screen.refresh(state);
  }
  last_interrupt_time = interrupt_time;
}

void test_melody() {
  char melody [][4] = {"G4", "E4", "E4", "F4", "D4", "D4", "C4", "E4", "G4", "G4", "E4", "E4", "F4", "D4", "D4", "C4", "E4", "C4"};
  byte timing[] = {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};
  
  for (int i = 0; i < 18; i++)
    notes.my_push(Note(melody[i], timing[i]));
  for (int i = 0; i < 18; i++) {
    Serial.println("loop step");
    Note n = notes.pop();
    n.play();
    delay(1300 / n.timing);
  }
}

void updateTimer() {
  if (state == SONG)
     screen.refresh(state);
}



