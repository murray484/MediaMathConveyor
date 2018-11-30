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
*  File:  XARMOPEN.C
*
*  This file contains the commands necessary to interface
*  with the standard terminal line of the CRSPLUS 250 arm.
*  This file is specific to this arm, but given its general
*  serial port handling, it could easily be modified to handle
*  different connections.  This file has been modified
*  slightly since the original version, adding a #define
*  to allow control of the pause between character transmission
*  and support for the flags to allow specification of the 
*  serial port.
*
*  ---------------------------------------------
*
*  Global Variables Defined in This Module:
*
*  static int fd - A file descriptor with its scope limited to
*                  this file.  Upon opening connections with the
*                  arm
*
*  --------------------------------------------
*
*  Functions Defined in This Module
*
*  rob_init() -
*     Opens communications with the CRS arm.  It refers to the
*     SerialPortName flag variable to determine which serial
*     port to use.
*
*  rob_quit() -
*     Closes communications with the arm.
*
*  write_cmnd( char *cmnd ) -
*     Writes the string cmnd to the arm's serial port,
*     and then checks for a reply.  In this case, the
*     command checks for the arm controller's prompt.  
*     CHANGE FROM ORIGINAL: In the original version,
*     write_cmnd() returned NULL if it succeded.
*     It now returns NULL on an error and a non-zero
*     value upon success.
*
*  read_resp( char *cmnd ) - 
*     Reads a response from the arm controller.  This function
*     is called by write_cmnd().
*
*  int compare_resp( char *buf, char *cmnd ) -
*     Compares response from controller and original 
*     command to determine if an error message was sent.
*     This function is called by read_resp().
*
***********************************************************/


# include <stdio.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <sys/file.h>
# include <sys/termios.h>
# include <termios.h>
# include <unistd.h>
#define _POSIX_C_SOURCE 199309
#include <time.h>
# include "flags.h"

# define  CHAR_PAUSE 50000
# define  SERIAL_LINE "/dev/ttyS0"

int fd;

/*
 * initialize the line to the robot at 9600 buad using the line specified.
 * This is the ``text-line'' currently
 */
conveyor_init()
{
  struct termios newterm;
  int	ret;

  if ( (fd=open(SerialPortName,O_RDWR)) < 0 )  {
    (void) fprintf(stderr, "Unable to open %s.\n", SerialPortName);
/*    DisplayMessage( "Unable to open communications." );*/
    return(-1);
  }

  if ( SetTermFlag )
    {
      if(tcgetattr(fd, &newterm) < 0){
		/*    DisplayMessage("TCGETA ioctl failure");*/
		(void) close(fd);
		return(-1);
      }
      
      /*
      newterm.c_iflag |= ( IGNBRK | IGNPAR | ISTRIP );
      newterm.c_cflag |= ( CREAD | CLOCAL );
      newterm.c_lflag = 0;
      newterm.c_cc[VMIN] = 1;
      newterm.c_cc[VTIME] = 0;
      */
      
      if (cfsetispeed(&newterm,B9600)<0) {
		(void) close(fd);
		return(-1);
      }
      if (cfsetospeed(&newterm,B9600)<0) {
		(void) close(fd);
		return(-1);
      }
      /* 8 bits, no parity, no stop bits */
	  newterm.c_cflag &= ~PARENB;
	  newterm.c_cflag &= ~CSTOPB;
	  newterm.c_cflag &= ~CSIZE;
	  newterm.c_cflag |= CS8;
	  /* no hardware flow control */
	  newterm.c_cflag &= ~CRTSCTS;
	  /* enable receiver, ignore status lines */
	  newterm.c_cflag |= CREAD | CLOCAL;
	  /* disable input/output flow control, disable restart chars */
	  newterm.c_iflag &= ~(IXON | IXOFF | IXANY);
	  /* disable canonical input, disable echo,
	  disable visually erase chars,
	  disable terminal-generated signals */
	  newterm.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	  /* disable output processing */
	  newterm.c_oflag &= ~OPOST;
	 
	  /* wait for 12 characters to come in before read returns */
	  /* WARNING! THIS CAUSES THE read() TO BLOCK UNTIL ALL */
	  /* CHARACTERS HAVE COME IN! */
	  newterm.c_cc[VMIN] = 1;
	  /* no minimum time to wait before read returns */
	  newterm.c_cc[VTIME] = 0;
     
	  /*
		newterm.c_iflag = newterm.c_oflag = newterm.c_lflag = (ushort) 0;
		newterm.c_cflag = (IGNPAR | IGNBRK | ISTRIP | CREAD | CLOCAL);
		newterm.c_cflag &= ~CBAUD;
		newterm.c_cflag |=  B9600;
		newterm.c_cflag |=  CSTOPB;
		newterm.c_cflag |= CS8;
		newterm.c_cc[VMIN] = 1;
		newterm.c_cc[VTIME] = 0;
		newterm.c_oflag |= INLCR;
      */
      
      if(tcsetattr(fd, TCSANOW, &newterm) < 0){
		/*    DisplayMessage("TCSETA ioctl failure");*/
		(void) close(fd);
		return (-1);
      }
    }
  
# ifdef DEBUG
  printf("device %s opened\n",SERIAL_LINE);
# endif
  return (0);
}

