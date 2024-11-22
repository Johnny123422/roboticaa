#ifndef SPI_H
#define SPI_H

#include <Arduino.h>
#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

// Pin definitions
// SLAVE-MASTER

#define SCK PB5
#define MISO PB4
#define MOSI PB3
#define SS PB2



extern volatile int scoreP1;
extern volatile int scoreP2;


void SPI_init();
uint8_t SPI_exchange(uint8_t data);
// void slave_init();
// uint8_t Slave_receive();
// void Slave_transmit(uint8_t data);

#endif
