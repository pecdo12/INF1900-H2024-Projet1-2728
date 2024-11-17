#include "timer.hpp"

TimerCTC8Bits::TimerCTC8Bits(NumeroTimer numeroTimer, Unite uniteDeTemps):
TimerCTC<uint8_t>(numeroTimer, uniteDeTemps)  
{
  
    switch (numeroTimer_)
    {
        case NumeroTimer::TIMER0:
            compteur_ = &TCNT0;
            valeurComparaison_ = &OCR0A;
            registreControleA_ = &TCCR0A;
            registreControleB_ = &TCCR0B;
            registreInterruption_ = &TIMSK0;
            break;
        case NumeroTimer::TIMER2:
            compteur_ = &TCNT2;
            valeurComparaison_ = &OCR2A;
            registreControleA_ = &TCCR2A;
            registreControleB_ = &TCCR2B;
            registreInterruption_ = &TIMSK2;
            break;
        default:
            break;
    }
}


void TimerCTC8Bits::initialiserTimer(void){
    cli();
    *compteur_ = cycleDepart_ ; 
    *valeurComparaison_ = nCycles_ ;
    // choix des modes d'operation 
    *registreControleA_ |= (1<<WGM01) ;
    if(numeroTimer_ == NumeroTimer::TIMER0) {*registreControleB_ |= ( (1<<CS02)  | (1 << CS00) );}
    else {*registreControleB_ |= ( (1<<CS22) | (1 << CS21) | (1 << CS20) );}
    // configuration de l'interruption
    *registreInterruption_ |= (1 <<OCIE0A) ;
    sei();
}

