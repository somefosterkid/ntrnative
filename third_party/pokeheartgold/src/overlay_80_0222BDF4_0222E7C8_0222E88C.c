#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "list_menu_2d.h"
#include "msgdata.h"
#include "pm_string.h"
#include "render_window.h"
#include "sys_task_api.h"
#include "unk_02005D10.h"

void ov80_0222E88C(void *arg0);

void ov80_0222E88C(void *ptr) {
    s32 v1;
    void *v0;
    void *v2;

    v0 = *(void **)ptr;
    PlaySE(0x5DCU);
    Delete2dMenu(*(struct ListMenu2D **)((u8 *)ptr + 0xB0), NULL);
    ClearWindowPixelBuffer(*(Window **)((u8 *)ptr + 0xA8), 0);
    RemoveWindow(*(Window **)((u8 *)ptr + 0xA8));
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
