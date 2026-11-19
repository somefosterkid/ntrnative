#include "global.h"

#include "math_util.h"
#include "save.h"
#include "save_vars_flags.h"
#include "field_system.h"
#include "follow_mon_interact_work.h"

s32 ov02_02250738(s32, u8);

s32 ov02_02250594(u32, int);

s32 ov02_02250628(u32, u32);

s32 ov02_022506D4(u32, u32);

typedef struct UnkStruct_ov02_0224F108 {
    u8 b0;
    u8 b1a : 4;
    u8 b1b : 4;
    u8 b2a : 3;
    u8 b2b : 2;
    u8 b2c : 3;
    u8 b3 : 5;
    u8 pad3 : 3;
    u8 b4a : 5;
    u8 b4b : 3;
    u8 b5;
    u8 b6;
    u8 b7;
    u16 w8a : 3;
    u16 w8pad : 10;
    u16 w8b : 3;
    u16 wAa : 3;
    u16 wAb : 3;
    u16 wApad : 10;
    u16 wC;
    u16 wE;
    u8 b10pad : 1;
    u8 b10a : 2;
    u8 b10b : 2;
    u8 b10c : 3;
    u8 b11;
    u16 w12;
} UnkStruct_ov02_0224F108;

s32 ov02_0224F108(FieldSystem *arg0, UnkStruct_ov02_0224F108 *arg1, FollowMonInteractWork *arg2);

s32 ov02_0224F108(FieldSystem *arg0, UnkStruct_ov02_0224F108 *arg1, FollowMonInteractWork *arg2) {
    u16 v;
    int new_var;
    new_var = 0;
    if (((s32)(LCRandom() % 100)) >= arg1->b11) {
        return 0;
    }
    v = arg1->w12;
    if ((v != new_var) && (Save_VarsFlags_CheckFlagInArray(Save_VarsFlags_Get(arg0->saveData), v) == 0)) {
        return new_var;
    }
    if (arg1->b3 != new_var) {
        if (arg1->b3 == 9) {
            if (arg2->hasItem == new_var) {
                return new_var;
            }
        } else if (arg1->b3 != arg2->itemClass) {
            return 0;
        }
    }
    if ((arg1->b0 != new_var) && (arg1->b0 != arg2->hpClass)) {
        return new_var;
    }
    if (arg1->b2c != new_var) {
        if (arg1->b2c == 7) {
            if (((((arg2->statusClass != 2) && (arg2->statusClass != 3)) && (arg2->statusClass != 4)) && (arg2->statusClass != 5)) && (arg2->statusClass != 8)) {
                return 0;
            }
        } else if (arg1->b2c != arg2->statusClass) {
            return new_var;
        }
    }
    if (arg1->wAa != new_var) {
        if (arg1->wAa == 5) {
            if (arg2->nearbyObjCount < 5) {
                return 0;
            }
        } else if (arg1->wAa != arg2->nearbyObjCount) {
            return 0;
        }
    }
    if (arg1->b10c != 0) {
        if (arg1->b10c == 4) {
            if (arg2->hiddenItemCount < 4) {
                return 0;
            }
        } else if (arg1->b10c != arg2->hiddenItemCount) {
            return new_var;
        }
    }
    if ((arg1->b10a != 0) && (arg1->b10a != arg2->levelClass)) {
        return 0;
    }
    if (arg1->b10b != 0) {
        if ((arg1->b10b == 3) && (arg2->nearSprite55 == 0)) {
            return new_var;
        }
        if ((arg1->b10b == 1) && (arg2->nearSprite54 == new_var)) {
            return new_var;
        }
        return 0;
    }
    if (((arg1->b4a != 0) && (arg1->b4a != arg2->type1Class)) && (arg1->b4a != arg2->type2Class)) {
        return new_var;
    }
    if ((arg1->w8a != new_var) && (arg1->w8a != arg2->unk_11)) {
        return 0;
    }
    if ((arg1->wE != 0) && (arg1->wE != arg2->unk_12)) {
        return new_var;
    }
    if ((arg1->b5 != new_var) && (arg1->b5 != arg2->unk_13)) {
        return 0;
    }
    if ((arg1->wC != 0) && ((arg1->wC - 1) != arg2->mapId)) {
        return 0;
    }
    if ((arg1->wAb != new_var) && (arg1->wAb != arg2->unk_14)) {
        return new_var;
    }
    if ((arg1->b1a != new_var) && (ov02_02250628(arg1->b1a, arg2->mood) == new_var)) {
        return new_var;
    }
    if ((arg1->b4b != new_var) && (arg1->b4b != arg2->unk_16)) {
        return new_var;
    }
    if ((arg1->b1b != 0) && (ov02_02250594(arg1->b1b, arg2->unk_07) == 0)) {
        return 0;
    }
    if ((arg1->b2a != 0) && (arg1->b2a != arg2->natureClass)) {
        return new_var;
    }
    if ((arg1->b6 != 0) && (ov02_022506D4(arg1->b6, arg2->unk_0A) == 0)) {
        return new_var;
    }
    if ((arg1->b7 != 0) && (ov02_02250738(arg1->b7, arg2->unk_0B) == 0)) {
        return 0;
    }
    if ((arg1->b2b != new_var) && (arg1->b2b != arg2->unk_09)) {
        return 0;
    }
    if ((arg1->w8b != 0) && (arg1->w8b != arg2->unk_17)) {
        return new_var;
    }
    return 1;
}
