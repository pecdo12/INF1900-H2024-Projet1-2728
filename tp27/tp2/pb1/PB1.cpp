/*
 * Nom: Mohamed Omar Zedek / Souhayl Sejjari
 * Groupe 02
 * Equipe 27
 * TP2 PB1
 * Description: Le programme contrôle le comportement d'une LED
 * en réponse à l'interaction avec un bouton. Il suit une séquence
 * d'états définis : ledLibre, bouttonAppuye1, bouttonRelache1, 
 * bouttonAppuye2, bouttonRelache2, bouttonAppuye3, bouttonRelache3.
 * Lorsque l'utilisateur appuie et relâche le bouton trois fois,
 * la LED passe à un état allumé en vert pendant un court laps de
 * temps (défini par delaiLedAllumee) et revient ensuite à l'état
 * initial (LED éteinte). 
 * 
<<<<<<< HEAD
 * Identifications matérielles : interrupteur relie a la 3e broche D2 du
 * port D | led reliee aux 2 premieres broches du port A tel que
 * A0 --> (-) et A1 --> (+) 
 * */
=======
 * Identifications matérielles : interrupteur relie au 3e bit du
 * port D | led reliee aux 2 premieres broches du port A tel que
 * A1 --> (-) et A2 --> (+)
 */
>>>>>>> a0b33e549275c29448cc1f3a476f3ce2f121cd5f

/* Tableau d'etats
|    Etat present   |   Interupteur   |     Etat suivant    |  PORTA |
|:-----------------:|:---------------:|:-------------------:|:------:|
| BOUTTON_APPUYE_1  |        0        | BOUTTON_APPUYE_1    | eteint |
|                   |        1        | BOUTTON_RELACHE_1   | eteint |

| BOUTTON_RELACHE_1 |        0        | BOUTTON_RELACHE_1   | eteint |
|                   |        1        | BOUTTON_APPUYE_2    | eteint |

| BOUTTON_APPUYE_2  |        0        | BOUTTON_APPUYE_2    | eteint |
|                   |        1        | BOUTTON_RELACHE_2   | eteint |

| BOUTTON_RELACHE_2 |        0        | BOUTTON_RELACHE_2   | eteint |
|                   |        1        | BOUTTON_APPUYE_3    | eteint |

| BOUTTON_APPUYE_3  |        0        | BOUTTON_APPUYE_3    | eteint |
|                   |        1        | BOUTTON_RELACHE_3   | eteint |

| BOUTTON_RELACHE_3 |        0        | BOUTTON_RELACHE_3   | eteint |
|                   |        1        | BOUTTON_APPUYE_1    |  vert  |
*/



#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>

const int8_t MASK_PORTA{(1 << PA1) | (1 << PA0)};
const int8_t MASK_PORTD{~(1 << PD2)};
const uint8_t MASK_BUTTON = 1 << PA2;
const uint8_t DELAI_DEBOUNCE = 25;
const uint16_t DELAI_ALLUME = 2000;

enum class Etats {
    LED_LIBRE, BOUTTON_APPUYE_1, BOUTTON_RELACHE_1, BOUTTON_APPUYE_2, BOUTTON_RELACHE_2, BOUTTON_APPUYE_3, BOUTTON_RELACHE_3
};

bool estBoutonPresse() {
    return (PIND & MASK_BUTTON);
}

bool estSansRebond() {
    if (estBoutonPresse()) {
        _delay_ms(DELAI_DEBOUNCE);
        if (estBoutonPresse()) {
            return true;
        }
        return false;
    }
    return false;
}

void allumerDelVert()
{
    PORTA |= (1 << PA0);
    PORTA &= ~(1 << PA1);
}

void eteindreDel()
{
    uint8_t maskEteint{((1 << PA1) | (1 << PA0))};
    PORTA |= maskEteint;
}

int main() {
    DDRA |= MASK_PORTA;
    DDRD &= MASK_PORTD;

    Etats etatCourant = Etats::LED_LIBRE;

    while (1) {
        switch (etatCourant) {

            case Etats::LED_LIBRE :
                eteindreDel() ; 
                if (estSansRebond()) {
                    etatCourant = Etats::BOUTTON_APPUYE_1;
                }
                break;

            case Etats::BOUTTON_APPUYE_1 :
                if (!estBoutonPresse()) {
                    etatCourant = Etats::BOUTTON_RELACHE_1;
                }
                break;

            case Etats::BOUTTON_RELACHE_1 :
                if (estSansRebond()) {
                    etatCourant = Etats::BOUTTON_APPUYE_2;
                }
                break;

            case Etats::BOUTTON_APPUYE_2 :
                if (!estBoutonPresse()) {
                    etatCourant = Etats::BOUTTON_RELACHE_2;
                }
                break;

            case Etats::BOUTTON_RELACHE_2 :
                if (estSansRebond()) {
                    etatCourant = Etats::BOUTTON_APPUYE_3;
                }
                break;

<<<<<<< HEAD
            case Etats::BOUTTON_APPUYE_3 : 
=======
            case Etats::BOUTTON_APPUYE_3 :
>>>>>>> a0b33e549275c29448cc1f3a476f3ce2f121cd5f
                if (!estBoutonPresse()) {
                    etatCourant = Etats::BOUTTON_RELACHE_3;
                }
                break;

            case Etats::BOUTTON_RELACHE_3 :
                allumerDelVert() ;
                _delay_ms(DELAI_ALLUME);
                etatCourant = Etats::LED_LIBRE;
                break;

            default :
                break;
        }
    }
}
