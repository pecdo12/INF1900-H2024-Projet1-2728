
#include <avr/io.h> 
#define F_CPU 8000000
#include <util/delay.h>
#include <avr/interrupt.h>


const uint8_t LedRed = (1 << PA0);
const uint8_t LedGreen = (1 << PA1);
const uint8_t LedOff = PA0;

const uint8_t SortieLed = PD3;


volatile uint8_t gMinuterieExpiree;
volatile uint8_t gBoutonPoussoir;

void motorLOff(){
    PORTD |= (1 << PD1); 
    PORTD &= ~(1 << PD1);
}

void motorROff(){
    PORTD |= (1 << PD3); 
    PORTD &= ~(1 << PD3);
}

void allumerRouge(){
    PORTA |= (1 << PA0);
    PORTA &= ~(1 << PA1);
}

void allumerVert(){
    PORTA |= (1 << PA1);
    PORTA &= ~(1 << PA0);
}




uint16_t convertSecondsTimer(uint16_t s, uint16_t prescaler=1)
{
    double periode_prescaled = F_CPU/prescaler;
    return s * periode_prescaled;
}


void ajustementPwm (uint8_t cycle) {

    // mise à un des sorties OC1A et OC1B sur comparaison
    // réussie en mode PWM 8 bits, phase correcte
    // et valeur de TOP fixe à 0xFF (mode #1 de la table 16-5
    // page 130 de la description technique du ATmega324PA)


    OCR1A = cycle ; // dutyCycle * TOP

    OCR1B = cycle ;

    // division d'horloge par 8 - implique une fréquence de PWM fixe

    TCCR1A |= (1 << WGM10) | (1 << COM1A1) | (1 << COM1B1);
    TCCR1A &= ~(1 << WGM11);

    TCCR1B |= (1 << CS11) ;
    TCCR1B &= ~(1 << WGM12) & ~(1 << WGM13);

    TCCR1C = 0;

}


void initialisation() {

    DDRA |= (1<< PA1) | (1<< PA0);
    DDRD = 0xff;


}


int main()
{
    initialisation();

    while (1)
    {
        ajustementPwm(64);
        allumerRouge();
        _delay_ms(4000);

    
        ajustementPwm(128);
        allumerVert();
        _delay_ms(4000);

        ajustementPwm(192);
        PORTA &= LedOff;
        _delay_ms(4000);

        ajustementPwm(255);
        allumerVert();
        _delay_ms(4000);    
    }
    

    return 0; 
}