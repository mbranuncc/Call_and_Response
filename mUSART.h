#ifndef __mUSART_H__
#define __mUSART_H__

#include "main.h"
#include <avr/interrupt.h>
#include <avr/io.h>

#define FOSC 16000000
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD - 1

void USART_Init( unsigned int ubrr );
void USART_Transmit( char data );
int USART_Transmit_str( char *data );
int mprintf( const char *format, ... );

#endif
