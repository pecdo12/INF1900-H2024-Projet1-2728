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
    moteurDroit_.avancer(pourcentage+1);
    moteurGauche_.avancer(pourcentage);
};
void Motricite::marcheArriere(uint8_t pourcentage){
    moteurGauche_.reculer(pourcentage);
    moteurDroit_.reculer(pourcentage+4);
};
void Motricite::tourner(uint8_t vitesseGauche, uint8_t vitesseDroite){
    moteurDroit_.avancer(vitesseDroite);
    moteurGauche_.avancer(vitesseGauche);
}
void Motricite::pivoterDroite(uint8_t vitesse){
    moteurDroit_.reculer(vitesse+5);
    moteurGauche_.avancer(vitesse);
}
void Motricite::pivoterGauche(uint8_t vitesse){
    moteurGauche_.reculer(vitesse);
    moteurDroit_.avancer(vitesse);
}
void Motricite::tournerDroite90(){
    Motricite::pivoterDroite(100);
    _delay_ms(25);
    Motricite::pivoterDroite(50);
    _delay_ms(980);
    Motricite::arret();
}
void Motricite::tournerGauche90(){
    Motricite::pivoterGauche(100);
    _delay_ms(25);
    Motricite::pivoterGauche(50);
    _delay_ms(980);
    Motricite::arret();
}



// void Motricite::tournerDroite(uint8_t degre, uint8_t vitesse){
//     Motricite::arret();
//     moteurGauche_.avancer(vitesse+8);
//     moteurDroit_.reculer(vitesse);
//     for (uint8_t i = 0; i < degre/2; ++i){
//         _delay_loop_2(DELAY_LOOP_CYCLES_10_MS);//delay loop pour 10 ms soit pour que le robot tourne de 1 degre
//     }
//    Motricite::arret();
// };
// void Motricite::tournerGauche(uint8_t degre, uint8_t vitesse){
//     Motricite::arret();
//     moteurDroit_.avancer(vitesse);
//     moteurGauche_.reculer(vitesse);
//     for (uint8_t i = 0; i < degre/2; ++i){
//         _delay_loop_2(DELAY_LOOP_CYCLES_10_MS);//delay loop pour 10 ms soit pour que le robot tourne de 1 degre
//     }
//     Motricite::arret();
// };
// void Motricite::twerk(uint8_t nombre, uint8_t angle){
//     Motricite::arret();
//     for (uint8_t i = 0; i < nombre; ++i){
//         moteurDroit_.avancer();
//         moteurGauche_.reculer();
//         for (uint8_t i = 0; i < angle; ++i){
//             _delay_loop_2(DELAY_LOOP_CYCLES_10_MS);//delay loop pour 10 ms soit pour que le robot tourne de 1 degre
//         }
//         moteurGauche_.avancer();
//         moteurDroit_.reculer();
//         for (uint8_t i = 0; i < angle; ++i){
//             _delay_loop_2(DELAY_LOOP_CYCLES_10_MS);//delay loop pour 10 ms soit pour que le robot tourne de 1 degre
//         }
//     }
//     Motricite::arret();
    
// };
