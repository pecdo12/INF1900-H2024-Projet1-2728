#include <util/crc16.h>
#include "debug.h"

uint8_t lireData();

// capteur : (D6-GND-VCC)
void recupererDonnee(uint8_t tableauBits[]);

//appeler recupererDonnee pour recuperer les infos dans un tableau temporairement
uint8_t valeurDonnee(uint8_t tableauBits[]);

uint8_t crcDonnee(uint8_t tableauBits[]);

//verifier CRC pour une donnee
bool verifierDonnee(uint8_t Donnee, uint8_t crcDonnee);
