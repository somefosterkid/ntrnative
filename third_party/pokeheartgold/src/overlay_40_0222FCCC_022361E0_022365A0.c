#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "overlay_40.h"
#include "save_arrays.h"
#include "touch_hitbox_controller.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

s32 ov40_0222BF80(void *, s32);

s32 ov40_0222D9E8(void *, void *, s32);

void ov40_02236230(u32 index, u32 event, void *arg);

void ov40_022362E4(u32 index, u32 event, void *arg);

void ov40_02236320(u32 index, u32 event, void *arg);

void ov40_0223635C(u32 index, u32 event, void *arg);

void ov40_022363BC(u32 index, u32 event, void *arg);

s32 ov40_0223655C(void *);

s32 ov40_02236578(SaveData *, PCStorage *, s32, void *);

void *sub_020314A4(s32);

s32 sub_020314C4(void *, SaveData *);

extern TouchscreenHitbox ov40_0224526C;

extern TouchscreenHitbox ov40_02245284;

extern TouchscreenHitbox ov40_022452CC;

extern TouchscreenHitbox ov40_022452F4;

s32 ov40_022365A0(UnkStruct_0208C06C *callbackArg) {
    void *v0;

    v0 = Heap_Alloc(HEAP_ID_109, 0x2F70U);
    memset(v0, 0, 0x2F70U);
    callbackArg->unk_860 = v0;
    BgClearTilemapBufferAndCommit(callbackArg->unk_14.unk_10, 2U);
    BgClearTilemapBufferAndCommit(callbackArg->unk_14.unk_10, 3U);
    BgClearTilemapBufferAndCommit(callbackArg->unk_14.unk_10, 6U);
    BgClearTilemapBufferAndCommit(callbackArg->unk_14.unk_10, 7U);
    *(PCStorage **)v0 = SaveArray_PCStorage_Get(((SaveData *)callbackArg->saveData));
    ov40_02236578(((SaveData *)callbackArg->saveData), *(PCStorage **)v0, *(s32 *)((u8 *)v0 + 0x1A0), v0 + 4);
    ov40_0223655C(v0);
    ov40_0222D9E8(v0 + 0x1A4, v0 + 0x1A8, 0);
    *(TouchHitboxController **)((u8 *)v0 + 0x330) = TouchHitboxController_Create(&ov40_022452CC, 4U, ov40_02236230, callbackArg, HEAP_ID_109);
    *(TouchHitboxController **)((u8 *)v0 + 0x334) = TouchHitboxController_Create(&ov40_022452F4, 7U, ov40_022362E4, callbackArg, HEAP_ID_109);
    *(TouchHitboxController **)((u8 *)v0 + 0x338) = TouchHitboxController_Create(&ov40_022452F4, 7U, ov40_02236320, callbackArg, HEAP_ID_109);
    *(TouchHitboxController **)((u8 *)v0 + 0x33C) = TouchHitboxController_Create(&ov40_02245284, 3U, ov40_0223635C, callbackArg, HEAP_ID_109);
    *(TouchHitboxController **)((u8 *)v0 + 0x340) = TouchHitboxController_Create(&ov40_0224526C, 2U, ov40_022363BC, callbackArg, HEAP_ID_109);
    *(void **)((u8 *)v0 + 0x384) = sub_020314A4(0x6D);
    sub_020314C4(*(void **)((u8 *)v0 + 0x384), ((SaveData *)callbackArg->saveData));
    ov40_0222BF80(callbackArg, 1);
    return 0;
}
