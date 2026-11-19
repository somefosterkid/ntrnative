#include "global.h"

#include "constants/heap.h"

#include "overlay_96_ov96hurdle_hud.h"

void ov96_021EB588(void *param_1, VecFx32 *param_2);

VecFx32 *ov96_021EB594(void *param_1);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov96_022082BC(Ov96HurdleHud *hud, u32 param_2, u32 param_3);

void ov96_022088AC(Ov96HurdleHud *hud, s32 param_2);

u32 ov96_02208448(Ov96HurdleHud *hud, u32 param_2, s32 param_3);

u32 ov96_02208448(Ov96HurdleHud *hud, u32 param_2, s32 param_3) {
    VecFx32 *pfVar1;
    u32 remainder;
    VecFx32 VStack_20;
    VecFx32 VStack_2c;
    VecFx32 VStack_38;

    switch (hud->unk_19C) {
    case 0:
        pfVar1 = ov96_021EB594(hud->unk_130[hud->unk_1A0]);
        VStack_20 = *pfVar1;
        VStack_20.y += 0x20000;
        ov96_021EB588(hud->unk_130[hud->unk_1A0], &VStack_20);
        if (VStack_20.y >= 0x39C000) {
            VStack_20.y = 0x294000;
            ov96_021EB588(hud->unk_130[hud->unk_1A0], &VStack_20);
            hud->unk_19C = hud->unk_19C + 1;
        }
        break;
    case 1:
        ov96_022082BC(hud, (u8)hud->unk_1A0, param_2);
        hud->unk_1A0 = (hud->unk_1A0 + 1) % 2;
        hud->unk_19C = hud->unk_19C + 1;
        break;
    case 2:
        pfVar1 = ov96_021EB594(hud->unk_130[hud->unk_1A0]);
        VStack_2c = *pfVar1;
        VStack_2c.y += 0x20000;
        ov96_021EB588(hud->unk_130[hud->unk_1A0], &VStack_2c);
        if (VStack_2c.y >= 0x344000) {
            VStack_2c.y = 0x344000;
            ov96_021EB588(hud->unk_130[hud->unk_1A0], &VStack_2c);
            hud->unk_1A0 = (hud->unk_1A0 + 1) % 2;
            hud->unk_19C = hud->unk_19C + 1;
        }
        break;
    case 3:
        pfVar1 = ov96_021EB594(hud->unk_130[hud->unk_1A0]);
        VStack_38 = *pfVar1;
        VStack_38.y += 0x20000;
        ov96_021EB588(hud->unk_130[hud->unk_1A0], &VStack_38);
        if (VStack_38.y >= 0x2EC000) {
            VStack_38.y = 0x2EC000;
            ov96_021EB588(hud->unk_130[hud->unk_1A0], &VStack_38);
            hud->unk_1A0 = (hud->unk_1A0 + 1) % 2;
            remainder = (s32)(param_2 + 2) % (s32)(3);
            ov96_022088AC(hud, (u8)remainder);
            hud->unk_19C = 0;
            return 1;
        }
    }
    return 0;
}
#else
void _s32_div_f(u32, u32);

void ov96_022082BC(s32 param_1, u8 param_2, u32 param_3);

void ov96_022088AC(enum HeapID *param_1, u8 param_2);

u32 ov96_02208448(u8 param_1, u32 param_2);

u32 ov96_02208448(u8 param_1, u32 param_2) {
    VecFx32 *pfVar1;
    u32 remainder;
    VecFx32 VStack_20;
    VecFx32 VStack_2c;
    VecFx32 VStack_38;

    switch (*(u32 *)(param_1 + 0x19c)) {
    case 0:
        pfVar1 = ov96_021EB594(*(void **)(param_1 + (u32) * (u16 *)(param_1 + 0x1a0) * 4 + 0x130));
        VStack_20 = *pfVar1;
        VStack_20.y += 0x20000;
        ov96_021EB588(*(void **)(param_1 + (u32) * (u16 *)(param_1 + 0x1a0) * 4 + 0x130), &VStack_20);
        if (VStack_20.y >= 0x39C000) {
            VStack_20.y = 0x294000;
            ov96_021EB588(*(void **)(param_1 + (u32) * (u16 *)((s32)param_1 + 0x1a0) * 4 + 0x130), &VStack_20);
            *(s32 *)(param_1 + 0x19c) = *(s32 *)(param_1 + 0x19c) + 1;
        }
        break;
    case 1:
        ov96_022082BC(param_1, (u8) * (u16 *)(param_1 + 0x1a0), param_2);
        *(u16 *)(param_1 + 0x1a0) = (*(u16 *)(param_1 + 0x1a0) + 1) % 2;
        *(s32 *)(param_1 + 0x19c) = *(s32 *)(param_1 + 0x19c) + 1;
        break;
    case 2:
        pfVar1 = ov96_021EB594(*(void **)(param_1 + (u32) * (u16 *)(param_1 + 0x1a0) * 4 + 0x130));
        VStack_2c = *pfVar1;
        VStack_2c.y += 0x20000;
        ov96_021EB588(*(void **)(param_1 + (u32) * (u16 *)(param_1 + 0x1a0) * 4 + 0x130), &VStack_2c);
        if (VStack_2c.y >= 0x344000) {
            VStack_2c.y = 0x344000;
            ov96_021EB588(*(void **)(param_1 + (u32) * (u16 *)(param_1 + 0x1a0) * 4 + 0x130), &VStack_2c);
            *(u16 *)(param_1 + 0x1a0) = (*(u16 *)(param_1 + 0x1a0) + 1) % 2;
            *(s32 *)(param_1 + 0x19c) = *(s32 *)(param_1 + 0x19c) + 1;
        }
        break;
    case 3:
        pfVar1 = ov96_021EB594(*(void **)(param_1 + (u32) * (u16 *)(param_1 + 0x1a0) * 4 + 0x130));
        VStack_38 = *pfVar1;
        VStack_38.y += 0x20000;
        ov96_021EB588(*(void **)(param_1 + (u32) * (u16 *)(param_1 + 0x1a0) * 4 + 0x130), &VStack_38);
        if (VStack_38.y >= 0x2EC000) {
            VStack_38.y = 0x2EC000;
            ov96_021EB588(*(void **)(param_1 + (u32) * (u16 *)(param_1 + 0x1a0) * 4 + 0x130), &VStack_38);
            *(u16 *)(param_1 + 0x1a0) = (*(u16 *)(param_1 + 0x1a0) + 1) % 2;
            remainder = (s32)(param_2 + 2) % (s32)(3);
            ov96_022088AC((enum HeapID *)param_1, (u8)remainder);
            *(u32 *)(param_1 + 0x19c) = 0;
            return 1;
        }
    }
    return 0;
}
#endif
