/*********************************************************************************
 *  Arduino Serial Interrupt Example
 *  http://umeshdutta.blogspot.com/2018/07/serial-receive-interrupt-programming-in.html
 *  http://www.cplusplus.com/reference/vector/vector/vector/
 *  https://github.com/janelia-arduino/Vector <- Must be installed through Manage Libraries before compilation
 *  https://www.robotshop.com/community/forum/t/arduino-101-timers-and-interrupts/13072
 */

#include "main.h"

/***************************************************************
 * Function prototypes which couldn't get their own cpp files
 * because Arduino IDE is finicky...
 */

 void Timer1_Init( unsigned int hz );

 ////////////////////////////////////////////////////////////////

// temp var used in ISR Routine for USART capture
char temp;
char storage_array[ ELEMENT_COUNT ];

String buf;
bool Ready2Print = false;

double cmds_rec = 1;

struct cmdSet t[ 2 ] = {
  { "aRead", AnalogRead },
  { "dRead", DigitalRead } 
};
int cmdSetSize = 2;



void setup() {
  // put your setup code here, to run once:
  USART_Init( MYUBRR );
  
  pinMode( LED, OUTPUT );
  Timer1_Init( 1 );

  pinMode( TURBIDITY, INPUT );
  pinMode( PH, INPUT );
}

void loop() {
  // put your main code here, to run repeatedly:
  if( Ready2Print ) {

    interpret_msg( buf, t, &cmds_rec, cmdSetSize );
      
    buf.remove( 0, buf.length() );
    Ready2Print = false;
  }
}


/***************************************************8
 * For some silly reason, the compile won't allow the 
 * noInterrupts and interrupts commands to be outside 
 * of the main file. So here they are...
 */
void USART_Init( unsigned int ubrr )
{
  noInterrupts();
  
  UBRR0 = ubrr;
  UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); 
  // Use 8-bit character sizes
  UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);  
  // Turn on the transmission, reception, and Receive interrupt 
   
  interrupts();// enable global interrupt
}

void USART_Transmit( char data ) 
{
  noInterrupts();
  while( !( UCSR0A & (1<<UDRE0)) );
  
  UDR0 = data;
  interrupts();
}
//////////////////////////////////////////////////////////////////////////

/***********************************************************************
 * Timer related stuff. Prototype is above
 */

void Timer1_Init( unsigned int hz )
{
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  OCR1A = FOSC/256/hz;

  TCCR1B |= ( 1 << WGM12 ); // CTC mode
  TCCR1B |= ( 1 << CS12 ); // 256 prescaler
  TIMSK1 |= ( 1 << OCIE1A ); // enable timer compare interrupt

  interrupts();
}


/****************************************************************
 * Interrupt routines because they probably won't outside of the 
 * the arduino IDE...Need to work on external toolchain
 */

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
  digitalWrite(LED, !digitalRead(LED));   // toggle LED pin
}

ISR(USART_RX_vect)
{ 
  temp=UDR0;// read the received data byte in temp
  buf += temp;
  if( strcmp( temp, '\n' ) == 0 ) Ready2Print = true;
}
