#include "Emission.hpp"

// LED : borche D6 -> resistance -> LED -> GND 
void initialiserTransmission()
{
    DDRD |= 1 << PD5;// | 1 << PD4;
    // PORTD &= ~(1 << PD4);
    TCNT1 = 0 ;
    TCCR1A = (1 << COM1A0);
    TCCR1B = (1 << WGM12 | 1 << CS10);
    TCCR1C = 0;

    OCR1A = 104;
}

void genererFrequence()
{
    TCCR1A |= (1 << COM1A0);
}

void arreterFrequence()
{
    TCCR1A &= ~(1 << COM1A0);
}

void genererStart()
{
    genererFrequence();
    _delay_us(2400);
    arreterFrequence();
    _delay_us(600);
}

void genererBit(uint8_t val)
{
    genererFrequence();
    switch (val)
    {
    case 1:
        _delay_us(1200);
        break;
    case 0:
        _delay_us(600);
        break;
    default:
        break;
    }
    arreterFrequence();
    _delay_us(600);
}

// Genere un start + 7 bits de donnee + 5 bits de crc
void genererSequence(uint8_t val)
{
    genererStart();

    uint8_t crc = _crc16_update(0xFFFF, val & 0x7F);
    for (uint8_t i = 0; i < 5; i++){ // generer 5 bits crc
        genererBit((crc >> (4 - i)) & 0x01);
    }

    for (uint8_t i = 0; i < 7; i++){ // genere IR 7bits donnee
        genererBit((val >> (6 - i)) & 0x01);
    }

    // for (uint8_t i = 0; i < 8; i++){ // genere IR 7bits donnee
    //     genererBit((val >> (7 - i)) & 0x01);
    // }
}
