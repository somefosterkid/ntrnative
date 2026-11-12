// libdwcutil's effect.c (Black's ov016 0x021d551c-0x021d5824): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"
#include "nitro/fx/fx_cp.h"

static void *data_ov016_021de67c;
typedef struct EffectWork {
    void *task;
    s16 count;
    u16 frame;
    u8 kind;
    u8 flag;
} EffectWork;
typedef EffectWork EffectWork1;
typedef EffectWork EffectWork2;

void taskWait(u32 arg0, EffectWork2 *arg1);
void taskFade(u32 task, EffectWork1 *work);

void func_ov016_021d551c(void) {
    data_ov016_021de67c = DWCi_HEAPlAllocEx(0x18, 4);
    G2_SetBlendBrightness(0x3f, 0x10);
    G2S_SetBlendBrightness(0x3f, 0x10);
}

void DWCi_EFFlEnd(void) {
    DWCi_HEAPlFree_((u32 *)&data_ov016_021de67c);
}

u8 DWCi_EFFlCheck(s32 lcd) {
    void *ptr;

    if (lcd == 1) {
        ptr = data_ov016_021de67c;
    } else {
        ptr = (u8 *)data_ov016_021de67c + 0xC;
    }
    return *(u8 *)((u8 *)(u8 *)ptr + 9);
}

u32 func_ov016_021d55a4(s32 kind, s32 lcd, s32 plane, u16 frame) {
    s8 BRIGHT[4] = {-16, 0, 16, 0};
    EffectWork *work;

    if (lcd == 1) {
        work = data_ov016_021de67c;
    } else {
        work = (EffectWork *)((u8 *)data_ov016_021de67c + 0xC);
    }
    if (work->flag != 0) {
        return FALSE;
    }
    if (lcd == 1) {
        G2S_SetBlendBrightness(plane, BRIGHT[kind]);
    } else {
        G2_SetBlendBrightness(plane, BRIGHT[kind]);
    }
    work->task = DWCi_TSKlForm(1, (s32)taskFade, work, 200);
    work->count = 0;
    work->kind = kind;
    work->frame = frame;
    work->flag = TRUE;
    return TRUE;
}


void taskFade(u32 task, EffectWork1 *work) {
    u8 TABLE[4] = {17, 16, 1, 0};
    s8 BRIGHT[4] = {0, -16, 0, 16};
    s32 value;

    work->count++;
    value = FX_DivS32(work->count << 4, work->frame);
    if (TABLE[work->kind] & 1) {
        value = 16 - value;
    }
    if (TABLE[work->kind] & 0x10) {
        value = -value;
    }
    if (work == data_ov016_021de67c) {
        G2S_ChangeBlendBrightness(value);
    } else {
        G2_ChangeBlendBrightness(value);
    }
    if (work->count >= work->frame) {
        if (work == data_ov016_021de67c) {
            G2S_ChangeBlendBrightness(BRIGHT[work->kind]);
        } else {
            G2_ChangeBlendBrightness(BRIGHT[work->kind]);
        }
        work->flag = 0;
        DWCi_TSKlDelete(1, task);
    }
}

u32 func_ov016_021d5790(u16 param_1) {
    void *ptr;

    ptr = data_ov016_021de67c;
    if (*(u8 *)((s32)(u8 *)data_ov016_021de67c + 9) == 0) {
        *(s32 **)ptr = DWCi_TSKlForm(1, (s32)taskWait, data_ov016_021de67c, 200);
        *(u16 *)((s32)(u8 *)ptr + 4) = 0;
        *(u16 *)((s32)(u8 *)ptr + 6) = param_1;
        *(u8 *)((s32)(u8 *)ptr + 9) = 1;
        return 1;
    }
    return 0;
}

void taskWait(u32 task, EffectWork2 *work) {
    work->count = (s16) (work->count + 1);
    if ((s32) work->count < (s32) work->frame) {
        return;
    }
    work->flag = 0;
    DWCi_TSKlDelete(1, task);
}
