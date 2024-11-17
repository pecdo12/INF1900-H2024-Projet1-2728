#include <util/crc16.h>
#include "Global.hpp"

void initialiserTransmission();
void arreterFrequence();
void genererStart();
void genererBit(uint8_t val);
void genererSequence(uint8_t val);