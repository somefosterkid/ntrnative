// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91: include/nitro/dtcm_end.h, as it is (tools/port_lib.py)
#if defined(SDK_CW) || defined(__MWERKS__)
    #pragma section DTCM end
#elif defined(SDK_ADS)
    TO BE DEFINED
#elif defined(SDK_GCC)
    TO BE DEFINED
#endif
