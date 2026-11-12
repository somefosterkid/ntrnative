// libdwcutil's scrollBar.c (Black's ov016 0x021ce874-0x021cef20): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"
#include "nitro/fx/fx_cp.h"

static s32 data_ov016_021de63c;
void calcRect(u32 param_1, u16 *param_2);
void disp_scrollBar(s32 param_1);
static const u8 data_ov016_021da8bb[4] = { 85, 54, 30, 0 };
void playSound_scrollBar(s32 param_1);
void moveBar(void);
void stepBar(s32 param_1);
void setBar(void);
s32 isTrigger(void);
s32 isTouch_scrollBar(u32 param_1);


static const u8 data_ov016_021da8b8[3] = { 16, 15, 14 };
void taskMain_scrollBar(void);
void DWCi_InputCoord(u16 param_1, u16 param_2, u16 *param_3);
void DWCi_SBlInit(u8 kind, u8 max, u16 x, u16 y, int pos) {
    data_ov016_021de63c = (s32)DWCi_HEAPlAllocEx(0x20, 4);
    *(u8 *)(data_ov016_021de63c + 0x1b) = kind;
    *(u8 *)(data_ov016_021de63c + 0x19) = max;
    *(s8 *)(data_ov016_021de63c + 0x1a) = pos;
    DWCi_InputCoord(x, y, (u16 *)(data_ov016_021de63c + 0x10));
    *(s32 **)data_ov016_021de63c = DWCi_CEINlSetExCell(0, data_ov016_021da8b8[kind], 1);
    func_ov016_021d5118(*(s32 *)data_ov016_021de63c, -1, x, y + pos);
    DWCi_CELLlSetPriority(*(void **)data_ov016_021de63c, -1, 1);
    *(s32 **)(data_ov016_021de63c + 0xc) = DWCi_TSKlForm(0, (s32)taskMain_scrollBar, 0, 0x80);
}

void DWCi_SBlEnd(void) {
    DWCi_TSKlDeleteEx(0, *(s32 **)(data_ov016_021de63c + 0xc));
    DWCi_CELLlDelete(*(s32 **)data_ov016_021de63c);
    DWCi_HEAPlFree_((u32 *)&data_ov016_021de63c);
}

u8 DWCi_SBlGet(void) {
    return *(u8 *)(data_ov016_021de63c + 0x1a);
}

u8 DWCi_SBlGetState(void) {
    return *(u8 *)(data_ov016_021de63c + 0x1d);
}

void DWCi_SBlSet(s32 pos) {
    disp_scrollBar(pos);
}

void DWCi_SBlEnable(void) {
    *(u8 *)(data_ov016_021de63c + 0x1e) = 0;
}

void DWCi_SBlDisable(void) {
    *(u8 *)(data_ov016_021de63c + 0x1e) = 1;
}

void taskMain_scrollBar(void) {
    *(u8 *)(data_ov016_021de63c + 0x1d) = 0;
    switch (*(u8 *)(data_ov016_021de63c + 0x1c)) {
    case 0:
        if (*(u8 *)(data_ov016_021de63c + 0x1e) != 0) {
            break;
        }
        switch (isTrigger()) {
        case 1:
            if (*(u8 *)(data_ov016_021de63c + 0x1b) == 0) {
                return;
            }
            DWCi_SNDlPlay(0x16);
            DWCi_SNDlSetVolume(0);
            *(u8 *)(data_ov016_021de63c + 0x1d) = 1;
            func_ov016_021d6b64((u16 *)(data_ov016_021de63c + 0x14));
            *(u8 *)(data_ov016_021de63c + 0x18) = *(u8 *)(data_ov016_021de63c + 0x1a);
            *(u8 *)(data_ov016_021de63c + 0x1c) = 1;
            return;
        case 2:
            stepBar(2);
            return;
        case 3:
            stepBar(3);
            return;
        case 4:
            setBar();
            return;
        }
        break;
    case 1:
        moveBar();
        break;
    case 2:
        if (isTouch_scrollBar(2) != 2) {
            *(u8 *)(data_ov016_021de63c + 0x1d) = 5;
            *(u8 *)(data_ov016_021de63c + 0x1c) = 0;
            break;
        }
        if (isTrigger() != 2) {
            break;
        }
        stepBar(2);
        break;
    case 3:
        if (isTouch_scrollBar(3) != 3) {
            *(u8 *)(data_ov016_021de63c + 0x1d) = 7;
            *(u8 *)(data_ov016_021de63c + 0x1c) = 0;
            break;
        }
        if (isTrigger() != 3) {
            break;
        }
        stepBar(3);
        break;
    }
}


