#include "global.h"

#include "font.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_manager.h"
#include "render_text.h"
#include "screen_fade.h"
#include "system.h"
#include "unk_02034B0C.h"
#include "unk_020915B0.h"
#include "overlay_72_wfc_connect.h"

s32 ov72_02238160(NintendoWfcConnect *);

s32 ov72_02238800(NintendoWfcConnect *);

BOOL NintendoWfc_Exit(OverlayManager *man, int *state) {
    NintendoWfcConnect *v0;

    v0 = OverlayManager_GetData(man);
    if (IsPaletteFadeFinished() == 0) {
        return 0;
    }
    TextFlags_SetCanTouchSpeedUpPrint(0);
    FontID_Release(FONT_UNK_4);
    Heap_Free(v0->heapMem);
    UnloadOVY38();
    UnloadDwcOverlay();
    ov72_02238800(v0);
    DestroyMsgData(v0->msgDataError);
    DestroyMsgData(v0->msgDataWfc);
    DestroyMsgData(v0->msgDataTower);
    MessageFormat_Delete(v0->msgFormat);
    ov72_02238160(v0);
    sub_02034DE0();
    Heap_Free(v0->bgConfig);
    OverlayManager_FreeData(man);
    Main_SetVBlankIntrCB(NULL, NULL);
    Heap_Destroy(HEAP_ID_67);
    return 1;
}
