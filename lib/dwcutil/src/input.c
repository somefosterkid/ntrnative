// libdwcutil's input.c (Black's ov016 0x021d64b8-0x021d6c78): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"
#include "gfl/key.h"
#include "stdbool.h"
#include "nitro/fx/fx_cp.h"

typedef struct {
    u16 x;
    u16 y;
} COORD;

typedef struct {
    u16 l;
    u16 t;
    u16 r;
    u16 b;
} RECT;

typedef struct {
    u16 l;
    u16 t;
    u16 w;
    u16 h;
} AREA;

typedef struct {
    u8 input : 1;
    u8 trigger : 1;
    u8 repeat : 1;
    u8 release : 1;
} TOUCH;

typedef struct {
    TPData samplingBuf[5];
    COORD cTouch;
    COORD cTouchLast;
    u16 input;
    u16 trigger;
    u16 repeat;
    u16 release;
    TOUCH touch;
} MEMINPUT;

// The names of the file's data are the prebuilt library's (its DWARF)
static MEMINPUT *MemInput;
void func_ov016_021d657c(void);
void readTouch(void);
BOOL func_ov016_021d6b04(const AREA *a);
void DWCi_InputCoord(u16 x, u16 y, u16 *coord);

void DWCi_IPTlInit(void) {
    BOOL BVar1;
    TPCalibrateParam TStack_10;

    MemInput = DWCi_HEAPlAllocEx(sizeof(MEMINPUT), 4);
    BVar1 = TP_GetUserInfo(&TStack_10);
    if (BVar1 == 0) {
        OS_Terminate();
    }
    TP_SetCalibrateParam(&TStack_10);
    TP_RequestAutoSamplingStartAsync(0, 4, MemInput->samplingBuf, 5);
    TP_WaitBusy(2);
    if (TP_CheckError(2) != 0) {
        OS_Terminate();
    }
    DWCi_IPTlRead();
}

void DWCi_IPTlEnd(void) {
    do {
        TP_RequestAutoSamplingStopAsync();
        TP_WaitBusy(4);
    } while (TP_CheckError(4) != 0);
    DWCi_HEAPlFree_((u32 *)&MemInput);
}

void DWCi_IPTlRead(void) {
    func_ov016_021d657c();
    readTouch();
}

void func_ov016_021d657c(void) {
    static u8 count[14];
    u16 input;
    u16 pad;
    MEMINPUT *m;
    int i;

    pad = PAD_Read();
    m = MemInput;
    input = pad & ~((pad & PAD_KEY_UP) << 1) & ~((pad & PAD_KEY_LEFT) >> 1);
    m->trigger = (m->input ^ input) & input;
    MemInput->release = m->input & (m->input ^ input);
    MemInput->input = input;
    MemInput->repeat = MemInput->trigger;
    for (i = 0; i < 14; i++) {
        u16 bit = 1 << i;
        if (!(input & bit)) {
            count[i] = 0;
            continue;
        }
        count[i]++;
        if (count[i] == 40) {
            MemInput->repeat |= bit;
        } else if (count[i] == 47) {
            MemInput->repeat |= bit;
            count[i] = 40;
        }
    }
}

void readTouch(void) {
    static u8 count;
    TPData p;
    s32 idx;
    s32 i;
    u8 touch;
    BOOL last;

    last = MemInput->touch.input ? TRUE : FALSE;
    touch = FALSE;
    idx = TP_GetLatestIndexInAuto();
    MemInput->cTouchLast = MemInput->cTouch;
    for (i = 0; i < 4; i++, idx = FX_ModS32(idx + 4, 5)) {
        if (MemInput->samplingBuf[idx].touch == TP_TOUCH_ON
            && MemInput->samplingBuf[idx].validity == TP_VALIDITY_VALID) {
            touch = TRUE;
            TP_GetCalibratedPoint(&p, &MemInput->samplingBuf[idx]);
            DWCi_InputCoord(p.x, p.y, (u16 *)&MemInput->cTouch);
            break;
        }
    }
    MemInput->touch.trigger = touch & (touch ^ last);
    MemInput->touch.release = last & (touch ^ last);
    MemInput->touch.input = touch;
    MemInput->touch.repeat = MemInput->touch.trigger;
    if (touch == FALSE) {
        count = 0;
    } else {
        count++;
        if (count == 40) {
            MemInput->touch.repeat = 1;
        } else if (count == 47) {
            MemInput->touch.repeat = 1;
            count = 40;
        }
    }
}



BOOL DWCi_IPTlCheckTrigger(u32 input) {
    if (PAD_DetectFold()) {
        return 0;
    }
    if (input == (input & MemInput->trigger)) {
        return 1;
    }
    return 0;
}

BOOL DWCi_IPTlCheckRepeat(u32 input) {
    if (PAD_DetectFold()) {
        return 0;
    }
    if (input == (input & MemInput->repeat)) {
        return 1;
    }
    return 0;
}

