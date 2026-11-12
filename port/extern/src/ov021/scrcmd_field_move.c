// The PC build compiles src/ov021/scrcmd_field_move.c through this file (tools/port_sources.py). The port's transition
// window (port/extern/src/system/game_event.c; the maintainers' notes, 4. and 5.): the Waterfall climb is a field task the
// script starts (s00C6_CallWaterfall) and waits for (ScriptNative_WaterfallTCBWait, which frees it when it ends), so the
// window is open from the task's start to its free. Only this file's two calls are renamed. The DS build never sees this file.
#pragma push_macro("FieldWaterfallTCB_Create")
#define FieldWaterfallTCB_Create PortWaterfall_Create
#pragma push_macro("FieldWaterfallTCB_Free")
#define FieldWaterfallTCB_Free PortWaterfall_Free
#include "../../../../src/ov021/scrcmd_field_move.c"
#pragma pop_macro("FieldWaterfallTCB_Create")
#pragma pop_macro("FieldWaterfallTCB_Free")

void *FieldWaterfallTCB_Create(FieldPlayer *player, u32 direction, u32 param, HeapID heapId);
void FieldWaterfallTCB_Free(void *tcb);
extern BOOL PortTransition_waterfall;

void *PortWaterfall_Create(FieldPlayer *player, u32 direction, u32 param, HeapID heapId) {
    PortTransition_waterfall = TRUE;
    return FieldWaterfallTCB_Create(player, direction, param, heapId);
}

void PortWaterfall_Free(void *tcb) {
    FieldWaterfallTCB_Free(tcb);
    PortTransition_waterfall = FALSE;
}
