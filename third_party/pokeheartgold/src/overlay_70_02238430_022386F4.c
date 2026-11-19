#include "global.h"

#include "font.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_manager.h"
#include "render_text.h"
#include "system.h"
#include "unk_02034B0C.h"
#include "unk_020915B0.h"
#include "overlay_70_gtsapplication_state.h"

s32 ov70_0223887C(void *);

s32 ov70_02238E98(void *);

s32 ov70_022386F4(OverlayManager *man, s32 *state);

s32 ov70_022386F4(OverlayManager *man, s32 *state) {
    void *v0;

    v0 = OverlayManager_GetData(man);
    Heap_Free(((GTSApplicationState *)v0)->dwcHeapPointer);
    UnloadOVY38();
    UnloadDwcOverlay();
    ov70_02238E98(v0);
    DestroyMsgData(((GTSApplicationState *)v0)->speciesMessageLoader);
    DestroyMsgData(((GTSApplicationState *)v0)->disconnStatusMessageLoader);
    DestroyMsgData(((GTSApplicationState *)v0)->connProgressMessageLoader);
    DestroyMsgData(((GTSApplicationState *)v0)->gtsMessageLoader);
    DestroyMsgData(((GTSApplicationState *)v0)->countryMessageLoader);
    MessageFormat_Delete(((GTSApplicationState *)v0)->stringTemplate);
    ov70_0223887C(v0);
    sub_02034DE0();
    TextFlags_SetCanTouchSpeedUpPrint(0);
    Heap_Free(((void *)((GTSApplicationState *)v0)->bgConfig));
    Heap_Free(*(void **)v0);
    OverlayManager_FreeData(man);
    FontID_Release(FONT_UNK_4);
    *(s32 *)REG_DISPCNT_ADDR &= 0xFFFF1FFF;
    *(s32 *)REG_DB_DISPCNT_ADDR &= 0xFFFF1FFF;
    Main_SetVBlankIntrCB(NULL, NULL);
    Heap_Destroy(HEAP_ID_61);
    return 1;
}
