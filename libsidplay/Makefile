# Generated automatically from Makefile.in by configure.
# Makefile template for libsidplay.

version 		= 1.36.28
majorversion 	= 1.36

SHELL			= /bin/sh

BASEDIR			= .
SRCDIR			= $(BASEDIR)
prefix			= /usr
exec_prefix		= ${prefix}
bindir			= ${exec_prefix}/bin
libdir          = ${exec_prefix}/lib
includedir      = ${prefix}/include
includedest     = $(includedir)/sidplay
DOCDIR			= $prefix/doc

CXX				= c++
CXXFLAGS		= -g -O2
LN			    = ln -s
RM              = /bin/rm
AR              = /usr/bin/ar
RANLIB			= ranlib
INSTALL			= /usr/bin/install -c
INSTALL_DATA	= ${INSTALL} -m 644

LDFLAGS			= 
DEFS			= -DHAVE_CONFIG_H
INCLUDES		= -I$(SRCDIR) -I$(SRCDIR)/emu -I$(SRCDIR)/include

LIBM			= -lm
LIBIOS			= 
LIBS			=  $(LIBM) $(LIBIOS)

#--------------------------------------------------------------

srcdirs = . emu fformat include

sources = emu/6510_.cpp emu/6581_.cpp emu/envelope.cpp emu/mixing.cpp emu/samples.cpp \
		fformat/fformat_.cpp fformat/info_.cpp fformat/mus_.cpp fformat/pp_.cpp \
		fformat/psid_.cpp fformat/sid_.cpp \
		eeconfig.cpp player.cpp sidtune.cpp

srcobjs = emu/6510_.o emu/6581_.o emu/envelope.o emu/mixing.o emu/samples.o  \
		fformat/fformat_.o fformat/info_.o fformat/mus_.o fformat/pp_.o  \
		fformat/psid_.o fformat/sid_.o \
		eeconfig.o player.o sidtune.o

install_headers = include/compconf.h include/config.h include/emucfg.h \
		include/fformat.h include/myendian.h include/mytypes.h \
		include/player.h include/sidtune.h include/version.h \
		include/fixpoint.h

.EXPORT_ALL_VARIABLES:

.SUFFIXES:
.SUFFIXES: .cpp .h .o

%.o:	%.cpp
		$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFS) -c $< -o $@

.PHONY: all
all:	engine libsidplay.a

engine: $(srcobjs)

libsidplay.a: $(srcobjs)
		$(RM) -f libsidplay.a
		$(AR) crv libsidplay.a $(srcobjs)
		$(RANLIB) libsidplay.a

libsidplay.so: $(srcobjs)
		$(CXX) -shared -Wl,-soname,libsidplay.so.$(majorversion) \
		-o libsidplay.so $(srcobjs)

.PHONY: clean
clean:
		@for subdir in $(srcdirs); do \
			(cd $$subdir && $(RM) -f *.d *.o *~ *.a *.so) || exit 1; \
		done

.PHONY: depend
depend:
		makedepend $(INCLUDES) $(sources) 2> /dev/null

.PHONY: install
install:	install-headers install-static

install-static:
		$(INSTALL) -d -m 755 $(libdir)
		$(INSTALL_DATA) libsidplay.a $(libdir)/libsidplay.a

