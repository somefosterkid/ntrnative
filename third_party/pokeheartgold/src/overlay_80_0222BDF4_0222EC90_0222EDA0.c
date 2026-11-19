#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "list_menu.h"
#include "msgdata.h"
#include "pm_string.h"
#include "render_window.h"
#include "sys_task_api.h"
#include "unk_02005D10.h"

void ov80_0222EDA0(void *arg0, s32 arg1);

void ov80_0222EDA0(void *ptr, s32 arg1) {
    s32 v1;
    void *v0;
    void *v2;

    v0 = *(void **)ptr;
    if (arg1 == 1) {
        PlaySE(0x5DCU);
    }
    DestroyListMenu(*(ListMenu **)((u8 *)ptr + 0x1B4), NULL, NULL);
    ClearWindowPixelBuffer(*(Window **)((u8 *)ptr + 0x1A0), 0);
    RemoveWindow(ptr + 8);
    v1 = 0;
    v2 = ptr;
    do {
        String_Delete(*(String **)((u8 *)v2 + 0x1C));
        v1 += 1;
        v2 += 4;
    } while (v1 < 0x1C);
    if (((u32)(*(u8 *)((u8 *)ptr + 0x97) << 0x1E) >> 0x1F) == 1) {
        DestroyMsgData(*(MsgData **)((u8 *)ptr + 0x8C));
    }
    SysTask_Destroy(*(SysTask **)((u8 *)ptr + 4));
    Heap_Free(ptr);
    *(s32 *)((u8 *)v0 + 0x60) = 0;
}
