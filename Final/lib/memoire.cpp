#include "memoire.hpp"
#include "Global.hpp"


uint8_t Memoire24CXXX::m_adresse_peripherique = 0xA0;

Memoire24CXXX::Memoire24CXXX()
  : PAGE_SIZE(128)
{
   init();
}

Memoire24CXXX::~Memoire24CXXX()
{
}

void Memoire24CXXX::init()
{
   choisir_banc(0);
   TWSR = 0;
   TWBR =  (F_CPU / 100000UL - 16) / 2;

}

uint8_t Memoire24CXXX::choisir_banc(const uint8_t banc)
{
   uint8_t temp = banc & 0x03;
   uint8_t rv = 255;
   if(banc == temp)
   {
      Memoire24CXXX::m_adresse_peripherique = (0xA0 | ( banc << 1 ));
      rv = Memoire24CXXX::m_adresse_peripherique;
   }
   return rv;
}

uint8_t Memoire24CXXX::lecture(const uint16_t adresse, uint8_t *donnee)
{
  uint8_t rv = 0;

  for (;;)
  {
    TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);  
    while ((TWCR & _BV(TWINT)) == 0)  
      ;

    TWDR = m_adresse_peripherique;   
    TWCR = _BV(TWINT) | _BV(TWEN);    
    while ((TWCR & _BV(TWINT)) == 0)   
      ;
    if (TWSR==0x18)     
       break;
  }


  TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);  
  while ((TWCR & _BV(TWINT)) == 0)      
    ;


  TWDR = m_adresse_peripherique;       
  TWCR = _BV(TWINT) | _BV(TWEN);   
  while ((TWCR & _BV(TWINT)) == 0)    
    ;


  TWDR =  ( adresse >> 8 );           
  TWCR = _BV(TWINT) | _BV(TWEN);    
  while ((TWCR & _BV(TWINT)) == 0) 
     ;

  TWDR = adresse;                   
  TWCR = _BV(TWINT) | _BV(TWEN);       
  while ((TWCR & _BV(TWINT)) == 0)     
    ;


  while ((TWCR & _BV(TWINT)) == 0)     
    ;
  TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);
  while ((TWCR & _BV(TWINT)) == 0)     
    ;

  TWDR =  m_adresse_peripherique + 1;   
  TWCR = _BV(TWINT) | _BV(TWEN);     
  while ((TWCR & _BV(TWINT)) == 0)   
    ;

  TWCR = _BV(TWINT) | _BV(TWEN);    
  while ((TWCR & _BV(TWINT)) == 0)  
    ;
  *donnee = TWDR;

  TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);
  return rv;
}

