#include "global.h"

#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_manager.h"
#include "system.h"
#include "unk_02034B0C.h"
#include "unk_020915B0.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

s32 ov73_021E8164(void *);

s32 ov73_021E847C(void *);

s32 ov73_021E808C(OverlayManager *man, s32 *state);

s32 ov73_021E808C(OverlayManager *man, s32 *state) {
    WifiBattleTowerAppState *v0;

    v0 = OverlayManager_GetData(man);
    Heap_Free(v0->dwcHeapBuffer);
    UnloadOVY38();
    UnloadDwcOverlay();
    ov73_021E847C(v0);
    DestroyMsgData(v0->msgLoader3);
    DestroyMsgData(v0->msgLoader2);
    DestroyMsgData(v0->msgLoader1);
    MessageFormat_Delete(v0->stringTemplate);
    ov73_021E8164(v0);
    sub_02034DE0();
    Heap_Free(v0->bgConfig);
    OverlayManager_FreeData(man);
    Main_SetVBlankIntrCB(NULL, NULL);
    Heap_Destroy(HEAP_ID_150);
    return 1;
}
