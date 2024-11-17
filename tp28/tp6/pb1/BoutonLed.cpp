
/*
 * Nom: Sebastian Cristescu ; Mohamed Yassir Merzouki
 * Groupe 2 ; Equipe 28
 * TP6 PB1
 * Description: Programme qui attend que le bouton soit appuye avant de partir un compteur. Si celui-ci compte jusqu'a 120 secondes ou le bouton est relache, la LED clignote en vert pendant 2 secondes. Ensuite, la LED clignote en rouge pendant la moitie du temps que le compteur a compte. 
 * Finalement, la LED reste allumee en vert pendant 1 secondes avant de revenir a l'etat initial.
 * Identifications matérielles : Les deux premieres broches du port B sont un output pour le LED et les deux premieres broches du port D sont un input pour le bouton poussoir.
 */



#include <avr/io.h> 
#define F_CPU 8000000
#include <util/delay.h>
#include <avr/interrupt.h>


const uint8_t LedOff = PB0;

const uint8_t EntreeBouton = PD2;
const uint8_t SortieLed = PD3;

volatile uint8_t compteur = 0;
volatile uint8_t gBoutonPoussoir;

void allumerRouge(){
    PORTB |= (1 << PB0);
    PORTB &= ~(1 << PB1);
}

void allumerVert(){
    PORTB |= (1 << PB1);
    PORTB &= ~(1 << PB0);
}

ISR (TIMER1_COMPA_vect)
{
    compteur++;
}

ISR(INT0_vect)
{    
    _delay_ms(30);
    gBoutonPoussoir = !gBoutonPoussoir;

    EIFR |= (1 << INTF0);
}

void partirMinuterie () {

    cli();

    TCNT1 = 0  ; 
    OCR1A = 781;
    
    TCCR1B |= (1 << CS12) | (1 << CS10) | (1 << WGM12);
    TCCR1A &= ~(1 << WGM11) & ~(1 << WGM10);

    TCCR1C = 0;

    TIMSK1 |= (1 << OCIE1A);

    sei();
}


void initialisation() {

    cli();
    gBoutonPoussoir = 0;

    DDRB |= SortieLed; 
    DDRD |= EntreeBouton;

    EIMSK |= (1 << INT0);
    EICRA |= (1 << ISC00);

    sei();

}

int main()
{
    initialisation();

    enum class state {INIT , PARTIRCOMPTEUR , ATTENDRECOMPTEUR , CLIGNOTERVERT , CLIGNOTERROUGE, TERMINER};

    state pres_state = state::INIT;


    while(true){

        switch(pres_state) {

            case state::INIT:
            {
                if(gBoutonPoussoir != 0){
                    pres_state = state::PARTIRCOMPTEUR;
                    break;
                }
                break;
            }

            case state::PARTIRCOMPTEUR:
            {
                compteur = 0;
                partirMinuterie();
                pres_state = state::ATTENDRECOMPTEUR;
                break;
                
            }

            case state::ATTENDRECOMPTEUR:
            {
                if(compteur >= 120 || gBoutonPoussoir == 0)
                {
                    pres_state = state::CLIGNOTERVERT;
                    break;
                }
                break;
            }

            case state::CLIGNOTERVERT:
            {
                cli();

                for(int i = 0; i < 10; i++)
                {
                    allumerVert();
                    _delay_ms(25);
                    PORTB &= LedOff;
                    _delay_ms(25);
                }

                _delay_ms(2000);
                pres_state = state::CLIGNOTERROUGE;
                break;
            }

            case state::CLIGNOTERROUGE:
            {
                for(int i = 0; i < (compteur/2); i++)
                {
                    allumerRouge();
                    _delay_ms(250);
                    PORTB &= LedOff;
                    _delay_ms(250);
                }

                pres_state = state::TERMINER;
                break;
            }

            case state::TERMINER:
            {
                allumerVert();
                _delay_ms(1000);
                PORTB &= LedOff;

                sei();
                gBoutonPoussoir = 0;
                pres_state = state::INIT;
                break;
            }

        }
    }

    return 0; 
}
