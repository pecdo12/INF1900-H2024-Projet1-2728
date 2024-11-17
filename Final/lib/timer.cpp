#include "timer.hpp"



template <typename T>

TimerCTC<T>::TimerCTC(NumeroTimer numeroTimer , Unite uniteDeTemps)
{      
    uniteDeTemps_ = uniteDeTemps;
    numeroTimer_ = numeroTimer ;   
    cycleDepart_ = 0 ; 
    nCycles_ = 0 ; 
}


// prescaler toujours a  1024 

template <typename T>

void TimerCTC<T>::initialiserTimer(void){ } ;

template <typename T>

void TimerCTC<T>::lancerEvenementIntervalle(double intervalle){
    convertirTempsEnCycles(intervalle) ; 
    cycleDepart_ = 0 ;
    initialiserTimer();
} 

template <typename T>

void TimerCTC<T>::demarrerChronometre(){
    convertirTempsEnCycles(DUREE_1_UNITE) ; // conversion de 1 sec / ms en cycles 
    cycleDepart_ = 0 ; // commencer au cycle 0 
    initialiserTimer() ;  
}

template <typename T>

void TimerCTC<T>::reprendreChronometre(void){
    initialiserTimer(); // relance le TimerCTC en commencant par le cycle atteint au dernier arret
}

template <typename T>

void TimerCTC<T>::arreterChronometre(){
    
    cycleDepart_ = *compteur_; // sauvegarder la derniere valeur TimerCTC
    *registreInterruption_ &= ~(1 <<OCIE1A) ; // arreter les interruptions (fonctionne pour tous les timers)
}


template <typename T>

void TimerCTC<T>::convertirTempsEnCycles(double duree){
    switch (uniteDeTemps_) 
    {
        case Unite::SECS :
            nCycles_ = freqAvecPrescaler ; // nombre de cycles equivalent a 1 seconde
            break;
        case Unite::MS :
            nCycles_ = freqAvecPrescaler/SECTOMS; // nombre de cycles equivalent a 1 ms
            break;            
        default:
            break;
    }  
    nCycles_ = int(nCycles_* duree); // nombre de cycles equivalent a X secondes / ms (max ~8 secondes / ~8000 ms)
}


template class TimerCTC<uint16_t> ; 
template class TimerCTC<uint8_t> ; 
