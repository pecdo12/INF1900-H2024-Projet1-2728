/*
Auteurs: Mohamed Omar Zedek, Sejjari Souhayl, Cristescu Sebastian, Mohamed Yassir Merzouki
Description: Permet d'utiliser des macros DEBUG qui peuvent être insérer dans des programmes qu'on utilise pour voir la valeur de variables. Ceux-ci peuvent être activer en utilisant make debug.
Identifications matérielles: Ces fonctions utilisent les fonctions dans UART pour communiquer avec le microcontroleur. Donc, ils possèdent les mêmes restrictions.
*/

#include "Global.hpp"
#include "uart.hpp"

void initDebug();
void afficherDebug(const char* str);
void valeurDebug(uint8_t value);
void largeValeurDebug(uint16_t value);


#ifdef DEBUG

#define DEBUG_INIT() initDebug();
#define DEBUG_PRINT(str) afficherDebug(str);
#define DEBUG_VALUE(value) valeurDebug(value);
#define DEBUG_LARGE_VALUE(value) largeValeurDebug(value);

#else

#define DEBUG_INIT() {}
#define DEBUG_PRINT(str) {}
#define DEBUG_VALUE(value) {}
#define DEBUG_LARGE_VALUE(value) {}

#endif

