#include "Global.hpp"
#include "can.h"
#include "Motricite.hpp"
#include "debug.h"
#include "ReceptionIR.hpp"

class Robot1Parcours{

public:
    Robot1Parcours(can* convertisseur, Motricite* maMotricite, Moteur* moteurD, Moteur* moteurG) {
        convertisseur_ = convertisseur;
        maMotricite_ = maMotricite;
        moteurD_ = moteurD;
        moteurG_ = moteurG;
    };

    uint8_t moyenneDistance();
    void positionnerCentre();
    uint8_t trouverTrou();

private:
    can* convertisseur_;
    Motricite* maMotricite_;
    Moteur* moteurG_;
    Moteur* moteurD_;
};
