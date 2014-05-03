# Makefile for ``sidplay'' and ``sid2wav''.
#
# make depend
# make

#--------------------------------------------------------------
# Where libsidplay and header files are stored.

ifeq ($(SIDLIB),)
SIDLIB = ../libsidplay
endif

ifeq ($(SIDINC),)
SIDINC = ../libsidplay/include
endif

#--------------------------------------------------------------
# Root of any files that will be installed.

ifeq ($(DESTPREFIX),)
DESTPREFIX = /usr/local
endif

#--------------------------------------------------------------
# Compiler settings.

CC      =  g++
CFLAGS  =  -O2
LDFLAGS =

#--------------------------------------------------------------
# Due to conditional compilation, the audio driver needs a hint
# on the following platforms: HP-UX, SGI and maybe: SunOS
# -Dhpux, -Dsgi, -Dsun and -Dsparc
# 
# The systems Linux and FreeBSD are likely to provide a global
# setting, which means you don't need to set DEFS values
# -Dlinux, -D__FreeBSD__ here explicitly.
#
# Use -DCHECKFORSPEAKERBOX on Sparcstations where the
# availability of a speakerbox means better audio capabilities.

# Sparcstation running Solaris2.
#DEFS  +=  -Dsun -Dsparc -Dsolaris2
##DEFS  +=  -DCHECKFORSPEAKERBOX

# Sparcstation running SunOS.
#DEFS  +=  -Dsun -Dsparc
##DEFS  +=  -DCHECKFORSPEAKERBOX

#--------------------------------------------------------------
# Uncomment this if you use an old libg++.so

#OLDIOS = -liostream

#--------------------------------------------------------------
# Flags for includes and libraries.

INCS     +=  -I. -I$(SIDINC)
LIBS	 +=	 -lm $(OLDIOS)

sources =  sidplay.cpp audiodrv.cpp sid2wav.cpp

.PHONY: all
all:	sidplay sid2wav
		
sidplay:	sidplay.o audiodrv.o
		$(CC) -o $@ $(LDFLAGS) sidplay.o audiodrv.o $(LIBS) -L$(SIDLIB) -lsidplay

sid2wav:	sid2wav.o
		$(CC) -o $@ $(LDFLAGS) sid2wav.o $(LIBS) -L$(SIDLIB) -lsidplay

.PHONY: clean
clean:
		rm -f *.d *.o *~ sidplay sid2wav

.PHONY: depend
depend:
		@makedepend $(INCS) $(sources) 2> /dev/null

install:
		install -m 755 sidplay $(DESTPREFIX)/bin
		install -m 755 sid2wav $(DESTPREFIX)/bin

#--------------------------------------------------------------
# Automatic dependencies and compilation.

.EXPORT_ALL_VARIABLES:

.SUFFIXES:
.SUFFIXES: .cpp .d .h .o

%.d:	%.cpp
		$(CC) -M $(INCS) $(DEFS) $< > $@

%.o:	%.cpp
		$(CC) $(CFLAGS) $(INCS) $(DEFS) -c $< -o $@

# ---------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- make depend depends on it.

sidplay.o: ../libsidplay/include/player.h ../libsidplay/include/compconf.h
sidplay.o: ../libsidplay/include/mytypes.h ../libsidplay/include/emucfg.h
sidplay.o: ../libsidplay/include/sidtune.h ../libsidplay/include/version.h
sidplay.o: ../libsidplay/include/myendian.h audiodrv.h
audiodrv.o: audiodrv.h 
audiodrv.o: ../libsidplay/include/mytypes.h ../libsidplay/include/emucfg.h
audiodrv.o: ../libsidplay/include/compconf.h ../libsidplay/include/sidtune.h
sid2wav.o: ../libsidplay/include/player.h ../libsidplay/include/compconf.h
sid2wav.o: ../libsidplay/include/mytypes.h ../libsidplay/include/emucfg.h
sid2wav.o: ../libsidplay/include/sidtune.h ../libsidplay/include/version.h
sid2wav.o: ../libsidplay/include/myendian.h
