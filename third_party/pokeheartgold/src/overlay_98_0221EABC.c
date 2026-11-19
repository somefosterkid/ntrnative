#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_98.h"
#include "pm_string.h"


Ov98TextSys *ov98_0221EABC(enum HeapID heapId, BgConfig *bgConfig, s32 windowCount, WindowTemplate *templates, s32 msgBank) {
    Ov98TextSys *textSys;
    s32 i;

    FontID_Alloc(FONT_UNK_4, heapId);
    textSys = Heap_Alloc(heapId, sizeof(Ov98TextSys));
    MI_CpuFill8(textSys, 0, sizeof(Ov98TextSys));
    textSys->windows = Heap_Alloc(heapId, windowCount * sizeof(Window));
    MI_CpuFill8(textSys->windows, 0, windowCount * sizeof(Window));
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_12_OFFSET, heapId);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_12_OFFSET, heapId);
    for (i = 0; i < windowCount; i++) {
        AddWindow(bgConfig, &textSys->windows[i], templates);
        FillWindowPixelBuffer(&textSys->windows[i], 0);
        ClearWindowTilemap(&textSys->windows[i]);
        templates++;
    }
    textSys->commonMsgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0xed, heapId);
    textSys->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, msgBank, heapId);
    textSys->msgFmt = MessageFormat_New(heapId);
    textSys->string = String_New(0x400, heapId);
    textSys->heapId = heapId;
    textSys->windowCount = windowCount;
    return textSys;
}

void ov98_0221EB84(Ov98TextSys *textSys, s32 windowCount) {
    s32 i;

    FontID_Release(FONT_UNK_4);
    String_Delete(textSys->string);
    MessageFormat_Delete(textSys->msgFmt);
    DestroyMsgData(textSys->commonMsgData);
    DestroyMsgData(textSys->msgData);
    for (i = 0; i < windowCount; i++) {
        ClearWindowTilemapAndCopyToVram(&textSys->windows[i]);
        RemoveWindow(&textSys->windows[i]);
    }
    Heap_Free(textSys->windows);
    Heap_Free(textSys);
}
