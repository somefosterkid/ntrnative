#include "global.h"
struct MoveRelearner;

#include "overlay_manager.h"
#include "unk_02019BA4.h"
#include "overlay_68_move_relearner.h"

s32 ov68_021E5A58(MoveRelearner *);

s32 ov68_021E73A4(MoveRelearner *, u32, s32);

s32 ov68_021E7A18(MoveRelearner *, s32);

s32 ov68_021E7568(MoveRelearner *arg0);

s32 ov68_021E7568(MoveRelearner *arg0) {
    if (OverlayManager_Run(arg0->summaryManager) != 0) {
        OverlayManager_Delete(arg0->summaryManager);
        ov68_021E5A58(arg0);
        ov68_021E7A18(arg0, arg0->args->cursorPos);
        ov68_021E73A4(arg0, (u8)arg0->args->cursorPos, 3);
        arg0->args->moveSlot = (u8)arg0->summary.unk16;
        arg0->nextState = 7;
        return 0;
    }
    return 0xC;
}

s32 ov68_021E7898(MoveRelearner *, s32);

s32 ov68_021E7910(MoveRelearner *);

extern GridCallbacks ov68_021E7C08;

extern TouchscreenHitbox ov68_021E7CF0;

extern DpadMenuBox ov68_021E7D64;

void ov68_021E75C0(struct MoveRelearner *callbackParam);

void ov68_021E75C0(MoveRelearner *callbackParam) {
    *((GridInputHandler * *)&callbackParam->gridInput) = GridInputHandler_Create(&ov68_021E7CF0, &ov68_021E7D64, &ov68_021E7C08, callbackParam, 1, 0U, HEAP_ID_66);
    ov68_021E7898(callbackParam, 0);
    ov68_021E7910(callbackParam);
}
