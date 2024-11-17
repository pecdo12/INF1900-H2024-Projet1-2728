
#include "Motricite.hpp"

// Methodes de la classe Moteur

Moteur::Moteur(PortDirection port, uint8_t brocheDirection, PinPWM pinPWM) : brocheAction_(PWM(pinPWM)){
    // brocheAction_ = PWM(pinPWM);
    port_ = port;
    brocheDirection_ = brocheDirection - 1;
    switch (port_)
    {
    case PortDirection::B:
        DDRB |= 1 << brocheDirection_;
        break;
    case PortDirection::D:
        DDRD |= 1 << brocheDirection_;
        break;
    default:
        break;
    }
};
void Moteur::avancer(uint8_t pourcentage){
    switch (port_)
    {
    case PortDirection::B:
        PORTB &= ~(1 << brocheDirection_);
        break;
    case PortDirection::D:
        PORTD &= ~(1 << brocheDirection_);
        break;
    default:
        break;
    }
    brocheAction_.setPourcentage(100);
    brocheAction_.setPourcentage(pourcentage);
};
void Moteur::reculer(uint8_t pourcentage){
    switch (port_)
    {
    case PortDirection::B:
        PORTB |= 1 << brocheDirection_;
        break;
    case PortDirection::D:
        PORTD |= 1 << brocheDirection_;
        break;
    default:
        break;
    }
    brocheAction_.setPourcentage(100);
    brocheAction_.setPourcentage(pourcentage);
};
void Moteur::arret(){
    brocheAction_.setPourcentage(0);
};

// Methodes de la classe Motricite

Motricite::Motricite(Moteur moteurGauche, Moteur moteurDroit) : moteurGauche_(moteurGauche), moteurDroit_(moteurDroit) {
};
void Motricite::arret(){
    moteurGauche_.arret();
    moteurDroit_.arret();
};
void Motricite::marcheAvant(uint8_t pourcentage){
    moteurGauche_.avancer(pourcentage);
    moteurDroit_.avancer(pourcentage);
};
void Motricite::marcheArriere(uint8_t pourcentage){
    moteurGauche_.reculer(pourcentage);
    moteurDroit_.reculer(pourcentage);
};
void Motricite::tournerDroite(uint8_t degre, uint8_t vitesse){
    Motricite::arret();
    moteurGauche_.avancer(vitesse);
    for (uint8_t i = 0; i < degre; ++i){
        _delay_loop_2(DELAY_LOOP_CYCLES_10_MS);//delay loop pour 10 ms soit pour que le robot tourne de 1 degre
    }
    moteurGauche_.arret();
};
void Motricite::tournerGauche(uint8_t degre, uint8_t vitesse){
    Motricite::arret();
    moteurDroit_.avancer(vitesse);
    for (uint8_t i = 0; i < degre; ++i){
        _delay_loop_2(DELAY_LOOP_CYCLES_10_MS);//delay loop pour 10 ms soit pour que le robot tourne de 1 degre
    }
    moteurDroit_.arret();
};
void Motricite::twerk(uint8_t nombre, uint8_t angle){
    for (uint8_t i = 0; i < nombre; ++i){
        moteurDroit_.avancer();
        moteurGauche_.reculer();
        for (uint8_t i = 0; i < angle; ++i){
            _delay_loop_2(DELAY_LOOP_CYCLES_10_MS);//delay loop pour 10 ms soit pour que le robot tourne de 1 degre
        }
        moteurGauche_.avancer();
        moteurDroit_.reculer();
        for (uint8_t i = 0; i < angle; ++i){
            _delay_loop_2(DELAY_LOOP_CYCLES_10_MS);//delay loop pour 10 ms soit pour que le robot tourne de 1 degre
        }
    }
    moteurDroit_.arret();
    moteurGauche_.arret();
    
};
