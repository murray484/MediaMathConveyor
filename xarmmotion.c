/************************************************
*
*  Xarm - Robot Arm Simulator/Controller
*  Version 2.0
*
*  Matt Robinson
*  York University
*  June 1992
*  
*  Version 1.0 
*  David Cowperthwaite
*
*  ---------------------------------------------
*
*  File:  XARMMOTION.C
*
*  This file contains high-level commands for moving 
*  the joints on the CRSPLUS 250.  These functions act
*  primarily as a wrapper for the SerialCommand() function.
*  This file is very arm-specific, and may not necessarily 
*  be portable to other arms.  This file remains relatively
*  unchanged since the original version except for
*  some bug fixes.
*
*  ---------------------------------------------
*
*  Global Variables Defined in This Module:
*
*  static char cmnd[CMND_LEN] - Utility string to write commands
*                               to serial port.
*
*  --------------------------------------------
*
*  Functions Defined in This Module
*
*  jog( float dX, float dY, float dZ ) -
*     This function performs a cartesian movement
*     on the arm, by interpolating the motions between
*     several joints.  (For example, lowering the shoulder
*     and the elbow at the same time, there is motion that
*     runs straight along the z-axis.  All values are 
*     relative.  The interpolation is done in the controller.
*
*  ready() -
*     Moves the arm to the ready position.
*
*  joint( int joi_numb, int degrees ) -
*     Moves the joint #joi_numb by degrees. All values are
*     relative to it's current position.
*
*  servo_open( int force ) -
*     Opens the toolgrip with force.  Since the toolgrip
*     cannot be referenced by a joint number, all commands
*     must use the servo_open(), servo_close(), and grip().
*     
*  servo_close( int force ) -
*     Closes the toolgrip using force as the closing force.
* 
*  nohelp() -
*     Turn off command completion and prompting.  This mode
*     is automatically set by rob_init() when communications
*     are opened.
*   
*  speed( int percentage ) -
*     Set the speed to percentage, where percentage is a percentage
*     of full speed.  Thus, speed( 20 ) would set the speed to
*     20% of full speed.
*
*  align() -
*     Aligns the arm with the nearest major axis.
*
*  approach( char *point, float distance ) -
*     Approaches to within distance of point, where point
*     is a pre-defined position within the controller's
*     memory.
*
*  depart( char *point ) -
*     Moves the arm the distance specified by the variable
*     named by point (the variable is stored in the 
*     controller's memory) along the tool axis.
*
*  finish() -
*     Finishes any current movement before going on to 
*     the next command in sequence.
*
*  home() -
*     Home the robot.  This command does not return the arm to
*     its home position.  Rather, it attempts to set its home 
*     position.  This should only be set once when the arm is powered
*     up each session according to the instructions in the
*     robot manual.  To move the arm to its home position,
*     use ready().
*
*  dlocn( char *location ) -
*     Delete a location from the controller's memory.
*
*  dvar( char *variable ) -
*     Delete a variable from the controller's memory.
*
*  grip( float distance ) -
*     Set the distance between the fingers on the grip
*     to float distance.  
*     WARNING - Care must be used when calling this command
*     as it uses full (100%) torque when moving the fingers.
*     If anything is caught in the gripper, then the gripper
*     could easily be burnt out, or the object damaged.
*
*  here( char *location ) -
*     Defines the current commanded robot position.
*
*  motor( int jnt, int pulses ) -
*     Performs precision movement of a joint, by moving
*     the given joint by the given number of encoder
*     pulses.  For more information, please consult the
*     _RAPL-II Programming Manual_.
*
*  move( ) -
*     Moves the robot to the given location which is 
*     stored in the controller's memory.
* 
*  moves( char *location ) -
*     Moves the robot to the given location (stored in 
*     the controller's memory) along a straight line.
*
***********************************************************/


#include <stdio.h>
#include <string.h>
# include "xarmrobot.h"
# include "flags.h"

# define CMND_LEN 1024
    
    /*
     *	The JOG command permits the operator to move the robot by a
     *	specified cartesian increment in inches (or millimeters). The
     *	move is compled using straight line motion with the gripper
     *	flange ending with the same orientation wherever possible.
     */
    static char cmnd[CMND_LEN];

move()
{
    (void) sprintf(cmnd,"%s\n",MOVE);
    return(SerialCommand(cmnd));
}

res()
{
	(void) sprintf(cmnd, "%s\n", RES);	
	double result = SerialCommandRes(cmnd);
	return result;
}










