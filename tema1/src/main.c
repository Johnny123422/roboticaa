#include <Arduino.h>

#define RED_LED 1
#define GREEN_LED 2
#define LED_1 3
#define LED_2 4
#define LED_3 5
#define LED_4 6

#define Charge_button 10
#define Stop_button 11

unsigned long delayTime = 500;
bool stopPressed = false;      
unsigned long debounce = 3000;  
unsigned long Timp_buton = 0;
byte Stare_buton_ant;


void delayWithStopCheck(unsigned long time) {
  unsigned long start = millis();
  while (millis() - start < time) {
    button_stop();
    if (stopPressed) return;  
  }
}

void blinkAll() {
  for (int i = 0; i <= 3; i++) {
    if (stopPressed) return;  

    digitalWrite(LED_1, HIGH);
    digitalWrite(LED_2, HIGH);
    digitalWrite(LED_3, HIGH);
    digitalWrite(LED_4, HIGH);
    delayWithStopCheck(delayTime);
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, LOW);
    digitalWrite(LED_3, LOW);
    digitalWrite(LED_4, LOW);
    delayWithStopCheck(delayTime);
  }

  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);
}

void button_stop() {
  if (millis() - Timp_buton >= debounce) {
    byte Stare_buton = digitalRead(Stop_button);

    if (Stare_buton != Stare_buton_ant) {
      Timp_buton = millis();
      Stare_buton_ant = Stare_buton;


      if (Stare_buton == HIGH) {
        blinkAll();
        stopPressed = true;
         
      }
    }
  }
}


void blink(int led) {
  for (int i = 0; i < 2; i++) {
    if (stopPressed) return; 

    digitalWrite(led, HIGH);
    delayWithStopCheck(delayTime);
    digitalWrite(led, LOW);
    delayWithStopCheck(delayTime);
  }
}
void setup() {
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);
  pinMode(Charge_button, INPUT_PULLUP);
  pinMode(Stop_button, INPUT_PULLUP);
  Stare_buton_ant = digitalRead(Stop_button);
}

void loop() {
  stopPressed = false;  

  if (digitalRead(Charge_button) == LOW && !stopPressed) {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);

    blink(LED_1);
    if (stopPressed) return;

    digitalWrite(LED_1, HIGH);
    blink(LED_2);
    if (stopPressed) return;

    digitalWrite(LED_2, HIGH);
    blink(LED_3);
    if (stopPressed) return;

    digitalWrite(LED_3, HIGH);
    blink(LED_4);
    if (stopPressed) return;

    digitalWrite(LED_4, HIGH);
    blinkAll();
    if (stopPressed) return;
  }
}