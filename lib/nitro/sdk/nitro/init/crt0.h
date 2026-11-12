// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91: include/nitro/init/crt0.h, as it is (tools/port_lib.py)
#ifndef NITRO_INIT_CRT0_H_
#define NITRO_INIT_CRT0_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef SDK_ARM9
    void NitroMain(void);
    void NitroStartUp(void);
#endif
#ifdef SDK_ARM7
    void NitroSpMain(void);
    void NitroSpStartUp(void);

    #define NitroMain(x)     NitroSpMain(x)
    #define NitroStartUp(x)  NitroSpStartUp(x)
#endif
#ifdef SDK_X86
    void NitroMain(void);
    void NitroStartUp(void);
#endif

#ifdef SDK_AARCH64
    void NitroMain(void);
    void NitroStartUp(void);
#endif

#ifdef __cplusplus
}
#endif

#endif
