#include "mUSART.h"
#include "main.h"

int USART_Transmit_str( char *data ) 
{
  char *tmp = data;
  while( *tmp != '\0' )
  {
    USART_Transmit( *tmp );
    tmp++;
  }
  delay( 5 );
  return 1;
}

int mprintf( const char *format, ... )
{
  char buf[256];
  va_list args;
  va_start( args, format );
  vsprintf( buf, format, args );
  USART_Transmit_str( buf );
  va_end( args );
}
