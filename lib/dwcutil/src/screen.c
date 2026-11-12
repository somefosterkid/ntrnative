// libdwcutil's screen.c (Black's ov016 0x021ce668-0x021ce874): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

static s32 data_ov016_021de630[3];
void taskCopyPltt(u32 param_1);
void taskSetPltt(u32 param_1);
void taskLoad_screen(void);

void DWCi_SCREENlInit(void *scr) {
    data_ov016_021de630[0] = (s32)DWCi_HEAPlAllocEx(0x608, 4);
    MIi_CpuCopyFast(scr, (void *)(data_ov016_021de630[0] + 4), 0x600);
    *(s32 **)data_ov016_021de630[0] = DWCi_TSKlForm(1, (s32)taskLoad_screen, 0, 0x78);
}

void DWCi_SCREENlEnd(void) {
    DWCi_TSKlDeleteEx(1, *(s32 **)data_ov016_021de630[0]);
    DWCi_HEAPlFree_((u32 *)data_ov016_021de630);
}

void DWCi_SCREENlRenew(void) {
    *(u8 *)(data_ov016_021de630[0] + 0x604) = 1;
}

void DWCi_SCREENlCopy(void *src, s32 pos, s32 w, s32 h) {
    void *dest;
    s32 i;

    dest = (void *)(data_ov016_021de630[0] + 4 + pos * 2);
    i = 0;
    if (h <= 0) {
        return;
    }
    do {
        MIi_CpuCopy16(src, dest, w << 1);
        i = i + 1;
        src = (void *)((s32)(u8 *)src + 0x40);
        dest = (void *)((s32)(u8 *)dest + 0x40);
    } while (i < h);
}

void taskLoad_screen(void) {
    if (*(u8 *)(data_ov016_021de630[0] + 0x604) == 0) {
        return;
    }
    DC_FlushRange((void *)(data_ov016_021de630[0] + 4), 0x600);
    GX_LoadBG2Scr((void *)(data_ov016_021de630[0] + 4), 0, 0x600);
    *(u8 *)(data_ov016_021de630[0] + 0x604) = 0;
}

void DWCi_SCREENlCopyPalette(s32 src, s32 sColor, s32 dColor) {
    data_ov016_021de630[1] = src + sColor * 0x20;
    data_ov016_021de630[2] = dColor * 0x20 + 0x5000000;
    DWCi_TSKlForm(1, (s32)taskCopyPltt, 0, 0x78);
}

void taskCopyPltt(u32 task) {
    MIi_CpuCopy16((void *)data_ov016_021de630[1], (void *)data_ov016_021de630[2], 0x20);
    DWCi_TSKlDelete(1, task);
}

void DWCi_SCREENlSetPalette(u32 src) {
    data_ov016_021de630[1] = src;
    DWCi_TSKlForm(1, (s32)taskSetPltt, 0, 0x78);
}

void taskSetPltt(u32 task) {
    MIi_CpuCopy16((void *)data_ov016_021de630[1], (void *)0x5000000, 0x200);
    DWCi_TSKlDelete(1, task);
}
