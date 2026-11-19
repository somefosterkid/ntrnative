#include "global.h"

#include "error_handling.h"
#include "sprite.h"
#include "system.h"
#include "unk_02005D10.h"
#include "unk_struct_ov70_02242e58.h"

struct Sprite;

struct SysTask;

struct SysTaskQueue;

struct System;

extern u8 ov70_02245EA8[];

extern u8 ov70_02245EA9[];

extern u8 ov70_02245EAA[];

extern u8 ov70_02245EAB[];

extern u8 ov70_02245E26[];

extern u8 ov70_02245E27[];

extern int ov70_02242164(void *work, int a);

void ov70_02238F9C(Sprite *sprite, int x, int y);

int ov70_022421D0(UnkStruct_ov70_02242E58 *work, u8 *flags);

int ov70_022421D0(UnkStruct_ov70_02242E58 *work, u8 *flags) {
    int cur = work->unk_48;
    int keys;

    if (cur < 9) {
        work->unk_7E = cur;
    }
    keys = gSystem.newAndRepeatedKeys;
    if (keys & 0x40) {
        work->unk_48 = ov70_02245EA8[work->unk_48 * 4];
    } else if (keys & 0x80) {
        work->unk_48 = ov70_02245EA9[work->unk_48 * 4];
    } else if (keys & 0x20) {
        work->unk_48 = ov70_02245EAA[work->unk_48 * 4];
    } else if (keys & 0x10) {
        work->unk_48 = ov70_02245EAB[work->unk_48 * 4];
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
        ov70_02238F9C(*((void * *)&work->unk_C), (ov70_02245E26[n] + 0x10) << 3, ov70_02245E27[n] << 3);
        if (work->unk_48 == 9) {
            Sprite_SetAnimCtrlSeq(work->unk_C, 0x30);
        } else {
            Sprite_SetAnimCtrlSeq(work->unk_C, 0x3D);
        }
    }

    {
        u32 ret = ov70_02242164(work, 4);
        if (ret != 0xFFFFFFFF) {
            if (ret == 9) {
                PlaySE(0x5DC);
                return -2;
            }
            if (ret >= 9) {
                GF_AssertFail();
            }
            if (flags == NULL || flags[ret] != 0) {
                PlaySE(0x5DC);
                return ret;
            }
        } else {
            int newKeys = gSystem.newKeys;
            if (newKeys & 1) {
                int sel = work->unk_48;
                if (sel == 9) {
                    return -2;
                }
                if (sel >= 9) {
                    GF_AssertFail();
                }
                if (flags == NULL || flags[work->unk_48] != 0) {
                    PlaySE(0x5DC);
                    return work->unk_48;
                }
            } else if (newKeys & 2) {
                PlaySE(0x5DC);
                return -2;
            }
        }
    }
    return -1;
}
