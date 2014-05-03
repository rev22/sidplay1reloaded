# Makefile for libsidplay.
#
# make depend
# make
#
# Check ``include/compconf.h'' for items to configure !

#----------------------------------------------------------------------------
# Compiler settings. Try different or no optimizations if you get problems !

CC        =  gcc
CXX       =  g++

#CFLAGS    =  -O
#CFLAGS    =  -O2 -funroll-loops -finline-functions
CFLAGS    =  -pipe -O6 -funroll-loops -finline-functions -fomit-frame-pointer

CXXFLAGS  =  $(CFLAGS)

#----------------------------------------------------------------------------
version = 1.36.15
majorversion = 1

INCS += -I. -Iinclude -Iemu

sources = emu/6510_.cpp emu/6581_.cpp emu/envelope.cpp emu/mixing.cpp emu/samples.cpp \
		fformat/fformat_.cpp fformat/info_.cpp fformat/mus_.cpp fformat/pp_.cpp \
		fformat/psid_.cpp fformat/sid_.cpp \
		eeconfig.cpp player.cpp sidtune.cpp

srcdirs = . emu fformat include

srcobjs = emu/6510_.o emu/6581_.o emu/envelope.o emu/mixing.o emu/samples.o  \
		fformat/fformat_.o fformat/info_.o fformat/mus_.o fformat/pp_.o  \
		fformat/psid_.o fformat/sid_.o \
		eeconfig.o player.o	sidtune.o

.PHONY: all
all:	engine libsidplay.a

.PHONY: clean
clean:
		@for subdir in $(srcdirs); do \
			(cd $$subdir && rm -f *.d *.o *~ *.a *.so) || exit 1; \
		done

.PHONY: depend
depend:
		@makedepend $(INCS) $(sources) 2> /dev/null

.EXPORT_ALL_VARIABLES:

.SUFFIXES:
.SUFFIXES: .cpp .d .h .o

%.d:	%.cpp
		$(CXX) -M $(INCS) $(DEFS) $< > $@

%.o:	%.cpp
		$(CXX) $(CXXFLAGS) $(INCS) $(DEFS) -c $< -o $@

engine: $(srcobjs)

libsidplay.a: $(srcobjs)
		rm -f libsidplay.a
		ar crv libsidplay.a $(srcobjs)
		ranlib libsidplay.a

libsidplay.so: $(srcobjs)
		$(CXX) -shared -Wl,-soname,libsidplay.so.$(majorversion) \
		-o libsidplay.so $(srcobjs)

# ---------------------------------------------------------------------------
# Where to install the libraries and header files.
# Top level slash is important for ``ln'' to work correctly.

ifeq ($(DESTPREFIX),)
libdest = /usr/lib
incdest = /usr/include/sidplay
else
libdest = $(DESTPREFIX)/lib
incdest = $(DESTPREFIX)/include/sidplay
endif

.PHONY: install
install:
		install -d -m 755 $(libdest)
		install -m 644 libsidplay.a $(libdest)/libsidplay.a
		install -m 644 libsidplay.so $(libdest)/libsidplay.so.$(version)
		rm -f $(libdest)/libsidplay.so.$(majorversion)
		rm -f $(libdest)/libsidplay.so
		ln -sfv libsidplay.so.$(version) $(libdest)/libsidplay.so.$(majorversion)
		ln -sfv libsidplay.so.$(version) $(libdest)/libsidplay.so
		install -d -m 755 $(incdest)
		install -m 644 include/compconf.h $(incdest)
		install -m 644 include/emucfg.h $(incdest)
		install -m 644 include/myendian.h $(incdest)
		install -m 644 include/mytypes.h $(incdest)
		install -m 644 include/player.h $(incdest)
		install -m 644 include/sidtune.h $(incdest)
		install -m 644 include/version.h $(incdest)

# ---------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- make depend depends on it.

emu/6510_.o: emu/6510_.h include/mytypes.h include/myendian.h
emu/6510_.o: include/compconf.h include/emucfg.h include/sidtune.h
emu/6581_.o: include/mytypes.h include/myendian.h include/compconf.h
emu/6581_.o: include/sidtune.h emu/6510_.h include/emucfg.h emu/envelope.h
emu/6581_.o: emu/opstruct.h emu/samples.h emu/wave6581.h emu/wave8580.h
emu/envelope.o: emu/envelope.h include/mytypes.h
emu/envelope.o: include/myendian.h include/compconf.h emu/opstruct.h
emu/mixing.o: include/mytypes.h emu/opstruct.h include/myendian.h
emu/mixing.o: include/compconf.h emu/samples.h
emu/samples.o: emu/samples.h include/mytypes.h include/myendian.h
emu/samples.o: include/compconf.h
fformat/fformat_.o: include/fformat.h include/mytypes.h include/myendian.h
fformat/fformat_.o: include/compconf.h
fformat/info_.o: fformat/info_.h
fformat/info_.o: include/mytypes.h include/myendian.h include/compconf.h
fformat/info_.o: include/fformat.h include/smart.h
fformat/info_.o: include/sidtune.h
fformat/mus_.o: fformat/mus_.h include/mytypes.h include/myendian.h
fformat/mus_.o: include/compconf.h include/smart.h
fformat/mus_.o: include/sidtune.h
fformat/pp_.o: fformat/pp_.h
fformat/pp_.o: include/mytypes.h include/myendian.h include/compconf.h
fformat/psid_.o: fformat/psid_.h
fformat/psid_.o: include/mytypes.h include/myendian.h include/compconf.h
fformat/psid_.o: include/sidtune.h
fformat/sid_.o: fformat/sid_.h
fformat/sid_.o: include/mytypes.h include/myendian.h include/compconf.h
fformat/sid_.o: include/fformat.h include/sidtune.h
eeconfig.o: eeconfig.h
eeconfig.o: include/emucfg.h include/compconf.h include/mytypes.h
eeconfig.o: include/sidtune.h emu/6510_.h emu/opstruct.h include/myendian.h
eeconfig.o: emu/samples.h
player.o: include/player.h include/compconf.h include/mytypes.h
player.o: include/emucfg.h include/sidtune.h include/version.h
player.o: include/myendian.h emu/6510_.h
sidtune.o: include/sidtune.h include/mytypes.h include/fformat.h
sidtune.o: include/myendian.h include/compconf.h include/pp.h
