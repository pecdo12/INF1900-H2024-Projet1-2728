/*
 * Nom: Mohamed Omar Zedek / Souhayl Sejjari
 * Groupe 02
 * Equipe 27
 * TP6 PB1
 * Description: Ce programme contrôle une LED multicolore et un compteur en fonction de l'état d'un bouton-poussoir.
 * Lorsque le bouton est enfoncé, un compteur s'active, incrémentant 10 fois par seconde jusqu'à atteindre 120 ou lorsque le bouton est relâché.
 * Lorsque le compteur atteint 120 ou que le bouton est relâché, la LED clignote en vert pendant 1/2 seconde.
 * Ensuite, après deux secondes, la LED s'allume en rouge et clignote un nombre de fois égal à la moitié de la valeur du compteur, à un rythme de 2 fois par seconde.
 * Enfin, la LED passe au vert pendant une seconde avant de s'éteindre, ramenant le robot à son état initial.
 * Identifications matérielles : Un cable serti famelle-femelle relie une source de courant continu des broches 
 * de la carte mere afin d'alimenter le circuit, le circuit celui d'un bouton poussoir qui ouvre le circuit lorsque presse. 
 * Le circuit retourne une valeur de courant sur la pin PD2 de la carte mere. La LED libre est connectée aux broches B0-B1 de la carte mère.

Table d'etats;
+----------------+--------------------------+-------------------------+----------------+----------------------------+
|  ETAT PRESENT  | B (bouton appuyé ou non) | D (durée écoulée ou non)|  ETAT SUIVANT  | SORTIE (Couleur de la DEL) |
+----------------+--------------------------+-------------------------+----------------+----------------------------+
|    INITIAL     |            1             |            X            | ETEINT_PRESSE  |          ETEINTE           |
+----------------+--------------------------+-------------------------+----------------+----------------------------+
|    INITIAL     |            0             |            X            |    INITIAL     |          ETEINTE           |
+----------------+--------------------------+-------------------------+----------------+----------------------------+
| ETEINT_PRESSE  |            0             |            0            | CLIGNOTE_VERT  |          ETEINTE           |
+----------------+--------------------------+-------------------------+----------------+----------------------------+
| ETEINT_PRESSE  |            0             |            1            | CLIGNOTE_VERT  |          ETEINTE           |
+----------------+--------------------------+-------------------------+----------------+----------------------------+
| ETEINT_PRESSE  |            1             |            0            | ETEINT_PRESSE  |          ETEINTE           |
+----------------+--------------------------+-------------------------+----------------+----------------------------+
| ETEINT_PRESSE  |            1             |            1            | CLIGNOTE_VERT  |          ETEINTE           |
+----------------+--------------------------+-------------------------+----------------+----------------------------+
| CLIGNOTE_VERT  |            X             |            1            | ETEINT_RELACHE |           VERTE            |
+----------------+--------------------------+-------------------------+----------------+----------------------------+
| CLIGNOTE_VERT  |            X             |            0            | CLIGNOTE_VERT  |           VERTE            |
+----------------+--------------------------+-------------------------+----------------+----------------------------+
| ETEINT_RELACHE |            X             |            1            | CLIGNOTE_ROUGE |          ETEINTE           |
+----------------+--------------------------+-------------------------+----------------+----------------------------+
| ETEINT_RELACHE |            X             |            0            | ETEINT_RELACHE |          ETEINTE           |
+----------------+--------------------------+-------------------------+----------------+----------------------------+
| CLIGNOTE_ROUGE |            X             |            1            |  ALLUME_VERT   |           ROUGE            |
+----------------+--------------------------+-------------------------+----------------+----------------------------+
| CLIGNOTE_ROUGE |            X             |            0            | CLIGNOTE_ROUGE |           ROUGE            |
+----------------+--------------------------+-------------------------+----------------+----------------------------+
|  ALLUME_VERT   |            X             |            1            |    INITIAL     |           VERTE            |
+----------------+--------------------------+-------------------------+----------------+----------------------------+
|  ALLUME_VERT   |            X             |            0            |  ALLUME_VERT   |           VERTE            |
+----------------+--------------------------+-------------------------+----------------+----------------------------+ */



