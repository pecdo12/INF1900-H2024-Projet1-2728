#include "robot2.hpp"

volatile uint16_t gtempsActuel = 0;

ISR(TIMER1_COMPA_vect) { ++gtempsActuel; }

Robot2::Robot2(PortDirection portDirectionGauche, uint8_t pinDirectionGauche,
        PinPWM pinPwmGauche, PortDirection portDirectionDroite,
        uint8_t pinDirectionDroite, PinPWM pinPwmDroite, NumeroTimer numeroTimer,
        Unite unite, PinBuzzer pinBuzzer, uint8_t pinGND, PortLED port, uint8_t brochePos, uint8_t brocheNeg) : 
                        maMotricite_(Moteur(portDirectionGauche, pinDirectionGauche,
                            pinPwmGauche), Moteur(portDirectionDroite, pinDirectionDroite,
                            pinPwmDroite)),
                        compteur_(numeroTimer, unite) 
                        /*led_(port, brochePos, brocheNeg),
                        buzzer_(pinBuzzer, pinGND) */{}

Robot2::Robot2() :
    maMotricite_(Moteur(PortDirection::B, 3, PinPWM::B05), Moteur(PortDirection::B, 6, PinPWM::B04)),
    compteur_(NumeroTimer::TIMER1, Unite::MS)
    /*led_(PortLED::D, 5, 3),
    buzzer_(PinBuzzer::D27, 9)*/ {}

bool Robot2::getBroche(uint8_t broche) {return (PINA & (1 << broche));}
bool Robot2::estRotationDroite() {return getBroche(PC2) && getBroche(PC4);}
bool Robot2::estRotationGauche() {return getBroche(PC2) && getBroche(PC0);}
bool Robot2::estZoneVide() {return !getBroche(PC1) && !getBroche(PC2) && !getBroche(PC0) && !getBroche(PC3) && !getBroche(PC4);}

void Robot2::pauseRobot() {
    maMotricite_.arret();
    _delay_ms(DELAI_PAUSE);
}
void Robot2::pulse() {
    maMotricite_.marcheAvant();
    _delay_ms(DELAI_PULSE);
    maMotricite_.arret();
}
void Robot2::pulseArriere() {
    maMotricite_.marcheArriere();
    _delay_ms(DELAI_PULSE);
    maMotricite_.arret();
}
void Robot2::demarrerChronometre(){
    gtempsActuel = 0;
    compteur_.demarrerChronometre();
}

void Robot2::alignerRoues() {
    pulse();
    maMotricite_.marcheAvant(vitesse);
    _delay_ms(DELAI_ALIGNEMENT);
    maMotricite_.arret();
}
void Robot2::rotation() {
    maMotricite_.pivoterDroite(100);
    _delay_ms(DELAI_PULSE);
    maMotricite_.pivoterDroite(vitesse);
    while (!estZoneVide()){} // continuer de tourner vers la bande tant que le Robot2
                                // est sur le contour du parcours
    
    demarrerChronometre(); //commencer a compter le temps de rotation

    while ((gtempsActuel < DUREE_ROTATION_DEFAUT) && (getBroche(PC3) == 0)){
        // continuer de tourner vers la bande tant que la duree est correcte
            // ou tant que le Robot2 ne rencontre pas la bande
    }

    maMotricite_.arret();
}
void Robot2::rotationFin() {
    // meme logique que rotation mais pour la derniere bande qui fait sortir du circuit
    maMotricite_.pivoterGauche(100);
    _delay_ms(DELAI_PULSE);
    maMotricite_.pivoterGauche(vitesse);

    while (!estZoneVide()){}

    demarrerChronometre();

    while ((gtempsActuel < DUREE_ROTATION_DEFAUT) && (getBroche(PC1) == 0)){}

    maMotricite_.arret();
}
void Robot2::suivreLigne() {
    if (getBroche(PC2) == 1){ // pin milieu
        if (getBroche(PC1) == 1){ // pin intermediaire gauche
            maMotricite_.tourner(vitesse, vitesse + 5);
            // bool capteurGaucheInterieur = ;
            while (getBroche(PC1) && getBroche(PC2) && !getBroche(PC0) && !getBroche(PC3) && !getBroche(PC4)){}
        }

        else if (getBroche(PC3) == 1){ // pin intermediaire droite
            maMotricite_.tourner(vitesse + 3, vitesse);
            // bool capteurDroitInterieur = ;
            while (getBroche(PC3) && getBroche(PC2) && !getBroche(PC4) && !getBroche(PC0) && !getBroche(PC1)){}
        }

        maMotricite_.marcheAvant(vitesse);
    }

    else if (getBroche(PC2) == 0){
        bool capteursExtremesGauche = getBroche(PC0) || getBroche(PC1);
        bool capteursExtremesDroits = getBroche(PC3) || getBroche(PC4);
        if (capteursExtremesGauche){// pins gauche
            maMotricite_.tourner(vitesse - 10, vitesse + 10);
            while (getBroche(PC0)){} // tant que le capteur extreme gauche est allume
        }
        else if (capteursExtremesDroits){ // pins droite
            maMotricite_.tourner(vitesse + 10, vitesse - 10);
            while (getBroche(PC4)){} // tant que le capteur extreme droit est allume
        }
        else{
            maMotricite_.arret();
        }
    }
}
void Robot2::alignerRouesSuivi() {
    pulse();

    demarrerChronometre();

    while (gtempsActuel < DELAI_ALIGNEMENT_SUIVI){
        suivreLigne();
    }

    maMotricite_.arret();

    compteur_.arreterChronometre();
}

