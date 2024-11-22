#include "spi.hpp"
#include <LiquidCrystal.h>
#include <Servo.h>
Servo servo; // Obiect pentru controlul servomotorului

int currentPosition = 0; 
int targetPosition = 180; 

const int rs = 7, en = 6, d4 = A5, d5 = A4, d6 = A3, d7 = A2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int slaveSelect = 10;

void moveServoTo(int target, int speedDelay)
{
    if (currentPosition < target)
    {
        
        for (int pos = currentPosition; pos <= target; pos++)
        {
            servo.write(pos); 
            delay(speedDelay); 
        }
        currentPosition = target; 
    }
    else if (currentPosition > target)
    {
        
        for (int pos = currentPosition; pos >= target; pos--)
        {
            servo.write(pos);  
            delay(speedDelay); 
        }
        currentPosition = target; 
    }
}

void updateScore(int scoreP1, int scoreP2)
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Player 1: ");
    lcd.setCursor(11, 0);
    lcd.print(scoreP1);

    lcd.setCursor(0, 1);
    lcd.print("Player 2: ");
    lcd.setCursor(11, 1);
    lcd.print(scoreP2);
}

void setup()
{
    servo.attach(9);              
    servo.write(currentPosition); 
    delay(1000);                 

    SPI_init();
    pinMode(slaveSelect, OUTPUT);
    digitalWrite(slaveSelect, HIGH);

    lcd.begin(16, 2);
    lcd.setCursor(0, 0);
    lcd.print("Master ready");
}

void loop()
{
    
    moveServoTo(targetPosition, 50); 
  
    digitalWrite(slaveSelect, LOW);
    uint8_t scoreP1 = SPI_exchange(0x01); 
    digitalWrite(slaveSelect, HIGH);

 
  
    digitalWrite(slaveSelect, LOW);
    uint8_t scoreP2 = SPI_exchange(0x02);
    digitalWrite(slaveSelect, HIGH);

    // Actualizează LCD
    updateScore(scoreP1, scoreP2);

    delay(500); 
}
