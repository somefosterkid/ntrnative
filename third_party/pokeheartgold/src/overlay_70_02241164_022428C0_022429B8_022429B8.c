#include "global.h"

#include "sprite.h"
#include "system.h"
#include "unk_02005D10.h"
#include "unk_struct_ov70_02242e58.h"

struct Sprite;

struct SysTask;

struct SysTaskQueue;

struct System;

extern u8 ov70_02245EFC[];

extern u8 ov70_02245EFD[];

extern u8 ov70_02245EFE[];

extern u8 ov70_02245EFF[];

extern u8 ov70_02245DF8[];

extern u8 ov70_02245DF9[];

extern int ov70_02242164(void *work, int a);

extern int ov70_022429A0(void *work, int a);

extern void ov70_02238F9C(void *sprite, int x, int y);

int ov70_022429B8(UnkStruct_ov70_02242E58 *work, u8 *flags);

int ov70_022429B8(UnkStruct_ov70_02242E58 *work, u8 *flags) {
    int cur = work->unk_48;
    int keys;
    int ret;

    if (cur < 9) {
        work->unk_7E = cur;
    }
    keys = gSystem.newAndRepeatedKeys;
    if (keys & 0x40) {
        work->unk_48 = ov70_02245EFC[work->unk_48 * 4];
    } else if (keys & 0x80) {
        work->unk_48 = ov70_02245EFD[work->unk_48 * 4];
    } else if (keys & 0x20) {
        work->unk_48 = ov70_02245EFE[work->unk_48 * 4];
    } else if (keys & 0x10) {
        work->unk_48 = ov70_02245EFF[work->unk_48 * 4];
    }

    if (cur >= 9 && work->unk_48 < 9) {
        keys = gSystem.newAndRepeatedKeys;
        if (keys & 0x40) {
            int v = work->unk_7E;
            work->unk_48 = v;
            if (v + 4 < 9) {
                do {
                    work->unk_48 = work->unk_48 + 4;
                } while (work->unk_48 + 4 < 9);
            }
        } else if (keys & 0x80) {
            int v = work->unk_7E;
            work->unk_48 = v;
            if (v - 4 >= 0) {
                do {
                    work->unk_48 = work->unk_48 - 4;
                } while (work->unk_48 - 4 >= 0);
            }
        }
    }

    if (cur != work->unk_48) {
        int n;
        PlaySE(0x5DC);
        n = work->unk_48 * 2;
        ov70_02238F9C(*((void * *)&work->unk_C), (ov70_02245DF8[n] + 0x10) << 3, ov70_02245DF9[n] << 3);
        switch (work->unk_48) {
        case 9:
            Sprite_SetAnimCtrlSeq(work->unk_C, 0x30);
            break;
        case 10:
            Sprite_SetAnimCtrlSeq(work->unk_C, 0x31);
            break;
        default:
            Sprite_SetAnimCtrlSeq(work->unk_C, 0x3D);
            break;
        }
    }

    ret = ov70_02242164(work, 6);
    if (ret != -1) {
        int r5 = ov70_022429A0(work, ret);
        if (r5 == -2 || r5 == 0xB || flags == NULL || flags[r5] != 0) {
            PlaySE(0x5DC);
            return r5;
        }
    } else {
        keys = gSystem.newKeys;
        if (keys & 1) {
            int r5 = ov70_022429A0(work, work->unk_48);
            if (r5 == -2 || r5 == 0xB || flags == NULL || flags[r5] != 0) {
                PlaySE(0x5DC);
                return r5;
            }
        } else if (keys & 2) {
            PlaySE(0x5DC);
            return -2;
        }
    }
    return -1;
}
