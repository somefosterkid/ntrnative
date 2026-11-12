// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91: include/nitro/ext/ext_scrntest.h, as it is (tools/port_lib.py)
#ifndef EXT_SCRNTEST_H_
#define EXT_SCRNTEST_H_

#include <nitro/types.h>
#include <nitro/gx/gx_vramcnt.h>

#ifdef __cplusplus
extern "C" {
#endif

void EXT_Printf(const char * fmt, ...);
void EXT_CompPrint(const char * src1, const char * src2_fmt, ...);
void EXT_TestScreenShot(u32 testFrames, u32 checkSum);
void EXT_TestTickCounter(void);
void EXT_TestResetCounter(void);
void EXT_TestSetVRAMForScreenShot(GXVRamLCDC vram);

#ifdef __cplusplus
}
#endif

#endif
