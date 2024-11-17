// A1 : Enable
// AO : Direction

#define F_CPU 8000000UL // 8 MHz
#include <avr/io.h>
#include <util/delay.h>

using namespace std ;

// prototypes


/*Nuances de Couleur */
void ShortDelay(double);
void TimePWM(double  , double  , double  ) ; 
void SingleCyclePWM(double  , double ) ; 
void Arriere() ; 
void Avant() ; 


/* Constantes */

const int8_t mask_PORTA{(1 << PA1) | (1 << PA0)};


// Delay loop2 function
const double TimeLimitDL2{0.2621} ; // duree en secondes
const double MaxIterationsDL2 {65536} ; // Nb max d'iterations


// CPU
const long FrequenceMC{8'000'000}; // Frequence du micro*controleur en Hz
const double PeriodeCPU{double(1) / FrequenceMC}; // duree en secondes

// Donnees du probleme 
int16_t AllFrequences [] {100 , 1000} ; // frequences en Hz
int16_t NbVitessesPositives {4} ; // nb de vitesses non nuls sachant qu'on suit le meme patterne 
double  SingleSpeedDuration {2} ;  // duree en secondes


int main()
{
    // Configure entree / sortie des ports
    DDRA |= mask_PORTA; // A0 et A1 en sortie
    // main 
    Avant() ; 
    while(true){
        for (int16_t Frequence : AllFrequences){
            for(int16_t vitesse {} ; vitesse <= NbVitessesPositives ; ++vitesse ){
                double OffPercentage {double(vitesse) / NbVitessesPositives} ; 
                TimePWM(Frequence , OffPercentage , SingleSpeedDuration) ; 
            }
        }}

}


// Definition de fonctions

// Couleurs
void EteindreMoteur(){
    PORTA &=  ~(1 <<PA1) ;
}

void AllumerMoteur(){
    PORTA |=  (1 << PA1) ;
}

void Arriere(){
    PORTA |=  (1 << PA0) ;
}

void Avant(){
    PORTA &=  ~(1 <<PA0) ;    
}

// ShortDelay : performe une duree precise avec un nombre de cycles de CPU

void ShortDelay(double Duree) // duree en secondes  --> ne peut pas etre superieure a limite de delayloop2 (donc ps inferieure a une frequence)
{                   
    if ( Duree != 0 && Duree < TimeLimitDL2 ) // Ne pas mettre de delai si le delai necessaire est 0
    {
        double Duree4Cycles{4 * PeriodeCPU};         // Une iteration = 4 cycles de CPU
        int NbIterations{int(Duree / Duree4Cycles)}; // Nb iterations pour delay loop

        if (NbIterations == MaxIterationsDL2) // Pour le max d'iterations, il faut passer 0 en argument de la fonction delayloop2
            NbIterations = 0 ; 

        _delay_loop_2(NbIterations);
    }
}


void TimePWM(double FrequenceSignal , double OffPercentage , double TotalDuration ) // permet de generer un signal a une frequence et un ration On/Off precis pendant une duree totale
{ 
    double SignalPeriod {double(1) / FrequenceSignal} ; 
    for (double timer {} ; timer <= TotalDuration ; timer += SignalPeriod){
        SingleCyclePWM( SignalPeriod ,  OffPercentage) ;        
    }
}

void SingleCyclePWM(double SignalPeriod , double OffPercentage){

    double OffDuration {OffPercentage * SignalPeriod} ; 
    double OnDuration {SignalPeriod - OffDuration} ; 
    
    AllumerMoteur() ;  
    ShortDelay(OnDuration) ; 
    EteindreMoteur() ; 
    ShortDelay(OffDuration) ; 
}


