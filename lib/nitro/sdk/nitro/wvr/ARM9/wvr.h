// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91: include/nitro/wvr/ARM9/wvr.h, as it is (tools/port_lib.py)
#ifndef NITRO_WVR_ARM9_WVR_H_
#define NITRO_WVR_ARM9_WVR_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include <nitro/gx/gx_vramcnt.h>

typedef void (*WVRCallbackFunc) (void * arg, WVRResult result);

WVRResult WVR_StartUpAsync(GXVRamARM7 vram, WVRCallbackFunc callback, void * arg);
WVRResult WVR_TerminateAsync(WVRCallbackFunc callback, void * arg);

#ifdef  __cplusplus
}
#endif

#endif
