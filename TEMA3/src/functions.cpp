#include "spi.hpp"

volatile int scoreP1 = 0;
volatile int scoreP2 = 0;


void SPI_init()
{
    DDRB |= (1 << MISO);  // MISO ca output
    DDRB &= ~((1 << MOSI) | (1 << SCK) | (1 << SS));  // MOSI, SCK și SS ca input

    // Activează SPI ca Slave
    // SPCR |= (1 << SPE);

    // /* TODO1: set MOSI and SCK output, all others input */
    // DDRB |= (1 << SCK) | (1 << MOSI) | (1<<MISO);

    
  
    SPCR |=  (1 << SPE);
}

uint8_t SPI_exchange(uint8_t data)
{
    

    SPDR = data;
    /* Wait for transmission complete */
    while (!(SPSR & (1 << SPIF)))
        ;
    return SPDR;
}


void Slave_receive()
{
    if (SPSR & (1 << SPIF)) 
    {
        uint8_t command = SPDR;  // Citește comanda de la Master
        Serial.print("Received command: ");
        Serial.println(command, HEX);  // Afișează comanda primită

        // Răspunde la comenzi
        switch (command)
        {
        case 0x01:  // Trimite scorul Player 1
            SPDR = scoreP1;
            break;
        case 0x02:  // Trimite scorul Player 2
            SPDR = scoreP2;
            break;
        default:  
            SPDR = 0x00;
            break;
        }

        // Debugging pentru răspuns
        Serial.print("Response sent: ");
        Serial.println(SPDR, HEX);
    }
}

void Slave_transmit(uint8_t data)
{
 

    SPI_exchange(data);
}
