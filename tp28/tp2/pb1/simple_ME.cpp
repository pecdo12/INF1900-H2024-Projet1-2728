
/*
 * Nom: Sebastian Cristescu ; Mohamed Yassir Merzouki
 * Groupe 2 ; Equipe 28
 * TP2 PB1
 * Description: Programme qui attend que le bouton soit appuye 3 fois. Ensuite, le programme allume le LED d'une couleur rouge pour 2 seconds. 
 *              Puis il retourne a son etat initiale.
 * Identifications matérielles : La broche 2 dans le port A est un output et la broche 4 dans le port D est un input.
 */



//  | État présent      | PIND2 | État suivant      | DEL   |
//  |-------------------|-------|-------------------|-------|
//  | INIT              | 0     | INIT              | OFF   |
//  | INIT              | 1     | CLICK1            | OFF   |
//  | CLICK1            | 0     | CLICK1            | OFF   |
//  | CLICK1            | 1     | CLICK2            | OFF   |
//  | CLICK2            | 0     | CLICK2            | OFF   |
//  | CLICK2            | 1     | ALLUMAGE_DEL_2SEC | OFF   |
//  | ALLUMAGE_DEL_2SEC | X     | INIT              | Rouge |


#include <avr/io.h> 
#define F_CPU 8000000
#include <util/delay.h>



const uint8_t LedVert = (1 << PA1);
const uint8_t LedOff = PA0;

const double DelayLed2S = 2000;
const int DelayMesure10Ms = 10;

const uint8_t EntreeBouton = PD2;
const uint8_t SortieLed = PD3;


bool boutonAppuyer(){
    if ( PIND & (1 << PD2) ){
        _delay_ms(DelayMesure10Ms);
        if ( PIND & (1 << PD2) )
            return true;
    }
    return false;
}


int main()
{

    DDRA = SortieLed; 
    DDRD = EntreeBouton;


    enum class state {INIT,  CLICK1, RELACHER1, CLICK2, RELACHER2, CLICK3, ALLUMAGE_DEL_2SEC};

    state pres_state = state::INIT;

    while (true){

       switch(pres_state) {

        case state::INIT:

            if (boutonAppuyer())
                pres_state = state::CLICK1;            
            break;


        case state::CLICK1:

            if (!boutonAppuyer())
                pres_state = state::RELACHER1;
            break;        

        case state::RELACHER1:

            if (boutonAppuyer())
                pres_state = state::CLICK2   ;
            break;     


        case state::CLICK2:

            if (!boutonAppuyer())
                pres_state = state::RELACHER2;
           
            break;
        
        case state::RELACHER2:

            if (boutonAppuyer())
                pres_state = state::CLICK3;
            break;

        case state::CLICK3:

            if (!boutonAppuyer())
                pres_state = state::ALLUMAGE_DEL_2SEC;
            break;


        case state::ALLUMAGE_DEL_2SEC:

            PORTA |= LedVert;
            _delay_ms(DelayLed2S);
            PORTA &= LedOff;
            pres_state = state::INIT;
            break;

        }

    }

    return 0; 
}