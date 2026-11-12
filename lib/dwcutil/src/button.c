// libdwcutil's button.c (Black's ov016 0x021bc9e4-0x021bd04c): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

// pokebw2's DWCUtilUiObject: the cells, the slide task and the release task, a frame counter, the button pressed
// (-1 none, -2 not yet shown), the kind, the touch lock and the end flag
typedef struct DWCiButton {
    s32 *cell[3];
    s32 *task;
    s32 *releaseTask;
    u16 counter;
    s8 state;
    u8 kind;
    u8 disabled;
    u8 end;
} DWCiButton;

void taskEnd_button(s32 param_1);
static s32 data_ov016_021de508;
void disp_button(u32 param_1);
void taskStart1_button(s32 param_1);
void taskEnd1_button(u32 param_1);
void taskMain_button(void);
static const s16 data_ov016_021da004[2] = { 0x78, 0x10 };
static const u8 data_ov016_021da008[7] = { 2, 1, 1, 2, 1, 1, 2 };
static const u16 data_ov016_021da010[2][2] = { { 0x08, 0xac }, { 0x84, 0xac } };
static const u8 data_ov016_021da026[7][2] = { { 0, 1 }, { 0, 0 }, { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 0 }, { 0, 1 } };
static const u8 data_ov016_021da018[7][2] = { { 0x27, 0x1f }, { 0x25, 0x00 }, { 0x27, 0x00 }, { 0x23, 0x1d }, { 0x21, 0x00 }, { 0x59, 0x00 }, { 0x27, 0x21 } };
const u16 data_ov016_021da034[2][4] = { { 0x0d, 0x3c, 0xe6, 0x5e }, { 0x0d, 0x28, 0xe6, 0x70 } };
void taskStart(s32 param_1);
void taskRelease(u32 param_1);


void DWCi_BTNlInit(s32 kind) {
    s32 btnNum = data_ov016_021da008[kind];
    s32 i;

    data_ov016_021de508 = (s32)DWCi_HEAPlAllocEx(sizeof(DWCiButton), 4);
    ((DWCiButton *)data_ov016_021de508)->state = -2;
    ((DWCiButton *)data_ov016_021de508)->kind = kind;
    for (i = 0; i < btnNum; i++) {
        ((DWCiButton *)data_ov016_021de508)->cell[i] = DWCi_CEINlSetExCell(0, data_ov016_021da018[kind][i], 1);
        DWCi_CELLlSetPriority(((DWCiButton *)data_ov016_021de508)->cell[i], -1, 1);
    }
    ((DWCiButton *)data_ov016_021de508)->cell[2] = DWCi_CEINlSetExCell(0, 1, 1);
    DWCi_CELLlSetPriority(((DWCiButton *)data_ov016_021de508)->cell[2], -1, 1);
    disp_button(0xc0);
    ((DWCiButton *)data_ov016_021de508)->task = DWCi_TSKlForm(0, (s32)taskStart, NULL, 0x78);
}

void DWCi_BTNlEnd(void) {
    *(u8 *)(data_ov016_021de508 + 0x19) = 1;
    DWCi_TSKlChangeFunc((void *)*(s32 *)(data_ov016_021de508 + 0xc), (u32)taskEnd_button);
}

s32 DWCi_BTNlGet(void) {
    return (s32)*(s8 *)(data_ov016_021de508 + 0x16);
}

void func_ov016_021bcb28(u8 idx) {
    if (*(s8 *)(data_ov016_021de508 + 0x16) == -1) {
        *(u8 *)(data_ov016_021de508 + 0x16) = idx;
    }
}

void DWCi_BTNlSetForce(s8 idx) {
    *(u8 *)(data_ov016_021de508 + 0x16) = idx;
}

BOOL DWCi_BTNlIsEnd(void) {
    if (data_ov016_021de508 == 0) {
        return 1;
    }
    if (*(u8 *)((u8 *)data_ov016_021de508 + 0x19) == 0) {
        return 1;
    }
    return 0;
}

void DWCi_BTNlEnable(void) {
    *(u8 *)(data_ov016_021de508 + 0x18) = 0;
}

void DWCi_BTNlDisable(void) {
    *(u8 *)(data_ov016_021de508 + 0x18) = 1;
}

void disp_button(u32 y) {
    s32 btnNum;
    s32 i;

    btnNum = data_ov016_021da008[*(u8 *)(data_ov016_021de508 + 0x17)];
    func_ov016_021d5118(*(s32 *)(data_ov016_021de508 + 8), -1, 0, y);
    for (i = 0; i < btnNum; i++) {
        func_ov016_021d5118(*(s32 *)(data_ov016_021de508 + i * 4), -1,
                            data_ov016_021da010[data_ov016_021da026[*(u8 *)(data_ov016_021de508 + 0x17)][i]][0], y + 4);
    }
}

void taskStart(s32 task) {
    u32 uStack_c;
    u32 uStack_10;


    DWCi_CELLlGetPosition((void *)*(s32 *)(data_ov016_021de508 + 8), 0, &uStack_c, &uStack_10);
    uStack_10 = uStack_10 - 4;
    disp_button(uStack_10);
    if ((s32)uStack_10 <= 0xa8) {
        disp_button(0xa8);
        DWCi_TSKlChangeFunc((void *)task, (u32)taskStart1_button);
        return;
    }
}