uint8_t Memoire24CXXX::lecture(const uint16_t adresse, uint8_t *donnee, uint8_t longueur)
{
  uint8_t twcr;

  //______________ Attente de la fin d'un cycle d'ecriture ______________
  for (;;)
  {
    TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);    // Condition de depart
    while ((TWCR & _BV(TWINT)) == 0) ;   // Attente de fin de transmission

    TWDR = m_adresse_peripherique;       // Controle - bit 0 a 0, ecriture
    TWCR = _BV(TWINT) | _BV(TWEN);       // R. a Z., interrupt. - Depart de transm.
    while ((TWCR & _BV(TWINT)) == 0)     // Attente de fin de transmission
       ;
    if (TWSR==0x18)                      // 0x18 = cycle d'ecriture termine
       break;
  }

  //_______________ Transmission de la condition de depart ________________
  TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);     // Condition de depart
  while ((TWCR & _BV(TWINT)) == 0)    // Attente de fin de transmission
     ;

  //__________________ Transmission du code de controle ___________________
  TWDR = m_adresse_peripherique;        // Controle - bit 0 a 0, ecriture
  TWCR = _BV(TWINT) | _BV(TWEN);        // R. a Z., interrupt. - Depart de transm.
  while ((TWCR & _BV(TWINT)) == 0)      // Attente de fin de transmission
     ;

  //______________ Transmission du poids fort de l'adresse ________________
  TWDR = adresse >> 8;                  // 8 bits de poids fort de l'addresse
  TWCR = _BV(TWINT) | _BV(TWEN);        // R. a Z., interrupt. - Depart de transm.
  while ((TWCR & _BV(TWINT)) == 0)      // Attente de fin de transmission
     ;

  //_____________ Transmission du poids faible de l'adresse _______________
  TWDR = adresse;                       // 8 bits de poids faible de l'addresse
  TWCR = _BV(TWINT) | _BV(TWEN);        // R. a Z., interrupt. - Depart de transm.
  while ((TWCR & _BV(TWINT)) == 0)      // Attente de fin de transmission
     ;

  //_______________ Transmission de la condition de depart ________________
  TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);    // Condition de depart
  while ((TWCR & _BV(TWINT)) == 0)      // Attente de fin de transmission
     ;

  //__________________ Transmission du code de controle ___________________
  TWDR =  m_adresse_peripherique + 1;  // Controle - bit 0 a 1, lecture
  TWCR = _BV(TWINT) | _BV(TWEN);       // R. a Z., interrupt. - Depart de transm.
  while ((TWCR & _BV(TWINT)) == 0)     // Attente de fin de transmission
     ;

  for (twcr = _BV(TWINT) | _BV(TWEN) | _BV(TWEA) ; longueur > 0; longueur--)
  {
      if (longueur == 1)
         twcr = _BV(TWINT) | _BV(TWEN);  // Derniere donnee, NACK
      TWCR = twcr;                       // R. a Z., interrupt. - Depart de transm.
      while ((TWCR & _BV(TWINT)) == 0) ; // Attente de fin de transmission
     *donnee++ = TWDR;               // Lecture
  }

  TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);

  return 0;
}

uint8_t Memoire24CXXX::ecriture(const uint16_t adresse, const uint8_t donnee)
{
  //______________ Attente de la fin d'un cycle d'ecriture ______________
  for ( ; ; )
  {
    TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);    // Condition de depart
    while ((TWCR & _BV(TWINT)) == 0)      // Attente de fin de transmission
       ;

    TWDR = m_adresse_peripherique;       // Controle - bit 0 a 0, ecriture
    TWCR = _BV(TWINT) | _BV(TWEN);       // R. a Z., interrupt. - Depart de transm.
    while ((TWCR & _BV(TWINT)) == 0)     // Attente de fin de transmission
       ;

    if (TWSR==0x18)
       break;               // 0x18 = cycle d'ecriture termine
  }

  //_______________ Transmission de la condition de depart ________________
  TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);     // Condition de depart
  while ((TWCR & _BV(TWINT)) == 0)    // Attente de fin de transmission
     ;

  //__________________ Transmission du code de controle ___________________
  TWDR = m_adresse_peripherique;        // Controle - bit 0 a 0, ecriture
  TWCR = _BV(TWINT) | _BV(TWEN);        // R. a Z., interrupt. - Depart de transm.
  while ((TWCR & _BV(TWINT)) == 0)      // Attente de fin de transmission
     ;

  //______________ Transmission du poids fort de l'adresse ________________
  TWDR = adresse >> 8;                 // 8 bits de poids fort de l'adresse
  TWCR = _BV(TWINT) | _BV(TWEN);        // R. a Z., interrupt. - Depart de transm.
  while ((TWCR & _BV(TWINT)) == 0)      // Attente de fin de transmission
     ;

  //_____________ Transmission du poids faible de l'adresse _______________
  TWDR = adresse;                      // 8 bits de poids faible de l'adresse
  TWCR = _BV(TWINT) | _BV(TWEN);        // R. a Z., interrupt. - Depart de transm.
  while ((TWCR & _BV(TWINT)) == 0)      // Attente de fin de transmission
     ;

  //______________________ Transmission de la donnee ______________________
  TWDR = donnee;
  TWCR = _BV(TWINT) | _BV(TWEN);        // R. a Z., interrupt. - Depart de transm.
  while ((TWCR & _BV(TWINT)) == 0)      // Attente de fin de transmission
     ;

  //________________ Transmission de la condition d'arret _________________
  TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);  // Demarrage du cycle d'ecriture

  return 0;
}

