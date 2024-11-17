
/*
 * Nom: Sebastian Cristescu ; Mohamed Yassir Merzouki
 * Groupe 2 ; Equipe 28
 * TP6 PB2
 * Description: Programme qui attend qui mesure la lumiere ambiante et allume le LED d'une couleur rouge si la lumiere actuelle est plus grande que la lumiere ambiante, ou vert si la lumiere actuelle
 * est plus petite que la lumiere ambiante. Si la lumiere actuelle est egale a la lumiere ambiante, le LED s'allume ambre.
 * Identifications matérielles : Les deux premieres broches dans le port B sont un output pour le LED et les deux premieres broches dans le port D sont un input qui recoit des informations de la photoresistance.
 */



#include <avr/io.h> 
#define F_CPU 8000000
#include <util/delay.h>
#include <avr/interrupt.h>
#include <can.h>
#include <can.cpp>

const uint8_t LedOff = PB0;

const uint8_t fluxInformation = PD3;
const uint8_t sortieLed = PD3;

uint8_t lumiereActuel = 0;
uint8_t lumiereAmbiant = 0;

void allumerRouge(){
    PORTB |= (1 << PB0);
    PORTB &= ~(1 << PB1);
}

void allumerVert(){
    PORTB |= (1 << PB1);
    PORTB &= ~(1 << PB0);
}


int main()
{
    DDRB = sortieLed; 
    DDRA = fluxInformation;

    can convertisseur;
    lumiereAmbiant = convertisseur.lecture(0) >> 2;

    while(true){
        lumiereActuel = convertisseur.lecture(0) >> 2;

        if(lumiereActuel < (lumiereAmbiant))
        {
            allumerVert();
        }

        else if(lumiereActuel == lumiereAmbiant)
        {
            allumerRouge();
            _delay_ms(10);
            allumerVert();
            _delay_ms(10);
        }

        else if(lumiereActuel > (lumiereAmbiant))
        {
            allumerRouge();
        }

        else
        {
            PORTB &= LedOff;
        }
    }
        
    

    return 0; 
}