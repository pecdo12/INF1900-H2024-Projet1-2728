/*
Auteurs: Mohamed Omar Zedek, Sejjari Souhayl, Cristescu Sebastian, Mohamed Yassir Merzouki
Description: Code fournit dans le cadre du cours. Classe memoire qui permet de lire et écrire des données dans la mémoire EEPROM. On utilise les méthodes lecture et ecriture qui peuvent soit être utilisés pour écrire un uint8_t ou une chaine de uint8_t.
Identifications matérielles: S'assurer qu'un cavalier soit présent sur MemEN, C0-C1 ne soit pas utilisés et placer le cavalier DbgEN.
*/

#ifndef MEMOIRE_24_H
#define MEMOIRE_24_H

#include "Global.hpp"

class Memoire24CXXX
{
public:

   Memoire24CXXX(); 
   ~Memoire24CXXX();

   void init();
   
   static uint8_t choisir_banc(const uint8_t banc);
   
   uint8_t lecture(const uint16_t adresse, uint8_t *donnee);
   uint8_t lecture(const uint16_t adresse, uint8_t *donnee,
                   const uint8_t longueur);

   uint8_t ecriture(const uint16_t adresse, const uint8_t donnee);
   uint8_t ecriture(const uint16_t adresse, uint8_t *donnee,
                    const uint8_t longueur);

private:
   uint8_t ecrire_page(const uint16_t adresse, uint8_t *donnee,
                       const uint8_t longueur);

private:
   static uint8_t m_adresse_peripherique;
   const uint8_t PAGE_SIZE;
};

#endif
