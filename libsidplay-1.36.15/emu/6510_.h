//
// 1997/05/30 13:36:14
//

#ifndef __6510_H
  #define __6510_H


#include "mytypes.h"


extern bool sidKeysOff[32];
extern bool sidKeysOn[32];
extern ubyte optr3readWave;
extern ubyte optr3readEnve;
extern ubyte* c64mem1;
extern ubyte* c64mem2;

extern bool c64memAlloc();
extern bool c64memFree();
extern void c64memClear();
extern void c64memReset(int clockSpeed, ubyte randomSeed);
extern ubyte c64memRamRom(uword address);

extern void initInterpreter(int memoryMode);
extern bool interpreter(uword pc, ubyte ramrom, ubyte a, ubyte x, ubyte y);


#endif
