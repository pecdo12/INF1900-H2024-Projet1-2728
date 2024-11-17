/*
 * Nom: Mohamed Omar Zedek / Souhayl Sejjari / Sebastian Cristescu / Mohamed Yassir Merzouki
 * Groupe 02
 * Equipe 2728
 * Projet Robot 1
*/


#include "dessin.hpp"
#include "LED.hpp"
#include "Buzzer.hpp"
#include "ReceptionIR.hpp"
#include "can.h"
#include "robot1.hpp"



bool donneesValide = true;
uint8_t tableauReception[18]  = {};
uint8_t tableauParcours1[5] = {};
uint8_t tableauParcours2[5] = {};
uint8_t tableauBande1[4] = {};
uint8_t tableauBande2[4] = {};
uint8_t tableauBits[12] = {};
uint8_t donneeRecue = 0;
uint8_t crcRecu = 0;

uint16_t distanceTotale1 = 0;
uint16_t distanceTotale2 = 0;
uint8_t dAct1 = 0;
uint8_t dAct2 = 0;
bool donneesValides = true;

can convertisseur;

const uint8_t fluxIR = PA0;
const uint8_t donneIR = PA4;
uint8_t distance = 0;
uint8_t distanceDebut = 0;
OrientationRobot orientation ;

Moteur moteurG(PortDirection::B, 7, PinPWM::B05);
Moteur moteurD(PortDirection::B, 6, PinPWM::B04);
Motricite maMotricite(moteurG, moteurD);
uint8_t nombreCycle = 0;
uint8_t ligneDepart = 7 ;
int mappedValue = 0;
LED led(PortLED::D, 5, 6);
Buzzer buzzer(PinBuzzer::D27, 8);

Cadre cadrePrincipale(nLignesCadrePrincipale,nColonnesCadrePrincipale) ;
CadreRobot1 cadreRobot1(nLignesCadreRobot1,nColonnesCadreRobot1,decalageLignesRobot1,decalageColonnesRobot1);

CadreRobot2 cadreRobot2(nLignesCadreRobot2,nColonnesCadreRobot2,decalageLignesRobot2,decalageColonnesRobot2);
Robot1Parcours robot1 = Robot1Parcours(&convertisseur, &maMotricite, &moteurD, &moteurG);

