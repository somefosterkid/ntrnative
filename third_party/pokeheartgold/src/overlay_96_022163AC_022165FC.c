#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"

#include "overlay_96_ov96_rd_work.h"

extern u8 ov96_0221D738[];

void ov96_022165FC(Ov96RdWork *work);

void ov96_022165FC(Ov96RdWork *work) {
    s32 iVar1;
    u32 *puVar2;

    RemoveWindow(&work->window);
    MessageFormat_Delete(work->msgFormat);
    DestroyMsgData(work->msgData);
    puVar2 = (u32 *)ov96_0221D738;
    iVar1 = 0;
    do {
        FreeBgTilemapBuffer(work->bgConfig, (u8)*puVar2);
        iVar1 = iVar1 + 1;
        puVar2 = puVar2 + 1;
    } while (iVar1 < 7);
    FontID_Release(FONT_UNK_4);
    Heap_Free(work->bgConfig);
    return;
}
