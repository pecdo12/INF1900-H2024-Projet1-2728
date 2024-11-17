/*
Auteurs: Mohamed Omar Zedek, Sejjari Souhayl, Cristescu Sebastian, Mohamed Yassir Merzouki
Description: Classe PWM qui permet de générer un signal PWM sur des broches spécifiques. Composée de 3 méthodes qui permet de contrôler le pourcentage du PWM généré.
Tous les timers utilises generent un pwm sur 8 bits.
Identifications matérielles: Lors de la création d'un objet PWM, il faut passer en paramètre sur quelle broche on veut le signal. Les broches possibles sont 
les borches B4, B5, D5, D6, D7 et D8
*/

#include "Global.hpp"

enum class PinPWM
{
    B04,
    B05,
    D15,
    D16,
    D27,
    D28
};

class PWM
{
public:
    // PWM(PinPWM pinPWM);
    PWM(PinPWM pinPWM, uint8_t pourcentage = 0);
    void setPourcentage(uint8_t pourcentage);
    void augmenterPourcentage(uint8_t pourcentage);
    void diminuerPourcentage(uint8_t pourcentage);
private:
    PinPWM  pin_;
    uint8_t pourcentage_;
};