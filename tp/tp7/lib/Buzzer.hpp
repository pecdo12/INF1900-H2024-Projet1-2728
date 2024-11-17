#include "Global.hpp"

enum class PinBuzzer
{
    B04,
    B05,
    D15,
    D16,
    D27,
    D28
};

class Buzzer{
    public:
        Buzzer(PinBuzzer pinBuzzer, uint8_t pinGND, uint16_t frequence500k_7 = 0);
        void setFrequence(uint16_t frequence500k_7);
        void jouerMarioBros();
        void jouerPirateDesCaraibes();
    private :
        uint16_t tempsActive_;
        PinBuzzer pin_;
        static constexpr uint8_t periodeCLKTimer02 = 32;
};