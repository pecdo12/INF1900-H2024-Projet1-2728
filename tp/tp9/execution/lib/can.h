/*
Auteurs: Mohamed Omar Zedek, Sejjari Souhayl, Cristescu Sebastian, Mohamed Yassir Merzouki
Description: Code fournit dans le cadre du cours. Classe can qui permet de convertir des données analogiques en valeurs numériques. Possède une seule méthode pour la lecture des données.
Identifications matérielles: Le convertisseur ne peut prendre les données analogiques que du port A. Par contre, lors de l'utilisation de la méthode lecture, il est possible de spécifier quelles broches du port A on veut utiliser. Il n'y a pas de broches pas défaut.
*/

#ifndef CAN_H
#define CAN_H

#include <avr/io.h> 


class can
{
public:

   can();
   ~can();

   uint16_t lecture(uint8_t pos);

private:


};

#endif
