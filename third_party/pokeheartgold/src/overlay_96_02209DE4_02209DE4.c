#include "global.h"

#include "constants/heap.h"

#include "pokeathlon/pokeathlon.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "overlay_96_ov96_0220_data.h"

extern u8 ov96_0221CC60[];

void ov96_02209DE4(PokeathlonCourseData *param_1);

void ov96_02209DE4(PokeathlonCourseData *param_1) {
    Ov96_0220Data *pHVar1;
    MsgData *pMVar2;
    MessageFormat *pMVar3;
    BOOL index;
#if !defined(SDK_PORT) && !defined(NONMATCHING)
    enum HeapID *pHVar5; // DS: walks the String slots through the heapId word
#endif
    u8 *puVar4;
    String *string;
    WindowTemplate *template;
    Window *window;
    s32 iVar6;
    pHVar1 = PokeathlonCourse_GetHeapAllocPtr4(param_1);
    pMVar2 = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x135, pHVar1->heapId);
    pHVar1->msgData = pMVar2;
    pMVar3 = MessageFormat_New(pHVar1->heapId);
    pHVar1->msgFormat = pMVar3;
    template = (WindowTemplate *)ov96_0221CC60;
    iVar6 = 0;
    window = pHVar1->windows;
    do {
        AddWindow(pHVar1->bgConfig, window, template);
        FillWindowPixelBuffer(window, 0);
        ClearWindowTilemap(window);
        iVar6 = iVar6 + 1;
        template = template + 1;
        window = window + 1;
    } while (iVar6 < 4);
    index = ov96_021E5F24(param_1);
    puVar4 = PokeathlonCourse_GetParticipantUnk04(param_1, index);
    iVar6 = 0;
#if defined(SDK_PORT) || defined(NONMATCHING)
    do {
        string = String_New(0xb, pHVar1->heapId);
        pHVar1->names[iVar6] = string;
        CopyU16ArrayToString(string, (u16 *)(puVar4 + 0x12));
        iVar6 = iVar6 + 1;
        puVar4 = puVar4 + 0x28;
    } while (iVar6 < 3);
#else
    pHVar5 = (enum HeapID *)pHVar1;
    do {
        string = String_New(0xb, pHVar1->heapId);
        pHVar5[6] = (enum HeapID)string;
        CopyU16ArrayToString(string, (u16 *)(puVar4 + 0x12));
        pHVar5 = pHVar5 + 1;
        iVar6 = iVar6 + 1;
        puVar4 = puVar4 + 0x28;
    } while (iVar6 < 3);
#endif
    return;
}
