#include "global.h"

#include "save.h"
#include "unk_0202C034.h"
#include "filesystem.h"
#include "gf_gfx_planes.h"
#include "render_text.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "camera.h"
#include "pm_string.h"
#include "sprite.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"
#include "vram_transfer_manager.h"
#include "overlay_43_palpad.h"

s32 sub_0202C2DC(WiFiList *, s32);

void ov43_0222A420(PalPadShared *shared);

void ov43_0222A550(void);
void ov43_0222A570(PalPadGfx *gfx, enum HeapID heapID);
void ov43_0222A690(PalPadGfx *gfx, enum HeapID heapID);
void ov43_0222A87C(PalPadGfx *gfx, enum HeapID heapID);
void ov43_0222A8C0(PalPadGfx *gfx, PalPadShared *shared, enum HeapID heapID);
void ov43_0222A998(PalPadGfx *gfx, enum HeapID heapID);
void ov43_0222AC28(PalPadGfx *gfx, enum HeapID heapID);
void ov43_0222A48C(PalPadGfx *gfx, PalPadShared *shared, enum HeapID heapID);

void ov43_0222A500(PalPadGfx *gfx);
void ov43_0222A50C(PalPadGfx *gfx);

void ov43_0222A66C(PalPadGfx *gfx);
void ov43_0222A81C(PalPadGfx *gfx);
void ov43_0222A8A8(PalPadGfx *gfx);
void ov43_0222A960(PalPadGfx *gfx);
void ov43_0222A9D8(PalPadGfx *gfx);
void ov43_0222ACDC(PalPadGfx *gfx);
void ov43_0222A520(PalPadGfx *gfx);

void ov43_0222A420(PalPadShared *shared) {
    u8 number;
    WiFiList *wifiList;
    s32 count;
    s32 i;

    memset(&shared->friends, 0, sizeof(PalPadFriendList));
    wifiList = sub_0202C6F4(shared->saveData);
    i = 0;
    shared->friends.count = 0;
    do {
        if (sub_0202C2DC(wifiList, i) == 1) {
            shared->friends.slots[shared->friends.count] = (s8)i;
            if (sub_0202C090(wifiList, i, 8) == 2) {
                count = shared->friends.count;
                number = 0xff;
                shared->friends.numbers[count] = number;
            } else {
                number = (u8)shared->friends.numberedCount;
                shared->friends.numberedCount = shared->friends.numberedCount + 1;
                count = shared->friends.count;
                shared->friends.numbers[count] = number;
            }
            shared->friends.count = shared->friends.count + 1;
        }
        i = i + 1;
    } while (i < 0x20);
}

void ov43_0222A48C(PalPadGfx *gfx, PalPadShared *shared, enum HeapID heapID) {
    gSystem.screensFlipped = 1;
    GfGfx_SwapDisplay();
    gfx->narc = NARC_New(NARC_a_0_8_5, heapID);
    ov43_0222A550();
    ov43_0222A570(gfx, heapID);
    ov43_0222A690(gfx, heapID);
    ov43_0222A87C(gfx, heapID);
    ov43_0222A8C0(gfx, shared, heapID);
    ov43_0222AC28(gfx, heapID);
    ov43_0222A998(gfx, heapID);
    GfGfx_EngineATogglePlanes(0x10U, 1U);
    TextFlags_SetCanABSpeedUpPrint(1);
    TextFlags_SetAutoScrollParam(0);
    TextFlags_SetCanTouchSpeedUpPrint(0);
}

void ov43_0222A500(PalPadGfx *gfx) {
    SpriteList_RenderAndAnimateSprites(gfx->spriteList);
}

void ov43_0222A50C(PalPadGfx *gfx) {
    DoScheduledBgGpuUpdates(gfx->bgConfig);
    GF_RunVramTransferTasks();
    OamManager_ApplyAndResetBuffers();
}

void ov43_0222A520(PalPadGfx *gfx) {
    ov43_0222ACDC(gfx);
    ov43_0222A9D8(gfx);
    ov43_0222A960(gfx);
    ov43_0222A8A8(gfx);
    ov43_0222A81C(gfx);
    ov43_0222A66C(gfx);
    NARC_Delete(gfx->narc);
}
