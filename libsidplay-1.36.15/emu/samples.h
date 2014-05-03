//
// 1997/05/11 11:29:47
//

#ifndef __6581_SAMPLES_H
  #define __6581_SAMPLES_H


#include "mytypes.h"
#include "myendian.h"


extern void sampleEmuCheckForInit();
extern void sampleEmuInit();          // precalculate tables + reset
extern void sampleEmuReset();         // reset some important variables

extern sbyte (*sampleEmuRout)();


#endif
