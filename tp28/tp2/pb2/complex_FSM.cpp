
/*
 * Nom: Sebastian Cristescu ; Mohamed Yassir Merzouki
 * Groupe 2 ; Equipe 28
 * TP2 PB2
 * Description: Quand la carte mère démarre, la DEL libre doit s'allumer en rouge. 
 *              Si le bouton-poussoir libre pour usage général est pesé, la DEL affiche la couleur ambre. 
 *              Quand le bouton-poussoir est relâché, la DEL devient verte. Si le bouton est de nouveau pesé, la DEL prend la couleur rouge encore. 
 *              Quand il est relâché, la DEL s'éteint. Si le bouton est de nouveau pesé, la DEL affiche la couleur verte. 
 *              Quand il est relâché, la DEL tourne au rouge ce qui fait que la carte mère est de retour à son état initial et tout peut recommencer.
 * Identifications matérielles : La broche 2 dans le port A est un output et la broche 4 dans le port D est un input.
 */


//  | État présent | PIND2 | État suivant | DEL    |
//  |--------------|-------|--------------|--------|
//  | INIT         | 0     | INIT         | Rouge  |
//  | INIT         | 1     | APPUYER1     | Rouge  |
//  | APPUYER1     | 0     | RELACHER1    | Ambre  |
//  | APPUYER1     | 1     | APPUYER1     | Ambre  |
//  | RELACHER1    | 0     | RELACHER1    | Vert   |
//  | RELACHER1    | 1     | APPUYER2     | Vert   |
//  | APPUYER2     | 0     | RELACHER2    | Rouge  |
//  | APPUYER2     | 1     | APPUYER2     | Rouge  |
//  | RELACHER2    | 0     | RELACHER2    | Eteint |
//  | RELACHER2    | 1     | APPUYER3     | Eteint |
//  | APPUYER3     | 0     | INIT         | Vert   |
//  | APPUYER3     | 1     | APPUYER3     | Vert   |


#include <avr/io.h> 
#define F_CPU 8000000
#include <util/delay.h>


const uint8_t LedRed = (1 << PA0);
const uint8_t LedGreen = (1 << PA1);
const uint8_t LedOff = PA0;

const double DelayLed200Ms = 200;
const double DelayLed5Ms = 5;
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

void allumerRouge(){
    PORTA |= (1 << PA0);
    PORTA &= ~(1 << PA1);
}

void allumerVert(){
    PORTA |= (1 << PA1);
    PORTA &= ~(1 << PA0);
}

void afficherAmbre(){
    while(boutonAppuyer()) {
        allumerVert();
        _delay_ms(2*DelayLed5Ms);

        allumerRouge();
        _delay_ms(DelayLed5Ms);
    }
}



int main()
{

    DDRA = SortieLed; 
    DDRD = EntreeBouton;

    enum class state {INIT , APPUYER1 , RELACHER1 , APPUYER2 , RELACHER2 , APPUYER3};

    state pres_state = state::INIT;

    while (true){

       switch(pres_state) {

        case state::INIT: // Commence DEL est rouge
        {
            
            allumerRouge();

            if (boutonAppuyer())
                pres_state = state::APPUYER1;
            break;
        }
        


        case state::APPUYER1: // Appuie alors DEL est ambre
        {        

            afficherAmbre();
            
            if (!boutonAppuyer())
                pres_state = state::RELACHER1;
            break;
        }



        case state::RELACHER1: // Relache alors DEL est vert
        {
            
            allumerVert();

            if (boutonAppuyer())
                pres_state = state::APPUYER2;
            break;
        }



        case state::APPUYER2: // Appuie alors DEL est rouge
        {
            
            allumerRouge();

            if (!boutonAppuyer())
                pres_state = state::RELACHER2;
            break;
        }

        case state::RELACHER2: // Relache alors DEL eteint
        {
            
            PORTA &= LedOff;
            
            if (boutonAppuyer())
                pres_state = state::APPUYER3;
            break;
        }

        case state::APPUYER3: // Appuie alors DEL est vert
        {
            
            allumerVert();

            if (!boutonAppuyer())
                pres_state = state::INIT;

            break;
        }
    
       }

    }

    return 0; 
}