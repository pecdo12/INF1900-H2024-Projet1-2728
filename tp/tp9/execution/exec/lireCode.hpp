#include "Global.hpp"
#include "can.h"
#include "debug.h"
#include "interruptions_externes.hpp"
#include "LED.hpp"
#include "memoire.hpp"
#include "Motricite.hpp"
//#include "PWM.hpp"
#include "timer.hpp"
#include "uart.hpp"
#include "Buzzer.hpp"

const uint8_t debut = 0x01; 
const uint8_t attendre = 0x02; 
const uint8_t allumerLed = 0x44; 
const uint8_t eteindreLed = 0x45; 
const uint8_t jouerSon = 0x48; 
const uint8_t arreterSon = 0x09; 
const uint8_t arreterMoteur1 = 0x60; 
const uint8_t arreterMoteur2 = 0x61; 
const uint8_t avancer = 0x62; 
const uint8_t reculer = 0x63; 
const uint8_t tournerDroite = 0x64; 
const uint8_t tournerGauche = 0x65; 
const uint8_t debutBoucle = 0xC0;
const uint8_t finBoucle = 0xC1;
const uint8_t fin = 0xFF;
