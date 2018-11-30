# Makefile for dynamicaly linked modules.
# To add a source file edit the SRC macro below.
# To add a header file edit the HDREXP or HDRLOC macros below. The HDREXP
# macros will be exported to other modules. HDRLOC contain at least proc.h
# and declarations.h. Both are created when you type make setup.
# You also have to change the TRGT.SO macro to the name you want.

ISMODULE = 1

# The INSTALLROOT macro contains the directory where the MediaMath module will
# eventualy live. If you are a user of MediaMath this should be ${HOME}
# or ${HOME}/Projects. If you are maintaining MediaMath for your site
# it should be /usr/local or something similar.
INSTALLROOT = ${HOME}
INSTALLROOT = /cs/fac

# The following macros indicate where the various MediaMath files will go.
# To avoid clattering these standard directories, all go to subdirectories
# called MediaMath (except for bin; there are not that many binaries and
# shellscripts and we avoid having to augment everybody's path)
# The BINDIR will most probably contain nothing. The INC will contain all
# the include files that you want public. The LIBDIR will contain all your
# *.so and *.mm files and the DOCDIR directory is in GLIBDIR so that it is
# shared among binaries of different architectures. The binaries and
# libraries created by different architectures are kept in directories
# indexed with `arch` a UNIX command that returns the application level
# architecture.
# NOTE: avoid changing these macros. If you change them, then change
# the same macros consistently in the rest of the makefiles.
INC    = $(INSTALLROOT)/include/MediaMath
BINDIR = $(INSTALLROOT)/bin/
GLIBDIR = $(INSTALLROOT)/lib/MediaMath
LIBDIR = $(GLIBDIR)/
DOCDIR = $(GLIBDIR)/Doc/

# MEDIAROOT is where the main modules were installed at your site. Your own
# modules don't go there normaly, so it should be different from
# INSTALLROOT. Typical values are /usr/local and /cs/fac
MEDIAROOT = ${INSTALLROOT}
MEDIAROOT = /cs/fac

# The MEDIA_INC contains the directory where all the include files for
# MediaMath can be found. Normaly this should be
# /usr/local/include/MediaMath. Similarly for the rest of the
# macros. Again the directories are indexed as above.
MEDIA_INC = $(MEDIAROOT)/include/MediaMath
MEDIA_BIN = $(MEDIAROOT)/bin/
MEDIA_GLIB= $(MEDIAROOT)/lib/MediaMath
MEDIA_LIB = $(MEDIA_GLIB)
MEDIA_DOC = $(MEDIA_GLIB)/Doc/

# The following macros are the source, header, object, declaration etc
# files for this module. HDREXP are the ones that will be eventualy
# exported by the install target. HDR files are the ones imported from
# MEDIA_INC and HDRLOC and HDREXP are in this directory. DCL, DFN and CODE
# files are created with make setup (might be recreated with compilation)
HDR = media_all.h media_protect.h one-and-two-D.h
SRC =    xarmmotion.c    xarmopen.c    l2-crs.c    init.c
OBJ =    xarmmotion.o    xarmopen.o    l2-crs.o    init.o
DCL = DCLxarmmotion.h DCLxarmopen.h DCLl2-crs.h DCLinit.h
DFN = DFNxarmmotion.h DFNxarmopen.h DFNl2-crs.h DFNinit.h
CODE=   Cxarmmotion.h   Cxarmopen.h   Cl2-crs.h   Cinit.h
HDRLOC = proc.h declarations.h
HDREXP = xarmrobot.h flags.h conveyor.mm
TRGT.SO = conveyor.so

# This file contains the configuration macros for the architecture.
include config_make

CFLAGS= $(PIC) $(OPTIM) -DMODULE -I$(MEDIA_INC) -I$(INC)
## CPPFLAGS= -I$(MEDIA_INC) -I$(INC)
LINT=lint
LINTFLAGS=
LINKFLAGS= $(LD_TEXT)
LIBS = $(EXTRA_LIBS)

## COMPILE.c=  $(CC) $(CFLAGS) $(CPPFLAGS)

$(TRGT.SO): $(OBJ)
	rm -f $(TRGT.SO)
	$(LINK) $(LINKFLAGS) $(OBJ) $(LIBS) -o $(TRGT.SO)

# The install target can be much faster if you ommit the last command that
# copies Doc/* (there can be MANY files there)
install: $(TRGT.SO)
	if test ! -d $(GLIBDIR); then mkdir $(GLIBDIR); fi;
	if test ! -d $(LIBDIR); then mkdir $(LIBDIR); fi;
	if test ! -d $(INC); then mkdir $(INC); fi;
	if test ! -d $(DOCDIR); then mkdir $(DOCDIR); fi;
	cp $(TRGT.SO) *.mm $(LIBDIR)
	if cp $(HDREXP) $(INC); then echo OK; else echo NOTHING TO COPY; fi
	cp Doc/* $(DOCDIR)

include ${MEDIA_INC}/media_make

