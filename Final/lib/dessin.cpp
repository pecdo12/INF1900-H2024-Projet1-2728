#include "dessin.hpp"
Cadre::Cadre(uint8_t nLignes, uint8_t nColonnes){
    nLignes_ = nLignes ; 
    nColonnes_ = nColonnes ; 
}
uint16_t Cadre::obtenirAdresse(int8_t ligne,int8_t colonne){
    return ligne *nColonnesCadrePrincipale + colonne ;
}
uint8_t Cadre::obtenirElement(uint8_t ligne,uint8_t colonne){
    uint8_t donnee ;
    memoire_.lecture(obtenirAdresse(ligne,colonne),&donnee) ;
    _delay_ms(DELAI_MEMOIRE) ;
    return donnee ;
}
void Cadre::ajouterElement(uint8_t donnee, int8_t ligne,int8_t colonne ){
    memoire_.ecriture(obtenirAdresse(ligne,colonne),donnee) ;
    _delay_ms(DELAI_MEMOIRE) ;
}
void Cadre::dessinerCadre(bool estDessinerEspace){
    uint8_t nouvelElement ;
    for (uint8_t ligne = 0; ligne < nLignes_; ++ligne)
        {
            for (uint8_t colonne = 0; colonne < nColonnes_; ++colonne)
            {
                if ((ligne == 0) && (colonne == 0))
                {
                    nouvelElement = CLE_ANGLE_SUPERIEUR_GAUCHE;
                }
                else if ((ligne == 0) && (colonne == nColonnes_ - 1))
                {
                    nouvelElement = CLE_ANGLE_SUPERIEUR_DROIT;
                }
                else if ((ligne == nLignes_ - 1) && (colonne == 0))
                {
                    nouvelElement = CLE_ANGLE_INFERIEUR_GAUCHE;
                }
                else if ((ligne == nLignes_ - 1) && (colonne == nColonnes_ - 1))
                {
                    nouvelElement = CLE_ANGLE_INFERIEUR_DROIT;
                }
                else if (((ligne == 0) || (ligne == nLignes_ - 1)) && (1 <= colonne && colonne <= nColonnes_ - 2))
                {
                    nouvelElement = CLE_CHEMIN_HORIZONTALE;
                }
                else if ((colonne == 0 || colonne == nColonnes_ - 1) && 1<= ligne &&  ligne <= nColonnes_ - 2)
                {
                    nouvelElement = CLE_CHEMIN_VERTICAL;
                }
                else
                {
                    if(!estDessinerEspace){ // ne pas ecrire un espace dans la memoire_ s'il est deja ecrit
                        continue;
                    }
                    nouvelElement = CLE_ESPACE;
                }
                ajouterElement(nouvelElement,ligne,colonne) ;
            }
        }
}
void Cadre::signerDessin(){
    afficherChaine("Produit par : Georges le MAGNIFIQUE") ;
}
void Cadre::afficherDessin(bool estAfficherSignature){
    initialisationUART() ;
    uint8_t cle ;
    const char* symbole ;
    for(uint8_t ligne = 0 ; ligne < nLignes_ ; ++ligne){
        for(uint8_t colonne = 0 ; colonne < nColonnes_ ; ++colonne){
            cle= obtenirElement(ligne,colonne);
            symbole = Symboles[cle];
            afficherChaine(symbole) ;
        }
        afficherChaine(Symboles[CLE_SAUT_DE_LIGNE]) ; // on passe a la prochaine ligne du cadre
    }
    if(estAfficherSignature){
        signerDessin();
    }
}
uint16_t CadreInterne::obtenirAdresse(int8_t ligne,int8_t colonne){
    return (ligne + decalageLignes_)*nColonnesCadrePrincipale + decalageColonnes_ + colonne ;
}
void  CadreRobot1::dessinerSortieBoite(){
    uint8_t debutSortieBoite = nLignes_ / 2 - decalageSortieBoite ;
    uint8_t finSortieBoite = nLignes_ / 2 + decalageSortieBoite ;
    for(uint8_t ligne = debutSortieBoite ; ligne < finSortieBoite ; ++ligne){
        ajouterElement(CLE_ESPACE,ligne,nColonnes_-1) ;
    }
}
uint8_t CadreRobot1::determinerOrientation(uint8_t ligne, OrientationRobot orientation){
    uint8_t cleSymbole ;
    
    if(ligne == 1){
      if(orientation == OrientationRobot::BAS){
            cleSymbole = CLE_FLECHE_BAS ;
      }
    else{
            cleSymbole = CLE_FLECHE_HAUT ;
        }  
    }
    else if(ligne == 2){
        if(orientation == OrientationRobot::BAS){
            cleSymbole = CLE_ROUE ;
        }
        else{
            cleSymbole = CLE_ARETE_VERTICALE ;
        }
    }
    else if(ligne == 3){
        if(orientation == OrientationRobot::BAS){
            cleSymbole = CLE_ARETE_VERTICALE ;
        }
        else{
            cleSymbole = CLE_ROUE  ;
        }
    }
    return cleSymbole ;
}
void CadreRobot1::dessinerPartieRobot(uint8_t ligne, uint8_t cleSymbole1, uint8_t cleSymbole2){
    uint8_t cleSymbole = 0 ;
    for(uint8_t colonne = 0 ; colonne < LARGEUR_ROBOT ; ++ colonne ){
        if(colonne == 0 || colonne == LARGEUR_ROBOT - 1){
            cleSymbole = cleSymbole1 ;
        }
        else{
            cleSymbole = cleSymbole2 ;
        }
        ajouterElement(cleSymbole,ligne,colonneDepartRobot+colonne) ;
    }
}
void CadreRobot1::dessinerRobot(uint8_t ligneDepartRobot ,OrientationRobot orientation){
    uint8_t cleSymbole1, cleSymbole2 ;
    for(uint8_t ligne = 0 ; ligne < HAUTEUR_ROBOT ; ++ ligne){
        switch (ligne)
        {
            case 0:
                cleSymbole1 = CLE_ESPACE ; 
                cleSymbole2 = CLE_ARETE_HORIZONTALE ;
                break;
            case 1:
                cleSymbole1 = CLE_DIAGONALE_DROITE ; 
                cleSymbole2 = determinerOrientation(ligne,orientation) ;
                break;
            case 2:
                cleSymbole1 = determinerOrientation(ligne,orientation) ; // assigner une roue ou une arete verticale selon l'orientation
                cleSymbole2 = CLE_ESPACE ;
            case 3:
                cleSymbole1 = determinerOrientation(ligne,orientation) ; 
                cleSymbole2 = CLE_ESPACE ;
                break;
            case 4:
                cleSymbole1 = CLE_DIAGONALE_GAUCHE ; 
                cleSymbole2 = CLE_ARETE_HORIZONTALE ;
                break; 
            default:
                break;
        }
        dessinerPartieRobot(ligneDepartRobot + ligne,cleSymbole1,cleSymbole2) ;
    }
    ajouterElement(CLE_DIAGONALE_GAUCHE,ligneDepartRobot+1,colonneDepartRobot+LARGEUR_ROBOT-1);
    ajouterElement(CLE_DIAGONALE_DROITE,ligneDepartRobot+HAUTEUR_ROBOT-1,colonneDepartRobot+LARGEUR_ROBOT-1);
}
void CadreRobot1::dessinerBoite(){
    dessinerCadre(false) ;
    dessinerSortieBoite() ;
}
void CadreRobot1::dessinerBoiteAvecRobot(uint8_t ligneDepart , OrientationRobot orientation){
    dessinerBoite() ; 
    dessinerRobot(ligneDepart,orientation) ;
}
void CadreRobot2::dessinerSortieCircuit()
{
    ajouterElement(CLE_BANDE_DROITE,nLignesCadreRobot2/2,0) ;
    for(uint8_t decalage = 1 ; decalage < decalageEntreeCircuit ; ++ decalage){
        ajouterElement(CLE_CHEMIN_HORIZONTALE,nLignesCadreRobot2/2,-decalage) ;
    }
}
void CadreRobot2::ajouterBandeVerticale(Bande typeBande,uint8_t position ,uint8_t longueur ){
    uint8_t ligneDebutBande = 0 , ligneDebutChemin = 0; 
    if(typeBande == Bande::BANDE_SUPERIEURE){
        ligneDebutBande= 0 ; 
        ligneDebutChemin = 1 ; 
        ajouterElement(CLE_BANDE_SUPERIEURE ,ligneDebutBande,position-1)  ;
    }
    else if(typeBande == Bande::BANDE_INFERIEURE){
        ligneDebutBande = nLignes_ - 1  ; 
        ligneDebutChemin = ligneDebutBande - longueur ; 
        ajouterElement(CLE_BANDE_INFERIEURE ,ligneDebutBande,position-1)  ;
    }
    for(uint8_t decalage = 0 ; decalage < longueur ; ++ decalage){
        ajouterElement(CLE_CHEMIN_VERTICAL,ligneDebutChemin+decalage,position-1)  ;
    }
}
void CadreRobot2::dessinerCircuit(){
    dessinerCadre(false) ;
    dessinerSortieCircuit() ;
}
