#include "exec.hpp"

bool estBoutonAppuye() ;

enum class Test{TEST_LED , TEST_MOTRICITE, TEST_MODECTC_CHRONO , TEST_MODECTC_EVENEMENT, TEST_INTERRUPTION_EXTERNE, TEST_TIMER8BITS } ;

volatile Test gTestActuel ;
volatile int gTempsActuel = 0 ; 
volatile int gIncrementTemps = 1 ;
volatile bool gestEteint ;

int main(){
    DEBUG_INIT();

    gTestActuel = Test::TEST_MODECTC_CHRONO; // variable a changer selon le test que vous voulez effectuer


    // Testes de la classe LED :
    // La LED doit etre branchee sur les broches A0-A1 positif-negatif
    // la led s'allumera pendant 2 seconde en :
    // rouge puis ambre puis vert et s'eteindra enfin       
    LED maLed(PortLED::A, 1, 2);

    // Testes des classes PWM Moteur et Motricite
    // Les deux moteurs seront branches sur le PORTB
    // La construction des deux moteurs instancie deux PWM sur les broches 4 et 5 du PORTB a un pourcentage initial de 0%
    // Le Enable du moteur gauche doit etre branche sur la broche 4 et la Direction sur la broche 3
    Moteur moteurGauche(PortDirection::B, 3, PinPWM::B04);
    // Le Enable du moteur droit doit etre branche sur la broche 5 et la Direction sur la broche 6
    Moteur moteurDroit(PortDirection::B, 6, PinPWM::B05);
    // On met en relation les deux moteurs avec un objet Motricite
    Motricite maMotricite(moteurGauche, moteurDroit);



    // Demonstration classe timer 
    TimerCTC16Bits minuteur_16 (NumeroTimer::TIMER1 , Unite::SECS) ; 
    TimerCTC8Bits  minuteur_8 (NumeroTimer::TIMER2 , Unite::MS) ;
    int duree = 5 ; // en secondes 
    int intervalle = 2 ; 

    // Demonstration interruption externe
    InterruptionExterne boutonPoussoir (Broche::D2 , ConditionInterruption::FRONT) ;


    
    switch (gTestActuel)
        {
          
        case  Test::TEST_LED:
            // DEBUG_PRINT(("Test LED"));
            maLed.allumerRouge();
            _delay_ms(2000);
            maLed.allumerAmbre(2000);
            maLed.allumerVert();
            _delay_ms(2000);
            maLed.eteindre();
            _delay_ms(2000);        
            break;

        case  Test::TEST_MOTRICITE:
            // DEBUG_PRINT(("Test MOTRICITE")) ;
            // On fait avancer pendant 1 seconde le robot puis reculer pendant 1 seconde a une vitesse de 50%
            maMotricite.marcheAvant(50);
            _delay_ms(1000);
            maMotricite.marcheArriere(50);
            _delay_ms(1000);
            // On teste la rotation du robot avec la methode twerk avec des valeurs aleatoires
            maMotricite.twerk(10, 40);         
            maMotricite.arret();      
            break;
        case  Test::TEST_MODECTC_CHRONO:
            DEBUG_PRINT(("Test MODE CTC CHRONO"));
            /*TEST avec timer 16 bits */
            DEBUG_PRINT(("Test avec timer 16 bits"));

            gTempsActuel = 0 ; 
            minuteur_16.demarrerChronometre();
            maLed.allumerRouge(); 
            DEBUG_PRINT(("LED ALLUME ROUGE \n"));
            while(gTempsActuel < duree){
                // la LED reste allumee pendant 5 secondes
                DEBUG_PRINT(("BOUCLE WHILE"))   ;
            }    
            minuteur_16.arreterChronometre(); // le chronometre s'arrete, la variable volatile arrete de s'incrementer
            maLed.eteindre();
            DEBUG_PRINT(("LED ETEINTE"));
            duree =10 ;
            minuteur_16.reprendreChronometre(); // le chronometre reprend son compte a 5 secondes
            while(gTempsActuel < duree){
                // la LED reste eteinte pendant 5 secondes 

            }      
            /*TEST avec timer 8 bits */
            DEBUG_PRINT(("Test avec timer 8 bits"));
            minuteur_16.arreterChronometre();

            duree = 3000 ; // duree en MS car on utilise le timer 2
            gTempsActuel = 0 ;
            minuteur_8.demarrerChronometre() ;

            maLed.allumerVert();
            // DEBUG_PRINT(("LED ALLUME VERT"));

            while(gTempsActuel < duree){
                // la LED reste allumee verte pendant 3 secondes
            }         
            // minuteur_8.arreterChronometre();
            maLed.eteindre();
            DEBUG_PRINT(("LED ETEINTE"));

            break;            
        case  Test::TEST_MODECTC_EVENEMENT:
            DEBUG_PRINT(("Test MODE CTC EVENEMENT chaque 2 secondes"));
            gestEteint = true ; 
            gIncrementTemps= intervalle;
            gTempsActuel = 0 ; // remettre le comptage du temps a 0 
            minuteur_16.lancerEvenementIntervalle(intervalle) ; 
            duree =10 ;

            // Pendant 10 secondes : la led alterne toutes les 2 secondes entre un etat eteint et allume rouge.
            while(gTempsActuel < duree){
                if (gestEteint){
                    // DEBUG_PRINT(("LED ETEINTE"));
                    maLed.eteindre();
                }
                else{
                    // DEBUG_PRINT(("LED ALLUMEE"));
                    maLed.allumerRouge();
                }
            }
            break;

        case  Test::TEST_INTERRUPTION_EXTERNE:
            DEBUG_PRINT(("Test INTERRUPTION EXTERNE avec le bouton"));
            boutonPoussoir.activerInterruptionExterne() ; 

            maLed.eteindre();
            gestEteint = true ; 

            while(gestEteint){
                // reste eteint tant que le bouton n'est pas presse 
            }

            maLed.allumerRouge() ; // s'allume des que le bouton est presse
            boutonPoussoir.desactiverInterruptionExterne();

            break;            
        default:
            break;
        }

    return 0;
}

bool estBoutonAppuye(){
    _delay_ms(30); // anti-rebond
    if (PIND & (1 << PD2)){
        return true ; 
    }
    return false;
}

ISR(TIMER1_COMPA_vect){
    if (gTestActuel == Test::TEST_MODECTC_CHRONO){
        // demarrerChrono , arreterChrono , reprendreChrono
        gTempsActuel += gIncrementTemps ; 
    }
    else if (gTestActuel == Test::TEST_MODECTC_EVENEMENT)
        // lancerEvenmementIntervalle
        gTempsActuel += gIncrementTemps ; 
        if (gestEteint){
            gestEteint = false ;
        }
        else{
            gestEteint = true ;
        }
}

ISR(TIMER2_COMPA_vect){
    ++gTempsActuel ; 
}


ISR(INT0_vect){
    DEBUG_PRINT(("Interruption from button pressed"))
    if(estBoutonAppuye()){
        gestEteint = false;
    }
    else{
        gestEteint = true ;
    }
}