#include <SoftwareSerial.h>

SoftwareSerial hc05(RXPIN, TXPIN);

class Bluetooth {
public:
  Bluetooth(Buffer & buf) : buffer(buf) {}
  Buffer & buffer;
  void my_write(short n) {
    char x1 = n >> 8;
    char x2 = n & 0xff;
    hc05.write(x1);
    hc05.write(x2);
    Serial.write(x1);
    Serial.write(x2);
  }
  
  void require_songs(short first, short last) {
    hc05.write('P');
    my_write(first);
    my_write(last);
    buffer.init();
  }
  
  void require_notes(short song_index, short first_note, short last_note) {
    hc05.write('N');
    my_write(song_index);
    my_write(first_note);
    my_write(last_note);
    buffer.init();
  }

  void read() {
    int n = hc05.available();
    //Serial.print("n: "); Serial.println(n);
    while (n--) {
      char x = hc05.read();
      // Serial.print("receive: "); Serial.println(x);
      buffer.push(x);
    }
  }

  
};
