#include <Arduino.h>
#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "spi.hpp"

#define LED_RED_P1 PC2
#define LED_BLUE_P1 PC3
#define LED_GREEN_P1 PC4

#define LED_RED_P2 PC5
#define LED_BLUE_P2 PD2
#define LED_GREEN_P2 PD3

// RGB
#define RGB_LED_RED_P1 PD4
#define RGB_LED_GREEN_P1 PD5
#define RGB_LED_BLUE_P1 PD6

#define RGB_LED_RED_P2 PD7
#define RGB_LED_GREEN_P2 PB0
#define RGB_LED_BLUE_P2 PB1

// BUTTONS

#define BUTTON_PIN PC1
#define BUTTON_PIN2 PC0

volatile bool startGame = false;
int randColor;
int randColor2;

void setup()
{
    Serial.begin(9600);

    SPI_init();
    // MISO trebuie să fie OUTPUT în modul Slave

    DDRC |= (1 << LED_RED_P1) | (1 << LED_BLUE_P1) | (1 << LED_GREEN_P1) | (1 << LED_RED_P2);
    DDRD |= (1 << LED_BLUE_P2) | (1 << LED_GREEN_P2) | (1 << RGB_LED_RED_P1) | (1 << RGB_LED_GREEN_P1) | (1 << RGB_LED_BLUE_P1) | (1 << RGB_LED_RED_P2);
    DDRB |= (1 << RGB_LED_GREEN_P2) | (1 << RGB_LED_BLUE_P2);

    DDRC |= (1 << BUTTON_PIN) | (1 << BUTTON_PIN2);
    PORTC &= ~(1 << BUTTON_PIN) | (1 << BUTTON_PIN2);

    srand(millis());
}

void rgbColor()
{
    randColor = rand() % 3;
    switch (randColor)
    {
    case 0:
        PORTD |= (1 << RGB_LED_BLUE_P1);
        PORTD &= ~(1 << RGB_LED_GREEN_P1);
        PORTD &= ~(1 << RGB_LED_RED_P1);
        break;

    case 1:
        PORTD |= (1 << RGB_LED_GREEN_P1);
        PORTD &= ~(1 << RGB_LED_RED_P1);
        PORTD &= ~(1 << RGB_LED_BLUE_P1);
        break;

    case 2:
        PORTD |= (1 << RGB_LED_RED_P1);
        PORTD &= ~(1 << RGB_LED_GREEN_P1);
        PORTD &= ~(1 << RGB_LED_BLUE_P1);
        break;
    }
}

void rgbColorP2()
{
    randColor2 = rand() % 3;
    switch (randColor2)
    {
    case 0:
        PORTB |= (1 << RGB_LED_BLUE_P2);
        PORTB &= ~(1 << RGB_LED_GREEN_P2);
        PORTD &= ~(1 << RGB_LED_RED_P2);
        break;

    case 1:
        PORTB |= (1 << RGB_LED_GREEN_P2);
        PORTD &= ~(1 << RGB_LED_RED_P2);
        PORTB &= ~(1 << RGB_LED_BLUE_P2);
        break;

    case 2:
        PORTD |= (1 << RGB_LED_RED_P2);
        PORTB &= ~(1 << RGB_LED_GREEN_P2);
        PORTB &= ~(1 << RGB_LED_BLUE_P2);
        break;
    }
}

void openLeds()
{
    int value = analogRead(BUTTON_PIN); // Citește tensiunea pe pinul analogic
                                        // Afișează valoarea brută
    if (value >= 443 && !startGame)
    {
        startGame = true;
        rgbColor();
        rgbColorP2();
    }

    if (value >= 443 && startGame)
    {

        PORTC |= (1 << LED_RED_P1);
        PORTC &= ~(1 << LED_GREEN_P1);
        PORTC &= ~(1 << LED_BLUE_P1);
        if (randColor == 2)
        {
            rgbColor();
            _delay_ms(100);
            scoreP1 += 10;
            Serial.print("Score P1: ");
            Serial.println(scoreP1);
        }
    }

    else if (value >= 60 && value <= 90 && startGame)
    {

        PORTC |= (1 << LED_GREEN_P1);
        PORTC &= ~(1 << LED_RED_P1);
        PORTC &= ~(1 << LED_BLUE_P1);
        if (randColor == 1)
        {
            rgbColor();
            _delay_ms(100);
            scoreP1 += 10;
            Serial.print("Score P1: ");
            Serial.println(scoreP1);
        }
    }

    else if (value >= 30 && value <= 59 && startGame)
    {

        PORTC |= (1 << LED_BLUE_P1);
        PORTC &= ~(1 << LED_RED_P1);
        PORTC &= ~(1 << LED_GREEN_P1);
        if (randColor == 0)
        {
            rgbColor();
            _delay_ms(100);
            scoreP1 += 10;
            Serial.print("Score P1: ");
            Serial.println(scoreP1);
        }
    }
}

void openLedsP2()
{
    int value2 = analogRead(BUTTON_PIN2);

    // if (value2 >= 443 && !startGame)
    // {
    //         startGame = true;
    //         rgbColorP2();
    // }

    if (value2 >= 443 && startGame)
    {

        PORTC |= (1 << LED_RED_P2);
        PORTD &= ~(1 << LED_GREEN_P2);
        PORTD &= ~(1 << LED_BLUE_P2);
        if (randColor2 == 2)
        {
            rgbColorP2();
            //_delay_ms(100);

            scoreP2 += 10;
            Serial.print("Score P2: ");
            Serial.println(scoreP2);
        }
    }

    else if (value2 >= 40 && value2 <= 90 && startGame)
    {

        PORTD |= (1 << LED_GREEN_P2);
        PORTC &= ~(1 << LED_RED_P2);
        PORTD &= ~(1 << LED_BLUE_P2);
        if (randColor2 == 1)
        {
            rgbColorP2();
            // _delay_ms(100);
            scoreP2 += 10;
            Serial.print("Score P2: ");
            Serial.println(scoreP2);
        }
    }

    else if (value2 >= 30 && value2 <= 39 && startGame)
    {

        PORTD |= (1 << LED_BLUE_P2);
        PORTC &= ~(1 << LED_RED_P2);
        PORTD &= ~(1 << LED_GREEN_P2);
        if (randColor2 == 0)
        {
            rgbColorP2();
            // _delay_ms(100);
            scoreP2 += 10;
            Serial.print("Score P2: ");
            Serial.println(scoreP2);
        }
    }
}

void loop()
{
    openLeds();
    openLedsP2();

    if (SPSR & (1 << SPIF))
    {                           // Verifică dacă datele au sosit
        uint8_t command = SPDR; // Citește comanda primită de la Master

        // Debugging: Afișează valoarea scorului înainte de a-l trimite
        Serial.print("Command received: ");
        Serial.println(command, HEX);

        // Răspunde pe baza comenzii primite
        switch (command)
        {
        case 0x01: // Trimite scorul Player 1
            Serial.print("Score Player 1: ");
            Serial.println(scoreP1);
            SPDR = scoreP1;
            break;
        case 0x02: // Trimite scorul Player 2
            Serial.print("Score Player 2: ");
            Serial.println(scoreP2);
            SPDR = scoreP2;
            break;
        default: // Comandă necunoscută
            SPDR = 0x00;
            break;
        }

        // Debugging
        Serial.print("Response sent: ");
        Serial.println(SPDR, HEX);
    }
}