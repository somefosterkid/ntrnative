#include "global.h"
struct Ov27Task;

#include "field_system.h"
#include "field_types_def.h"
#include "overlay_27.h"
#include "screen_fade.h"
#include "sprite.h"
#include "task.h"
#include "touchscreen.h"
#include "unk_02009D48.h"
#include "overlay_27_ov27task.h"

s32 ov27_0225A4D0(Ov27Task *arg0);

void ov27_0225A66C(Ov27Task *arg0);

s32 ov27_0225A89C(FieldSystem *arg0);

s32 ov27_0225A48C(Ov27Task *, s32);

s32 ov27_0225A530(Ov27Task *, s32);

s32 ov27_0225A7FC(Ov27Task *);

s32 ov27_0225A86C(Ov27Task *);

s32 ov27_0225A8E8(Ov27Task *, s32);

s32 ov27_0225B4D8(Ov27Task *);

s32 ov27_0225BDFC(void *);

extern TouchscreenHitbox ov27_0225CECC;

void ov27_0225A468(Ov27Task *arg0, s32 arg1);

void ov27_0225A320(SysTask *sysTask, void *data) {
    Ov27Task *task = data;
    FieldSystem *fieldSystem;
    FieldSystem *v1;
    s32 v2;
    s32 v3;
    s32 v4;
    s32 v5;
    u32 v0;

    v2 = ov27_0225A89C(task->fieldSystem);
    v3 = FieldSystem_IsPlayerMovementAllowed(task->fieldSystem);
    if (task->fieldSystem->unkD2_7 != 0) {
        if ((v3 != 0) && (IsPaletteFadeFinished() != 0)) {
            fieldSystem = task->fieldSystem;
            fieldSystem->unkD2_7 = FALSE;
        } else {
            *(s16 *)REG_DB_BLDCNT_ADDR = 0;
        }
    } else {
        ov27_0225A8E8(task, v2);
    }
    if ((task->unk_51C_5) != 0) {
        Sprite_TryChangeAnimSeq(task->unk_390[task->unk_14], 2);
    }
    v0 = task->unk_51C_6;
    if (v0 == 1) {
        Sprite_SetAnimCtrlSeq(task->unk_390[7], 2);
        task->unk_51C_6 = 0;
    } else if (v0 == 2) {
        Sprite_SetAnimCtrlSeq(task->unk_390[8], 2);
        task->unk_51C_6 = 0;
    }
    if (FieldSystem_TaskIsRunning(task->fieldSystem) != 0) {
        ov27_0225A7FC(task);
    } else {
        if (task->fieldSystem->unkD2_0 == 2) {
            ov27_0225A86C(task);
            v1 = task->fieldSystem;
            v1->unkD2_0 = 0;
        }
        if (v3 != 0) {
            ov27_0225A66C(task);
        }
        if (v2 == 0) {
            ov27_0225B4D8(task);
        }
    }
    v4 = TouchscreenHitbox_FindRectAtTouchHeld(&ov27_0225CECC);
    v5 = TouchscreenHitbox_FindRectAtTouchNew(&ov27_0225CECC);
    ov27_0225A530(task, v4);
    if (((task->unk_51C_0) != 0) && (ov27_0225A4D0(task) != 0) && (v2 == 0)) {
        ov27_0225A48C(task, v5);
    }
    ov27_0225BDFC(&task->unk_520);
    SpriteList_RenderAndAnimateSprites(task->unk_18);
}

void ov27_0225A468(Ov27Task *arg0, s32 arg1) {
    Sprite_SetAnimCtrlSeq(arg0->unk_390[13], 0xB - (arg1 * 4));
    Sprite_SetAnimCtrlSeq(arg0->unk_390[11], arg1 + 3);
}