void moveBar(void) {
    struct {
        u16 x;
        u16 y;
    } c;
    s32 pos;
    s32 s;

    if (DWCi_TPlCheck((u16 *)(void *)&data_ov016_021daa7c) != 0) {
        func_ov016_021d6b64(&c.x);
        if (c.x >= *(u16 *)(data_ov016_021de63c + 0x10) - 0x1e) {
            pos = *(u8 *)(data_ov016_021de63c + 0x18) + (c.y - *(u16 *)(data_ov016_021de63c + 0x16));
            if (pos < 0) {
                pos = 0;
            } else {
                s = *(data_ov016_021da8bb + *(u8 *)(data_ov016_021de63c + 0x1b));
                if (pos >= *(u8 *)(data_ov016_021de63c + 0x19) - s) {
                    pos = *(u8 *)(data_ov016_021de63c + 0x19) - s;
                }
            }
            playSound_scrollBar(pos);
            disp_scrollBar(pos);
            *(u8 *)(data_ov016_021de63c + 0x1d) = 2;
            return;
        }
    }
    DWCi_SNDlStop();
    *(u8 *)(data_ov016_021de63c + 0x1c) = 0;
    *(u8 *)(data_ov016_021de63c + 0x1d) = 3;
}

void playSound_scrollBar(s32 pos) {
    s32 quotient;
    s32 val;

    val = (u32)*(u8 *)(data_ov016_021de63c + 0x1a) - pos;
    if (val < 0) {
        val = -val;
    }
    if (val < 2) {
        quotient = 0;
    }
    else {
        quotient = 0x7f;
        if (val < 6) {
            quotient = FX_DivS32(0x7f, 6 - val);
        }
    }
    DWCi_SNDlSetVolume(quotient);
    if (val < 2) {
        quotient = -0x100;
    }
    else if (val < 6) {
        quotient = FX_DivS32(0x200, 6 - val);
        quotient = quotient + -0x100;
    }
    else {
        quotient = 0x100;
    }
    DWCi_SNDlSetPitch(0xffff, quotient);
}

void stepBar(s32 idx) {
    u8 val;

    *(s8 *)(data_ov016_021de63c + 0x1c) = (s8)idx;
    if (idx == 2) {
        val = 4;
    }
    else {
        val = 6;
    }
    *(u8 *)(data_ov016_021de63c + 0x1d) = val;
}

void setBar(void) {
    struct { u16 x; u16 y; } c;
    s32 pos;
    s32 s;

    func_ov016_021d6b64(&c.x);
    s = *(data_ov016_021da8bb + *(u8 *)(data_ov016_021de63c + 0x1b));
    pos = c.y - *(u16 *)(data_ov016_021de63c + 0x12) - ((u32)s >> 1);
    if (pos < 0) {
        pos = 0;
    } else if (pos >= *(u8 *)(data_ov016_021de63c + 0x19) - s) {
        pos = *(u8 *)(data_ov016_021de63c + 0x19) - s;
    }
    disp_scrollBar(pos);
    *(u8 *)(data_ov016_021de63c + 0x1d) = 3;
}

s32 isTrigger(void) {
    BOOL result;
    s32 i;
    s32 val;
    u16 auStack_18 [4];

    calcRect(1, auStack_18);
    result = DWCi_TPlCheckRepeat(auStack_18);
    if (result) {
        return 1;
    }
    i = 2;
    do {
        calcRect(i, auStack_18);
        result = DWCi_TPlCheckRepeat(auStack_18);
        if (result) {
            return i;
        }
        i = i + 1;
    } while (i <= 3);
    val = 4;
    calcRect(4, auStack_18);
    result = DWCi_TPlCheckTrigger(auStack_18);
    if (!result) {
        val = 0;
    }
    return val;
}

s32 isTouch_scrollBar(u32 param_1) {
    s32 i;
    u16 auStack_18 [4];

    i = 2;
    do {
        calcRect(i, auStack_18);
        if (DWCi_TPlCheck(auStack_18)) {
            return i;
        }
        i = i + 1;
    } while (i <= 3);
    return 0;
}

void calcRect(u32 idx, u16 *r) {
    u16 x = *(u16 *)(data_ov016_021de63c + 0x10);
    u16 y;
    r[0] = x;
    r[2] = x + 0xc;
    switch (idx) {
    case 1:
        r[1] = *(u16 *)(data_ov016_021de63c + 0x12) + *(u8 *)(data_ov016_021de63c + 0x1a);
        r[3] = r[1] + data_ov016_021da8bb[*(u8 *)(data_ov016_021de63c + 0x1b)];
        break;
    case 2:
        r[1] = *(u16 *)(data_ov016_021de63c + 0x12) - 0xd;
        r[3] = *(u16 *)(data_ov016_021de63c + 0x12);
        break;
    case 3:
        r[1] = *(u16 *)(data_ov016_021de63c + 0x12) + *(u8 *)(data_ov016_021de63c + 0x19);
        r[3] = r[1] + 0xd;
        break;
    case 4:
        y = *(u16 *)(data_ov016_021de63c + 0x12);
        r[1] = y;
        r[3] = y + *(u8 *)(data_ov016_021de63c + 0x19);
        break;
    }
}


void disp_scrollBar(s32 pos) {
    func_ov016_021d5118(*(s32 *)data_ov016_021de63c, -1, (u32)*(u16 *)(data_ov016_021de63c + 0x10),
            pos + (u32)*(u16 *)(data_ov016_021de63c + 0x12));
    *(s8 *)(data_ov016_021de63c + 0x1a) = (s8)pos;
}
