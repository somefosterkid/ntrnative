#include "global.h"

#include "bag.h"
#include "bag_types_def.h"
#include "field_types_def.h"
#include "player_avatar.h"
#include "save.h"
#include "screen_fade.h"
#include "system.h"
#include "touchscreen.h"
#include "unk_02035900.h"
#include "unk_02056D7C.h"
#include "unk_02058034.h"
#include "overlay_27_ov27task.h"

void ov27_0225B398(Ov27Task *task, s32 arg1);

s32 ov27_0225BDAC(Ov27Task *task);

s32 ov27_0225C170(Ov27Task *task, s32 arg1);

s32 sub_02056EE0(void);

s32 sub_02058258(void);

extern const TouchscreenHitbox ov27_0225CF68[];

s32 ov27_0225B4D8(Ov27Task *task);

s32 ov27_0225B4D8(Ov27Task *task) {
    s32 hit;
    Bag *bag;

    if (PlayerAvatar_GetPlayerMoveState(task->fieldSystem->playerAvatar) != 0 || (gSystem.newKeys & PAD_PLUS_KEY_MASK) != 0) {
        return 1;
    }
    if (task->unk_51C_1 == 5) {
        u16 netId = sub_0203769C();
        if (!sub_02058740() || !sub_02058258() || !sub_02056EE0() || !sub_02057A0C() || sub_02057F18(netId) || sub_02037958()) {
            return 1;
        }
    }
    if (!IsPaletteFadeFinished()) {
        return 1;
    }
    if (task->unk_51C_5 == 1) {
        return 1;
    }
    hit = TouchscreenHitbox_FindRectAtTouchNew(ov27_0225CF68);
    if (hit > 0 && hit < 8) {
        s32 i = hit - 1;
        if (task->unk_470[i].unk_0 == 0) {
            return 0;
        }
    }
    bag = Save_Bag_Get(task->fieldSystem->saveData);
    if ((u32)(hit - 8) <= 1) {
        if (task->unk_51C_0 == 0) {
            return 1;
        }
        if (ov27_0225BDAC(task) == 0) {
            return 1;
        }
    }
    if (hit == 8) {
        if (Bag_GetRegisteredItem1(bag) == 0) {
            return 1;
        }
    } else if (hit == 9 && Bag_GetRegisteredItem2(bag) == 0) {
        return 1;
    }
    if (hit != -1) {
        if (hit == 0) {
            *task->unk_0C = 1;
        } else {
            s32 i = hit - 1;
            if (task->unk_390[i] != NULL) {
                if (hit < 8) {
                    task->unk_14 = i;
                    task->fieldSystem->unkD3 = ov27_0225C170(task, i);
                    ov27_0225B398(task, task->unk_14);
                }
                *task->unk_0C = ov27_0225C170(task, hit - 1) + 2;
            }
        }
        return 0;
    }
    return 1;
}
