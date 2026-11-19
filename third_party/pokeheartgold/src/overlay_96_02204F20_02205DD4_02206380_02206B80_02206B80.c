#include "global.h"

#include "overlay_96_ov96hurdle_hud.h"
#include "overlay_96_ov96hurdle_work.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov96_021EB52C(void *, s32, s32);

s32 ov96_021EB564(void *, s32);

s32 ov96_021EB588(void *, s32 *);

s32 ov96_022076C0(Ov96HurdleSlot *, s32);

void ov96_02206B80(Ov96HurdleWork *arg0, u8 *arg1, s32 arg2, s32 arg3);

void ov96_02206B80(Ov96HurdleWork *arg0, u8 *arg1, s32 arg2, s32 arg3) {
    s32 pos[3];
    s32 x;
    s32 y;
    s32 idx;
    u8 i;

    for (i = 0; i < 5; i++) {
        idx = *(u16 *)(arg1 + i * 2 + 0x18) & 0x1F;
        x = arg0->points_5C8[idx].x - arg2 + 0x80;
        y = arg0->points_5C8[idx].y - arg3 + 0x60;
        pos[2] = 0;
        pos[0] = x << 12;
        pos[1] = y << 12;

        ov96_021EB588(arg0->slots[i].unk_4, pos);
        ov96_021EB588(arg0->slots[i].unk_8, pos);
        if (x >= -0x20 && x <= 0x120 && y >= -0x20 && y <= 0xE0) {
            ov96_021EB52C(arg0->slots[i].unk_4, 1, 1);
            ov96_021EB52C(arg0->slots[i].unk_8, 1, 1);
        } else {
            ov96_021EB52C(arg0->slots[i].unk_4, 1, 0);
            ov96_021EB52C(arg0->slots[i].unk_8, 1, 0);
        }
        if (idx != arg0->unk_513[i]) {
            ov96_021EB564(arg0->slots[i].unk_8, 4);
            arg0->unk_513[i] = idx;
        }
        ov96_022076C0(&arg0->slots[i], y);
    }
}
#else
s32 ov96_021EB52C(void *, s32, s32);

s32 ov96_021EB564(void *, s32);

s32 ov96_021EB588(void *, s32 *);

s32 ov96_022076C0(u8 *, s32);

void ov96_02206B80(u8 *arg0, u8 *arg1, s32 arg2, s32 arg3);

void ov96_02206B80(u8 *arg0, u8 *arg1, s32 arg2, s32 arg3) {
    s32 pos[3];
    s32 x;
    s32 y;
    s32 idx;
    u8 i;

    for (i = 0; i < 5; i++) {
        idx = *(u16 *)(arg1 + i * 2 + 0x18) & 0x1F;
        x = *(u16 *)(arg0 + idx * 4 + 0x5C8) - arg2 + 0x80;
        y = *(u16 *)(arg0 + idx * 4 + 0x5CA) - arg3 + 0x60;
        pos[2] = 0;
        pos[0] = x << 12;
        pos[1] = y << 12;

        ov96_021EB588(*(void **)(arg0 + 0x14 * i + 0x568), pos);
        ov96_021EB588(*(void **)(arg0 + 0x14 * i + 0x56C), pos);
        if (x >= -0x20 && x <= 0x120 && y >= -0x20 && y <= 0xE0) {
            ov96_021EB52C(*(void **)(arg0 + 0x14 * i + 0x568), 1, 1);
            ov96_021EB52C(*(void **)(arg0 + 0x14 * i + 0x56C), 1, 1);
        } else {
            ov96_021EB52C(*(void **)(arg0 + 0x14 * i + 0x568), 1, 0);
            ov96_021EB52C(*(void **)(arg0 + 0x14 * i + 0x56C), 1, 0);
        }
        if (idx != arg0[i + 0x513]) {
            ov96_021EB564(*(void **)(arg0 + 0x14 * i + 0x56C), 4);
            arg0[i + 0x513] = idx;
        }
        ov96_022076C0(arg0 + 0x564 + 0x14 * i, y);
    }
}
#endif

s32 ov96_021EAB38(void *, s32, s32, s32);

u32 ov96_02208448(Ov96HurdleHud *, u32, s32);

s32 ov96_02208608(Ov96HurdleHud *, u32);

void ov96_02206C90(Ov96HurdleHud *arg0, Ov96HurdleRunner *arg1, s32 arg2, s32 arg3, void *arg4, s32 arg5, s32 arg6);

void ov96_02206C90(Ov96HurdleHud *arg0, Ov96HurdleRunner *arg1, s32 arg2, s32 arg3, void *arg4, s32 arg5, s32 arg6) {
    Ov96HurdleRunner *var_r4 = arg1;
    u8 temp_r1;
    u8 *var_r5;
    temp_r1 = arg1->unk_9F;
    var_r5 = arg4;
    switch (temp_r1) {
    case 0:
        var_r4->unk_A0[0] = 0;
        var_r4->unk_9F++;
        break;

    case 1:
        var_r4->unk_70 += 8;
        var_r4->unk_A0[0]++;
        if ((u32)var_r4->unk_A0[0] >= 0x20) {
            ov96_021EAB38(var_r4->unk_00[arg3], 0, arg2, arg3);
            var_r4->unk_9F++;
        }
        break;

    default:
    case 3:
        break;

    case 2:
        if (arg6 != 0) {
            if (ov96_02208448(arg0, arg3, arg2) != 0) {
                var_r5[8] = 1;
                var_r4->unk_9F++;
            }
        } else if (arg5 != 0) {
            if (ov96_02208608(arg0, arg2) != 0) {
                var_r5[8] = 1;
                var_r4->unk_9F++;
            }
        } else {
            var_r4->unk_9F++;
        }
        break;
    }
}