void Robot2::actionLigneDroite() {
    suivreLigne();

    if (estRotationDroite()){
        compteur_.arreterChronometre();
        etatPresent = EtatParcours::pointRotation;
    }
    else if(estRotationGauche()){
        etatPresent = EtatParcours::dernierTournant;
    }
}
void Robot2::actionPointRotation() {
    pauseRobot();

    pulse();

    if (estZoneVide() == 0){
        nBande++;

        alignerRouesSuivi();
        pauseRobot();

        rotation();

        pauseRobot();

        etatPresent = EtatParcours::bande;

        if(estZoneVide()){//Robot2 detecte pas de bande donc bande de 4.5"

            dureeMarcheArriere = 0;

            if (nCorner == 1){
                ajouterBandesTableau(tableauBandes1);
            }
            else if (nCorner == 3){
                ajouterBandesTableau(tableauBandes2);
            }

            maMotricite_.pivoterGauche(100);
            _delay_ms(DELAI_PULSE);
            maMotricite_.pivoterGauche(vitesse);

            while (estZoneVide()){}//revenir sur le parcours
            
            pauseRobot();
            etatPresent = EtatParcours::ligneDroite;
        }
    }
    else {//tournant normal

        nBande = 0;
        nCorner++; 

        alignerRoues();
        pauseRobot();

        maMotricite_.pivoterDroite(100);
        _delay_ms(DELAI_PULSE);
        maMotricite_.pivoterDroite(vitesse);

        while (estZoneVide()){}

        pauseRobot();

        etatPresent = EtatParcours::ligneDroite;

    }
}
void Robot2::actionBande() {
    demarrerChronometre();

    pulse();

    while (estZoneVide() == 0){ // tant qu'il y a une bande
        suivreLigne();
    }

    compteur_.arreterChronometre();
    dureeMarcheArriere = gtempsActuel;

    etatPresent = EtatParcours::boutBande;

    if (dureeMarcheArriere < 150){
        etatPresent = EtatParcours::ligneDroite;
        maMotricite_.pivoterGauche(100);
        _delay_ms(DELAI_PULSE);
        maMotricite_.pivoterGauche(vitesse);
        while (estZoneVide() == 0){}
        while (estZoneVide()){}
        maMotricite_.arret();        
    }

}
void Robot2::actionBoutBande() {
    pauseRobot();

    pulseArriere();

    maMotricite_.marcheArriere(vitesse);

    demarrerChronometre();

    while (gtempsActuel < dureeMarcheArriere){}

    dureeMarcheArriere = 0;

    pauseRobot();

    maMotricite_.pivoterGauche(100);
    _delay_ms(DELAI_PULSE);
    maMotricite_.pivoterGauche(vitesse);

    while (!estZoneVide()){}

    while (estZoneVide()){}


    pauseRobot();

    etatPresent = EtatParcours::ligneDroite;    
}
void Robot2::actionDernierTournant() {
    pauseRobot();

    alignerRouesSuivi();

    pauseRobot();

    rotationFin();

    pauseRobot();

    pulse();

    while (estZoneVide() == 0){
        suivreLigne();
    }

    pauseRobot();

    etatPresent = EtatParcours::arrivee;
}
void Robot2::actionArrivee() {
    //pour l'instant rien faire
}

