#include "debug.h"
#include "Global.hpp"

void uint16ToCharArray(uint16_t value, char *buffer, size_t bufferSize) 
{
    if (bufferSize >= 20) 
    {
        snprintf(buffer, 20, "%d", value);
    }
}

void initDebug() {
    initialisationUART();
}

void afficherChaine(const char* str) {
    while (*str) {
        transmissionUART(*str++);
    }
}

void afficherValeurNumerique(uint16_t value) {
    char myBuffer[20];
    uint16ToCharArray(value, myBuffer, sizeof(myBuffer));
    afficherChaine(myBuffer);
}

void largeValeurDebug(     
uint16_t value) {
    transmissionUART(uint8_t (value >> 8));
    transmissionUART(uint8_t (value << 8));       
}

