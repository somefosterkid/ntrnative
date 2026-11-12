// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91: include/nitro/codereset.h, as it is (tools/port_lib.py)
#ifdef SDK_CODE_ARM
    #include <nitro/code32.h>
#else
    #include <nitro/code16.h>
#endif
