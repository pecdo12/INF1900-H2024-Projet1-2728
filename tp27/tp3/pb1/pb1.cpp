// Changement :
// 2e boucle for
// fonction delai

#define F_CPU 8000000UL // 8 MHz
#include <avr/io.h>
#include <util/delay.h>

using namespace std ;

// prototypes

/* Couleur */
void DEL_libre_Eteinte();
void DEL_libre_Rouge();
void DEL_libre_Vert();
void DEL_libre_Ambre();

/*Nuances de Couleur */
void ShortDelay(double);
void CycleCouleur(double , double ) ; 

/* Constantes */

const int8_t mask_PORTA{(1 << PA1) | (1 << PA0)};

// Duree Totale : 10 secondes
const int DureeTotale{10};

// Intensite lumineuse
const int NbDegresLumineux{10}; // Nb de degres lumineux excluant l'etat eteint
const double DureeDegres{double(DureeTotale) / (NbDegresLumineux + 1)}; // Comme on a n degres Lumineux + l'etat eteint, on a de deviser la duree totale en n+1 periodes

// Delay loop2 function
const double TimeLimitDL2{0.2621} ; // duree en secondes
const double MaxIterationsDL2 {65536 } ; // Nb max d'iterations


// CPU
const long FrequenceMC{8'000'000}; // Frequence du micro*controleur en Hz
const double PeriodeCPU{double(1) / FrequenceMC}; 

// fonction delai : performe une duree precise avec un nombre de cycles de CPU
int main()
{

    DDRA |= mask_PORTA; // A0 et A1 en sortie

    double FrequenceSignal {1000} ; 
    double PourcentageEteint{};

    while (true)
    {
        for (int16_t NumDegre{}; NumDegre <= NbDegresLumineux; ++NumDegre)
        {

            PourcentageEteint = (double(NumDegre) / (NbDegresLumineux)); // Variation de l'intensite lumineuse
            // Frequence *= 0.9  // variation de la frequence du signal

            CycleCouleur(FrequenceSignal , PourcentageEteint) ;
        }
    }
}


// Definition de fonctions

/*Couleur de la DEL libre*/

// On considere qu'on utilise les broches A0 / A1 pour la DEL libre

const uint8_t delay_couleur_ambre{10} ;

void DEL_libre_Eteinte()
{
    uint8_t mask_eteint{((1 << PA1) | (1 << PA0))};
    PORTA |= mask_eteint;
}

void DEL_libre_Rouge()
{
    PORTA |= (1 << PA1);
    PORTA &= ~(1 << PA0);
}

void DEL_libre_Vert()
{
    PORTA |= (1 << PA0);
    PORTA &= ~(1 << PA1);
}

void DEL_libre_Ambre()
{
    DEL_libre_Rouge();
    _delay_ms(delay_couleur_ambre);
    DEL_libre_Vert();
    _delay_ms(delay_couleur_ambre);
}



void ShortDelay(double Duree) // duree en secondes
{                   
    if ( Duree !=0 && Duree < TimeLimitDL2 ) // Ne pas mettre de delai si le delai necessaire est 0
    {
        double Duree4Cycles{4 * PeriodeCPU};         // Une iteration = 4 cycles de CPU
        int NbIterations{int(Duree / Duree4Cycles)}; // Nb iterations pour delay loop

        if (NbIterations == MaxIterationsDL2) // Pour le max d'iterations, il faut passer 0 en argument de la fonction delayloop2
            NbIterations = 0 ; 

        _delay_loop_2(NbIterations);
    }
}

void CycleCouleur(double FrequenceSignal , double PourcentageEteint ) // Frequence en Hz 
{ 
    // Signal electrique
    double PeriodeSignal { 1 / FrequenceSignal} ; 

    // Calcul de la duree d'allumage / eteint en fonction de la frequence demandee
    double DureeEteint  {PourcentageEteint * PeriodeSignal};
    double DureeAllumee {PeriodeSignal - DureeEteint};

    for (double TimeCounter{}; TimeCounter <= DureeDegres ; TimeCounter += PeriodeSignal)
    {
        DEL_libre_Rouge();
        ShortDelay(DureeAllumee);
        DEL_libre_Eteinte();
        ShortDelay(DureeEteint);
    }
}
