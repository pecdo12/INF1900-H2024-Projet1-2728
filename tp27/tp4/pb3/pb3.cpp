/*Configuration des PORTS*/

// A1 A0 : sortie - DEL
// D2 : entree - bouton poussoir

#define F_CPU 8000000UL // 8 MHz

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/*Prototypes*/
void Lancer2RouesMemeVitesse(double);

// Temps
void AttendreSecondes(const int);

/*Declaration constantes et variables*/

int main()
{

    // Configuration Entree / Sortie des ports
    DDRA |= (1 << PA1 | 1 << PA0);
    DDRD |= (1 << PD4 | 1 << PD5 | 1 << PD6 | 1 << PD7);

    // Programme principale
    double listVitesses[] = {0, 0.25, 0.5, 0.75, 1};
    const int delai2Secondes = 2;
    while (true)
    {
        for (double vitesse : listVitesses)
        {
            Lancer2RouesMemeVitesse(vitesse);
            AttendreSecondes(delai2Secondes);
        }
    }
}

// Temps

const int DELAI_1SEC{1000}; // en ms

void AttendreSecondes(const int dureeSecondes)
{
    for (int compteurSecondes = 0; compteurSecondes < dureeSecondes; ++compteurSecondes)
        _delay_ms(DELAI_1SEC);
}

const int TOP = 255;
int calculerValeurComparaison(double pourcentageVitesse)
{
    return int(pourcentageVitesse * TOP);
}

// Mode PWM phase correct 8 bit - COM = clear on upcounting - Prescaler 1024
void Lancer2RouesMemeVitesse(double pourcentageVitesse)
{

    int valeurDeComparaison = calculerValeurComparaison(pourcentageVitesse);
    TCNT1 = 0; // initialiser le timer a 0
    OCR1A = valeurDeComparaison;
    OCR1B = valeurDeComparaison; // assigner la valeur de comparaison    // Configuration des registres de controle
    TCCR1A |= (1 << COM1A1 | 1 << COM1B1 | 1 << WGM10);
    TCCR1B |= (1 << CS12 | 1 << CS10);
    TCCR1C = 0;
}
