#include "exec.hpp"

int main(){
    LED maLed(PortLED::A, 1, 2);
    maLed.allumerRouge();
}