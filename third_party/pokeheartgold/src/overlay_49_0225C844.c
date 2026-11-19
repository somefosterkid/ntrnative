#include "global.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "touch_hitbox_controller.h"
#include "heap.h"
#include "overlay_49_unk_struct_ov70_0225_dee8_t.h"

void ov49_0225CB50(u32 index, u32 event, void *arg);

extern TouchscreenHitbox ov49_022696F4;

extern WindowTemplate ov49_0226970C;

void ov49_0225C844(UnkStruct_ov70_02260744 *arg0, UnkStruct_ov70_0225E4EC *arg1, NARC *arg2, enum HeapID arg3);

void ov49_0225C8A8(UnkStruct_ov70_02260744 *arg0);

void ov49_0225C844(UnkStruct_ov70_02260744 *arg0, UnkStruct_ov70_0225E4EC *arg1, NARC *arg2, enum HeapID heapID) {
    NARC *narc;
    UnkStruct_ov70_02260744 *callbackArg;
    s32 v1;

    callbackArg = arg0;
    narc = arg2;
    AddWindow(arg1->unk_00, &callbackArg->unk_0C, &ov49_0226970C);
    v1 = 0;
    do {
        callbackArg->unk_20[v1] = GfGfxLoader_GetScrnDataFromOpenNarc(narc, v1 + 0x38, 0, &callbackArg->unk_2C[v1], heapID);
        v1 += 1;
    } while (v1 < 3);
    callbackArg->unk_1C = TouchHitboxController_Create(&ov49_022696F4, 1U, ov49_0225CB50, callbackArg, heapID);
    callbackArg->unk_06 = 1;
}

void ov49_0225C8A8(UnkStruct_ov70_02260744 *arg0) {
    s32 v0;

    TouchHitboxController_Destroy(arg0->unk_1C);
    RemoveWindow(&arg0->unk_0C);
    v0 = 0;
    do {
        Heap_Free(arg0->unk_20[v0]);
        v0 += 1;
    } while (v0 < 3);
    arg0->unk_02 = 0;
}