#define F_CPU 8000000UL // 8 MHz

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/*Prototypes*/

// Timer
void lancerTimer(double);

void attendreSecondes(double);

// couleur de la DEL
void allumeerDELRouge();
void allumeerDELAmbre();
void allumeerDELVerte();
void eteindreLed();
void clignoterDELRouge(double);

// Bouton 
void initialiserBouton(void);

//Etats
void changerEtat(double&, int&, int = 0);

enum class Etat
{
    INITIAL,
    ETEINT_PRESSE,
    CLIGNOTE_VERT,
    ETEINT_RELACHE,
    CLIGNOTE_ROUGE,
    ALLUME_VERT

};

volatile Etat gEtatPresent = Etat::INITIAL;
volatile int16_t gCompteurUniteDeTemps = 0;
volatile bool gEstBoutonPresse = false;


int main()
{
    DDRB |= ((1 << PB1) | (1 << PB0)); 
    initialiserBouton();               
    double uniteDeTemps = 0;           
    double valeurFinaleDuCompteur = 0;
    double periodeClignotement = 0.5;
    bool estDureeEcoulee = false;
    int maxCompteur = 0;

    while (true)
    {
        estDureeEcoulee = gCompteurUniteDeTemps >= maxCompteur;

        switch (gEtatPresent)
        {
        case Etat::INITIAL:
        {
            eteindreLed();
            if (gEstBoutonPresse)
            {
                changerEtat(uniteDeTemps, maxCompteur);
                lancerTimer(uniteDeTemps);
            }
            break;
        }
        case Etat::ETEINT_PRESSE:
        {
            eteindreLed();
            if (!gEstBoutonPresse || estDureeEcoulee)
            {

                valeurFinaleDuCompteur = gCompteurUniteDeTemps;
                changerEtat(uniteDeTemps, maxCompteur);
                lancerTimer(uniteDeTemps);
            }

            break;
        }
        case Etat::CLIGNOTE_VERT:
        {
            allumeerDELVerte();
            if (estDureeEcoulee)
            {
                changerEtat(uniteDeTemps, maxCompteur);
                lancerTimer(uniteDeTemps);
            }

            break;
        }
        case Etat::ETEINT_RELACHE:
        {
            eteindreLed();
            if (estDureeEcoulee)
            {
                changerEtat(uniteDeTemps, maxCompteur, valeurFinaleDuCompteur);
                lancerTimer(uniteDeTemps);
            }

            break;
        }
        case Etat::CLIGNOTE_ROUGE:
        {
            clignoterDELRouge(periodeClignotement);
            if (estDureeEcoulee)
            {
                changerEtat(uniteDeTemps, maxCompteur);
                lancerTimer(uniteDeTemps);
            }

            break;
        }
        case Etat::ALLUME_VERT:
        {
            allumeerDELVerte();
            if (estDureeEcoulee)
            {
                changerEtat(uniteDeTemps, maxCompteur);
            }

            break;
        }
        default:
            break;
        }
    }
}


const int DELAI_DEBOUNCE = 30;

ISR(INT0_vect)
{
    _delay_ms(DELAI_DEBOUNCE);
    if (gEtatPresent == Etat::INITIAL)
    {
        gEstBoutonPresse = true;
    }
    else if (gEtatPresent == Etat::ETEINT_PRESSE)
    {
        gEstBoutonPresse = false;
    }

    // Clear le interrupt flag de D2
    EIFR |= (1 << INTF0);
}

void initialiserBouton(void)
{

    cli();                    // desactiver toutes les interruptions
    gEstBoutonPresse = false; // reinitialiser etat du bouton
    // Configuration Entree / Sortie des ports
    DDRD &= ~(1 << PD2); // bouton poussoir en entree

    // Activer les interruptions dans les PINS specifiques
    EIMSK |= (1 << INT0);

    // Configurer les conditions d'interruption pour chaque PIN (clock)
    EICRA |= (1 << ISC00);
    EICRA &= ~(1 << ISC01);

    sei(); // Activer toutes les interruptions
}

// Timer

ISR(TIMER1_COMPA_vect)
{
    ++gCompteurUniteDeTemps; // ajouter une unite de temps
}