install-headers:
		$(RM) -f $(includedest)/*
		$(INSTALL) -d -m 755 $(includedest)
		for file in $(install_headers); do \
			($(INSTALL_DATA) $$file $(includedest) ) || exit 1; \
		done

install-shared:
		$(INSTALL) -d -m 755 $(libdir)
		$(INSTALL_DATA) libsidplay.so $(libdir)/libsidplay.so.$(version)
		$(RM) -f $(libdir)/libsidplay.so.$(majorversion)
		$(RM) -f $(libdir)/libsidplay.so
		$(LN) -sfv libsidplay.so.$(version) $(libdir)/libsidplay.so.$(majorversion)
		$(LN) -sfv libsidplay.so.$(version) $(libdir)/libsidplay.so

#--------------------------------------------------------------
# DO NOT DELETE THIS LINE -- make depend depends on it.

emu/6510_.o: emu/6510_.h ./include/mytypes.h ./include/compconf.h
emu/6510_.o: ./include/config.h ./include/myendian.h ./include/emucfg.h
emu/6510_.o: ./include/sidtune.h
emu/6581_.o: ./include/mytypes.h ./include/compconf.h ./include/config.h
emu/6581_.o: ./include/myendian.h ./include/sidtune.h emu/6510_.h
emu/6581_.o: ./include/emucfg.h emu/envelope.h emu/opstruct.h emu/samples.h
emu/6581_.o: emu/wave6581.h emu/wave8580.h
emu/envelope.o: /usr/include/math.h /usr/include/features.h
emu/envelope.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
emu/envelope.o: /usr/include/huge_val.h /usr/include/mathcalls.h
emu/envelope.o: /usr/lib/gcc-lib/i486-linux/2.7.2.3/include/float.h
emu/envelope.o: ./include/compconf.h ./include/config.h emu/envelope.h
emu/envelope.o: ./include/mytypes.h ./include/myendian.h emu/opstruct.h
emu/mixing.o: ./include/mytypes.h ./include/compconf.h ./include/config.h
emu/mixing.o: emu/opstruct.h ./include/myendian.h emu/samples.h
emu/samples.o: emu/samples.h ./include/mytypes.h ./include/compconf.h
emu/samples.o: ./include/config.h ./include/myendian.h
fformat/fformat_.o: /usr/include/ctype.h /usr/include/features.h
fformat/fformat_.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
fformat/fformat_.o: /usr/include/endian.h /usr/include/bytesex.h
fformat/fformat_.o: /usr/include/string.h
fformat/fformat_.o: /usr/lib/gcc-lib/i486-linux/2.7.2.3/include/stddef.h
fformat/fformat_.o: ./include/fformat.h ./include/compconf.h
fformat/fformat_.o: ./include/config.h ./include/mytypes.h
fformat/fformat_.o: ./include/myendian.h
fformat/info_.o: fformat/info_.h ./include/compconf.h ./include/config.h
fformat/info_.o: /usr/include/string.h /usr/include/features.h
fformat/info_.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
fformat/info_.o: /usr/lib/gcc-lib/i486-linux/2.7.2.3/include/stddef.h
fformat/info_.o: ./include/mytypes.h ./include/myendian.h ./include/fformat.h
fformat/info_.o: ./include/smart.h /usr/include/ctype.h /usr/include/endian.h
fformat/info_.o: /usr/include/bytesex.h ./include/sidtune.h
fformat/mus_.o: fformat/mus_.h ./include/compconf.h ./include/config.h
fformat/mus_.o: ./include/mytypes.h ./include/myendian.h ./include/smart.h
fformat/mus_.o: /usr/include/ctype.h /usr/include/features.h
fformat/mus_.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
fformat/mus_.o: /usr/include/endian.h /usr/include/bytesex.h
fformat/mus_.o: ./include/sidtune.h
fformat/pp_.o: fformat/pp_.h /usr/include/string.h /usr/include/features.h
fformat/pp_.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
fformat/pp_.o: /usr/lib/gcc-lib/i486-linux/2.7.2.3/include/stddef.h
fformat/pp_.o: /usr/include/limits.h /usr/include/posix1_lim.h
fformat/pp_.o: /usr/include/local_lim.h /usr/include/linux/limits.h
fformat/pp_.o: /usr/include/posix2_lim.h ./include/mytypes.h
fformat/pp_.o: ./include/compconf.h ./include/config.h ./include/myendian.h
fformat/psid_.o: fformat/psid_.h /usr/include/string.h
fformat/psid_.o: /usr/include/features.h /usr/include/sys/cdefs.h
fformat/psid_.o: /usr/include/gnu/stubs.h
fformat/psid_.o: /usr/lib/gcc-lib/i486-linux/2.7.2.3/include/stddef.h
fformat/psid_.o: ./include/mytypes.h ./include/compconf.h ./include/config.h
fformat/psid_.o: ./include/myendian.h ./include/sidtune.h
fformat/sid_.o: fformat/sid_.h ./include/compconf.h ./include/config.h
fformat/sid_.o: /usr/include/ctype.h /usr/include/features.h
fformat/sid_.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
fformat/sid_.o: /usr/include/endian.h /usr/include/bytesex.h
fformat/sid_.o: /usr/include/string.h
fformat/sid_.o: /usr/lib/gcc-lib/i486-linux/2.7.2.3/include/stddef.h
fformat/sid_.o: ./include/mytypes.h ./include/myendian.h ./include/fformat.h
fformat/sid_.o: ./include/sidtune.h
eeconfig.o: eeconfig.h /usr/include/math.h /usr/include/features.h
eeconfig.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
eeconfig.o: /usr/include/huge_val.h /usr/include/mathcalls.h
eeconfig.o: /usr/lib/gcc-lib/i486-linux/2.7.2.3/include/float.h
eeconfig.o: /usr/include/time.h
eeconfig.o: /usr/lib/gcc-lib/i486-linux/2.7.2.3/include/stddef.h
eeconfig.o: /usr/include/timebits.h /usr/include/gnu/types.h
eeconfig.o: ./include/emucfg.h ./include/compconf.h ./include/config.h
eeconfig.o: ./include/mytypes.h ./include/sidtune.h emu/6510_.h
eeconfig.o: emu/opstruct.h ./include/myendian.h emu/samples.h
eeconfig.o: ./include/version.h
player.o: ./include/player.h ./include/compconf.h ./include/config.h
player.o: ./include/mytypes.h ./include/emucfg.h ./include/sidtune.h
player.o: ./include/version.h ./include/myendian.h emu/6510_.h
sidtune.o: /usr/include/string.h /usr/include/features.h
sidtune.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
sidtune.o: /usr/lib/gcc-lib/i486-linux/2.7.2.3/include/stddef.h
sidtune.o: /usr/include/limits.h /usr/include/posix1_lim.h
sidtune.o: /usr/include/local_lim.h /usr/include/linux/limits.h
sidtune.o: /usr/include/posix2_lim.h ./include/sidtune.h ./include/mytypes.h
sidtune.o: ./include/compconf.h ./include/config.h ./include/fformat.h
sidtune.o: ./include/myendian.h ./include/pp.h
