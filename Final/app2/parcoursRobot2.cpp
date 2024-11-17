/*
 * Nom: Mohamed Omar Zedek / Souhayl Sejjari / Sebastian Cristescu / Mohamed Yassir Merzouki
 * Groupe 02
 * Equipe 2728
 * Projet Robot 1
 * Description: Voir readme
 * 
 * Identifications matérielles : Voir readme
 * */

#include "robot2.hpp"

enum class EtatsRobot2 {Parcours, Attente, EnvoiIR, EnvoiRS232};
int main() {
    Robot2 robot = Robot2();
    LED led(PortLED::D, 7, 5);
    Buzzer buzzer(PinBuzzer::D28, 9, 0);
    buzzer.stop();
    EtatsRobot2 etatRobot = EtatsRobot2::Parcours;
    
    Cadre cadrePrincipale(nLignesCadrePrincipale,nColonnesCadrePrincipale) ;
    cadrePrincipale.dessinerCadre(true) ;
    CadreRobot1 cadreRobot1(nLignesCadreRobot1,nColonnesCadreRobot1,decalageLignesRobot1,decalageColonnesRobot1);
    cadreRobot1.dessinerBoite() ;
    CadreRobot2 cadreRobot2(nLignesCadreRobot2,nColonnesCadreRobot2,decalageLignesRobot2,decalageColonnesRobot2);
    cadreRobot2.dessinerCircuit() ;
    
    while (1){
        switch (etatRobot){
            case EtatsRobot2::Parcours :
                robot.performerParcours();
                robot.gererDonnees();
                robot.ajouterBandesCadre(cadreRobot2);
                etatRobot = EtatsRobot2::Attente;
                break;
            case EtatsRobot2::Attente :
                led.allumerVert();
                if ((PINC & (1 << PC7)) == 0){
                    _delay_ms(25); // debounce
                    if ((PINC & (1 << PC7)) == 0){
                        etatRobot = EtatsRobot2::EnvoiRS232;
                    }
                }
                else if (PIND & (1 << PD2)){
                    etatRobot = EtatsRobot2::EnvoiIR;
                }
                break;
            
            case EtatsRobot2::EnvoiIR :
                led.eteindre();
                for(uint8_t i = 0; i < 3; i++){
                    buzzer.setFrequence(800);
                    _delay_ms(50);
                    buzzer.stop();
                    _delay_ms(50);
                }
                robot.emettreDonnees();
                etatRobot = EtatsRobot2::Attente;
                break;
            case EtatsRobot2::EnvoiRS232 :
                led.clignoter5HZ();
                cadrePrincipale.afficherDessin(false);
                etatRobot = EtatsRobot2::Attente;
                led.arretClignoter();
                break;
            default:
                break;
        }
    }
    
}