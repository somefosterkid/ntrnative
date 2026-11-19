#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "heap.h"
#include "sys_task_api.h"
#include "system.h"
#include "unk_02009D48.h"
#include "unk_0200B150.h"
#include "sprite.h"

void *ov102_021E8F68(void *);

s32 ov102_021E9198(void *);

SysTask *ov102_021E93D4(s32 *, void *, s32);

void ov102_021E93DC(SysTask *task, void *data);

extern s32 ov102_021E93E0;

void *ov102_021E909C(void *arg0, s32 arg1);

SysTask *ov102_021E91BC(void *);
void ov102_021E9144(void *arg0);

void *ov102_021E909C(void *arg0, s32 arg1) {
    s32 v1;
    void *v0;
    void *v2;

    v0 = Heap_Alloc(HEAP_ID_35, 0x234U);
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    *(void **)((u8 *)v0 + 0x18) = arg0;
    *(s32 *)((u8 *)v0 + 0x1C) = arg1;
    *(void **)((u8 *)v0 + 0x1F8) = ov102_021E8F68(arg0);
    ov102_021E9198(v0);
    NNS_G2dInitOamManagerModule();
    OamManager_Create(1, 0x7F, 0, 0x20, 0, 0x7F, 0, 0x20, HEAP_ID_35);
    *(SpriteList **)((u8 *)v0 + 0x24) = G2dRenderer_Init(0x80, v0 + 0x28, HEAP_ID_35);
    *(BgConfig **)((u8 *)v0 + 0x20) = BgConfig_Alloc(HEAP_ID_35);
    *(SysTask **)v0 = SysTask_CreateOnMainQueue(ov102_021E93DC, v0, 2U);
    *(void **)((u8 *)v0 + 4) = ov102_021E93D4(&ov102_021E93E0, v0, 1);
    v1 = 0;
    v2 = v0;
    do {
        v1 += 1;
        *(s32 *)((u8 *)v2 + 8) = 0;
        v2 += 4;
    } while (v1 < 4);
    FontID_Alloc(FONT_UNK_4, HEAP_ID_35);
    return v0;
}

void ov102_021E9144(void *ptr) {
    SysTask *task;
    s32 v0;
    void *v1;

    if (ptr != NULL) {
        Main_SetVBlankIntrCB(NULL, NULL);
        FontID_Release(FONT_UNK_4);
        v0 = 0;
        v1 = ptr;
        do {
            task = *(SysTask **)((u8 *)v1 + 8);
            if (task != NULL) {
                SysTask_Destroy(task);
            }
            v0 += 1;
            v1 += 4;
        } while (v0 < 4);
        SysTask_Destroy(*(SysTask **)ptr);
        SysTask_Destroy(*(SysTask **)((u8 *)ptr + 4));
        OamManager_Free();
        SpriteList_Delete(*(SpriteList **)((u8 *)ptr + 0x24));
        ov102_021E91BC(ptr);
        Heap_Free(*(void **)((u8 *)ptr + 0x20));
        Heap_Free(ptr);
    }
}
