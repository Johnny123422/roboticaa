#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Arduino.h>

// Pin definitions
#define LED_RED PB1
#define LED_GREEN PB2
#define LED_BLUE PB3
#define BUTTON_START PD2
#define BUTTON_DIFFICULTY PD3

// Global variables
extern volatile bool startGame; 
extern volatile unsigned int secondCounter; 
extern volatile unsigned int thirdCounter; 
extern char difficulty[10];
extern unsigned int easyTime; 
extern unsigned int mediumTime; 
extern unsigned int hardTime; 
extern unsigned int roundDuration; 
extern int score;

extern char currentWord[20]; 
extern char userInput[20];
extern int userIndex;
extern char buffer[10]; 
extern const char *words[]; 
extern const int numWords; 


void USART_init(long baudRate);
char USART_receiver();
void USART_transmit(char data);
void USART_sendString(const char *str);
void generateRandomWord(char *wordBuffer);
void resetGame();
void checkRoundEnd();
void handleDifficulty();

#endif
