#include "global.h"
struct Ov96DigitWork;

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_96_ov96_digit_work.h"

void ov96_022196E4(struct Ov96DigitWork *arg0, s32 arg1);

void ov96_022196E4(Ov96DigitWork *arg0, s32 arg1) {
    s32 v1;
    s32 v0;
    s32 v3;
    s32 flag;
    s32 v4;
    u8 v2;
#if !(defined(SDK_PORT) || defined(NONMATCHING))
    u32 *v5;
#endif

    v2 = (u8)(arg1 % 100);
    v1 = (s32)(u8)((s32)v2 / 10);
    v0 = (s32)(u8)((s32)v2 % 10);
    if ((arg1 < 0xA) || (arg1 > 0x37)) {
        v3 = 1;
    } else {
        v3 = 0;
    }
    arg0->unk_C4 = v3;
    v4 = 0;
#if !(defined(SDK_PORT) || defined(NONMATCHING))
    v5 = (u32 *)arg0;
#endif
    do {
        if (arg0->unk_C4 == 0) {
            flag = 1;
        } else {
            flag = 0;
        }
#if defined(SDK_PORT) || defined(NONMATCHING)
        ManagedSprite_SetDrawFlag(arg0->sprites[10 + v4], flag);
#else
        ManagedSprite_SetDrawFlag(*(ManagedSprite **)((u8 *)v5 + offsetof(Ov96DigitWork, sprites) + 10 * sizeof(ManagedSprite *)), flag);
#endif
        v4 += 1;
#if !(defined(SDK_PORT) || defined(NONMATCHING))
        v5 += 1;
#endif
    } while (v4 < 4);
    Sprite_SetAnimCtrlSeq(arg0->unk_A8[0], v1 + 1);
    Sprite_SetAnimCtrlSeq(arg0->unk_A8[1], v0 + 1);
}

void ov96_02219770(Ov96DigitWork *work, s32 param_2, s32 param_3);

void ov96_02219770(Ov96DigitWork *work, s32 param_2, s32 param_3) {
    ManagedSprite_SetDrawFlag(work->sprites[16], param_2);
    ManagedSprite_SetDrawFlag(work->sprites[17], param_3);
    return;
}

u16 ov96_0221978C(Ov96DigitWork *work);

u16 ov96_0221978C(Ov96DigitWork *work) {
    return work->unk_C2;
}
