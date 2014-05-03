/* libsidplay/include/config.h.  Generated automatically by configure.  */
/*
 * config.h (template)
 */
 
/* --------------------------------------------------------------------------
 * Portability adjustments.
 * --------------------------------------------------------------------------
 */

/* Define your operating system. Leave undefined if none applies.  */
#define HAVE_LINUX 1
/* #undef HAVE_FREEBSD */
/* #undef HAVE_SGI */
/* #undef HAVE_HPUX */
/* #undef HAVE_SUNOS */
/* #undef HAVE_SOLARIS */
/* #undef HAVE_MSWINDOWS */
/* #undef HAVE_MACOS */
/* #undef HAVE_OS2 */
/* #undef HAVE_AMIGAOS */
/* #undef HAVE_MSDOS */
/* #undef HAVE_BEOS */
/* #undef HAVE_RISCOS */

/* Define the file/path separator(s) that your filesystem uses:
   FS_IS_COLON_AND_BACKSLASH, FS_IS_COLON_AND_SLASH, FS_IS_BACKSLASH,
   FS_IS_COLON, FS_IS_SLASH  */
#if defined(HAVE_MSWINDOWS) || defined(HAVE_MSDOS) || defined(HAVE_OS2)
  #define FS_IS_COLON_AND_BACKSLASH
#elif defined(HAVE_MACOS)
  #define FS_IS_COLON
#elif defined(HAVE_AMIGAOS)
  #define FS_IS_COLON_AND_SLASH
#else
  #define FS_IS_SLASH
#endif	  

/* Define if your processor stores words with the most significant
   byte first (like Motorola and SPARC, unlike Intel and VAX).  */
/* #undef WORDS_BIGENDIAN */

/* Define if your compiler supports type ``bool''.
   If not, a user-defined signed integral type will be used.  */
#define HAVE_BOOL 1

/* Define if you support file names longer than 14 characters.  */
#define HAVE_LONG_FILE_NAMES 1

/* Define if you have the <strstrea.h> header file.  */
/* #undef HAVE_STRSTREA_H */

/* Define if you have the strcasecmp function.  */
#define HAVE_STRCASECMP 1

/* Define if you have the strncasecmp function.  */
#define HAVE_STRNCASECMP 1

/* Define if standard member ``ios::binary'' is called ``ios::bin''. */
/* #undef HAVE_IOS_BIN */

/* Define if standard member function ``fstream::is_open()'' is not available.  */
/* #undef DONT_HAVE_IS_OPEN */

/* Define whether istream member function ``seekg(streamoff,seekdir).offset()''
   should be used instead of standard ``seekg(streamoff,seekdir); tellg()''.  */
/* #undef HAVE_SEEKG_OFFSET */

/* Define if you have the <sys/ioctl.h> header file.  */
#define HAVE_SYS_IOCTL_H 1

/* Define if you have the <audio.h> header file.  */
/* #undef HAVE_AUDIO_H */

/* Define if you have the <dmedia/audio.h> header file.  */
/* #undef HAVE_DMEDIA_AUDIO_H */

/* Define if you have the <linux/soundcard.h> header file.  */
#define HAVE_LINUX_SOUNDCARD_H 1

/* Define if you have the <machine/soundcard.h> header file.  */
/* #undef HAVE_MACHINE_SOUNDCARD_H */

/* Define if you have the <sun/audioio.h> header file.  */
/* #undef HAVE_SUN_AUDIOIO_H */

/* Define if you have the <sun/dbriio.h> header file.  */
/* #undef HAVE_SUN_DBRIIO_H */

/* Define if you have the <sys/audio.h> header file.  */
/* #undef HAVE_SYS_AUDIO_H */

/* Define if you have the <sys/audioio.h> header file.  */
/* #undef HAVE_SYS_AUDIOIO_H */

/* --------------------------------------------------------------------------
 * Hardware-specific speed optimizations.
 * Check here for things you can configure manually only.
 * --------------------------------------------------------------------------
 *
 * Caution: This may not work on every hardware and therefore can result in
 * trouble. Some hardware-specific speed optimizations use a union to access
 * integer fixpoint operands in memory. An assumption is made about the
 * hardware and software architecture and therefore is considered a hack!
 * But try it in need for speed. You will notice if it doesn't work ;)
 *
 * This option is highly used and tested. A failing little endian system
 * has not been reported so far.  */
#define DIRECT_FIXPOINT

/* This will cause little(big) endian machines to directly access little(big)
 * endian values in memory structures or arrays, disregarding alignment.  */
#define OPTIMIZE_ENDIAN_ACCESS
	
/* --------------------------------------------------------------------------
 * Don't touch these!
 * --------------------------------------------------------------------------
 */
/* #undef NO_STDIN_LOADER */
#define SIDEMU_TIME_COUNT
/* #undef SID_LINENVE */
/* #undef SID_REFTIMES */
/* #undef SID_EXPENVE */
/* #undef LARGE_NOISE_TABLE */
/* #undef PORTABLE_FIXPOINT */
/* #undef NO_RTS_UPON_BRK */

/* Do filter calculations at floating-point precision.  */
#define SID_FPUFILTER

/* Do initialization of mixing tables at floating-point precision.  */
#define SID_FPUMIXERINIT

/* Do envelope calculations at floating-point precision. High-end FPU is
 * strongly recommended. Otherwise this may not be faster. Forget about
 * the Pentium FPU.  */
/* #undef SID_FPUENVE */

