#include "main.h"
#include "cmdSet.h"

int setPinMode( int plexed ) {
  
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

int testFunc(int i) {
  USART_Transmit_str( "Func call\n" );
  return 1;
}

int verify_cmd_num( char *cmd, double *cmds_rec ) {
  int i = atoi( cmd );
  if( i == *cmds_rec ) {
    return 0;
  } else {
    return i;
  }
}

int interpret_msg( String buf, struct cmdSet *t, double *cmds_rec, int cmdSetSize )
{
  USART_Transmit_str( "[R] :: " );

  USART_Transmit_str( buf.c_str() );

  *cmds_rec++;

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
      #if VERBOSE
        USART_Transmit_str( "Number\n" );
        // delay( 10 );
 
        if( verify_cmd_num( strip, cmds_rec ) != 0 ) {
          int i = verify_cmd_num( strip, cmds_rec );
          mprintf( "ERROR: Command Numers not alligned: CP - %d, Mu - %d\n", i, *cmds_rec );
          *cmds_rec = atoi( strip );
        }
  
        if( verify_cmd_num( strip, cmds_rec ) ) {
          USART_Transmit_str( "Command number aligned\n" );
        }
      #endif
      cmdNum = verify_cmd_num( strip, cmds_rec );
      if( cmdNum == 0 ) {
        cmdNum = *cmds_rec;
      }
    } else if( cmd == 'T' ) {
      #if VERBOSE
        USART_Transmit_str( "Time\n" );
      #endif
    } else if( cmd == 'C' ) {
      #if VERBOSE
        USART_Transmit_str( "Command\n" ); 
      #endif
      char strip[ 256 ];
      strcpy( strip, buf.substring( prev + 3, del ).c_str() );
      strip[ buf.substring( prev + 3, del ).length() - 1 ] = '\0';

      #if VERBOSE
        mprintf("Command: %s\n", strip );
      #endif

      for( int i = 0; i < cmdSetSize; i++ ) {
        #if VERBOSE
          mprintf("Searching for command: %s\n", t[i].cmd_name.c_str() );
        #endif
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
              // loop through number of arguments and extract
              int prevArgDel = 0;
              for( int j = 0; j < carg; j++ ) {
                int argDel = findChar( strip, strlen( strip ), 0, ',' );
                if( argDel == -1 ) {
                  mprintf( "[N]%d~[E]Insufficient Arguments Provided\n", cmdNum );
                }
                char s[ 256 ];
                memset( s, 0x0, 256 );
                #if VERBOSE
                  mprintf("Commas: start: %d, end: %d\n", argDel, strlen( strip ) );
                #endif
                
                if( substr( strip, argDel, strlen( strip ), s ) < 0 ) {
                  mprintf( "[N]%d~[E]Substring Error\n", cmdNum );
                  break; 
                } else {
                  tmp[j] = atoi( s );
                }
              }
            }
            #if VERBOSE
              mprintf( "[N]%d~[O]%d\n", cmdNum, tmp[ 0 ] );
            #endif

            int o = t[i].f( tmp[ 0 ] );

            sprintf( cmdOutput, "%d", o );
            delete tmp;
          } else {
            mprintf( "[N]%d~[E]Invalid Number of Arguments\n", cmdNum );
            break;
          }
        }
      }

    } else {
      #if VERBOSE
        USART_Transmit_str( "Instruction not found\n" );
      #endif
    }

    prev = del + 1;
    // delay( 20 );
  }

  mprintf("[N]%d~[T]%s~[O]%s\n", cmdNum, oTime, cmdOutput );
}
