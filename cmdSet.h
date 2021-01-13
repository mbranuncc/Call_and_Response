#ifndef __CMD_SET_H__
#define __CMD_SET_H__

typedef struct cmdSet {
  String cmd_name;
  int (*f)(int);
  int argIn;
};

int DigitalRead( int pin );
int AnalogRead( int pin );
int testFunc(int i);
int verify_cmd_num( char *cmd, double *cmds_rec );
int interpret_msg( String buf, struct cmdSet *t, double *cmds_rec, int cmdSetSize );

#endif
