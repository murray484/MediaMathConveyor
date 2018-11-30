#define _POSIX_SOURCE 1
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>

/*
   You might have to edit the includes: instead of ../include
   use /cs/fac/src/MediaMath/include and instead of ../ImageBasics
   use /cs/fac/src/MediaMath/ImageBasics
*/

#include <media_all.h>
#include <one-and-two-D.h>

#include "declarations.h"
#include "proc.h"

char *SerialPortName;
int SetTermFlag;

/*
   If you want to use functions from other modules then you include
   them in the import header
*/

/*awkimportstart***********************************************************
Module: imagemath.so
Symbol: fimgTypenum
Symbol: fmatTypenum
Symbol: fsclnTypenum
Symbol: NonPosSize
Function: mksimple_ftmpl
Function: mksimple_ucscln
***********************************************************awkimportstop*/

/*awkvarstart**************************************************************
Name: ConError
Doc: Error message. An error condition from the port connected to the robot.
Doc: Usualy comes from communication error.
Value: 
**************************************************************awkvarstop*/
/*awkvarstart**************************************************************
Name: ConIsOn
Doc: Global variable. It is t when the link to the robot is on.
Value: 
**************************************************************awkvarstop*/


/*awkstart**************************************************************
Name: conveyor_init_lisp
MinArgs: 0
MaxArgs: 0
Type: function
Synopsis: conveyor_init()
Doc: Opens communications with the CRS arm. Sets some usefull defaults
SEE: conveyor_quit
Example: conveyor_init();
**************************************************************awkstop*/

L_Ptr conveyor_init_lisp()
{
  if (!NULLP(boundp(SConIsOn)))
    error_signal(Sconveyor_init_lisp,SConError,SConIsOn,NIL);
  SerialPortName = "/dev/ttyUSB0";
  SetTermFlag = 1;
  if (conveyor_init())
    error_signal(Sconveyor_init_lisp,SConError,NIL,NIL);
  set(SConIsOn, St);
  return NIL;
}

/*awkstart**************************************************************
Name: conveyor_quit_lisp
MinArgs: 0
MaxArgs: 0
Type: function
Synopsis: conveyor_quit()
Doc: Close the connection to the robot
SEE: conveyor_init
Example: conveyor_quit()
**************************************************************awkstop*/

L_Ptr conveyor_quit_lisp()
{
  if (NULLP(boundp(SConIsOn)))
    error_signal(Sconveyor_quit_lisp,SConError,SConIsOn,NIL);
  conveyor_quit();
  makunbound(SConIsOn);
  return NIL;
}

/*awkstart**************************************************************
Name: write_cmnd_lisp
MinArgs: 1
MaxArgs: 1
Type: function
Synopsis: write_cmnd(<str>)
Doc: Write a command <str> to the output port. <str> is a string.
Doc: It then checks for reply. It returns t on success.
SEE: rob_init
Example: write_cmnd("READY");
**************************************************************awkstop*/

L_Ptr write_cmnd_lisp(str)
     L_Ptr str;
{
  char *s;
  if (NULLP(boundp(SConIsOn)))
    error_signal(Swrite_cmnd_lisp,SConError,SConIsOn,NIL);

  if (!STRNGP(str))
    error_signal(Swrite_cmnd_lisp,SWrongTypeArg,Sstringp,NIL);
  
  s = XSTRNG(str);
  write_cmnd(s);
  return NIL;
}

/*awkstart**************************************************************
Name: move_lisp
MinArgs: 0
MaxArgs: 0
Type: function
Synopsis: move()
Doc: Performs a cartesian movement on the arm, by interpolating the
Doc: motions between several joints. <float1>,<float2>,<float3> represent
Doc: relative displacements for axes X, Y, Z. The interpolation is done
Doc: in the CRS controller.
SEE: joint
Example: move();
**************************************************************awkstop*/

L_Ptr move_lisp()
{

  if (NULLP(boundp(SConIsOn)))
    error_signal(Smove_lisp ,SConError, SConIsOn, NIL);


  if (!move())
    error_signal(Smove_lisp,SConError,NIL,NIL);
  return NIL;
}

/*awkstart**************************************************************
Name: res_lisp
MinArgs: 0
MaxArgs: 0
Type: function
Synopsis: res()
Doc: Performs a cartesian movement on the arm, by interpolating the
Doc: motions between several joints. <float1>,<float2>,<float3> represent
Doc: relative displacements for axes X, Y, Z. The interpolation is done
Doc: in the CRS controller.
SEE: joint
Example: res();
**************************************************************awkstop*/

L_Ptr res_lisp()
{
	Float result = -1;
  //if (NULLP(boundp(SRobIsOn)))
   // error_signal(Sres_lisp ,SRobotError, SRobIsOn, NIL);


  //if (!(result = res()))
  //  error_signal(Sres_lisp,SRobotError,NIL,NIL);
	result = res();
  return C_2_L_float((Float)result);
}