s32 DWCi_IPTlCheckRelease(u32 input) {
    if (PAD_DetectFold()) {
        return 0;
    }
    if (input == (input & MemInput->release)) {
        return 1;
    }
    return 0;
}

BOOL DWCi_TPlCheck(u16 *r) {
    u16 val;
    u16 val2;

    if (PAD_DetectFold()) {
        return 0;
    }
    if (MemInput->touch.input == 0) {
        return 0;
    }
    val = MemInput->cTouch.x;
    if ((u32) *r > (u32) val) {
        return 0;
    }
    if ((u32) *(u16 *)((u8 *)r + 4) < (u32) val) {
        return 0;
    }
    val2 = MemInput->cTouch.y;
    if ((u32) *(u16 *)((u8 *)r + 2) > (u32) val2) {
        return 0;
    }
    if ((u32) *(u16 *)((u8 *)r + 6) >= (u32) val2) {
        return 1;
    }
    return 0;
}

BOOL DWCi_TPlCheckTrigger(u16 *r) {
    u16 val;
    u16 val2;

    if (PAD_DetectFold()) {
        return 0;
    }
    if (MemInput->touch.trigger == 0) {
        return 0;
    }
    val = MemInput->cTouch.x;
    if ((u32) *r > (u32) val) {
        return 0;
    }
    if ((u32) *(u16 *)((u8 *)r + 4) < (u32) val) {
        return 0;
    }
    val2 = MemInput->cTouch.y;
    if ((u32) *(u16 *)((u8 *)r + 2) > (u32) val2) {
        return 0;
    }
    if ((u32) *(u16 *)((u8 *)r + 6) >= (u32) val2) {
        return 1;
    }
    return 0;
}

BOOL DWCi_TPlCheckRepeat(u16 *r) {
    u16 val;
    u16 val2;

    if (PAD_DetectFold()) {
        return 0;
    }
    if (MemInput->touch.repeat == 0) {
        return 0;
    }
    val = MemInput->cTouch.x;
    if ((u32) *r > (u32) val) {
        return 0;
    }
    if ((u32) *(u16 *)((u8 *)r + 4) < (u32) val) {
        return 0;
    }
    val2 = MemInput->cTouch.y;
    if ((u32) *(u16 *)((u8 *)r + 2) > (u32) val2) {
        return 0;
    }
    if ((u32) *(u16 *)((u8 *)r + 6) >= (u32) val2) {
        return 1;
    }
    return 0;
}

BOOL DWCi_TPlCheckRelease(u16 *r) {
    u16 val;
    u16 val2;

    if (PAD_DetectFold()) {
        return 0;
    }
    if (MemInput->touch.release == 0) {
        return 0;
    }
    val = MemInput->cTouch.x;
    if ((u32) *r > (u32) val) {
        return 0;
    }
    if ((u32) *(u16 *)((u8 *)r + 4) < (u32) val) {
        return 0;
    }
    val2 = MemInput->cTouch.y;
    if ((u32) *(u16 *)((u8 *)r + 2) > (u32) val2) {
        return 0;
    }
    if ((u32) *(u16 *)((u8 *)r + 6) >= (u32) val2) {
        return 1;
    }
    return 0;
}

BOOL func_ov016_021d6b04(const AREA *a) {
    RECT r;

    if (PAD_DetectFold()) {
        return FALSE;
    }
    r.l = a->l;
    r.t = a->t;
    r.r = a->l + a->w;
    r.b = a->t + a->h;
    return DWCi_TPlCheckTrigger((u16 *)&r);
}

BOOL func_ov016_021d6b64(u16 *c) {
    BOOL ret;
    if (PAD_DetectFold()) {
        ret = FALSE;
    } else {
        if (!MemInput->touch.input) {
            *(COORD *)c = MemInput->cTouchLast;
            ret = FALSE;
        } else {
            *(COORD *)c = MemInput->cTouch;
            ret = TRUE;
        }
    }
    return ret;
}

void func_ov016_021d6bc8(void) {
    static u8 fold;

    if (fold != 0) {
        if (PAD_DetectFold()) {
            return;
        }
        if (PM_SetLCDPower(1)) {
            OS_WaitVBlankIntr();
            GX_DispOn();
            reg_GXS_DB_DISPCNT = reg_GXS_DB_DISPCNT | 0x10000;
            OS_WaitVBlankIntr();
            fold = 0;
        }
    } else {
        if (!PAD_DetectFold()) {
            return;
        }
        OS_WaitVBlankIntr();
        GX_DispOff();
        reg_GXS_DB_DISPCNT = reg_GXS_DB_DISPCNT & 0xfffeffff;
        OS_WaitVBlankIntr();
        if (PM_SetLCDPower(0)) {
            fold = 1;
        }
    }
}
