#define BTN_RED 2
#define BTN_BLUE 3
#define RXPIN 10
#define TXPIN 11
#define WZMACNIACZ_IN 5

#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 6, 7, 12, 13); // wyświetlacz
SoftwareSerial bluetooth(RXPIN, TXPIN);

enum State {OFF, MENU, LIST, SONG};
State state, old_state;
int option, options;

void setup()
{
  Serial.begin(9600);
  bluetooth.begin(9600);
  lcd.begin(16, 2);
  pinMode(WZMACNIACZ_IN, OUTPUT);
  pinMode(BTN_RED, INPUT_PULLUP);
  pinMode(BTN_BLUE, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BTN_RED), interrupt_red, FALLING);
  attachInterrupt(digitalPinToInterrupt(BTN_BLUE), interrupt_blue, FALLING);

  Serial.println("Start");
  state = MENU;
  option = 1;
  options = 2;
}

void loop()
{
  if (state == MENU) {
    if (state != old_state) {
      Serial.println("menu");
      old_state = state;
    }
     display_menu();
  }
  if (state == LIST) {
    if (state != old_state) {
      Serial.println("list");
      old_state = state;
    }
    display_list();
  }
  if (state == SONG) {
    if (state != old_state) {
      Serial.println("song");
      old_state = state;
    }
    display_info();
  }
  if (state == OFF) {
    if (state != old_state) {
      Serial.println("off");
      old_state = state;
    }
    delay(100);
  }
}

void interrupt_red()
{
  static unsigned long long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 500) {
    if (state == OFF) {
      state = MENU;
    }
    if (state == MENU) {
      if (option == 1)
        state = LIST;
      else
        state = OFF;
    }
    if (state == LIST) {
      state = SONG;
    }
    if (state == SONG) {
      state = MENU;
    }
  }
  last_interrupt_time = interrupt_time;
}

void interrupt_blue()
{
  static unsigned long long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 500) {
    option = (option++) % options;
  }
  last_interrupt_time = interrupt_time;
}

void disp(const char * first_line, const char * second_line) {
  lcd.setCursor(0, 0);
  lcd.print(first_line);
  lcd.setCursor(0, 1);
  lcd.print(second_line);
}


void display_menu() {
  if (option == 1)
    disp("-> Play", "Turn off");
   else
    disp("Play", "-> Turn off");
}

void display_list() {
  return;
}

void display_info() {
  return;
}

void choose_next() {
  return;
}

void play_next_song() {
  return;
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




