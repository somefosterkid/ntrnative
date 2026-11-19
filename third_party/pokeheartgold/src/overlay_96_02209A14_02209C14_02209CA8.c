#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "overlay_96_ov96_0220_data.h"

extern u8 ov96_0221CC48[];

void ov96_02209CA8(Ov96_0220Data *param_1);

void ov96_02209CA8(Ov96_0220Data *param_1) {
    s32 iVar1;
    Window *window;
    u32 *puVar2;
    s32 iVar3;

    iVar1 = 0;
    do {
        String_Delete(param_1->names[iVar1]);
        iVar1 = iVar1 + 1;
    } while (iVar1 < 3);
    MessageFormat_Delete(param_1->msgFormat);
    DestroyMsgData(param_1->msgData);
    iVar3 = 0;
    window = param_1->windows;
    do {
        RemoveWindow(window);
        iVar3 = iVar3 + 1;
        window = window + 1;
    } while (iVar3 < 4);
    puVar2 = (u32 *)ov96_0221CC48;
    iVar3 = 0;
    do {
        FreeBgTilemapBuffer(param_1->bgConfig, (u8)*puVar2);
        iVar3 = iVar3 + 1;
        puVar2 = puVar2 + 1;
    } while (iVar3 < 6);
    FontID_Release(FONT_UNK_4);
    Heap_Free(param_1->bgConfig);
    return;
}
