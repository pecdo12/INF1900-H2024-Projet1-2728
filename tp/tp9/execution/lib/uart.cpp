#include "Global.hpp"
#include "uart.hpp"

void initialisationUART(void)
{

	UBRR0H = 0;
	UBRR0L = 0xCF;

	UCSR0A = (1 << UDRE0);
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);


	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void transmissionUART(uint8_t donnee)
{
	while (!( UCSR0A & (1 << UDRE0)));
	UDR0 = donnee;
}

uint8_t receptionUART(void)
{
	while (!(UCSR0A & (1 << RXC0)));
	return UDR0;
}