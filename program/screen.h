#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 6, 7, 12, 13);

class Screen {
public:
  
  void refresh(State state) {
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

  void disp(const char * first_line, const char * second_line, bool arrow = true) {
    lcd.clear();
    lcd.setCursor(0, 0);
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
      
  }
  
  void display_list() {
    if (songs.count() == 0)
      disp("", "");
    else if (songs.count() == 1)
      disp(songs.front(), "");
    else
      disp(songs.front(), "");//songs.front2());
  }
  
  void display_song() {
    if (songs.count() == 0)
      disp("", "0:00");
    else {
      unsigned long seconds = (millis() - start_song) / 1000;      
      disp(songs.front(), time_to_str(seconds / 60, seconds % 60));
    }
  }
  char timex[5] = "0:00";
  char * time_to_str(unsigned long minutes, unsigned long seconds) {
    timex[0] = (char) minutes + '0';
    timex[2] = (char) (seconds / 10) + '0';
    timex[3] = (char) (seconds % 10) + '0';
    return timex;
  }
};
