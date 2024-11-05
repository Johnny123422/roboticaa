#include <Arduino.h>
#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "functions.h"


void USART_init(long baudRate)
{
    uint16_t ubrrValue = (F_CPU / 16 / baudRate) - 1; //calculate baud_rate
    UBRR0H = (unsigned char)(ubrrValue >> 8);
    UBRR0L = (unsigned char)ubrrValue;
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    UCSR0C = (1 << USBS0) | (1 << UCSZ00) | (1 << UCSZ01);
}

char USART_receiver()
{
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

void USART_transmit(char data)
{
    while (!(UCSR0A & (1 << UDRE0)))
        ;
    UDR0 = data;
}

void USART_sendString(const char *str)
{
    while (*str)
    {
        USART_transmit(*str++);
    }
    USART_transmit('\n');
}


void generateRandomWord(char *wordBuffer)
{
    int randomIndex = random() % numWords;
    strcpy(wordBuffer, words[randomIndex]);
}

void resetGame()
{
    score = 0;
    secondCounter = 0;
    thirdCounter = 0;
}


void handleDifficulty()
{
    
    if ((strcmp(difficulty, "Easy") == 0 && thirdCounter >= easyTime) ||
        (strcmp(difficulty, "Medium") == 0 && thirdCounter >= mediumTime) ||
        (strcmp(difficulty, "Hard") == 0 && thirdCounter >= hardTime))
    {
        generateRandomWord(currentWord);
        USART_sendString("New Word: ");
        USART_sendString(currentWord);
        USART_sendString("\n");

        thirdCounter = 0; 
    }
}

void checkRoundEnd()
{
    if (secondCounter >= roundDuration)
    {
        USART_sendString("Round over! Correct words: ");
        itoa(score, buffer, 10);
        USART_sendString(buffer);
        USART_sendString("\n");

        startGame = false; 
        resetGame();       
        PORTB &= ~((1 << LED_RED) | (1 << LED_GREEN) | (1 << LED_BLUE));
    }
}
