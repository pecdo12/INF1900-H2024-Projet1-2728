/*
 * Nom: Mohamed Omar Zedek / Souhayl Sejjari / Sebastian Cristescu / Mohamed Yassir Merzouki
 * Groupe 02
 * Equipe 2728
 * Projet Robot 1
 * Description: Ce programme permet de réaliser le parcours du robot 1. Il permet de positionner le robot au centre de la zone de départ, 
 * de trouver le trou et de recevoir les données de l'IR. Pour positionner le robot au centre de la zone de départ, le robot avance jusqu'au centre de
 * deux valeurs prédéterminées de 0x33 <-> 0x30. Lorsqu'il est placé au centre, il trouve le trou en tournant sur lui-même et en comptant le nombre de 
 * coups de moteurs qu'ils donnent. Cela (avec une prise de mesure sur la distance du mur au début) permet de déterminer la position de départ du robot.
 * Puis il rentre dans un état d'attente pour recevoir les données de l'IR. Lorsqu'il les reçoit, il les valide et les affiche sur le circuit.
 * 
 * Identifications matérielles : Les LED sont connectées aux broches 5 et 6 du port D, le bouton-poussoir est connecté à la broche 2 du port D.
 * Le buzzer est connecté à la broche 27 du port D. Les moteurs sont connectés aux broches 6 et 7 du port B. Les capteurs IR sont connectés aux broches 0 et 4 du port A.
*/



#include "parcoursRobot1.hpp"

int map(int value, int minInput, int maxInput, int minOutput, int maxOutput) {

    if (value < minInput) value = minInput;
    if (value > maxInput) value = maxInput;


    double percentage = (double)(value - minInput) / (maxInput - minInput);


    return minOutput + (percentage * (maxOutput - minOutput));
}

bool boutonAppuyer(){
    if ( PIND & (1 << PD2) ){
        _delay_ms(10);
        if ( PIND & (1 << PD2) )
            return true;
    }
    return false;
}


uint16_t calculerDistanceTotale(uint8_t *tableau){
    uint16_t distance = 0;
    for (uint8_t i = 0; i < 5; i++){
        if (tableau[i] != 0 )
        {
            distance += tableau[i];
        }
    }
    return distance;
}

void attendreAppuie(){
    while (!boutonAppuyer()){} ;
}

int main() {

    
    DDRA |= fluxIR | donneIR;
    buzzer.stop();    

    cadrePrincipale.dessinerCadre(true) ;
    cadreRobot2.dessinerCircuit() ;


    enum class ETAT
    {
        Positionnement,
        ModeAttente,
        ReceptionIR,
        TransmissionRS232
    };

    ETAT etat = ETAT::Positionnement;

    led.allumerVert();

    attendreAppuie(); 

    led.eteindre() ;

    while (true)
    {
    switch (etat)
    {
    case ETAT::Positionnement:

        distanceDebut = robot1.moyenneDistance(); 
        robot1.positionnerCentre();
        nombreCycle = robot1.trouverTrou();

        if (nombreCycle > 30)
        {
            mappedValue = map(distanceDebut, 0x15, 0x70, 1, nLignesCadreRobot1 - 6);
            ligneDepart = mappedValue; 
            orientation = OrientationRobot::BAS ;

        }
        else
        {
            mappedValue = map(distanceDebut, 0x15, 0x60, 6, nLignesCadreRobot1 - 1); 
            ligneDepart = nLignesCadreRobot1 - mappedValue; 
            orientation = OrientationRobot::HAUT;
        }

        cadreRobot1.dessinerBoiteAvecRobot(ligneDepart, orientation) ;

    
    case ETAT::ModeAttente:

        led.allumerRouge();
        attendreAppuie() ;
        led.eteindre();

        buzzer.emettreSonPeriodique(SON_AIGU) ;

    case ETAT::ReceptionIR:

        led.eteindre();

        for(uint8_t i = 0; i < 18; i++){
            recupererDonnee(tableauBits);
            donneeRecue = valeurDonnee(tableauBits);
            crcRecu = crcDonnee(tableauBits);
            donneesValide &= verifierDonnee(donneeRecue, crcRecu);
            tableauReception[i] = donneeRecue;
        }   


        for(uint8_t i = 0; i < 5; i++){
            tableauParcours1[i] = tableauReception[i];
        }
        for(uint8_t i = 5; i < 9; i++){
            tableauBande1[i-5] = tableauReception[i]; 

        }
        for(uint8_t i = 9; i < 14; i++){
            tableauParcours2[i-9] = tableauReception[i];
        }
        for(uint8_t i = 14; i < 18; i++){
            tableauBande2[i-14] = tableauReception[i];
        }

        distanceTotale1 = calculerDistanceTotale(tableauParcours1);
        distanceTotale2 = calculerDistanceTotale(tableauParcours2);

        for (uint8_t i = 0; i < 4; i++){

            if (tableauBande1[i] != 0 )
            {
                dAct1 += (tableauParcours1[i]*50)/distanceTotale1;
                cadreRobot2.ajouterBandeVerticale(Bande::BANDE_SUPERIEURE, dAct1, tableauBande1[i]);
            }

            if (tableauBande2[i] != 0 )
            {
                dAct2 += (tableauParcours2[i]*50)/distanceTotale2;
                cadreRobot2.ajouterBandeVerticale(Bande::BANDE_INFERIEURE, 50 - dAct2, tableauBande2[i]);
            }            
        }
    
        if (donneesValide)
        {
            etat = ETAT::TransmissionRS232;
        }
        else
        {
            buzzer.setFrequence(SON_GRAVE);
            _delay_ms(1000);
            buzzer.stop();
            etat = ETAT::ModeAttente;
        }

        break;


    case ETAT::TransmissionRS232:

        led.allumerVert();
        attendreAppuie();
        led.clignoter5HZ();
        cadrePrincipale.afficherDessin(true) ;
        led.arretClignoter();

        break;
    
    default:
        break;
        }
    }

    return 0;
}



