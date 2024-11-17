#include "interruptions_externes.hpp"

void InterruptionExterne::activerInterruptionExterne(void){
    cli() ;
    configurerPort() ;
    configurerConditionInterruption() ;
    sei();
}

void InterruptionExterne::desactiverInterruptionExterne(void){
    cli() ;
    switch (broche_)
    {
        case Broche::B2 :
            EIMSK &= ~(1 << INT2);
            break;
        case Broche::D2 :
            EIMSK &= ~(1 << INT0);
            break;
        case Broche::D3 :
            EIMSK &= ~(1 << INT1);
            break;        
        default:
            break;
    }
    sei();
}

// on peut generer des interruptions logiciels si on met DDRX en sortie
void InterruptionExterne::configurerPort(void){
    switch (broche_)
    {
        case Broche::B2 :
            DDRB &= ~(1 << DDB2);
            EIMSK |= (1 << INT2);
            break;
        case Broche::D2 :
            DDRD &= ~(1 << DDD2);
            EIMSK |= (1 << INT0);
            break;
        case Broche::D3 :
            DDRD &= ~(1 << DDD3);
            EIMSK |= (1 << INT1);
            break;        
        default:
            break;
    }
} 

void InterruptionExterne::configurerConditionInterruption(void){
    switch (conditionClock_)
    {
        case ConditionInterruption::BAS_NIVEAU :
            EICRA &= ~((1 << ISC10) | (1 << ISC11)) ; 
            break;
        case ConditionInterruption::FRONT :
            EICRA |= (1 << ISC10) ; 
            EICRA &= ~(1 << ISC11) ; 
            break;
        case ConditionInterruption::DESCENDANT :
            EICRA &= ~(1 << ISC10) ; 
            EICRA |= (1 << ISC11) ; 
            break; 
        case ConditionInterruption::MONTANT :
            EICRA |= ~((1 << ISC10) | (1 << ISC11)) ; 
            break;        
        default:
            break;
    }
}


