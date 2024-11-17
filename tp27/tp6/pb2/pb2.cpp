/*
 * Nom: Mohamed Omar Zedek / Souhayl Sejjari
 * Groupe 02
 * Equipe 27
 * TP6 PB2
 * Description: La led sur la carte mere du robot possede 3 couleurs possibles
 * qui varient en fonction de l'intensite lumineuse que recoit la resistance 
 * electroluminecente, allant de vert pour une intensite lumineuse faible, a rouge
 * pour une intensite lumineuse forte, en passant par ambre pour une intensite lumineuse
 * moderee.
 * Identifications matérielles : Un cable serti famelle-femelle relie une source de courant continu des broches 
 * de la carte mere afin d'alimenter le circuit. Le circuit retourne une valeur de courant modifiee
 * sur la pin A0 de la carte mere. De plus un cable serti femelle-femelle relie les pin B0-B1 a la DEL libre
 */

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "can.h"

const uint8_t IN = 0x00;
const uint8_t OUT = 0xFF;
const uint8_t DELAI_COULEUR_AMBRE = 10;
const uint8_t MIN = 0;
const uint8_t MAX = 255;
const uint8_t VALEUR_LUMIERE_TAMISEE = 213;
const uint8_t VALEUR_LUMIERE_AMBIANTE = 243;

void turnLedOff();
void allumerRouge();
void allumerVert();
void allumerAmbre();

int main(){
    DDRA = IN;
    DDRB = OUT;

    can conv = can();

    while (1){
        uint8_t resultatConversion = conv.lecture(0) >> 2;

        if ( MIN < resultatConversion && resultatConversion < VALEUR_LUMIERE_TAMISEE){
            allumerVert();
        }
        if ( VALEUR_LUMIERE_TAMISEE < resultatConversion && resultatConversion < VALEUR_LUMIERE_AMBIANTE){
            allumerAmbre();
        }
        if ( VALEUR_LUMIERE_AMBIANTE < resultatConversion && resultatConversion < MAX){
            allumerRouge();
        }
    }
    

}

void allumerRouge(){
    PORTB |= (1 << PA1);
    PORTB &= ~(1 << PA0);
}

void allumerVert(){
    PORTB |= (1 << PA0);
    PORTB &= ~(1 << PA1);
}

void allumerAmbre(){
    allumerRouge();
    _delay_ms(DELAI_COULEUR_AMBRE);
    allumerVert();
    _delay_ms(DELAI_COULEUR_AMBRE);
}
