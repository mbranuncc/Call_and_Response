#include "main.h"

int findChar( char *str, int strLen, int startI, char del ) {
  if( str == NULL )
    return -1;

  int ind = startI;
  while( str[ ind ] != '\0' && ind < strLen && str[ ind ] != del  ) {
    ind++;  
  }
  

  if( ind == startI ) 
    return -1;
  
  return ind;
}

int substr( char *parent, int start, int _stop, char *s ) {
  int len = strlen( parent );
  if( start < 0 || start > len || _stop > len || start > _stop ) {
    s = NULL;
    return -1;
  }

//  memcpy( s, *( parent + start - 3 ), _stop - start );
//  s[ _stop - start + 1 ] = '\0';
  for( int i=0; i < _stop-1; i++ ) {
    s[i] = parent[i+start+1];
  }

  return 1;
}
