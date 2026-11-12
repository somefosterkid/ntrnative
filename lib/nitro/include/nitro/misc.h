// NitroSDK's assertion and warning macros (nitro/misc.h), as they are in a final ROM, where they compile to nothing:
// the static inline functions of the SDK's headers use them. From libntr (MIT, https://github.com/cybervisi0n/libntr)
// at 1555d91, include/nitro/misc.h.
#ifndef BWPORT_NITRO_MISC_H
#define BWPORT_NITRO_MISC_H

#ifndef SDK_ASSERT
#define SDK_ASSERT(exp) ((void)0)
#endif

#ifndef SDK_ASSERTMSG
#define SDK_ASSERTMSG(exp, ...) ((void)0)
#endif

#ifndef SDK_TASSERTMSG
#define SDK_TASSERTMSG(exp, ...) ((void)0)
#endif

#ifndef SDK_WARNING
#define SDK_WARNING(exp, ...) ((void)0)
#endif

#ifndef SDK_TWARNING
#define SDK_TWARNING(exp, ...) ((void)0)
#endif

#ifndef SDK_NULL_ASSERT
#define SDK_NULL_ASSERT(exp) ((void)0)
#endif

#ifndef SDK_MIN_ASSERT
#define SDK_MIN_ASSERT(exp, min) ((void)0)
#endif

#ifndef SDK_MAX_ASSERT
#define SDK_MAX_ASSERT(exp, max) ((void)0)
#endif

#ifndef SDK_MINMAX_ASSERT
#define SDK_MINMAX_ASSERT(exp, min, max) ((void)0)
#endif

#ifndef SDK_FATAL_ERROR
#define SDK_FATAL_ERROR(...) ((void)0)
#endif

#ifndef SDK_TFATAL_ERROR
#define SDK_TFATAL_ERROR(...) ((void)0)
#endif

#endif // BWPORT_NITRO_MISC_H
