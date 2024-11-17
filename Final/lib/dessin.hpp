#include "Global.hpp"
#include "debug.h"
#include "memoire.hpp"
#include <stdint.h>
enum CleSymboles : int8_t {
    CLE_CHEMIN_HORIZONTALE,
    CLE_CHEMIN_VERTICAL,
    CLE_ANGLE_SUPERIEUR_GAUCHE,
    CLE_ANGLE_SUPERIEUR_DROIT,
    CLE_ANGLE_INFERIEUR_GAUCHE,
    CLE_ANGLE_INFERIEUR_DROIT,
    CLE_BANDE_GAUCHE,
    CLE_BANDE_DROITE,
    CLE_BANDE_INFERIEURE,
    CLE_BANDE_SUPERIEURE,
    CLE_ESPACE,
    CLE_SAUT_DE_LIGNE,
    CLE_ARETE_HORIZONTALE,
    CLE_ARETE_VERTICALE,
    CLE_DIAGONALE_GAUCHE,
    CLE_DIAGONALE_DROITE,
    CLE_ROUE,
    CLE_FLECHE_BAS,
    CLE_FLECHE_HAUT,
    TOTAL_CLES // This should be the last element and represents the total number of keys
};
const char Symboles[TOTAL_CLES][5] = {
    "\u2550", // CLE_CHEMIN_HORIZONTALE
    "\u2551", // CLE_CHEMIN_VERTICAL
    "\u2554", // CLE_ANGLE_SUPERIEUR_GAUCHE
    "\u2557", // CLE_ANGLE_SUPERIEUR_DROIT
    "\u255A", // CLE_ANGLE_INFERIEUR_GAUCHE
    "\u255D", // CLE_ANGLE_INFERIEUR_DROIT
    "\u2560", // CLE_BANDE_GAUCHE
    "\u2563", // CLE_BANDE_DROITE
    "\u2569", // CLE_BANDE_INFERIEURE
    "\u2566", // CLE_BANDE_SUPERIEURE
    " ",      // CLE_ESPACE
    "\n",     // CLE_SAUT_DE_LIGNE
    "\u23AF", // CLE_ARETE_HORIZONTALE
    "\u23A2", // CLE_ARETE_VERTICALE
    "\u2572", // CLE_DIAGONALE_GAUCHE
    "\u2571", // CLE_DIAGONALE_DROITE
    "\u2592", // CLE_ROUE
    "\u15D0", // CLE_FLECHE_BAS
    "\u15D1"  // CLE_FLECHE_HAUT
};
// Cadre Principale
const uint8_t nLignesCadrePrincipale  = 26 ; 
const uint8_t nColonnesCadrePrincipale = 101 ;
// Cadre Robot 1 
const uint8_t nLignesCadreRobot1  = 22 ; 
const uint8_t nColonnesCadreRobot1 = 30 ; 
const uint8_t decalageLignesRobot1 = 2 ; 
const uint8_t decalageColonnesRobot1 = 7 ; 
 
const uint8_t decalageSortieBoite = 2 ;
const uint8_t ligneDepartRobot = 15 ; // temporaire car change selon la position
const uint8_t colonneDepartRobot = nColonnesCadreRobot1/2 - 2 ;
const uint8_t HAUTEUR_ROBOT = 5 ;
const uint8_t LARGEUR_ROBOT = 4 ;
// Cadre Robot 2
const uint8_t nLignesCadreRobot2  = 18 ; 
const uint8_t nColonnesCadreRobot2 = 50 ; 
const uint8_t decalageLignesRobot2 = 4 ; 
const uint8_t decalageColonnesRobot2 = 45 ; 
const uint8_t decalageEntreeCircuit= 6 ;
enum class Bande {BANDE_INFERIEURE,BANDE_SUPERIEURE} ;
enum class OrientationRobot {HAUT,BAS} ;
const uint8_t DELAI_MEMOIRE  = 1 ;
class CadreRobot1 ;
class Cadre{
    public:
        Cadre() = default ; 
        Cadre(uint8_t nLignes, uint8_t nColonnes);
        virtual uint16_t obtenirAdresse(int8_t ligne,int8_t colonne);
        uint8_t obtenirElement(uint8_t ligne,uint8_t colonne); 
        void ajouterElement(uint8_t donnee, int8_t ligne,int8_t colonne );
        void dessinerCadre(bool estDessinerEspace) ;
        void signerDessin(); 
        void afficherDessin(bool estAfficherSignature);
        friend class CadreRobot1 ;
        friend class CadreRobot2 ;
    private:
        uint8_t nLignes_, nColonnes_ ;
        Memoire24CXXX memoire_ ; 
};
class CadreInterne : public Cadre {
    public:
        CadreInterne() = default ;
        CadreInterne(uint8_t nLignes, uint8_t nColonnes,uint8_t decalageLignes,uint8_t decalageColonnes)
            :Cadre(nLignes,nColonnes)
            {
                decalageLignes_   = decalageLignes ;
                decalageColonnes_ = decalageColonnes ; 
            }
        uint16_t obtenirAdresse(int8_t ligne,int8_t colonne) override ;
    private:
            uint8_t decalageLignes_,decalageColonnes_;        
} ;
class CadreRobot1 : public CadreInterne {
    
    public:
        CadreRobot1() = default ;
        CadreRobot1(uint8_t nLignes, uint8_t nColonnes,uint8_t decalageLignes,uint8_t decalageColonnes)
             :CadreInterne(nLignes,nColonnes,decalageLignes,decalageColonnes) 
            {}
                
        void dessinerSortieBoite() ;
        uint8_t determinerOrientation(uint8_t symbole, OrientationRobot orientation);
        void dessinerPartieRobot(uint8_t ligne, uint8_t cleSymbole1, uint8_t cleSymbole2) ;
        void dessinerRobot(uint8_t ligneDepartRobot ,OrientationRobot orientationRobot);
        void dessinerBoite();
        void dessinerBoiteAvecRobot(uint8_t ligneDepart , OrientationRobot orientation);
} ;
class CadreRobot2 : public CadreInterne {
    
    public:
        CadreRobot2() = default ;
        CadreRobot2(uint8_t nLignes, uint8_t nColonnes,uint8_t decalageLignes,uint8_t decalageColonnes)
             :CadreInterne(nLignes,nColonnes,decalageLignes,decalageColonnes) 
            {}
                
        void ajouterBandeVerticale(Bande typeBande,uint8_t position ,uint8_t longueur ) ;
        void dessinerSortieCircuit();
        void dessinerCircuit() ; 
} ;