#include "global.h"

#include "sprite.h"
#include "system.h"
#include "unk_02005D10.h"
#include "unk_struct_ov70_02242e58.h"

struct Sprite;

struct SysTask;

struct SysTaskQueue;

struct System;

extern u8 ov70_02245D8A[];

extern u8 ov70_02245D8B[];

extern int ov70_02242164(void *work, int a);

void ov70_02242BBC(UnkStruct_ov70_02242E58 *arg0, s32 arg1);

void ov70_02238F9C(Sprite *sprite, int x, int y);

s32 ov70_02242B5C(UnkStruct_ov70_02242E58 *arg0, s32 arg1);

int ov70_02242C64(UnkStruct_ov70_02242E58 *work);

int ov70_02242C64(UnkStruct_ov70_02242E58 *work) {
    int keys = gSystem.newAndRepeatedKeys;
    int cur = work->unk_48;
    int ret;

    if (keys & 0x40) {
        PlaySE(0x5DC);
        if (work->unk_48 != 0) {
            work->unk_48 = work->unk_48 - 1;
        } else {
            work->unk_48 = 5;
        }
    } else if (keys & 0x80) {
        PlaySE(0x5DC);
        if (work->unk_48 != 5) {
            work->unk_48 = work->unk_48 + 1;
        } else {
            work->unk_48 = 0;
        }
    } else if (keys & 0x20) {
        ov70_02242BBC(work, -1);
    } else if (keys & 0x10) {
        ov70_02242BBC(work, 1);
    }

    if (cur != work->unk_48) {
        int n = work->unk_48 * 2;
        ov70_02238F9C(work->unk_C, ov70_02245D8A[n], ov70_02245D8B[n]);
        if (work->unk_48 == 5) {
            Sprite_SetAnimCtrlSeq(work->unk_C, 0x30);
        } else {
            Sprite_SetAnimCtrlSeq(work->unk_C, 0x32);
        }
    }

    ret = ov70_02242164(work, 3);
    if (ret != -1) {
        return ov70_02242B5C(work, ret);
    }
    keys = gSystem.newKeys;
    if (keys & 1) {
        return ov70_02242B5C(work, work->unk_48);
    }
    if (keys & 2) {
        PlaySE(0x5DC);
        return -2;
    }
    return -1;
}
