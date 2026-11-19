#include "global.h"

#include "heap.h"
#include "overlay_manager.h"
#include "system.h"
#include "overlay_68_move_relearner.h"

s32 ov68_021E5A58(MoveRelearner *);

void ov68_021E5B6C(void *);

s32 ov68_021E73A4(MoveRelearner *, u32, s32);

s32 ov68_021E7424(MoveRelearner *, s32, s32);

s32 MoveRelearner_Init(OverlayManager *man, s32 *state);

s32 MoveRelearner_Init(OverlayManager *man, s32 *state) {
    MoveRelearner *arg;

    Heap_Create(HEAP_ID_3, HEAP_ID_66, 0x24000U);
    arg = OverlayManager_CreateAndGetData(man, sizeof(MoveRelearner), HEAP_ID_66);
    memset(arg, 0, sizeof(MoveRelearner));
    arg->args = OverlayManager_GetArgs(man);
    ov68_021E5A58(arg);
    ov68_021E73A4(arg, (u8)arg->args->cursorPos, 3);
    arg->nextState = 1;
    ov68_021E7424(arg, 0, 0);
    Main_SetVBlankIntrCB(ov68_021E5B6C, arg);
    return 1;
}
