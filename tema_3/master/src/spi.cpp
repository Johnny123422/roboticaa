#include "spi.hpp"


// Variabile globale pentru scoruri
volatile int scoreP1 = 0;
volatile int scoreP2 = 0;

void SPI_init()
{
    DDRB |= (1 << MOSI) | (1 << SCK) | (1 << SS);  // MOSI, SCK, SS ca output
    // DDRB &= ~(1 << MISO);                          // MISO ca input

    SPCR |= (1 << MSTR) | (1 << SPE) | (1 << SPR0);  // Master, enable SPI, fosc/16
}


uint8_t SPI_exchange(uint8_t data)
{
    SPDR = data;  // Trimite byte-ul către Slave
    while (!(SPSR & (1 << SPIF))) {
        // Așteaptă până când transmisia este completă
    }
    return SPDR;  // Returnează byte-ul primit de la Slave
}
