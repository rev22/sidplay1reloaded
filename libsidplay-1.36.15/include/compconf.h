//
// Libsidplay compile time definitions.
//
// Programmer's note: If you don't include ``player.h'' only,
// do not forget to include this in every file that uses libsidplay.
//

#ifndef __CONFIG_H
#define __CONFIG_H

//
// Specifies the endianess of your CPU's hardware.
//
#define IS_LITTLE_ENDIAN
//#define IS_BIG_ENDIAN

//
// Speed optimization.
//
// To gain speed (must not work on every hardware). Uses a union to access
// integer fixpoint operands in memory. This makes an assumption about the
// hardware and software architecture and therefore is considered a hack!
// Try it. You will notice if it doesn't work ;)
//
// However, this option is highly used and tested. A failing little endian
// system has not been reported so far.
//
#define DIRECT_FIXPOINT
//
// This will cause little(big) endian machines to directly access little(big)
// endian values in memory structures or arrays, disregarding alignment.
//
#define OPTIMIZE_ENDIAN_ACCESS

//
// Possible speed optimization on high-end machines.
//
// To calculate envelope with floating point precision. High-end FPU is
// strongly recommended, otherwise this may not be faster. Forget about
// the Pentium FPU.
//
//#define SID_FPUENVE
#define SID_FPUMIXERINIT

//
// Don't touch these !
//
//#define NO_STDIN_LOADER
#define SIDEMU_TIME_COUNT

//#define SIDEMU_OLD_WAVE30
//#define SIDEMU_OLD_WAVE50

//#define SID_LINENVE
//#define SID_REFTIMES
//#define SID_EXPENVE


#endif