uint8_t Memoire24CXXX::ecriture(const uint16_t adresse, uint8_t *donnee, const uint8_t longueur)
{
  uint8_t rv;
  uint16_t copieAdresse = adresse;
  uint8_t copieLongueur = longueur;
  do
  {
      rv = ecrire_page(copieAdresse, donnee, copieLongueur);
      copieAdresse += rv;      // On pointe une nouvelle page
      copieLongueur -= rv;     // On soustrait la partie ecrite precedemment
      donnee += rv;            // On avance le pointeur de donnees
  }
  while (copieLongueur > 0);

  return 0;
}

uint8_t Memoire24CXXX::ecrire_page(const uint16_t adresse, uint8_t *donnee, const uint8_t longueur)
{
  uint16_t addr_fin;
  uint8_t rv = 0;
  uint8_t copieLongueur = longueur;


  if (adresse + longueur < (adresse | (PAGE_SIZE - 1)))
    addr_fin = adresse + longueur;
  else
    addr_fin = (adresse | (PAGE_SIZE - 1)) + 1;
  copieLongueur = addr_fin - adresse;


  for ( ; ; )
  {
    TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);    // Condition de depart
    while ((TWCR & _BV(TWINT)) == 0) ;   // Attente de fin de transmission
    TWDR = m_adresse_peripherique;       // Controle - bit 0 a 0, ecriture
    TWCR = _BV(TWINT) | _BV(TWEN);       // R. a Z., interrupt. - Depart de transm.
    while ((TWCR & _BV(TWINT)) == 0)     // Attente de fin de transmission
       ;

    if (TWSR==0x18)
       break;               // 0x18 = cycle d'ecriture termine
  }

  TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);     // Condition de depart
  while ((TWCR & _BV(TWINT)) == 0)       // Attente de fin de transmission
     ;

  //__________________ Transmission du code de controle ___________________
  TWDR = m_adresse_peripherique;        // Controle - bit 0 a 0, ecriture
  TWCR = _BV(TWINT) | _BV(TWEN);        // R. a Z., interrupt. - Depart de transm.
  while ((TWCR & _BV(TWINT)) == 0)      // Attente de fin de transmission
     ;

  //______________ Transmission du poids fort de l'adresse ________________
  TWDR = adresse >> 8;                  // 8 bits de poids fort de l'adresse
  TWCR = _BV(TWINT) | _BV(TWEN);        // R. a Z., interrupt. - Depart de transm.
  while ((TWCR & _BV(TWINT)) == 0)      // Attente de fin de transmission
     ;

  //_____________ Transmission du poids faible de l'adresse _______________
  TWDR = adresse;                       // 8 bits de poids faible de l'adresse
  TWCR = _BV(TWINT) | _BV(TWEN);        // R. a Z., interrupt. - Depart de transm.
  while ((TWCR & _BV(TWINT)) == 0)      // Attente de fin de transmission
     ;

  //______________________ Transmission de la page ______________________
  for ( ; copieLongueur > 0; copieLongueur--)
  {
     TWDR = *donnee++;
     TWCR = _BV(TWINT) | _BV(TWEN);     // R. a Z., interrupt. - Depart de transm.
     while ((TWCR & _BV(TWINT)) == 0)   // Attente de fin de transmission
        ;
     rv++;                              // Compteur de donnees
  }

  TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN); // Demarrage du cycle d'ecriture

  return rv;
}
