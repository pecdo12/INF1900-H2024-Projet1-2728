/*
Auteurs: Mohamed Omar Zedek, Sejjari Souhayl, Cristescu Sebastian, Mohamed Yassir Merzouki
Description: fichier hpp de la classe LED avec constructeur et plusieurs méthodes. Celle-ci permet d'allumer une LED de couleur rouge, verte, ambre et de l'éteindre.
*/

#include "Global.hpp"

enum class PortLED{A, B, C, D};

class LED{
    public:
        LED();
        LED(PortLED port, uint8_t brochePos, uint8_t brocheNeg);
        void allumerRouge();
        void allumerVert();
        void allumerAmbre();
        void allumerAmbre(uint16_t dureeEnMS);
        void eteindre();
    private:
        static constexpr uint8_t DELAI_COULEUR_AMBRE = 10;
        PortLED port_;
        uint8_t brochePos_;
        uint8_t brocheNeg_;
};