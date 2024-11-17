#include "ecrireMem.hpp"
#include "uart.hpp"
#include "memoire.hpp"
#include "debug.h"

int main(){
	Memoire24CXXX memoire = Memoire24CXXX();
	initialisationUART();

	DEBUG_INIT();

	uint8_t donnee;
	uint16_t adresseEcriture = 0x00; 

	uint8_t partie1 = receptionUART();
	memoire.ecriture(adresseEcriture, partie1); 
	_delay_ms(5);
	adresseEcriture += 0x01;

	uint8_t partie2 = receptionUART();
	memoire.ecriture(adresseEcriture, partie2); 
	_delay_ms(5);
	adresseEcriture += 0x01;

	uint16_t code = partie1 << 8;
	code += partie2;

	DEBUG_VALUE(partie1);
	DEBUG_VALUE(partie2);



	while (adresseEcriture < code - 2){
		donnee = receptionUART();	
		DEBUG_VALUE(donnee);					
		memoire.ecriture(adresseEcriture, donnee); 
		_delay_ms(5);
		adresseEcriture += 0x01;
	}

}