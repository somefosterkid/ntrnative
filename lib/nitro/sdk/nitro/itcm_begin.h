// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91: include/nitro/itcm_begin.h, as it is (tools/port_lib.py)
#include <nitro/section.h>

#if defined(SDK_CW) || defined(__MWERKS__)
    #pragma section ITCM begin
#elif defined(SDK_ADS)
    TO BE DEFINED
#elif defined(SDK_GCC)
    TO BE DEFINED
#endif
