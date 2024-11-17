
#include <avr/io.h> 
#define F_CPU 8000000
#include <util/delay.h>
#include <avr/interrupt.h>


const uint8_t LedRed = (1 << PA0);
const uint8_t LedGreen = (1 << PA1);
const uint8_t LedOff = PA0;

const double DelayLed200Ms = 200;
const double DelayLed5Ms = 5;
const int DelayMesure10Ms = 10;

const uint8_t EntreeBouton = PD2;
const uint8_t SortieLed = PD3;

enum class state {INIT, APPUYER1, RELACHER1, APPUYER2, RELACHER2, APPUYER3};

volatile state pres_state = state::INIT;

volatile state prochainEtat;

//TCCR1B |= (1 << WGM12);

void allumerRouge(){
    PORTA |= (1 << PA0);
    PORTA &= ~(1 << PA1);
}

void allumerVert(){
    PORTA |= (1 << PA1);
    PORTA &= ~(1 << PA0);
}

void afficherAmbre(){
    allumerVert();
    _delay_ms(2*DelayLed5Ms);

    allumerRouge();
    _delay_ms(DelayLed5Ms);
}


ISR (INT0_vect)
{
    _delay_ms(30);

    pres_state = prochainEtat;

    EIFR |= (1 << INTF0); // Mettre les interruptions uns apres lautre
}

void initialisation ( void ) {

    cli (); // Bloquer toutes les interruptions

    EIMSK |= (1 << INT0); // Permettre les interruptions externes

    EICRA |= (1 << ISC00); // Modifier commment on reagit aux interruptions. Document de atmel p.67

    sei (); // Permet de recevoir des interruptions a nouveau

}

int main()
{

    initialisation();

    DDRA = SortieLed; 
    DDRD = EntreeBouton;

    while (true){

       switch(pres_state) {

        case state::INIT: // Commence DEL est rouge
        {
            allumerRouge();
            prochainEtat = state::APPUYER1;
            break;
        }
        

        case state::APPUYER1: // Appuie alors DEL est ambre
        {        

            afficherAmbre();
            prochainEtat = state::RELACHER1;
            break;
        }


        case state::RELACHER1: // Relache alors DEL est vert
        {
            
            allumerVert();
            prochainEtat = state::APPUYER2;
            break;
        }


        case state::APPUYER2: // Appuie alors DEL est rouge
        {
            
            allumerRouge();
            prochainEtat = state::RELACHER2;
            break;
        }

        case state::RELACHER2: // Relache alors DEL eteint
        {
            
            PORTA &= LedOff;
            prochainEtat = state::APPUYER3;
            break;
        }

        case state::APPUYER3: // Appuie alors DEL est vert
        {
            
            allumerVert();
            prochainEtat = state::INIT;
            break;
        }
    
       }

    }

    return 0; 
}