/*
 * close the fd
 */
conveyor_quit()
{
  (void) close(fd);
}

/*
 * Write a command to the output port.
 * 'cmnd' is assumed to be a null-terminated string
 */

write_cmnd(cmnd)
     char *cmnd;
{

  char *ptr;
  int ret, done = 1;
  int count = 0;
  struct timespec interv;

  /* The serial interface on the robot sucks. It can't
   * work very fast and it drops chars. Best to write
   * one at a time.
   */

  interv.tv_sec = 0;
  interv.tv_nsec= CHAR_PAUSE*1000;

  while( done != 0 )
  {
    count++;
    for(ptr=cmnd;*ptr;ptr++)
	{
	  nanosleep(&interv,NULL);
	  if(write(fd, ptr, 1) != 1){
	    /*      DisplayMessage("incomplete write");*/
	    return(0);
	  }
	}
      
    if(write( fd, "\n", 1) != 1)
	{
	  /*    DisplayMessage("can't write cr");*/
	  return(0);
	}

    done = read_resp(cmnd);
    if( count >= 1 ) done = 0;
  }
  return(1);
}

double get_double(str)
	char *str;
{
    /* First skip non-digit characters */
    /* Special case to handle negative numbers and the `+` sign */
    while (*str && !(isdigit(*str) || ((*str == '-' || *str == '+') && isdigit(*(str + 1)))))
        str++;
    printf("%s\n", str);
    double d;
    sscanf(str, "%lf", &d);
    /* The parse to a double */
    return d;
    
}

double read_val_resp(cmnd)
char *cmnd;
{
	char *ptr, *tempPtr, buf[1024], temp[1024];
	int nump;
	
	/*
	for(nump=0,ptr=buf;nump<15;ptr++)
	{
		
		if(read(fd, ptr, 1) != 1)
		   return -1;
		//if( ( *ptr == '>' ) || ( *ptr == 'J' ) )
		nump++;
	}
	*ptr = '\0';*/
	
	read(fd,buf,25);
	printf("%s\n", buf);
	double result2 = get_double(buf);
	printf("RESULT: %s\n", result2);
	/* any  response? */
	return(result2);	
	/*
	char buffer[30];
	int n = read(fd, buffer, sizeof(buffer));
	
	printf(buf);
	
	return 3.2;//get_double(buf);*/
}

double write_cmnd_return(cmnd)
     char *cmnd;
{

  char *ptr;
  int ret = 1;
  double done = 0;
  int count = 0;
  struct timespec interv;

  /* The serial interface on the robot sucks. It can't
   * work very fast and it drops chars. Best to write
   * one at a time.
   */

  interv.tv_sec = 0;
  interv.tv_nsec= CHAR_PAUSE*1000;

  //while( done == 0 )
  //{
   // count++;
   printf("%s\n", "Before Send");
    for(ptr=cmnd;*ptr;ptr++)
	{
	  nanosleep(&interv,NULL);
	  if(write(fd, ptr, 1) != 1){
	    return(0);
	  }
	}
      
    if(write( fd, "\n", 1) != 1)
	{
	  return(0);
	}
	printf("%s\n", "Before Read");
    done = read_val_resp(cmnd);
    //if( count >= 1 ) done = -1;
  //}
  return done;
}


/*
 * read what the robot has to say
 */
read_resp(cmnd)
char *cmnd;
{
  char *ptr, *tempPtr, buf[1024], temp[1024];
  int nump;

  /* A response is signalled by reading a ready
     prompt from the tty port. There may be a
     error message in front of it.
   */

  for(nump=0,ptr=buf;nump<2;ptr++)
  {
    if(read(fd, ptr, 1) != 1)
       return -1;
    if( ( *ptr == '>' ) || ( *ptr == 'J' ) )
    nump++;
  }
  *ptr = '\0';
	
  /* any  response? */
  if(buf[0] != '>')
  {
    return(compare_resp( buf, cmnd ));
  }
  return(strncmp( cmnd, buf, strlen(cmnd)));
}

	
int compare_resp( buf, cmnd )
char *buf, *cmnd;
{
	char *bufptr, *cmndptr;
	int count;

 	for( count=0; buf[count]!='\n'; count++);
        count++;  
	if( buf[count] != '>' ) return( -1 );
	else return (0);
}



int SerialCommand( comm )
char *comm;
{
        return write_cmnd( comm );
}

double SerialCommandRes(comm)
char *comm;
{
	return write_cmnd_return(comm);
}
