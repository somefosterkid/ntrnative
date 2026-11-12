// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/mb_parent/mbp.h: the lines in use so
// far.
#ifndef BWPORT_APP_MB_PARENT_MBP_H
#define BWPORT_APP_MB_PARENT_MBP_H

#include "types.h"
#include "gfl/heap.h"
#include "nitro/mb.h"

// Black 2's; twinned code confirms the values of MBP_STATE_STOP, MBP_STATE_REBOOTING, MBP_STATE_COMPLETE,
// MBP_STATE_CANCEL and MBP_STATE_ERROR in Black
// The parent's states
enum {
    MBP_STATE_STOP,
    MBP_STATE_IDLE,
    MBP_STATE_ENTRY,
    MBP_STATE_DATASENDING,
    MBP_STATE_REBOOTING,
    MBP_STATE_COMPLETE,
    MBP_STATE_CANCEL,
    MBP_STATE_ERROR,
};

// pokebw2's MBP_IsBootableAll
BOOL func_ov107_021f0a28(void);
// pokebw2's MBP_StartRebootAll
void func_ov107_021f0a64(void);
// pokebw2's MBP_Cancel
void func_ov107_021f0ae4(void);
// pokebw2's MBP_GetState
u16 func_ov107_021f0d28(void);
// pokebw2's MBP_FreeBuffers
// Frees the file buffer and the library's work
void func_ov107_021f0db4(void);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's MBP_Init
void func_ov107_021f0754(HeapID heapId, u32 ggid);
// pokebw2's MBP_Start
void func_ov107_021f080c(const MBGameRegistry *gameInfo, u16 channel);
// pokebw2's MBP_IsStarted
// Whether the library runs, from MBP_Start until it is ended
BOOL func_ov107_021f0ddc(void);

#endif // BWPORT_APP_MB_PARENT_MBP_H
