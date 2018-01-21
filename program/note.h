short freq_of_8[14] = {7040, 7458, 7902, -1, 4186, 4435, 4699, 4978, 5274, -1, 5588, 5920, 6272, 6644}; 
// freqs of: A8, AS8, B8, BS8, ..., G8, GS8 (value -1 if not exists) 

template <class T>
void logg(const char * str, T item) {
  Serial.print(str);
  Serial.println(item);
}

class Note {
public:
  uint8_t freq;  // example: E6 (E na 4 bitach i 6 na 4 bitach)
  byte timing;   // example: 8 (ósemka), 4 (ćwierćnuta)
  
  Note() {}
  Note(char _freq, byte _timing) {freq = _freq; timing = _timing; }
  Note(const char * str, int _timing) {
    freq = (str[0] - 'A') * 2;
    if (str[1] == 'S') {
      freq++;
      freq <<= 4;
      freq += (str[2] - '0');
    } else {
      freq <<= 4;
      freq += (str[1] - '0');
    }
    timing = _timing;
  }

  void play() {
    if (freq == 0)
      return;
    uint16_t f = freq_of_8[freq / 16];
    uint8_t x = (uint8_t) (freq & 15);
    f = ((int) f) >> (8 - x);
    tone(AMPLIFIER_IN, (short) f, 1000 / timing); // 1000 ms
  }
};
