// libdwcutil's sceneSetSelectWay.c (Black's ov016 0x021cc998-0x021cd160): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

void func_ov016_021ccc0c(void);
void procInit2_sceneSetSelectWay(void);
void procScene_sceneSetSelectWay(void);
void input_sceneSetSelectWay(void);
void draw_sceneSetSelectWay(void);
void button_sceneSetSelectWay(void);
void endScene1_sceneSetSelectWay(void);
static u32 *data_ov016_021de618;
static s8 data_ov016_021de614;
static u8 data_ov016_021de616;
static s8 data_ov016_021de615;
void endScene2_sceneSetSelectWay(void);
typedef struct SelectWaySetting {
    u8 unk_0[0xf4];
    u8 idx;
} SelectWaySetting;
const s8 data_ov016_021da7d8[7] = { 3, -1, -1, -1, -1, -1, 6 };
const u16 data_ov016_021da818[2][4][4] = {
    { { 8, 0x20, 0xf8, 0x5c }, { 8, 0x64, 0x7e, 0xa0 }, { 0, 0, 0, 0 }, { 0x83, 0x64, 0xf8, 0xa0 } },
    { { 8, 0x20, 0xf8, 0x5c }, { 8, 0x64, 0x64, 0xa0 }, { 0x68, 0x64, 0xc4, 0xa0 }, { 0xc8, 0x64, 0xf8, 0xa0 } }
};
const u16 data_ov016_021da858[2][4][4] = {
    { { 6, 0x1e, 0xea, 0x4e }, { 6, 0x62, 0x70, 0x92 }, { 0, 0, 0, 0 }, { 0x80, 0x62, 0xea, 0x92 } },
    { { 6, 0x1e, 0xea, 0x4e }, { 6, 0x62, 0x56, 0x92 }, { 0x66, 0x62, 0xb6, 0x92 }, { 0xc6, 0x62, 0xea, 0x92 } }
};
const s8 data_ov016_021da7df[2][4][4] = {
    { { -1, -2, -1, -2 }, { 3, 0, 3, 0 }, { -1, -1, -1, -1 }, { 1, 0, 1, 0 } },
    { { -1, -2, -1, -2 }, { 3, 0, 2, 0 }, { 1, 0, 3, 0 }, { 2, 0, 1, 0 } }
};
void initGraphics_sceneSetSelectWay(void);
void func_ov016_021ccba4(void);
void dispPush_sceneSetSelectWay(void);
void endScene_sceneSetSelectWay(void);
void DWCi_SceneSetAossButton(void);
void DWCi_SceneSetRakuReady(void);
void moveFocus_sceneSetSelectWay(s32 param_1);

void DWCi_SceneSetSelectWay(void) {
    SelectWaySetting *info;
    u8 lang;

    info = (SelectWaySetting *)DWCi_SETTIGNlGet();
    data_ov016_021de616 = 0;
    if (data_ov016_021de614 == 0) {
        data_ov016_021de614 = 1;
    }
    if (DWCi_Language() != 0) {
        if (data_ov016_021de615 == 2) {
            data_ov016_021de615 = 0;
        }
        if (data_ov016_021de614 == 2) {
            data_ov016_021de614 = 1;
        }
    }
    initGraphics_sceneSetSelectWay();
    DWCi_HLlLoadSet();
    lang = DWCi_Language();
    DWCi_COMMENTlDispEx(0x32, data_ov016_021da7d8[lang], info->idx + 1);
    DWCi_CMNlStep(1);
    DWCi_FOCUSlDisp4(data_ov016_021da858[DWCi_Flag(1)][data_ov016_021de615][0],
                     data_ov016_021da858[DWCi_Flag(1)][data_ov016_021de615][2],
                     data_ov016_021da858[DWCi_Flag(1)][data_ov016_021de615][1],
                     data_ov016_021da858[DWCi_Flag(1)][data_ov016_021de615][3]);
    DWCi_ChangeScene((u32)func_ov016_021ccba4);
}

