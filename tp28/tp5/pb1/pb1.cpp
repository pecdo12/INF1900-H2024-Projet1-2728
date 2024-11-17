#include "memoire_24.h"

#ifndef F_CPU
/* fournir un avertissement mais non une erreur */
# warning "F_CPU pas defini pour 'memoire_24.cpp'"
# define F_CPU 8000000UL
#endif
#include <avr/eeprom.h>

#include <util/delay.h>
#include "memoire_24.h"



const uint8_t SortieLed = PD3;
const uint8_t LedOff = PA0;

void allumerRouge(){
    PORTA |= (1 << PA0);
    PORTA &= ~(1 << PA1);
}

void allumerVert(){
    PORTA |= (1 << PA1);
    PORTA &= ~(1 << PA0);
}



int main() {
    DDRA = SortieLed; 
    uint16_t read;

    uint8_t *letters[] = {
        (uint8_t *)"P", (uint8_t *)"O", (uint8_t *)"L", (uint8_t *)"Y",
        (uint8_t *)"T", (uint8_t *)"E", (uint8_t *)"C", (uint8_t *)"H",
        (uint8_t *)"N", (uint8_t *)"I", (uint8_t *)"Q", (uint8_t *)"U",
        (uint8_t *)"E"
    };

    int length = sizeof(letters) / sizeof(letters[0]);

    // Create an instance of Memoire24CXXX
    Memoire24CXXX memoire;

    // Define the addresses for writing 'p' and 'o'
    uint16_t address = 0;
    allumerRouge();

    for (int i = 0; i < length; ++i) {
        memoire.ecriture(address, *letters[i]);
        address++;
        _delay_ms(10);
        PORTA &= LedOff;
    }

    _delay_ms(2000);


    address = 0;
    for (int i = 0; i < length; ++i){
        read = memoire.lecture(address, letters[i]);
        address++;
        _delay_ms(10);
        if (read == *letters[i]){
            allumerVert();
        }
        else
            allumerRouge();
    }

    return 0;
}