const unsigned long int freqCPU = 8'000'000;
const unsigned int prescaler = 1024;
const unsigned long int freqAvecPrescaler = freqCPU / prescaler; // nombre de cycles pour 1 seconde
const int TIMER_NULLE = 0 ; 

void lancerTimer(double uniteDeTemps)
{
    TCNT1 = TIMER_NULLE; 
    gCompteurUniteDeTemps = TIMER_NULLE;
    int16_t nCycles = uniteDeTemps * freqAvecPrescaler;
    OCR1A = nCycles;

    int maskTCCR1A{0b0001100};
    TCCR1A &= maskTCCR1A; // On utilise aucun bit de ce registre

    TCCR1B |= (1 << WGM12 | 1 << CS12 | 1 << CS10);
    TCCR1B &= ~(1 << WGM13 | 1 << CS11);

    TIMSK1 |= (1 << OCIE1A);
}

// couleur de la DEL

void eteindreLed()
{
    PORTB |= ((1 << PB1) | (1 << PB0));
}

void allumeerDELRouge()
{
    PORTB |= (1 << PB1);
    PORTB &= ~(1 << PB0);
}

void allumeerDELVerte()
{
    PORTB |= (1 << PB0);
    PORTB &= ~(1 << PB1);
}

void clignoterDELRouge(double periodeClignotement)
{

    double dureeCouleurUnique = periodeClignotement / 2;
    allumeerDELRouge();
    attendreSecondes(dureeCouleurUnique);
    eteindreLed();
    attendreSecondes(dureeCouleurUnique);
}

// Temps

const int DELAI1MS = 1;
const int facteurSecondeMilisecondes = 1000;

void attendreSecondes(double dureeAttenteSecondes)
{
    int dureeAttenteMs = int(dureeAttenteSecondes * facteurSecondeMilisecondes);
    for (int i = 0; i < dureeAttenteMs; ++i)
    {
        _delay_ms(DELAI1MS);
    }
}

// constantes compteur
const int MAX_COMPTEUR_INITIAL = 120;
const int MAX_COMPTEUR_1 = 1;
const int MAX_COMPTEUR_ALLUME_VERT = 2;
const int DIVISEUR_MOITIE = 2 ; 

// constantes Unite de Temps (en secondes)
const double DIXIEME_UNITE_TEMPS = 0.1;
const double DEMI_UNITE_TEMPS = 0.5;
const double ENTIERE_UNITE_TEMPS = 1;

void changerEtat(double &uniteDeTemps, int &maxCompteur, int valeurFinaleDuCompteur)
{
    switch (gEtatPresent)
    {
    case Etat::INITIAL:
    {
        gEtatPresent = Etat::ETEINT_PRESSE;
        maxCompteur = MAX_COMPTEUR_INITIAL;
        uniteDeTemps = DIXIEME_UNITE_TEMPS; 
        break;
    }
    case Etat::ETEINT_PRESSE:
    {
        gEtatPresent = Etat::CLIGNOTE_VERT;
        maxCompteur = MAX_COMPTEUR_1;
        uniteDeTemps = DEMI_UNITE_TEMPS; 
        break;
    }

    case Etat::CLIGNOTE_VERT:
    {
        gEtatPresent = Etat::ETEINT_RELACHE;
        maxCompteur = MAX_COMPTEUR_ALLUME_VERT;
        uniteDeTemps = ENTIERE_UNITE_TEMPS;
        break;
    }
    case Etat::ETEINT_RELACHE:
    {

        gEtatPresent = Etat::CLIGNOTE_ROUGE;
        maxCompteur = valeurFinaleDuCompteur / DIVISEUR_MOITIE;
        uniteDeTemps = DEMI_UNITE_TEMPS;
        break;
    }
    case Etat::CLIGNOTE_ROUGE:
    {
        gEtatPresent = Etat::ALLUME_VERT;
        maxCompteur = MAX_COMPTEUR_1;
        uniteDeTemps = ENTIERE_UNITE_TEMPS;
        break;
    }
    case Etat::ALLUME_VERT:
    {
        gEtatPresent = Etat::INITIAL;
        gEstBoutonPresse = false;
        break;
    }
    default:
        break;
    }
}