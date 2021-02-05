#ifndef __CMD_SET_H__
#define __CMD_SET_H__

typedef struct cmdSet {
  String cmd_name;
  int (*f)(int,...);
  int argIn;
};

int DigitalRead( int pin );
int AnalogRead( int pin );
int testFunc(int i);
int verify_cmd_num( char *cmd, int cmds_rec );
int interpret_msg( String buf, struct cmdSet *t, int cmds_rec, int cmdSetSize );
int mSetPinMode( int argc, ... );
int AnalogWrite( int argc, ... );
int DigitalWrite( int argc, ... );

#endif
