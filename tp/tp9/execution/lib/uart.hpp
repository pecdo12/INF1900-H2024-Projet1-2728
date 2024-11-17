/*
Auteurs: Mohamed Omar Zedek, Sejjari Souhayl, Cristescu Sebastian, Mohamed Yassir Merzouki
Description: Regroupement de fonctions pour communiquer avec le microcontroleur.
Identifications matérielles: S'assurer qu'un cavalier soit présent sur MemEN, C0-C1 ne soit pas utilisés et placer le cavalier DbgEN.
*/

#include "Global.hpp"

void initialisationUART(void);

void transmissionUART(uint8_t donnee);

uint8_t receptionUART(void);