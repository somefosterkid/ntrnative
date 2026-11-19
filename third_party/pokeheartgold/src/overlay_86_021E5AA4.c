#include "global.h"

#include "font.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_86_frontier_records_app.h"
#include "overlay_manager.h"
#include "palette.h"
#include "pm_string.h"
#include "system.h"
struct FrontierRecordsApp;

s32 ov86_021E5E54(BgConfig *);

u8 ov86_021E5E98(u8);

s32 ov86_021E6FF4(struct FrontierRecordsApp *arg0);

s32 ov86_021E7E40(Window *, u8);

SysTask *ov86_021E7E90(Window *);

s32 ov86_021E5AA4(OverlayManager *man, s32 *state);

s32 ov86_021E5AA4(OverlayManager *man, s32 *state) {
    FrontierRecordsApp *v0;

    v0 = OverlayManager_GetData(man);
    ov86_021E6FF4(v0);
    PaletteData_FreeBuffers(v0->plttData, PLTTBUF_MAIN_OBJ);
    PaletteData_FreeBuffers(v0->plttData, PLTTBUF_MAIN_BG);
    PaletteData_Free(v0->plttData);
    v0->plttData = NULL;
    DestroyMsgData(v0->msgData);
    MessageFormat_Delete(v0->msgFmt);
    String_Delete(v0->string);
    ov86_021E7E90(&v0->buttonWindow);
    ov86_021E7E40(v0->windows, ov86_021E5E98(v0->facility));
    ov86_021E5E54(v0->bgConfig);
    FontID_Release(FONT_UNK_4);
    OverlayManager_FreeData(man);
    Main_SetVBlankIntrCB(NULL, NULL);
    Heap_Destroy(HEAP_ID_121);
    return 1;
}
