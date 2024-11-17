

#include "exec.hpp"


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
    DEBUG_INIT();

    DDRA = SortieLed; 
    DDRD = EntreeBouton;

    enum class state {INIT , APPUYER1 , RELACHER1 , APPUYER2 , RELACHER2 , APPUYER3};

    state pres_state = state::INIT;

    
    while (true){

       switch(pres_state) {

        case state::INIT: // Commence DEL est rouge
        {
            
            allumerRouge();

            if (boutonAppuyer()){
                pres_state = state::APPUYER1;
            }
                
            break;
        }
        


        case state::APPUYER1: // Appuie alors DEL est ambre
        {        

            afficherAmbre();
            
            if (!boutonAppuyer()){
                //DEBUG_PRINT(("Etat du prog: %d;\n", static_cast<int>(pres_state)));
                pres_state = state::RELACHER1;
                DEBUG_PRINT(("appuyer1\n"));
            }
            break;
        }



        case state::RELACHER1: // Relache alors DEL est vert
        {
            
            allumerVert();

            if (boutonAppuyer()){
                pres_state = state::APPUYER2;
                DEBUG_PRINT(("relacher1\n"));
            }
            break;
        }



        case state::APPUYER2: // Appuie alors DEL est rouge
        {
            
            allumerRouge();

            if (!boutonAppuyer()){
                pres_state = state::RELACHER2;
                DEBUG_PRINT(("appuyer2\n"));
            }
            break;
        }

        case state::RELACHER2: // Relache alors DEL eteint
        {
            
            PORTA &= LedOff;
            
            if (boutonAppuyer()){
                pres_state = state::APPUYER3;
                DEBUG_PRINT(("relacher2\n"));
            }
            break;
        }

        case state::APPUYER3: // Appuie alors DEL est vert
        {
            
            allumerVert();

            if (!boutonAppuyer()){
                pres_state = state::INIT;
                DEBUG_PRINT(("appuyer3\n"));
            }
            break;
        }
    
       }

    }

    return 0; 
}