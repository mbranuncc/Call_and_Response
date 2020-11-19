%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% octave_serial_callback.m
%
% If running for the first time, it may be necessary to uncomment 
% the line which installs the instrument-control package which is
% needed to control com ports.
%
% DON'T FORGET TO UPDATE port VARIABLE!!
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% pkg install -forge instrument-control

clc; clear all; close all

pkg load instrument-control

port = "COM5";

s1 = serialport( port, 9600 );
flush( s1 );

fwrite( s1, [ "a\n" ] );

cmd_num = 1;
while 1
  i = input( "Input: ", "s" );
  if( strcmp( "q", i ) == 1 )
    break;
  endif
  
  tm = num2str( mktime( localtime( time() ) ) );
  tm = erase( tm, "\n" );
  % tm = "1000: 2000: 3000";
  fprintf( s1, "[N]%d~[T]%s~[C]%s\n", cmd_num, tm, i );
  pause( 0.5 );
  
  [ data, cnt, errmsg ] = fread( s1 );

  char( data )
  cmd_num++;
endwhile

clear all