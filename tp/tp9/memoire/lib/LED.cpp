/*
Auteurs: Mohamed Omar Zedek, Sejjari Souhayl, Cristescu Sebastian, Mohamed Yassir Merzouki
Description: Classe LED avec constructeur et plusieurs méthodes. Celle-ci permet d'allumer une LED de couleur rouge, verte, ambre et de l'éteindre.
Identifications matérielles: Cette classe permet de spécifier sur quelles broches se trouvent la LED. Par défaut, les broches A0-A1 sont utilisés.
*/

#include "LED.hpp"

LED::LED(){
    DDRA |= (1 << brochePos_);
    DDRA |= (1 << brocheNeg_);
    port_ = PortLED::A;
    brochePos_ = 0;
    brocheNeg_ = 1;
}
LED::LED(PortLED port, uint8_t brochePos, uint8_t brocheNeg){
    port_ = port;
    brochePos_ = brochePos-1;
    brocheNeg_ = brocheNeg-1;
    switch(port_) {
        case PortLED::A:
            DDRA |= (1 << brochePos_);
            DDRA |= (1 << brocheNeg_);
            break;
        case PortLED::B:
            DDRB |= (1 << brochePos_);
            DDRB |= (1 << brocheNeg_);
            break;
        case PortLED::C:
            DDRC |= (1 << brochePos_);
            DDRC |= (1 << brocheNeg_);
            break;
        case PortLED::D:
            DDRD |= (1 << brochePos_);
            DDRD |= (1 << brocheNeg_);
            break;
        default:
            break;
    }
};

void LED::allumerRouge(){
    switch(port_) {
        case PortLED::A:
            PORTA |= (1 << brochePos_);
            PORTA &= ~(1 << brocheNeg_);
            break;
        case PortLED::B:
            PORTB |= (1 << brochePos_);
            PORTB &= ~(1 << brocheNeg_);
            break;
        case PortLED::C:
            PORTC |= (1 << brochePos_);
            PORTC &= ~(1 << brocheNeg_);
            break;
        case PortLED::D:
            PORTD |= (1 << brochePos_);
            PORTD &= ~(1 << brocheNeg_);
            break;
        default:
            break;
    }
};

void LED::allumerVert(){
    switch(port_) {
        case PortLED::A:
            PORTA |= (1 << brocheNeg_);
            PORTA &= ~(1 << brochePos_);
            break;
        case PortLED::B:
            PORTB |= (1 << brocheNeg_);
            PORTB &= ~(1 << brochePos_);
            break;
        case PortLED::C:
            PORTC |= (1 << brocheNeg_);
            PORTC &= ~(1 << brochePos_);
            break;
        case PortLED::D:
            PORTD |= (1 << brocheNeg_);
            PORTD &= ~(1 << brochePos_);
            break;
        default:
            break;
    }
};

void LED::allumerAmbre(){
    allumerRouge();
    _delay_ms(DELAI_COULEUR_AMBRE);
    allumerVert();
    _delay_ms(DELAI_COULEUR_AMBRE);
    eteindre();

};

void LED::allumerAmbre(uint16_t dureeEnMS){
    uint16_t nombreCycles = dureeEnMS / (DELAI_COULEUR_AMBRE*2);
    for(uint16_t i = 0; i < nombreCycles; ++i){
        allumerRouge();
        _delay_ms(DELAI_COULEUR_AMBRE);
        allumerVert();
        _delay_ms(DELAI_COULEUR_AMBRE);
    }
    eteindre();

};

void LED::eteindre(){
    switch(port_) {
        case PortLED::A:
            PORTA &= ~((1 << brochePos_) | (1 << brocheNeg_));
            break;
        case PortLED::B:
            PORTB &= ~((1 << brochePos_) | (1 << brocheNeg_));
            break;
        case PortLED::C:
            PORTC &= ~((1 << brochePos_) | (1 << brocheNeg_));
            break;
        case PortLED::D:
            PORTD &= ~((1 << brochePos_) | (1 << brocheNeg_));
            break;
        default:
            break;
    }
};