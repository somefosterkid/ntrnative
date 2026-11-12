// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/gfl_use.h: the prototypes of Black's
// named functions and the types they need.
#ifndef BWPORT_SYSTEM_GFL_USE_H
#define BWPORT_SYSTEM_GFL_USE_H

#include "types.h"
#include "gfl/tcb.h"
#include "nitro/math.h"

typedef struct {
    MATHRandContext32 rand;
    TCBManager *hblankTCBMgr;
    void *hblankTCBWork;
    TCBManager *vblankTCBMgr;
    void *vblankTCBWork;
} GFLUseWork;

// The archive paths, by archive ID, in the ROM's NARC_PATH_LIST
extern const char *NARC_PATH_LIST[];

void finishFunc(void);
// pokebw2's GFLAppInit: the heaps, the archives, the GFL systems and the work of the VBlank and HBlank tasks
void GFLAppInit(void);
void func_020056d0(void);

void GFL_SndSetMaxVol(void *work);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// Silences the sound and restores it, as the callbacks of closing and opening the lid
void GFL_SndMute(void *work);
void GFLAppUpdate(void);
// Runs the VBlank callback, from the VBlank interrupt
void GFL_VBlankExecCallback(void);
// Runs the VBlank tasks and the library's VBlank updates, from the main loop after the VBlank
void GFL_VBlankUpdate(void);
// Does nothing, at the end of the main loop's updates
void func_0200560c(void);

#endif // BWPORT_SYSTEM_GFL_USE_H
