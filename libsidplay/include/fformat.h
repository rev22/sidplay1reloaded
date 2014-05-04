//
// /home/ms/source/sidplay/libsidplay/include/RCS/fformat.h,v
//

#ifndef fformat_h
#define fformat_h


#include "compconf.h"
#include "mytypes.h"

#include <string.h>
#include <sstream>

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

// Own version of strdup, which uses new instead of malloc.
extern char* myStrDup(const char *source);

// Return pointer to file name position in complete path.
extern char* fileNameWithoutPath(char* s);

// Return pointer to file name position in complete path.
// Special version: file separator = forward slash.
extern char* slashedFileNameWithoutPath(char* s);

// Return pointer to file name extension in path.
// Searching backwards until first dot is found.
extern char* fileExtOfPath(char* s);

// Parse input string stream. Read and convert a hexa-decimal number up 
// to a ``,'' or ``:'' or ``\0'' or end of stream.
extern udword readHex(std::istringstream& parseStream);

// Parse input string stream. Read and convert a decimal number up 
// to a ``,'' or ``:'' or ``\0'' or end of stream.
extern udword readDec(std::istringstream& parseStream);

// Search terminated string for next newline sequence.
// Skip it and return pointer to start of next line.
extern const char* returnNextLine(const char* pBuffer);

// Skip any characters in an input string stream up to '='.
extern void skipToEqu(std::istringstream& parseStream);

// Start at first character behind '=' and copy rest of string.
extern void copyStringValueToEOL(const char* pSourceStr, char* pDestStr, int destMaxLen);


#endif  // fformat_h
