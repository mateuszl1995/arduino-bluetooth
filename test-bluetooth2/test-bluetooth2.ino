#define POWER 2
#define RXPIN 10
#define TXPIN 11

#include <SoftwareSerial.h>

char k = 'a';
bool isPowered = false;
enum State {
  WAITING
};
SoftwareSerial bluetooth(RXPIN, TXPIN);

void setup()
{
  Serial.begin(9600);
  bluetooth.begin(9600);
  Serial.println("start");
  pinMode(3, OUTPUT);
  pinMode(POWER, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(POWER), power2, FALLING);
}

volatile int level = 0;

void loop()
{
  //k++;
  //if (k > 'z')
  //  k = 'a';
  
  if(bluetooth.available()){
    k = bluetooth.read();
    Serial.println(k);
    //k = bluetooth.read();
  } else {
    //bluetooth.write("he2llo");
  }
  
  delay(1000);
  
}

void power2()
{
  static unsigned long long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 500) {
    isPowered = !isPowered;
  }
  last_interrupt_time = interrupt_time;
}

