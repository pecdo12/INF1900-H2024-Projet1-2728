/*
Auteurs: Mohamed Omar Zedek, Sejjari Souhayl, Cristescu Sebastian, Mohamed Yassir Merzouki
Description: Classes Moteur et Motricite qui servent a controler les moteurs et faire avancer le robot. 
La classe moteur permet de contrôler les mouvements des moteurs, tandis que la classe motricité utilise les classes moteurs pour contrôler les mouvements du robots.
Identifications matérielles: La classe Moteur permet de spécifier a quelle broche sera reliee la broche D du moteur qui devra idealement etre proche de celle du PWM.
etant donne que la classe moteur utilise PWM cette classe est limitee aux memes broches de sortie que la classe PWM pour la borche Enable du moteur soit :
les broches B4, B5, D5, D6, D7 et D8. on peut ensuite choisir n'importe quelle autre borche des PORTB et PORTD pour choisir la direction autre que les precedentes
Concernant la classe Motricite, elle utilise les meme borches que les objets moteurs crees au prealable.
*/

#include "PWM.hpp"

enum class PortDirection{B, D};

class Moteur{
    private:
        PortDirection port_;
        uint8_t brocheDirection_;
        PWM brocheAction_;
    public:
        Moteur(PortDirection port, uint8_t brocheDirection, PinPWM pinPWM);
        void avancer(uint8_t pourcentage = 100);
        void reculer(uint8_t pourcentage = 100);
        void arret();
    
};

class Motricite{
    private:
        static constexpr uint16_t DELAY_LOOP_CYCLES_10_MS = 20000;
        Moteur moteurGauche_;
        Moteur moteurDroit_;
    public:
        Motricite(Moteur moteurGauche, Moteur moteurDroit);
        void arret();
        void marcheAvant(uint8_t pourcentage = 100);
        void marcheArriere(uint8_t pourcentage = 100);
        void tourner(uint8_t vitesseGauche, uint8_t vitesseDroite);
        void pivoterDroite(uint8_t vitesse);
        void pivoterGauche(uint8_t vitesse);
        void tournerDroite90();
        void tournerGauche90();
        void alignerRoues();
        // void tournerDroite(uint8_t degre, uint8_t vitesse = 50);
        // void tournerGauche(uint8_t degre, uint8_t vitesse = 50);
        // void twerk(uint8_t nombre, uint8_t angle);
};