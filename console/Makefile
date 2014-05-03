# Generated automatically from Makefile.in by configure.

SHELL			= /bin/sh

TOPDIR			= ..
BASEDIR			= .
SRCDIR			= $(BASEDIR)
prefix			= /usr
exec_prefix		= ${prefix}
bindir			= ${exec_prefix}/bin
libdir			= ${exec_prefix}/lib
includedir		= ${prefix}/include
DOCDIR			= $prefix/doc

CXX				= c++
CXXFLAGS		= -g -O2
LN				= ln -s
RM				= /bin/rm
AR				= /usr/bin/ar
RANLIB			= ranlib
INSTALL			= /usr/bin/install -c
INSTALL_DATA	= ${INSTALL} -m 644

LDFLAGS			= 
DEFS			= -DHAVE_CONFIG_H
INCLUDES		= -I$(SRCDIR) -I$(TOPDIR)/libsidplay/include

LIBM			= -lm
LIBIOS			= 
SIDLIB          = -L$(TOPDIR)/libsidplay -lsidplay
LIBS			=  $(LIBM) $(LIBIOS) $(SIDLIB)

#--------------------------------------------------------------

sources =  sidplay.cpp audiodrv.cpp sid2wav.cpp sidcon.cpp

.PHONY: all
all:	sidplay sid2wav sidcon

sidplay:	sidplay.o audiodrv.o
		$(CXX) -o $@ $(LDFLAGS) sidplay.o audiodrv.o $(LIBS)

sid2wav:	sid2wav.o
		$(CXX) -o $@ $(LDFLAGS) sid2wav.o $(LIBS)

sidcon:	sidcon.o
		$(CXX) -o $@ $(LDFLAGS) sidcon.o $(LIBS)

.PHONY: clean
clean:
		rm -f *.d *.o *~ sidplay sid2wav sidcon

.PHONY: depend
depend:
		makedepend $(INCLUDES) $(sources) 2> /dev/null

install:
		$(INSTALL) sidplay $(bindir)
		$(INSTALL) sid2wav $(bindir)
		$(INSTALL) sidcon $(bindir)

.EXPORT_ALL_VARIABLES:

.SUFFIXES:
.SUFFIXES: .cpp .d .h .o

%.o:	%.cpp
		$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFS) -c $< -o $@

# ---------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- make depend depends on it.

sidplay.o: /usr/include/stdlib.h /usr/include/features.h
sidplay.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
sidplay.o: /usr/lib/gcc-lib/i486-linux/2.7.2.3/include/stddef.h
sidplay.o: /usr/include/sys/types.h /usr/include/gnu/types.h
sidplay.o: /usr/include/time.h /usr/include/endian.h /usr/include/bytesex.h
sidplay.o: /usr/include/sys/select.h /usr/include/selectbits.h
sidplay.o: /usr/include/alloca.h /usr/include/unistd.h
sidplay.o: /usr/include/posix_opt.h /usr/include/confname.h
sidplay.o: ../libsidplay/include/player.h ../libsidplay/include/compconf.h
sidplay.o: ../libsidplay/include/config.h ../libsidplay/include/mytypes.h
sidplay.o: ../libsidplay/include/emucfg.h ../libsidplay/include/sidtune.h
sidplay.o: ../libsidplay/include/version.h ../libsidplay/include/myendian.h
sidplay.o: audiodrv.h /usr/include/fcntl.h /usr/include/fcntlbits.h
sidplay.o: /usr/include/sys/ioctl.h /usr/include/ioctls.h
sidplay.o: /usr/include/asm/ioctls.h /usr/include/asm/ioctl.h
sidplay.o: /usr/include/ioctl-types.h /usr/include/sys/ttydefaults.h
audiodrv.o: audiodrv.h /usr/include/unistd.h /usr/include/features.h
audiodrv.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
audiodrv.o: /usr/include/posix_opt.h /usr/include/gnu/types.h
audiodrv.o: /usr/lib/gcc-lib/i486-linux/2.7.2.3/include/stddef.h
audiodrv.o: /usr/include/confname.h /usr/include/fcntl.h
audiodrv.o: /usr/include/fcntlbits.h /usr/include/sys/types.h
audiodrv.o: /usr/include/time.h /usr/include/endian.h /usr/include/bytesex.h
audiodrv.o: /usr/include/sys/select.h /usr/include/selectbits.h
audiodrv.o: /usr/include/sys/ioctl.h /usr/include/ioctls.h
audiodrv.o: /usr/include/asm/ioctls.h /usr/include/asm/ioctl.h
audiodrv.o: /usr/include/ioctl-types.h /usr/include/sys/ttydefaults.h
audiodrv.o: ../libsidplay/include/compconf.h ../libsidplay/include/config.h
audiodrv.o: ../libsidplay/include/mytypes.h ../libsidplay/include/emucfg.h
audiodrv.o: ../libsidplay/include/sidtune.h
sid2wav.o: /usr/include/stdlib.h /usr/include/features.h
sid2wav.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
sid2wav.o: /usr/lib/gcc-lib/i486-linux/2.7.2.3/include/stddef.h
sid2wav.o: /usr/include/sys/types.h /usr/include/gnu/types.h
sid2wav.o: /usr/include/time.h /usr/include/endian.h /usr/include/bytesex.h
sid2wav.o: /usr/include/sys/select.h /usr/include/selectbits.h
sid2wav.o: /usr/include/alloca.h /usr/include/string.h
sid2wav.o: ../libsidplay/include/myendian.h ../libsidplay/include/compconf.h
sid2wav.o: ../libsidplay/include/config.h ../libsidplay/include/mytypes.h
sid2wav.o: ../libsidplay/include/player.h ../libsidplay/include/emucfg.h
sid2wav.o: ../libsidplay/include/sidtune.h ../libsidplay/include/version.h
sid2wav.o: ../libsidplay/include/fformat.h
sidcon.o: /usr/include/ctype.h /usr/include/features.h
sidcon.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
sidcon.o: /usr/include/endian.h /usr/include/bytesex.h /usr/include/stdlib.h
sidcon.o: /usr/lib/gcc-lib/i486-linux/2.7.2.3/include/stddef.h
sidcon.o: /usr/include/sys/types.h /usr/include/gnu/types.h
sidcon.o: /usr/include/time.h /usr/include/sys/select.h
sidcon.o: /usr/include/selectbits.h /usr/include/alloca.h
sidcon.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
sidcon.o: /usr/lib/gcc-lib/i486-linux/2.7.2.3/include/stdarg.h
sidcon.o: /usr/include/stdio_lim.h /usr/include/string.h
sidcon.o: ../libsidplay/include/sidtune.h ../libsidplay/include/mytypes.h
sidcon.o: ../libsidplay/include/compconf.h ../libsidplay/include/config.h
sidcon.o: ../libsidplay/include/fformat.h
