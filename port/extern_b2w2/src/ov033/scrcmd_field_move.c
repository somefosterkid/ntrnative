// Black 2's and White 2's transition window for the Waterfall climb (port/extern/src/ov021/scrcmd_field_move.c's twin;
// port/extern_b2w2/src/system/game_event.c): pokebw2's src/ov033/scrcmd_field_move.c compiled through this file. The
// climb is a field task the script starts (Scrcmd_FieldMove_CallWaterfall) and waits for (ScriptNative_WaterfallTCBWait,
// which frees it when it ends), so the window is open from the task's start to its free. Only this file's two calls
// are renamed. The DS build never sees this file.
#define FieldWaterfallTCB_Create PortWaterfall_Create
#define FieldWaterfallTCB_Free PortWaterfall_Free
#include "../../../../third_party/pokebw2/src/ov033/scrcmd_field_move.c"
#undef FieldWaterfallTCB_Create
#undef FieldWaterfallTCB_Free

TCB *FieldWaterfallTCB_Create(FieldPlayer *player, u32 direction, u32 param, HeapID heapId);
void FieldWaterfallTCB_Free(TCB *tcb);
extern BOOL PortTransition_waterfall;

TCB *PortWaterfall_Create(FieldPlayer *player, u32 direction, u32 param, HeapID heapId) {
    PortTransition_waterfall = TRUE;
    return FieldWaterfallTCB_Create(player, direction, param, heapId);
}

void PortWaterfall_Free(TCB *tcb) {
    FieldWaterfallTCB_Free(tcb);
    PortTransition_waterfall = FALSE;
}
