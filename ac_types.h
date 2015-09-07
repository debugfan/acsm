#ifndef AC_TYPES_H
#define AC_TYPES_H

#include <stdint.h>

#ifdef _MSC_VER
#define INLINE __forceinline /* use __forceinline (VC++ specific) */
#else
#define INLINE inline        /* use standard inline */
#endif

#define FatalError(x)

#endif
