#include <Arduino.h>
#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "functions.h"

volatile bool startGame = false;
volatile unsigned int secondCounter = 0;
volatile unsigned int thirdCounter = 0;
char difficulty[10] = "Easy";
unsigned int easyTime = 10;      // Seconds to wait for easy
unsigned int mediumTime = 5;     // Seconds to wait for medium
unsigned int hardTime = 3;       // Seconds to wait for hard
unsigned int roundDuration = 30; // Round duration in seconds
int score = 0;

char currentWord[20];
char userInput[20];
int userIndex = 0;
char buffer[10];

const char *words[] = {
    "apple", "banana", "cherry", "grape", "orange", "lemon", "peach", "pear", "plum", "melon",
    "carrot", "broccoli", "tomato", "onion", "potato", "cucumber", "pepper", "corn", "bean", "pea",
    "river", "ocean", "lake", "pond", "stream", "creek", "waterfall", "bay", "lagoon", "spring",
    "table", "chair", "desk", "sofa", "bed", "shelf", "drawer", "lamp", "couch", "closet",
    "car", "bike", "train", "plane", "boat", "bus", "truck", "scooter", "subway", "tram"};
const int numWords = sizeof(words) / sizeof(words[0]);

void startSequence()
{
    char buffer[4];
    for (int i = 3; i > 0; i--)
    {

        itoa(i, buffer, 10);
        USART_sendString(buffer);

        PORTB |= (1 << LED_RED) | (1 << LED_GREEN) | (1 << LED_BLUE);
        _delay_ms(500);
        PORTB &= ~((1 << LED_RED) | (1 << LED_GREEN) | (1 << LED_BLUE));
        _delay_ms(500);
    }
    generateRandomWord(currentWord);
    USART_sendString("New Word: ");
    USART_sendString(currentWord);
    USART_sendString("\n");
    thirdCounter = 0;
}

ISR(TIMER1_COMPA_vect)
{
    if (startGame)
    {
        secondCounter++;
        thirdCounter++;
    }
}

void timer1_init()
{
    TCCR1A = 0;
    TCCR1B = (1 << WGM12);
    TCCR1B |= (1 << CS12) | (1 << CS10);
    OCR1A = 15624;
    TIMSK1 = (1 << OCIE1A);
}

ISR(INT0_vect)
{
    static bool lastStartButtonState = HIGH;

    bool currentStartButtonState = (PIND & (1 << BUTTON_START)) == 0;

    if (lastStartButtonState == HIGH && currentStartButtonState == LOW)
    {

        startGame = !startGame;

        if (startGame)
        {
            USART_sendString("Start round \n");

            resetGame();
            startSequence();
            PORTB |= (1 << LED_GREEN);
        }

        else
        {
            PORTB &= ~((1 << LED_RED) | (1 << LED_GREEN) | (1 << LED_BLUE));
            USART_sendString("End round \n");
        }
    }

    lastStartButtonState = currentStartButtonState;
}

ISR(INT1_vect)
{
    static bool lastButtonState = HIGH;

    bool currentButtonState = (PIND & (1 << BUTTON_DIFFICULTY)) == 0;

    if (lastButtonState == HIGH && currentButtonState == LOW && !startGame)
    {

        if (strcmp(difficulty, "Easy") == 0)
        {
            strcpy(difficulty, "Medium");
        }
        else if (strcmp(difficulty, "Medium") == 0)
        {
            strcpy(difficulty, "Hard");
        }
        else
        {
            strcpy(difficulty, "Easy");
        }

        USART_sendString(difficulty);
        USART_sendString(" mode on!\n");
    }

    lastButtonState = currentButtonState;
}

int main()
{
    srand(millis());

    DDRB |= (1 << LED_RED) | (1 << LED_GREEN) | (1 << LED_BLUE);
    DDRD &= ~(1 << BUTTON_START) | (1 << BUTTON_DIFFICULTY);
    PORTD |= (1 << BUTTON_START) | (1 << BUTTON_DIFFICULTY);

    PORTB |= (1 << LED_RED) | (1 << LED_GREEN) | (1 << LED_BLUE);

    USART_init(9600);
    EIMSK |= (1 << INT0) | (1 << INT1);
    EICRA |= (1 << ISC00) | (1 << ISC10);
    timer1_init();
    sei();

    while (1)
    {
        if (startGame)
        {
            handleDifficulty();
            checkRoundEnd();
        }

        char c = USART_receiver();

        if (c == 8) // backspace
        {

            if (userIndex > 0)
            {
                userIndex--;
                userInput[userIndex] = '\0'; // Null-terminate the string
                PORTB |= (1 << LED_GREEN);
                PORTB &= ~(1 << LED_RED);
            }
        }
        else if (c != '\r' && c != '\n' && userIndex < sizeof(userInput) - 1)
        {

            userInput[userIndex++] = c;

            if (strncmp(currentWord, userInput, userIndex) != 0)
            {
                PORTB &= ~(1 << LED_GREEN);
                PORTB |= (1 << LED_RED);
            }
            else
            {
                PORTB &= ~(1 << LED_RED);
                PORTB |= (1 << LED_GREEN);
            }
        }
        else if (c == '\n' || c == '\r')
        {
            userInput[userIndex] = '\0';

            if (strcmp(currentWord, userInput) == 0)
            {
                score++;
                userIndex = 0;
                memset(userInput, 0, sizeof(userInput));

                USART_sendString("Correct! New Word: ");
                generateRandomWord(currentWord);
                USART_sendString(currentWord);
                USART_sendString("\n");

                thirdCounter = 0;
                PORTB &= ~(1 << LED_RED);
                PORTB |= (1 << LED_GREEN);
            }
        }
    }
}