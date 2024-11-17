#include "Motricite.hpp"
#include "timer.hpp"
#include "LED.hpp"
#include "Buzzer.hpp"
#include "dessin.hpp"
#include "debug.h"
#include "Emission.hpp"
#include "ReceptionIR.hpp"


enum class EtatParcours{ligneDroite, pointRotation,bande,dernierTournant,boutBande, arrivee, temp};

class Robot2{
    private:
    Motricite maMotricite_;
    TimerCTC16Bits compteur_;
    // LED led_;
    // Buzzer buzzer_;
    EtatParcours etatPresent = EtatParcours::ligneDroite;
    uint16_t dureeMarcheArriere;

    uint8_t nBande = 0;
    uint8_t nCorner = 0;
    uint8_t tableauReception[18];
    uint8_t tableauParcours1[5] = {0, 0, 0, 0, 0};
    uint8_t tableauBandes1[4] = {0, 0, 0, 0};
    uint8_t tableauParcours2[5] = {0, 0, 0, 0, 0};
    uint8_t tableauBandes2[4] = {0, 0, 0, 0};
    uint16_t distanceTotale1 = 0;
    uint16_t distanceTotale2 = 0;
    uint8_t nBande1 = 0;
    uint8_t nBande2 = 0;
    uint8_t dAct1 = 0;
    uint8_t dAct2 = 0;
    bool donneesValides = true;

    static const uint8_t vitesse = 35;
    static const uint8_t DELAI_PULSE = 35;
    static const uint16_t DELAI_PAUSE = 500;
    static const uint16_t DELAI_ALIGNEMENT  = 1700 ; 
    static const uint16_t DELAI_ALIGNEMENT_SUIVI  = 1100 ; 
    static const uint16_t DUREE_ROTATION_DEFAUT = 1300 ;
    static const uint16_t DUREE_ROTATION_DEFAUT_FIN = 2300 ;


    public :
    Robot2(PortDirection portDirectionGauche, uint8_t pinDirectionGauche, PinPWM pinPwmGauche, PortDirection portDirectionDroite, uint8_t pinDirectionDroite, PinPWM pinPwmDroite, NumeroTimer numeroTimer, Unite unite, PinBuzzer pinBuzzer, uint8_t pinGND, PortLED port, uint8_t brochePos, uint8_t brocheNeg);
    Robot2();

    bool getBroche(uint8_t broche);
    bool estRotationDroite();
    bool estRotationGauche();
    bool estZoneVide();

    void pauseRobot();
    void pulse();
    void pulseArriere();
    void alignerRoues();
    void demarrerChronometre();
    void rotation();
    void rotationFin();
    void suivreLigne();
    void alignerRouesSuivi();

    void actionLigneDroite();
    void actionPointRotation();
    void actionBande();
    void actionBoutBande();
    void actionDernierTournant();
    void actionArrivee();

    void performerParcours();

    void ajouterBandesTableau(uint8_t tableauBandes[]);
    void gererDonnees();
    void ajouterBandesCadre(CadreRobot2& cadreRobot2);
    // void emettreFrequence(uint8_t frequence, uint8_t nFois);
    // void arreterFrequence();
    void emettreDonnees();
    void recevoirDonnees();

    void afficherValeurs();
    void afficherDonneesIR();

    
};