#include "debug.h"
#include "Global.hpp"


void initDebug() {
    initialisationUART();
}

void afficherDebug(const char* str) {
    while (*str) {
        transmissionUART(*str++);
    }
}

void valeurDebug(uint8_t value) {
    transmissionUART(value);
}

void largeValeurDebug(uint16_t value) {
    transmissionUART(uint8_t (value >> 8));
    transmissionUART(uint8_t (value << 8));       
}