void Robot2::ajouterBandesTableau(uint8_t tableauBandes[]) {
    if (dureeMarcheArriere < 300){
        tableauBandes[nBande - 1] = 2;
    }
    else if (dureeMarcheArriere < 1200){
        tableauBandes[nBande - 1] = 4;
    }
    else {
        tableauBandes[nBande - 1] = 6;
    }
}
void Robot2::gererDonnees(){
    for (uint8_t i = 0; i < 5; ++i){
        if (tableauParcours1[i] != 0){
            nBande1++;
        }
        else {
            if(nBande1 > 0)  {
                nBande1--;
            }
            break;
        }
        if (nBande1 == 5){
            nBande1 --;
        }
    }
    for (uint8_t i = 0; i < 5; ++i){
        if (tableauParcours2[i] != 0){
            nBande2++;
        }
        else {
            if(nBande2 > 0)  {
                nBande2--;
            }
            break;
        }
        if (nBande2 == 5){
            nBande2 --;
        }
    }

    // for(uint8_t i = 0; i < 5; i ++){
    //     tableauParcours1[i] /= (distanceTotale1 / 50);
    // }

    // for(uint8_t i = 0; i < 5; i ++){
    //     tableauParcours2[i] /= (distanceTotale2 / 50);
    // }
}
void Robot2::ajouterBandesCadre(CadreRobot2& cadreRobot2){
    for (uint8_t i = 0; i < nBande1; i++){
        dAct1 += (tableauParcours1[i]*50)/distanceTotale1;
        cadreRobot2.ajouterBandeVerticale(Bande::BANDE_SUPERIEURE, dAct1, tableauBandes1[i]);
    }
    for (uint8_t i = 0; i < nBande2; i++){
        dAct2 += (tableauParcours2[i]*50)/distanceTotale2;
        cadreRobot2.ajouterBandeVerticale(Bande::BANDE_INFERIEURE, 50 - dAct2, tableauBandes2[i]);
    }
}
void Robot2::performerParcours() {
    while (etatPresent != EtatParcours::arrivee){
        switch (etatPresent){
            case EtatParcours::ligneDroite :
                demarrerChronometre();

                pulse();

                while (etatPresent == EtatParcours ::ligneDroite){
                    actionLigneDroite();
                }

                compteur_.arreterChronometre();

                // pulse(); // commenter

                if ((nCorner == 1)){
                    tableauParcours1[nBande] = gtempsActuel / 10;
                    if (tableauParcours1[nBande] < 90){
                        tableauParcours1[nBande] = 90;
                    }
                    distanceTotale1 += tableauParcours1[nBande];
                }
                else if ((nCorner == 3)){
                    tableauParcours2[nBande] = gtempsActuel / 10;
                    if(tableauParcours2[nBande] < 90){
                        tableauParcours2[nBande] = 90;
                    }
                    distanceTotale2 += tableauParcours2[nBande];
                } 
                break;
            
            case EtatParcours::pointRotation:
                actionPointRotation();
                break;
            
            case EtatParcours::bande:
                actionBande();
                if (nCorner == 1){
                    ajouterBandesTableau(tableauBandes1);
                }
                else if (nCorner == 3){
                    ajouterBandesTableau(tableauBandes2);
                }
                // DEBUG_VALUE(gtempsActuel);
                break;

            case EtatParcours::boutBande:
                actionBoutBande();
                break;

            case EtatParcours::dernierTournant:
                actionDernierTournant();
                break;
            
            case EtatParcours::arrivee:
                break;

            default:
                break;
        }
    }
    compteur_.arreterChronometre(); // arreter les interruptions du timer
    
}

void Robot2::emettreDonnees(){
    initialiserTransmission();
    for (uint8_t i = 0; i < 5; i++) {
        genererSequence(tableauParcours1[i]);
    }
    for (uint8_t i = 0; i < 4; i++){
        genererSequence(tableauBandes1[i]);
    }
    for (uint8_t i = 0; i < 5; i++){
        genererSequence(tableauParcours2[i]);
    }
    for (uint8_t i = 0; i < 4; i++){
        genererSequence(tableauBandes2[i]);
    }
}
void Robot2::afficherValeurs(){
    DEBUG_INIT();
    DEBUG_PRINT("\n");
    for(uint8_t i = 0; i < 5; i ++){
        DEBUG_VALUE(tableauParcours1[i]);
        DEBUG_PRINT(" ");
    }
    DEBUG_PRINT("\n");
    for(uint8_t i = 0; i < 4; i ++){
        DEBUG_VALUE(tableauBandes1[i]);
        DEBUG_PRINT(" ");
    }
    DEBUG_PRINT("\n");
    for(uint8_t i = 0; i < 5; i ++){
        DEBUG_VALUE(tableauParcours2[i]);
        DEBUG_PRINT(" ");
    }
    DEBUG_PRINT("\n");
    for(uint8_t i = 0; i < 4; i ++){
        DEBUG_VALUE(tableauBandes2[i]);
        DEBUG_PRINT(" ");
    }
    DEBUG_PRINT("\n");
    DEBUG_VALUE(distanceTotale1);
    DEBUG_PRINT("\n");
    DEBUG_VALUE(distanceTotale2);
    DEBUG_PRINT("\n");
    DEBUG_VALUE(dAct1);
    DEBUG_PRINT("\n");
    DEBUG_VALUE(dAct2);
    DEBUG_PRINT("\n");
}
void Robot2::recevoirDonnees(){
    //initialiser variables pour reception IR
    uint8_t tableauBits[12];
    uint8_t donneeRecue = 0;
    uint8_t crcRecu = 0;

    for(uint8_t i = 0; i < 18; i++){
        recupererDonnee(tableauBits);
        donneeRecue = valeurDonnee(tableauBits);
        crcRecu = crcDonnee(tableauBits);
        donneesValides &= verifierDonnee(donneeRecue, crcRecu);
        tableauReception[i] = donneeRecue;
    }
}
void Robot2::afficherDonneesIR(){
    DEBUG_INIT();
    for(uint8_t i = 0; i < 18; i++){
        DEBUG_VALUE(tableauReception[i]);
        DEBUG_PRINT(" ");
    }
}