void initGraphics_sceneSetSelectWay(void) {
    char name[] = "char/ybBgStep21.ncl.l";

    DWCi_CMNlLoadResource((u8 *)"char/jbBgStep2.ncg.l", (u8 *) GX_LoadBG2Char);
    DWCi_CMNlLoadResource((u8 *)"char/ybBgStep2.ncl.l", (u8 *) GX_LoadBGPltt);
    DWCi_CMNlLoadResource((u8 *)"char/jb3Way.nsc.l", (u8 *) GX_LoadBG2Scr);
    data_ov016_021de618 = DWCi_ARClReadEx((s8 *) DWCi_Char(name), NULL, 4);
    *(u16 *)((void *)0x04001008) = (u16) ((*(u16 *)((void *)0x04001008) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x04001008) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x04001008) + 2) & ~3) | 3);
    *(u16 *)((void *)0x0400000A) = (u16) ((*(u16 *)((void *)0x0400000A) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x0400000A) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x0400000A) + 2) & ~3) | 3);
}

void func_ov016_021ccba4(void) {
    func_ov016_021d55a4(2, 1, 1, 8);
    func_ov016_021d55a4(2, 0, 0x14, 8);
    DWCi_GXlVisible(1, 1);
    DWCi_GXlVisible(0, 0x14);
    DWCi_ChangeScene((u32)func_ov016_021ccc0c);
}

void func_ov016_021ccc0c(void) {
    u8 result;

    result = DWCi_EFFlCheck(1);
    if (result != 0) {
        return;
    }
    result = DWCi_EFFlCheck(0);
    if (result != 0) {
        return;
    }
    DWCi_CMNlButton(0);
    DWCi_ChangeScene((u32)procInit2_sceneSetSelectWay);
}

void procInit2_sceneSetSelectWay(void) {
    if (DWCi_BTNlGet() == -2) {
        return;
    }
    DWCi_BTNlEnable();
    DWCi_ChangeScene((u32)procScene_sceneSetSelectWay);
}

void procScene_sceneSetSelectWay(void) {
    input_sceneSetSelectWay();
    draw_sceneSetSelectWay();
    button_sceneSetSelectWay();
}

void input_sceneSetSelectWay(void) {
    BOOL flag0;
    BOOL flag1;
    BOOL flag2;
    BOOL flag3;
    u32 i;

    i = 0;
    do {
        flag0 = DWCi_Flag(1);
        flag0 = DWCi_TPlCheckTrigger((u16 *)((u8 *)data_ov016_021da818 + flag0 * 0x20 + i * 8));
        if (flag0 != 0) {
            func_ov016_021bcb28(1);
            data_ov016_021de615 = (s8)i;
            flag0 = DWCi_Flag(1);
            flag1 = DWCi_Flag(1);
            flag2 = DWCi_Flag(1);
            flag3 = DWCi_Flag(1);
            DWCi_FOCUSlDisp4(data_ov016_021da858[flag0][data_ov016_021de615][0],
                             data_ov016_021da858[flag1][data_ov016_021de615][2],
                             data_ov016_021da858[flag2][data_ov016_021de615][1],
                             data_ov016_021da858[flag3][data_ov016_021de615][3]);
            return;
        }
        i++;
    } while (i < 4);
    if (DWCi_IPTlCheckTrigger(1) != 0) {
        func_ov016_021bcb28(1);
        return;
    }
    if (DWCi_IPTlCheckTrigger(2) != 0) {
        func_ov016_021bcb28(0);
        return;
    }
    if (DWCi_IPTlCheckRepeat(0x40) != 0) {
        moveFocus_sceneSetSelectWay(1);
        return;
    }
    if (DWCi_IPTlCheckRepeat(0x80) != 0) {
        moveFocus_sceneSetSelectWay(3);
        return;
    }
    if (DWCi_IPTlCheckRepeat(0x20) != 0) {
        moveFocus_sceneSetSelectWay(0);
        return;
    }
    if (DWCi_IPTlCheckRepeat(0x10) == 0) {
        return;
    }
    moveFocus_sceneSetSelectWay(2);
}


