#include "lireCode.hpp"


const uint8_t EntreeBouton = PD2;

uint8_t instruction;
uint8_t operande;
uint16_t longueur;

uint16_t adresseLecture = 0x00; 
bool commencerProgramme = false;

bool initialiserBoucle = false;
uint16_t positionCode = 0;
uint8_t addressBoucle = 0;
uint8_t compteurBoucle = 0;

float frequence = 0;

Memoire24CXXX memoire = Memoire24CXXX();
LED led = LED(PortLED::A, 1, 2);
Buzzer buzzer = Buzzer(PinBuzzer::B05, 7);

Moteur moteurGauche(PortDirection::D, 7, PinPWM::D15);

Moteur moteurDroit(PortDirection::D, 4, PinPWM::D16);

Motricite maMotricite(moteurGauche, moteurDroit);

bool boutonAppuyer(){
    if ( PIND & (1 << PD2) ){
        _delay_ms(10);
        if ( PIND & (1 << PD2) )
            return true;
    }
    return false;
}


int main(){
	DEBUG_INIT();

	buzzer.stop();
	

	DEBUG_VALUE(0xFF);

	//Attendre bouton appuyer
	while (!boutonAppuyer())
	{
		_delay_ms(10);	
	}
	DEBUG_VALUE(0xFF);

	//Lire longueur du code
	memoire.lecture(adresseLecture, &instruction);
	adresseLecture++;
	_delay_ms(10);
	memoire.lecture(adresseLecture, &operande);
	adresseLecture++;
	_delay_ms(10);
	DEBUG_VALUE(instruction);
	DEBUG_VALUE(operande);

	longueur = (instruction << 8) | operande;

	//Lire code
	for (uint16_t i = 0; i < longueur; i+=2){

		//Lire instruction et operande
		memoire.lecture(adresseLecture, &instruction);
        adresseLecture++;
        _delay_ms(10);
		//DEBUG_VALUE(instruction);
		memoire.lecture(adresseLecture, &operande);
        adresseLecture++;
        _delay_ms(10);
		//DEBUG_VALUE(operande);


		//Vérifier si c'est le début du programme
		if (instruction == debut)
		{
			commencerProgramme = true;
		}		
		else;
		
		if (!commencerProgramme)
			continue;
		else;

		//Vérifier si c'est la fin du programme
		if (instruction == fin)
			break;
		else;


		//Exécuter instruction
		switch (instruction)
		{
		case attendre:
			DEBUG_PRINT("Attendre \n");
			for(uint8_t i = 0; i < operande; i++)
				_delay_ms(25);
			break;
		
		case allumerLed:
			DEBUG_PRINT("AllumerLed \n");
			if (operande == 1) //Vert
				led.allumerVert();
			else if (operande == 2) //Rouge
				led.allumerRouge();
			else; //Do nothing

			break;
		
		case eteindreLed:
			led.eteindre();
			break;

		case jouerSon:
			if (operande < 45 || operande > 81)
				break;
			// Description: jouer une sonorité du tableau 3. 
			// Il faut activer une sortie en mode PWM avec la fréquence donnée dans le tableau. 
			// Si la valeur n'est pas dans le tableau, la commande est ignorée.
			frequence = 440* pow(2, (float(operande-49)/12));
			buzzer.setFrequence(frequence);
			break;
		
		case arreterSon:
			//Description: arrêter de jouer la sonorité en cours (s'il y en a une qui joue).
			buzzer.setFrequence(0);
			break;

		case arreterMoteur1:
			DEBUG_PRINT("ArreterMoteur \n");
			//Description: arrêter les deux moteurs
			maMotricite.arret();
			break;
		
		case arreterMoteur2:
			DEBUG_PRINT("ArreterMoteur \n");
			//Description: arrêter les deux moteurs
			maMotricite.arret();
			break;
		
		case avancer:
			DEBUG_PRINT("Avancer \n");
			//Description: avancer en ligne droite à une vitesse donnée par (opérande / 255 * 100%)
			maMotricite.marcheAvant((float(operande)/255)*100);
			break;

		case reculer:
			//Description: reculer en ligne droite à la vitesse (opérande / 255 * 100%)
			maMotricite.marcheArriere((float(operande)/255)*100);
			break;
		
		case tournerDroite:
			DEBUG_PRINT("TournerDroite \n");
			//Description: virage du robot de 90 degrés à droite
			maMotricite.tournerDroite(90, 100);
			break;

		case tournerGauche:
			DEBUG_PRINT("TournerGauche \n");
			//Description: virage du robot de 90 degrés à gauche
			maMotricite.tournerGauche(90, 100);
			break;
		
		case debutBoucle:
			// emmagasiner l'adresse du présent point d'exécution du code dans une variable pour pouvoir y revenir. 
			// De plus, créer une variable qui conservera l'état du compteur de boucle. 
			// La boucle s'exécutera une fois de plus que la valeur précisée par l'opérande. 
			// Donc, si l'opérande est zéro, le code ne s'exécutera qu'une seule fois. Il ne peut y avoir deux boucles imbriquées (actives au même moment).
			if (!initialiserBoucle)
			{
				DEBUG_PRINT("DebutBoucle \n");
				addressBoucle = adresseLecture;
				compteurBoucle = operande + 1;
				positionCode = i;
				initialiserBoucle = true;
			}
			else
				DEBUG_PRINT("Boucle imbriquée");

			break;

		case finBoucle:
			//Description: si le compteur n'est pas égal à 0, retourner à l'adresse de début de boucle et décrémenter le compteur de boucle. Sinon, ne rien faire.
			if (compteurBoucle != 0)
			{
				adresseLecture = addressBoucle;
				i = positionCode;
				compteurBoucle--;
			}
			else
				initialiserBoucle = false;

			break;
		
		default:
			break;
		}

	}

	DEBUG_PRINT("Fin du programme \n");

}
