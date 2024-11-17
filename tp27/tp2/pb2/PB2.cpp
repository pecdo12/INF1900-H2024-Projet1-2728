/*
 * Nom: Mohamed Omar Zedek / Souhayl Sejjari
 * Groupe 02
 * Equipe 27
 * TP2 PB2
 * 
 * Description: Le programme gère le comportement de la LED
 * en fonction des actions sur le bouton, passant successivement
 * par les états suivants : rouge (bouton relâché),
 * ambre (lorsque le bouton est pressé), vert (bouton relâché),
 * rouge (lorsque le bouton est pressé à nouveau),
 * éteint (bouton relâché), vert (lorsque le bouton est pressé
 * une troisième fois). Les changements de couleur de la LED
 * sont déclenchés lorsqu'on maintient le bouton enfoncé ou
 * lorsqu'on le relâche.
 * 
<<<<<<< HEAD
 * Identifications matérielles : interrupteur relie a la 3e broche D2 du
 * port D | led reliee aux 2 premieres broches du port A tel que
 * A0 --> (-) et A1 --> (+)
=======
 * Identifications matérielles : interrupteur relie au 3e bit du
 * port D | led reliee aux 2 premieres broches du port A tel que
 * A1 --> (-) et A2 --> (+)
>>>>>>> a0b33e549275c29448cc1f3a476f3ce2f121cd5f
 */

/* Tableau d'etats
| État actuel        | Bouton Pressé?  | Nouvel État         | Action sur la LED  |
|--------------------|-----------------|---------------------|--------------------|
| ROUGE_LIBRE        | 0               | ROUGE_LIBRE         | éteint             |
| ROUGE_LIBRE        | 1               | AMBRE_PRESSE        | éteint             |
| AMBRE_PRESSE       | 0               | AMBRE_PRESSE        | éteint             |
| AMBRE_PRESSE       | 1               | VERT_LIBRE          | éteint             |
| VERT_LIBRE         | 0               | VERT_LIBRE          | éteint             |
| VERT_LIBRE         | 1               | ROUGE_PRESSE        | éteint             |
| ROUGE_PRESSE       | 0               | ROUGE_PRESSE        | éteint             |
| ROUGE_PRESSE       | 1               | ETEINT_LIBRE        | éteint             |
| ETEINT_LIBRE       | 0               | ETEINT_LIBRE        | éteint             |
| ETEINT_LIBRE       | 1               | VERT_PRESSE         | éteint             |
| VERT_PRESSE        | 0               | VERT_PRESSE         | vert               |
| VERT_PRESSE        | 1               | ROUGE_LIBRE         | vert               |

*/

#define F_CPU 8000000UL // 8 MHz
#include <avr/io.h>
#include <util/delay.h>

bool isButtonPressed();
bool isButtonReleased();

void turnLedOff();
void turnLedRed();
void turnLedGreen();
void turnLedAmber();

enum class EtatRobot
{
    ROUGE_LIBRE,
    AMBRE_PRESSE,
    VERT_LIBRE,
    ROUGE_PRESSE,
    ETEINT_LIBRE,
    VERT_PRESSE
};

const int8_t MASK_PORTA{(1 << PA1) | (1 << PA0)};
const int8_t MASK_PORTD{~(1 << PD2)};

int main()
{
    DDRA |= MASK_PORTA;
    DDRD &= MASK_PORTD;

    EtatRobot etatPresent{EtatRobot::ROUGE_LIBRE};

    while (true)
    {
        switch (etatPresent)
        {
        case EtatRobot::ROUGE_LIBRE :
        {
            turnLedRed();

            bool boutonAppuye{isButtonPressed()};
            if (boutonAppuye)
                etatPresent = EtatRobot::AMBRE_PRESSE;
            break;
        }
        case EtatRobot::AMBRE_PRESSE :
        {
            turnLedAmber();

            bool boutonLibre(isButtonReleased());
            if (boutonLibre)
                etatPresent = EtatRobot::VERT_LIBRE;
            break;
        }
        case EtatRobot::VERT_LIBRE :
        {
            turnLedGreen();

            bool boutonAppuye{isButtonPressed()};
            if (boutonAppuye)
                etatPresent = EtatRobot::ROUGE_PRESSE;
            break;
        }
        case EtatRobot::ROUGE_PRESSE :
        {
            turnLedRed();

            bool boutonLibre(isButtonReleased());
            if (boutonLibre)
                etatPresent = EtatRobot::ETEINT_LIBRE;
            break;
        }

        case EtatRobot::ETEINT_LIBRE :
        {
            turnLedOff();

            bool boutonAppuye{isButtonPressed()};
            if (boutonAppuye)
                etatPresent = EtatRobot::VERT_PRESSE;
            break;
        }

        case EtatRobot::VERT_PRESSE :
        {
            turnLedGreen();

            bool boutonLibre(isButtonReleased());
            if (boutonLibre)
                etatPresent = EtatRobot::ROUGE_LIBRE;
            break;
        }
        
        default :
        {
            break ; 
        }
        }
    }
}

const uint8_t DELAI_DEBOUNCE{10};
const int MASK_BUTTON{(1 << PD2)};

bool isButtonPressed()
{
    bool isStateButton = PIND & MASK_BUTTON;
    if (isStateButton)
    {
        _delay_ms(DELAI_DEBOUNCE);
        isStateButton = PIND & MASK_BUTTON;
        if (isStateButton)
            return true;
    }
    return false;
}

bool isButtonReleased() { return !isButtonPressed(); }

const uint8_t DELAI_COULEUR_AMBRE{10};

void turnLedOff()
{
    uint8_t maskEteint{((1 << PA1) | (1 << PA0))};
    PORTA |= maskEteint;
}

void turnLedRed()
{
    PORTA |= (1 << PA1);
    PORTA &= ~(1 << PA0);
}

void turnLedGreen()
{
    PORTA |= (1 << PA0);
    PORTA &= ~(1 << PA1);
}

void turnLedAmber()
{
    turnLedRed();
    _delay_ms(DELAI_COULEUR_AMBRE);
    turnLedGreen();
    _delay_ms(DELAI_COULEUR_AMBRE);
}