void taskStart1_button(s32 task) {
    *(s8 *)((u8 *)data_ov016_021de508 + 0x16) = -1;
    *(u16 *)((u8 *)data_ov016_021de508 + 0x14) = (u16) (*(u16 *)((u8 *)data_ov016_021de508 + 0x14) + 1);
    if ((u32) *(u16 *)((u8 *)data_ov016_021de508 + 0x14) < 4U) {
        return;
    }
    *(u16 *)((u8 *)data_ov016_021de508 + 0x14) = 0U;
    DWCi_TSKlChangeFunc((void *)task, (u32) taskMain_button);
}

void taskMain_button(void) {
    u16 r[4];
    s32 btnNum;
    s32 i;

    btnNum = data_ov016_021da008[*(u8 *)(data_ov016_021de508 + 0x17)];
    if (*(u8 *)(data_ov016_021de508 + 0x18) == 0) {
        if (*(s8 *)(data_ov016_021de508 + 0x16) != -1) {
            return;
        }
        for (i = 0; i < btnNum; i++) {
            func_ov016_021d600c((s16 *)data_ov016_021da010[data_ov016_021da026[*(u8 *)(data_ov016_021de508 + 0x17)][i]],
                                data_ov016_021da004, (s16 *)r);
            if (DWCi_TPlCheckTrigger(r)) {
                if (*(s32 *)(data_ov016_021de508 + 0x10) == 0) {
                    u8 c = *((u8 *)&data_ov016_021da018 + *(u8 *)(data_ov016_021de508 + 0x17) * 2 + i);
                    func_ov016_021d52d4(0, c + 1, (void *)DWCi_CELLlGetObj(*(void **)(data_ov016_021de508 + i * 4), 0));
                    func_ov016_021d5118(*(s32 *)(data_ov016_021de508 + i * 4), -1,
                                        data_ov016_021da010[data_ov016_021da026[*(u8 *)(data_ov016_021de508 + 0x17)][i]][0],
                                        data_ov016_021da010[data_ov016_021da026[*(u8 *)(data_ov016_021de508 + 0x17)][i]][1]);
                    DWCi_CELLlSetPriority(*(void **)(data_ov016_021de508 + i * 4), -1, 1);
                    *(s32 **)(data_ov016_021de508 + 0x10) = DWCi_TSKlForm(0, (s32)taskRelease, NULL, 0x6e);
                    *(s8 *)(data_ov016_021de508 + 0x16) = i;
                    return;
                }
                break;
            }
        }
    }
    *(s8 *)(data_ov016_021de508 + 0x16) = -1;
}


void taskEnd_button(s32 task) {
    u32 uStack_c;
    u32 uStack_10;


    DWCi_CELLlGetPosition((void *)*(s32 *)(data_ov016_021de508 + 8), 0, &uStack_c, &uStack_10);
    uStack_10 = uStack_10 + 4;
    disp_button(uStack_10);
    if ((s32)uStack_10 >= 0xc0) {
        DWCi_TSKlChangeFunc((void *)task, (u32)taskEnd1_button);
        return;
    }
}

void taskEnd1_button(u32 task) {
    s32 i;

    DWCi_TSKlDelete(0, task);
    if (*(u32 *)(data_ov016_021de508 + 0x10) != 0) {
        DWCi_TSKlDelete(0, *(u32 *)(data_ov016_021de508 + 0x10));
    }
    for (i = 0; i < 2; i++) {
        if (*(s32 **)(data_ov016_021de508 + i * 4) != NULL) {
            DWCi_CELLlDelete(*(s32 **)(data_ov016_021de508 + i * 4));
        }
    }
    DWCi_CELLlDelete(*(s32 **)(data_ov016_021de508 + 8));
    DWCi_HEAPlFree_((u32 *)&data_ov016_021de508);
}

void taskRelease(u32 task) {
    s32 val;
    s32 i;
    u8 val2;

    *(u16 *)((u8 *)data_ov016_021de508 + 0x14) = (u16) (*(u16 *)((u8 *)data_ov016_021de508 + 0x14) + 1);
    if ((u32) *(u16 *)((u8 *)data_ov016_021de508 + 0x14) < 0x10U) {
        return;
    }
    i = 0;
    val = (s32)(*(u8 *)(((u8 *)&data_ov016_021da008) + *(u8 *)((u8 *)data_ov016_021de508 + 0x17)));
    if (val > 0) {
        do {
            val2 = *(u8 *)(i + (((u8 *)&data_ov016_021da018) + (*(u8 *)((u8 *)data_ov016_021de508 + 0x17) * 2)));
            func_ov016_021d52d4(0, (s32) val2, (void *) DWCi_CELLlGetObj((void *)*(s32 *)(data_ov016_021de508 + (i * 4)), 0));
            DWCi_CELLlSetPriority((void *)*(s32 *)(data_ov016_021de508 + (i * 4)), -1, 1);
            i += 1;
        } while (i < val);
    }
    disp_button(0xA8U);
    *(u16 *)((u8 *)data_ov016_021de508 + 0x14) = 0U;
    *(s8 *)((u8 *)data_ov016_021de508 + 0x16) = -1;
    if (*(s32 *)((u8 *)data_ov016_021de508 + 0x10) == 0) {
        return;
    }
    *(s32 *)((u8 *)data_ov016_021de508 + 0x10) = 0;
    DWCi_TSKlDelete(0, task);
}
