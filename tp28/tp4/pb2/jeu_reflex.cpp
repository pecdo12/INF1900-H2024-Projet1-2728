
#include <avr/io.h> 
#define F_CPU 8000000
#include <util/delay.h>
#include <avr/interrupt.h>


const uint8_t LedRed = (1 << PA0);
const uint8_t LedGreen = (1 << PA1);
const uint8_t LedOff = PA0;

const uint8_t EntreeBouton = PD2;
const uint8_t SortieLed = PD3;

volatile state pres_state = state::INIT;
volatile state prochainEtat;

volatile uint8_t gMinuterieExpiree;
volatile uint8_t gBoutonPoussoir;

//TCCR1B |= (1 << WGM12);

void allumerRouge(){
    PORTA |= (1 << PA0);
    PORTA &= ~(1 << PA1);
}

void allumerVert(){
    PORTA |= (1 << PA1);
    PORTA &= ~(1 << PA0);
}


ISR (TIMER1_COMPA_vect) // Minuterie
{

    gMinuterieExpiree = 1;
}

ISR(INT0_vect) // Bouton appuyer
{

    _delay_ms(30);

    gBoutonPoussoir = 1;
}

void partirMinuterie ( uint16_t duree ) {

    cli();

    gMinuterieExpiree = 0;

    // mode CTC du timer 1 avec horloge divisée par 1024

    // interruption après la durée spécifiée
    TCNT1 = 0  ; //Counts up every tick so basically the speed of the cpu (8MHz). So we have to use a prescaler to come reduce the value to something more granular.

    OCR1A = duree;  // TCNT1 compare register

    // Sets the prescaler to 1024, this basically sets the incrementing value of TCNT1 to CPU/prescaler. In our case we need to set CSn2 and CSn0 to 1 to have a prescaler of 1024
    TCCR1B |= (1 << CS12) | (1 << CS10) | (1 << WGM12);

    // Set the timer to compare TCNT1 and OCR1A
    TCCR1A &= ~(1 << WGM11) & ~(1 << WGM10);

    // Timer control register, these bits must be set to zero when TCCRnA is written when operating in a PWM mode.
    TCCR1C = 0;

    // Timer/Counter1 Interrupt Mask Register: basically enables the compare with TCCR1A
    TIMSK1 |= (1 << OCIE1A);

    sei();
}



void initialisation() {

    cli(); // Bloquer toutes les interruptionsEIMSK

    EIMSK |= (1 << INT0); // Permettre les interruptions externes
    EICRA |= (1 << ISC00);


    sei(); // Permet de recevoir des interruptions a nouveau

}

uint16_t convertSecondsTimer(uint16_t s, uint16_t prescaler=1)
{
    double periode_prescaled = F_CPU/prescaler;
    return s * periode_prescaled;
}

int main()
{
    DDRA = SortieLed; 
    DDRD = EntreeBouton;

    initialisation();

    _delay_ms(3000);

    //allumer couleur rouge 1/10
    allumerRouge();
    _delay_ms(100);
    PORTA &= LedOff;

    partirMinuterie(convertSecondsTimer(3, 1024));


    

    do {

        // attendre qu'une des deux variables soit modifiée

        // par une ou l'autre des interruptions.

    } while ( gMinuterieExpiree == 0 && gBoutonPoussoir == 0 );


    // Une interruption s'est produite. Arrêter toute

    // forme d'interruption. Une seule réponse suffit.

    cli ();

    if (gBoutonPoussoir == 1)
        allumerVert();
    else
        allumerRouge();

    // // Verifier la réponse

    return 0; 
}