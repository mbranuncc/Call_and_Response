#include "main.h"
#include "cmdSet.h"

int realignCmdsRec( int num ) {
  //need to figure out how to change this
}

int mSetPinMode( int argc, ... ) {
  if( argc != 2 ) return -1;
  
  va_list valist;

  va_start( valist, argc );

  int holder[2];
  for( int i = 0; i < argc; i++ ) {
    holder[i] = va_arg( valist, int );
  }

  if( holder[ 1 ] == 1 ) {
    #if VERBOSE
      mprintf("mSetPinMode: pin %d to INPUT\n", holder[ 0 ] );
    #endif
    pinMode( holder[ 0 ], INPUT );
  } else if( holder[ 1 ] == 0 ) {
    #if VERBOSE
      mprintf("mSetPinMode: pin %d to OUTPUT\n", holder[ 0 ] );
    #endif
    pinMode( holder[ 0 ], OUTPUT );
  }

  return 1;
}

int DigitalRead( int pin ) {
  #if VERBOSE
    mprintf("DigitalRead: %d\n", pin );
  #endif
  return digitalRead( pin );
}

int AnalogRead( int pin ) {
  #if VERBOSE
    mprintf("AnalogRead: %d\n", pin );
  #endif
  return analogRead( pin );
}

int AnalogWrite( int argc, ... ) {
  if( argc != 2 ) return -1;
  
  va_list valist;

  va_start( valist, argc );

  int holder[2];
  for( int i = 0; i < argc; i++ ) {
    holder[i] = va_arg( valist, int );
  }

  if( holder[ 1 ] == 1 ) {
    #if VERBOSE
      mprintf("AnalogWrite: pin %d to %d\n", holder[ 0 ], holder[ 1 ] );
    #endif
    analogWrite( holder[ 0 ], holder[ 1 ] );
  } else if( holder[ 1 ] == 0 ) {
    #if VERBOSE
      mprintf("AnalogWrite: pin %d to %d\n", holder[ 0 ], holder[ 1 ] );
    #endif
    analogWrite( holder[ 0 ], holder[ 1 ] );
  }

  return 1;
}

int DigitalWrite( int argc, ... ) {
  if( argc != 2 ) return -1;
  
  va_list valist;

  va_start( valist, argc );

  int holder[2];
  for( int i = 0; i < argc; i++ ) {
    holder[i] = va_arg( valist, int );
  }

  if( holder[ 1 ] == 1 ) {
    #if VERBOSE
      mprintf("DigitalWrite: pin %d to %d\n", holder[ 0 ], holder[ 1 ] );
    #endif
    digitalWrite( holder[ 0 ], holder[ 1 ] );
  } else if( holder[ 1 ] == 0 ) {
    #if VERBOSE
      mprintf("DigitalWrite: pin %d to %d\n", holder[ 0 ], holder[ 1 ] );
    #endif
    digitalWrite( holder[ 0 ], holder[ 1 ] );
  }

  return 1;
}

int verify_cmd_num( char *cmd, int cmds_rec ) {
  int i = atoi( cmd );
//  if( i == cmds_rec ) {
//    return 0;
//  } else {
//    return i;
//  }
  return i;
}

int interpret_msg( String buf, struct cmdSet *t, int cmds_rec, int cmdSetSize )
{
  mprintf( "[R] :: %s", buf.c_str() );

  //*cmds_rec = *cmds_rec + 1;

  // vars to be used on output
  char oTime[256] = {0};
  int cmdNum = -1;
  char cmdOutput[256] = {0};

  // parse commands
  int del = 0;
  int prev = 0;
  while( del < buf.length() && del != -1 ) {
    del = findChar( buf.c_str(), buf.length(), del + 1, '~' );
    if( del == -1 ) break;

    // find command type
    char cmd = buf[ prev + 1 ];
    if( cmd == 'N' ) {
      char strip[ 10 ];
      strcpy( strip, buf.substring( prev + 3, del ).c_str() );

      cmdNum = verify_cmd_num( strip, cmds_rec );
    } else if( cmd == 'T' ) {
      int tmp = findChar( buf.c_str(), buf.length(), 0, '~' );
      if( tmp == -1 ) break;

      strcpy( oTime, buf.substring( tmp + 1, del - 1  ).c_str() );
    } else if( cmd == 'C' ) {
      char strip[ 256 ];
      strcpy( strip, buf.substring( prev + 3, del ).c_str() );
      strip[ buf.substring( prev + 3, del ).length() - 1 ] = '\0';

      for( int i = 0; i < cmdSetSize; i++ ) {
        if( strstr( strip, t[i].cmd_name.c_str() ) != NULL ) {
          // word verified to exist, find the requisite number of arguments
          int carg = t[i].argIn;
          if( carg > -1 ) {
            // number of arguments is proper
            // create variable to hold arguments
            int *tmp = new int[carg]; 
            if( tmp == NULL ) {
              mprintf( "[N]%d~[E]Failed Dynamic Allocation\n", cmdNum );
              break;
            } else {          
              // loop through number of arguments and find n-1 delimiters
              int argDel = -1;
              int delArr[10]; //this should be made dynamic
              memset( delArr, -1, 10 * sizeof( int ) );
              for( int j = 0; j < carg; j++ ) {
                argDel = findChar( strip, strlen( strip ), argDel + 1, ',' );
                if( argDel == -1 ) {
                  mprintf( "[N]%d~[E]Insufficient Arguments Provided\n", cmdNum );
                  break;
                }
                delArr[ j ] = argDel;
              }

              // loop through and create substrings which are converted to
              // integers and stored in a dynamic array
              for( int o=0; o < 10-1; o++ ) {
                if( delArr[ o ] != -1 ) {
                  char r[ 256 ] = { 0 };
                  int st = delArr[ o ], ss = delArr[ o + 1 ];
                  if( ss == -1 ) ss = strlen( strip );
                  if( substr( strip, st, ss, r ) == -1 ) {
                    mprintf("[N]%d~[E]Substrings Error\n", cmdNum );
                    break;
                  } else {
                    tmp[ o ] = atoi( r );
                  }
                }
              }

              int o = 0;
              switch( carg ) {
                case 1:
                  o = t[i].f( tmp[ 0 ] );
                  break;
                case 2:
                  o = t[i].f( carg, tmp[ 0 ], tmp[ 1 ] );
                  break;
                case 3:
                  o = t[i].f( carg, tmp[ 0 ], tmp[ 1 ], tmp[ 2 ] );
                  break;
                default:
                  break;
              }
  
              sprintf( cmdOutput, "%d", o );
              delete tmp;
            }
          } else {
            mprintf( "[N]%d~[E]Invalid Number of Arguments\n", cmdNum );
            break;
          }
        }
      }

    } else {
      #if VERBOSE
        mprintf( "Instruction not found\n" );
      #endif
    }

    prev = del + 1;
    // delay( 20 );
  }

  mprintf("[N]%d:%d~[T]%s~[O]%s\n", cmds_rec, cmdNum, oTime, cmdOutput );

  return cmdNum;
}
