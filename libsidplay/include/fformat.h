//
// /home/ms/source/sidplay/libsidplay/include/RCS/fformat.h,v
//

#ifndef FFORMAT_H
#define FFORMAT_H


#include "compconf.h"
#include "mytypes.h"

#if defined(HAVE_STRSTREA_H)
  #include <strstrea.h>
#else
  #include <strstream.h>
#endif

#if defined(HAVE_STRCASECMP)
  #undef stricmp
  #define stricmp strcasecmp
#endif

#if defined(HAVE_STRNCASECMP)
  #undef strnicmp
  #define strnicmp strncasecmp
#endif

// Wrapper for ``strnicmp'' without third argument.
inline int myStrNcaseCmp(const char* s1, const char* s2)
{
    return strnicmp(s1,s2,strlen(s2));
}

extern char* myStrDup(const char *source);
extern char* fileNameWithoutPath(char* s);
extern char* fileExtOfPath(char* s);
extern udword readHex(istrstream& parseStream);
extern udword readDec(istrstream& parseStream);
extern const char* returnNextLine(const char* pBuffer);
extern void skipToEqu(istrstream& parseStream);
extern void copyStringValueToEOL(const char* pSourceStr, char* pDestStr, int destMaxLen);


#endif
