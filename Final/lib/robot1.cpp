#include "robot1.hpp"

uint8_t Robot1Parcours::moyenneDistance(){
    static uint16_t moyenne = 0;

    for (uint8_t i = 0; i < 5; i++)
    {
        moyenne += convertisseur_->lecture(0) >> 2;
    }
    moyenne /= 5;

    return moyenne;
}

void Robot1Parcours::positionnerCentre(){
    uint8_t distance = 0;
    for (uint8_t i = 0; i < 40; i++)
    {
        _delay_ms(500);
        distance = moyenneDistance(); 
        if (distance <= 0x33)
        {
            maMotricite_->marcheAvant(100);
            _delay_ms(20);
            maMotricite_->arret();
            moteurD_->avancer(50);
            moteurG_->avancer(50);
            _delay_ms(200);
            maMotricite_->arret();
        }
        else if (distance >= 0x30)
        {
            maMotricite_->marcheArriere(100);
            _delay_ms(20);
            maMotricite_->arret();
            moteurD_->reculer(50);
            moteurG_->reculer(50);
            _delay_ms(200);
            maMotricite_->arret();
        }
        else
        {
            break;
        }
    }

}

uint8_t Robot1Parcours::trouverTrou(){
    const uint8_t nombresCycleTourComplet = 60;
    uint8_t nombreCycle = 0;
    uint8_t distance = 0;
    while (true)
        {
            _delay_ms(500);
            distance = moyenneDistance(); 
            if (distance > 0x15)
            {
                moteurD_->reculer(100);
                _delay_ms(5);
                moteurD_->reculer(60);
                _delay_ms(200);
                moteurD_->arret();
                _delay_ms(400);
                ++nombreCycle;
                if (distance <= 0x15)
                {
                    break;
                }
                moteurG_->avancer(100);
                _delay_ms(5);
                moteurG_->avancer(50);
                _delay_ms(200);
                moteurG_->arret();
                ++nombreCycle;
            }

            else
            {
                break;
            }
        }

    if (nombreCycle > nombresCycleTourComplet)
    {
        nombreCycle -= nombresCycleTourComplet;
    }
    
    return nombreCycle;
}