void draw_sceneSetSelectWay(void) {
}

void button_sceneSetSelectWay(void) {
    switch (DWCi_BTNlGet()) {
    case 0:
        DWCi_SNDlPlay(7);
        break;
    case 1:
        DWCi_SNDlPlay(6);
        dispPush_sceneSetSelectWay();
        data_ov016_021de616 = 1;
        break;
    default:
        return;
    }
    DWCi_ChangeScene((u32)endScene_sceneSetSelectWay);
}

void endScene_sceneSetSelectWay(void) {
    DWCi_BTNlDisable();
    func_ov016_021d5790(8);
    DWCi_ChangeScene((u32)endScene1_sceneSetSelectWay);
}

void endScene1_sceneSetSelectWay(void) {
    if (DWCi_EFFlCheck(1) != 0) {
        return;
    }
    if (data_ov016_021de616 != 0) {
        DWCi_BTNlEnd();
    }
    func_ov016_021d55a4(3, 1, 1, 8);
    func_ov016_021d55a4(3, 0, 0x14, 8);
    DWCi_ChangeScene((u32)endScene2_sceneSetSelectWay);
}

void endScene2_sceneSetSelectWay(void) {
    if (DWCi_EFFlCheck(1) != 0) {
        return;
    }
    if (DWCi_EFFlCheck(0) != 0) {
        return;
    }
    if (DWCi_BTNlIsEnd() == 0) {
        return;
    }
    DWCi_FOCUSlDelete();
    DWCi_COMMENTlDelete();
    DWCi_ARClRelease((u32)data_ov016_021de618);
    DWCi_GXlInvisible(1, 1);
    DWCi_GXlInvisible(0, 0x14);
    if (data_ov016_021de616 == 0) {
        DWCi_SetParam_(2, 0);
        DWCi_ChangeScene((u32)DWCi_SceneSetSelectAp);
        return;
    }
    switch (data_ov016_021de615) {
    default:
        break;
    case 0:
        DWCi_SetParam_(2, 1);
        DWCi_ChangeScene((u32)DWCi_SceneSetSearch);
        break;
    case 1:
        DWCi_SetParam_(2, 1);
        DWCi_ChangeScene((u32)DWCi_SceneSetAossButton);
        break;
    case 2:
        DWCi_SetParam_(2, 1);
        DWCi_ChangeScene((u32)DWCi_SceneSetRakuReady);
        break;
    case 3:
        DWCi_SetParam_(2, 0);
        DWCi_SetParam2_(0, 0);
        DWCi_SetListReturn(1);
        DWCi_ChangeScene((u32)DWCi_SceneSetList);
        break;
    }
}


void moveFocus_sceneSetSelectWay(s32 dir) {
    s8 prev;
    s8 next;

    prev = data_ov016_021de615;
    next = data_ov016_021da7df[DWCi_Flag(1)][prev][dir];
    if (next == -1) {
        return;
    }
    if (next == 0) {
        data_ov016_021de614 = prev;
    }
    if (next == -2) {
        data_ov016_021de615 = data_ov016_021de614;
    } else {
        data_ov016_021de615 = next;
    }
    DWCi_SNDlPlay(8);
    DWCi_FOCUSlDisp4(data_ov016_021da858[DWCi_Flag(1)][data_ov016_021de615][0],
                     data_ov016_021da858[DWCi_Flag(1)][data_ov016_021de615][2],
                     data_ov016_021da858[DWCi_Flag(1)][data_ov016_021de615][1],
                     data_ov016_021da858[DWCi_Flag(1)][data_ov016_021de615][3]);
}


void dispPush_sceneSetSelectWay(void) {
    u8 tbl[4] = {1, 2, 3, 4};
    s8 sel;

    sel = data_ov016_021de615;
    DWCi_SCREENlCopyPalette((s32)data_ov016_021de618, (u32)tbl[sel], (u32)tbl[sel]);
}
