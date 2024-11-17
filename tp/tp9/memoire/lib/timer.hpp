/*
Auteurs: Mohamed Omar Zedek, Sejjari Souhayl, Cristescu Sebastian, Mohamed Yassir Merzouki
Description: 
Identifications matérielles: 
*/


#include "Global.hpp"

enum class Unite
{
    SECS,
    MS
};


enum class NumeroTimer
{
    TIMER0,
    TIMER1,
    TIMER2
};

const unsigned long int freqCPU = 8'000'000 ;
const unsigned  int prescaler = 1024 ;
const unsigned long int freqAvecPrescaler = freqCPU / prescaler ; // = frequence du TimerCTC
const uint16_t SECTOMS = 1000 ; 
const int DUREE_1_UNITE = 1 ; 


// TimerCTC 1 : secondes et millisecondes possibles
// TimerCTC 0 / TimerCTC 2 : millisecondes possibles uniquement

template <typename T>
class TimerCTC
{
    public:
        TimerCTC() = default ;
        TimerCTC(NumeroTimer numeroTimer, Unite uniteDeTemps);         
        Unite getUniteTemps(){return uniteDeTemps_ ;}        
        void setUniteTemps(Unite nouvelleUniteTemps){ uniteDeTemps_ = nouvelleUniteTemps ; }
        

        void demarrerChronometre(void); 
        void arreterChronometre(void); 
        void reprendreChronometre(void);

        // TimerCTC 16 bits : max 8s / max 8000ms
        // timers 8 bits : max 36 ms
        void lancerEvenementIntervalle(double intervalle);  

        friend class TimerCTC8Bits ;
        friend class TimerCTC16Bits ;

    private:
        Unite uniteDeTemps_ ;
        NumeroTimer numeroTimer_ ; 
        int nCycles_ ; 
        int cycleDepart_ ; 
        volatile uint8_t* registreControleA_  ; 
        volatile uint8_t* registreControleB_  ; 
        volatile uint8_t* registreInterruption_  ; 
        volatile T* compteur_ ;  
        volatile T* valeurComparaison_ ;           
        
        virtual void initialiserTimer(void); // lancer le TimerCTC a partir de cycleDepart_         
        void convertirTempsEnCycles(double duree); // convertir la duree et l'unite de temps -> en nombre de cycles (voir frequence prescaler)
};


class TimerCTC8Bits : public TimerCTC<uint8_t> {
    public :
        TimerCTC8Bits(NumeroTimer numeroTimer , Unite uniteDeTemps) ;
    private : 
        void initialiserTimer(void); // lancer le TimerCTC a partir de cycleDepart_ 


} ;



class TimerCTC16Bits : public TimerCTC<uint16_t> {
    public :
        TimerCTC16Bits(NumeroTimer numeroTimer , Unite uniteDeTemps) ;
    private : 
        void initialiserTimer(void); // lancer le TimerCTC a partir de cycleDepart_ 

} ;