/*Configuration des PORTS*/

// A1 A0 : sortie - DEL
// D2 : entree - bouton poussoir


#define F_CPU 8000000UL // 8 MHz

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/*Prototypes*/

// interruptions
void initializeButtonInterruption();
void initializeTimerInterruption() ; 


// couleur de la DEL
void allumerLedRouge();
void allumerLedAmbre();
void allumerLedVerte();
void eteindreLed();

// Bouton poussoir
bool estBoutonAppuye();

// Temps
void AttendreSecondes(const int) ; 


/*Declaration constantes et variables*/

// variables globales
volatile bool gEstAppuyeDansIntervalle = false ; 
volatile int gTempsActuel = 0 ; 

int main()
{

    // Configuration Entree / Sortie des ports
    DDRA |= ((1 << PA1 )| (1 << PA0));
    DDRD &= ~(1 << PD2);

    // Programme principale

    // Phase 1 : attente

    const int duree10Secondes = 3 ; 
    AttendreSecondes(duree10Secondes) ; 
    allumerLedRouge() ; 
    const int duree100ms = 100 ; 
    _delay_ms(duree100ms) ; 
    eteindreLed();

    // Phase 2 : epreuve reflexe

    initializeButtonInterruption() ; 
    initializeTimerInterruption() ; 


    const int dureeLimite = 3 ; // en secondes

    do {
        // Sauvegarder les etats dans ISR
    }while(gTempsActuel < dureeLimite && !gEstAppuyeDansIntervalle);

    cli (); // Suspendre les interruptions, on ne les utilise plus

    // Phase 3 : Verification

    if (gEstAppuyeDansIntervalle)
        allumerLedVerte() ; 
    else // non appuye pendant l'intervalle
        allumerLedRouge() ;  

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


// Minuterie 


ISR(TIMER1_COMPA_vect)
{
    ++gTempsActuel ; // ajouter 1 seconde
}

// constantes
const unsigned long int freqCPU = 8'000'000 ;
const unsigned  int prescaler = 1024 ;
const unsigned long int freqAvecPrescaler = freqCPU / prescaler ; // = frequence du timer

// TOP = freq pour reinitialiser a une seconde (rendre plus flexible apres)

void initializeTimerInterruption(void){
    // initialiser le timer
    TCNT1 = 0 ; // 
    // initialiser le TOP
    OCR1A = freqAvecPrescaler ; 

    // Configurer le clock source et le mode d'operation (output mode, wave generation)
    
    //1 - Configurer Output Mode
    int maskTCCR1A {0b0001100};
    TCCR1A &= maskTCCR1A; // On utilise aucun bit de ce registre

    //2 - Configure WGM et Clock source
    TCCR1B |= (1 << WGM12 |1 << CS12 | 1 << CS10) ; 
    TCCR1B &= ~(1 << WGM13 | 1 << CS11) ; 
    
    // Activation des interruptions lors du compare match event
    TIMSK1 |= (1 <<OCIE1A) ; 

}


// Bouton

ISR(INT0_vect)
{
    // Memoriser que le bouton a ete appuye
    gEstAppuyeDansIntervalle = true ; 
    // Clear le interrupt flag de D2
    EIFR |= (1 << INTF0);
}

void initializeButtonInterruption(void)
{

    cli (); // desactiver toutes les interruptions

    // Activer interruption sur le bouton poussoir (PIND2)
    EIMSK |= (1 << INT0);

    // Configurer les condtions d'interruption pour chaque PIN (clock)
    EICRA |= (1 << ISC00 ); 
    EICRA &= ~(1<< ISC01) ;

    sei (); // Activer toutes les interruptions
}

const int8_t DELAI_DEBOUNCE {30} ; 
bool estBoutonAppuye(){
    _delay_ms(DELAI_DEBOUNCE) ;     
    bool estPresse = PIND & (1 << PIND2) ; 
    if (estPresse)
        return true ; 
    return false ;
}


// Temps 

const int DELAI_1SEC {1000} ; // en ms 
void AttendreSecondes(const int dureeSecondes){ 
    for (int compteurSecondes = 0 ; compteurSecondes < dureeSecondes ; ++compteurSecondes)
        _delay_ms(DELAI_1SEC) ; 
}

