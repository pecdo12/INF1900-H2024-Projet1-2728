#include "ReceptionIR.hpp"

uint8_t lireData(){ // lire si la led eet un 1, 0, ou S
    while (PIND & (1 << PD3)){} // RAISING
    uint16_t compteur = 0;
    while (!(PIND & (1 << PD3))){
        compteur++;
        _delay_us(50);
    } // FALLING
    if (compteur <= 15){
        return 0;
    }else if (compteur <= 35){
        return 1;
    }else   {
        return 2;
    }
}

// capteur : (D6-GND-VCC)
void recupererDonnee(uint8_t tableauBits[]){ // prendre une sequence de 12 bits et la stocker dans un tableau de taille 12
    while (lireData() != 2){}
    for (uint8_t i = 12; i > 0; i--){
        tableauBits[i - 1] = lireData();
    }
}

void afficherDonnee(uint8_t tableauBits[]){
    for (uint8_t i = 0; i < 12; i++){
        DEBUG_VALUE(tableauBits[i]);
    }
    DEBUG_PRINT("\n");
}

//appeler recupererDonnee pour recuperer les infos dans un tableau temporairement
uint8_t valeurDonnee(uint8_t tableauBits[]){ // recueperer a partir d'un tableau de taille 12 la valeur emise
    uint8_t result = 0;
    for (uint8_t i = 0; i < 7; i++) {
        result |= tableauBits[i] << i;
    }
    return result;
}

uint8_t crcDonnee(uint8_t tableauBits[]){ // recueperer a partir d'un tableau de taille 12 le crcRecu emis
    uint8_t result = 0;
    for (uint8_t i = 0; i < 5; i++) {
        result |= tableauBits[i+7] << i;
    }
    return result;
}

bool verifierDonnee(uint8_t Donnee, uint8_t crcDonnee){ // verifier a partir d'une donneeRecue et d'un crcRecu si la donneeRecue est correctement emise
    return (_crc16_update(0xFFFF, Donnee) & (0x1F)) == crcDonnee;
}
