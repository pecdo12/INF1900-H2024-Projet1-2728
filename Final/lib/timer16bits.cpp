#include "timer.hpp"

TimerCTC16Bits::TimerCTC16Bits(NumeroTimer numeroTimer, Unite uniteDeTemps):
TimerCTC<uint16_t>(numeroTimer, uniteDeTemps) 
{
        uniteDeTemps_ = uniteDeTemps;
        numeroTimer_ = numeroTimer ;   
        cycleDepart_ = 0 ; 
        nCycles_ = 0 ;    
        
        compteur_ = &TCNT1;
        valeurComparaison_ = &OCR1A;
        registreControleA_ = &TCCR1A;
        registreControleB_ = &TCCR1B;
        registreInterruption_ = &TIMSK1;   
}


void TimerCTC16Bits::initialiserTimer(void)
{
    cli();
    *compteur_ = cycleDepart_ ; 
    *valeurComparaison_ = nCycles_ ;
    // choix des modes d'operation 
    *registreControleB_ |= (1 << WGM12 |1 << CS12 | 1 << CS10) ; 
    *registreControleB_ &= ~(1 << WGM13 | 1 << CS11) ; 
    // configuration de l'interruption
    *registreInterruption_ |= (1 <<OCIE1A) ;
    sei();
}
