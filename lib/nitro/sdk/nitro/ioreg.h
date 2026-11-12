// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91: include/nitro/ioreg.h, as it is (tools/port_lib.py)
#ifdef SDK_PORT
    #include <nitro/hw/X86/ioreg.h> //TODO: Rename me
#else
#ifdef SDK_ARM9
    #include <nitro/hw/ARM9/ioreg.h>
#endif
#ifdef  SDK_ARM7
    #include <nitro/hw/ARM7/ioreg.h>
#endif
#endif