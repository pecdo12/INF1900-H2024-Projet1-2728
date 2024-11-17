/*Configuration des PORTS*/

// A1 A0 : sortie - DEL
// D2 :    entree - bouton poussoir

#define F_CPU 8000000UL // 8 MHz

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/*Prototypes*/

// interruption
void initialization(void);

// couleur de la DEL
void allumerLedRouge();
void allumerLedAmbre();
void allumerLedVerte();
void eteindreLed();

//etat
void passerEtatPresse( ) ; 
void passerEtatRelache( ) ; 


// Bouton poussoir
bool estBoutonAppuye();


// Etats
enum class Etat
{
    ROUGE_RELACHE,
    AMBRE_PRESSE,
    VERT_RELACHE,
    ROUGE_PRESSE,
    ETEINT_RELACHE,
    VERT_PRESSE
};

// variables globales
volatile Etat gEtatPresent = Etat::ROUGE_RELACHE; 

int main()
{
    initialization() ; 
    while (true)
    {
        switch (gEtatPresent)
        {
            case Etat::ROUGE_RELACHE:
                allumerLedRouge();
                break;
            case Etat::AMBRE_PRESSE:
                allumerLedAmbre();
                break;
            case Etat::VERT_RELACHE:
                allumerLedVerte();
                break;
            case Etat::ROUGE_PRESSE:
                allumerLedRouge();
                break;
            case Etat::ETEINT_RELACHE:
                eteindreLed();
                break;
            case Etat::VERT_PRESSE:
                allumerLedVerte();
                break;                                                                                
            default:
                break;
            }
    }
}

void initialization(void)
{

    cli (); // desactiver toutes les interruptions

    // Configuration Entree / Sortie des ports
    DDRA |= ((1 << PA1 )| (1 << PA0));
    DDRD &= ~(1 << PD2);

    // Activer les interruptions dans les PINS specifiques
    EIMSK |= (1 << INT0);

    // Configurer les condtions d'interruption pour chaque PIN (clock)

    EICRA |= (1 << ISC00 );
    EICRA &= ~(1<< ISC01) ;

    sei (); // Activer toutes les interruptions
}

const int8_t DELAI_DEBOUNCE {30} ; 

ISR(INT0_vect)
{
    _delay_ms(DELAI_DEBOUNCE) ;     
    if (estBoutonAppuye())
        passerEtatPresse() ; 
    else
        passerEtatRelache() ;

    // Clear le interrupt flag de D2
    EIFR |= (1 << INTF0);
}

// couleur de la DEL

void eteindreLed()
{
    PORTA |= ((1 << PA1 )| (1 << PA0));
}

void allumerLedRouge()
{
    PORTA |= (1 << PA1);
    PORTA &= ~(1 << PA0);
}

void allumerLedVerte()
{
    PORTA |= (1 << PA0);
    PORTA &= ~(1 << PA1);
}

const int8_t DELAI_COULEUR_AMBRE{10};

void allumerLedAmbre()
{
    allumerLedRouge();
    _delay_ms(DELAI_COULEUR_AMBRE);
    allumerLedVerte();
    _delay_ms(DELAI_COULEUR_AMBRE);
}

// Bouton


bool estBoutonAppuye(){
    bool estPresse = PIND & (1 << PIND2) ; 
    if (estPresse)
        return true ; 
    return false ;
}

// Etats

// Passer d'un etat presse a un etat relache
void passerEtatPresse(){ 
    switch (gEtatPresent)
    {
        case Etat::ROUGE_RELACHE:
            gEtatPresent = Etat::AMBRE_PRESSE ; 
            break;
        case Etat::VERT_RELACHE:
            gEtatPresent = Etat::ROUGE_PRESSE; 
            break;
        case Etat::ETEINT_RELACHE:
            gEtatPresent = Etat::VERT_PRESSE ; 
            break;
        default:
            break;
        }    

}

// Passer d'un etat relache a un etat presse

void passerEtatRelache() {
    switch (gEtatPresent)
    {
        case Etat::AMBRE_PRESSE:
            gEtatPresent = Etat::VERT_RELACHE ; 
            break;
        case Etat::ROUGE_PRESSE:
            gEtatPresent = Etat::ETEINT_RELACHE; 
            break;
        case Etat::VERT_PRESSE:
            gEtatPresent = Etat::ROUGE_RELACHE ; 
            break;
        default:
            break;
        }      
}
