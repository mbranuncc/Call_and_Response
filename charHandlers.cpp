#include "main.h"

int findChar( char *str, int strLen, int startI, char del ) {
  if( str == NULL )
    return -1;

  int ind = startI;
  while( str[ ind ] != '\0' && ind < strLen && str[ ind ] != del  )
    ind++;

  if( ind == startI ) 
    return -1;
  
  return ind;
}
