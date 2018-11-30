/* This is an example of an init file. Do not delete it */
/* For most purposes the only modification you need for it */
/* is to edit the STARTUP_MM macro and the printf("Example being loaded\n");*/

#define _POSIX_SOURCE 1
#include <malloc.h>
#include <stdio.h>

#include "media_all.h"

#include "declarations.h"
#include "definitions.h"
#include "media_protect.h"

#define STARTUP_MM "conveyor.mm"


static L_Ptr new_funcs;		/* head of the list of functions that */
static L_Ptr Snew_funcs;	/* were loaded by this module         */

/***********************************************************************
		init
************************************************************************/

void init()
{
  L_Ptr silence;
  char stamp[400];		/* The unique stamp */
  L_Ptr awktemp;
  DCLPROT;
  STRTPROT;
  PROTVAR(new_funcs);


  new_funcs = NIL;
  sprintf(stamp,"new_funcs%d",objectnum++);
  Snew_funcs = intern(C_2_L_string(stamp),NIL);
  set(Snew_funcs,new_funcs);	/* puts it in the hash table and it is */
				/* protected from GC                   */
  /*printf("Example being loaded\n");*/
#include "codeinit.h"

				/* Put a unique timestamp so that  */
				/* all the loaded files have their */
				/* own list of symbols to kill on  */
				/* exit.                           */
  
  set(Snew_funcs,new_funcs);	/* puts it in the hash table and it is */
				/* protected from GC                   */
#ifdef STARTUP_MM
  silence = symbol_value(SSilent);
  set(SSilent,St);
  read_lisp(C_2_L_string(STARTUP_MM));
  set(SSilent,silence);
#endif
  UNPROT;
}

/***********************************************************************
		fini
************************************************************************/

void fini()
{
  L_Ptr temp;

  new_funcs = symbol_value(Snew_funcs);
  for ( ; !NULLP(new_funcs); new_funcs = cdr(new_funcs))
    {
      temp = car(new_funcs);
      if (!CHK_C_TYPE(temp))
	error_signal(C_2_L_string("fini"),
		     C_2_L_string("CorruptedList"),NIL,NIL);
      XVOID(temp) = (void*)NIL;
    }
  makunbound(Snew_funcs);
}

