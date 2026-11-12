// libdwcutil's focus.c (Black's ov016 0x021bd3e4-0x021bd5e8): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

static void *data_ov016_021de550;
static const u8 data_ov016_021da084[6][2] = {
    { 10, 11 },
    { 4, 5 },
    { 2, 3 },
    { 12, 13 },
    { 229, 0 },
    { 38, 0 },
};
void DWCi_FOCUSlInit(void);

void DWCi_FOCUSlInit(void) {
    data_ov016_021de550 = DWCi_HEAPlAllocEx(0x10, 4);
}

void DWCi_FOCUSlEnd(void) {
    DWCi_FOCUSlDelete();
    DWCi_HEAPlFree_((u32 *)&data_ov016_021de550);
}

void DWCi_FOCUSlDisp(s32 kind, u32 x0, u32 x1, u32 y) {
    s32 i;

    DWCi_FOCUSlDelete();
    for (i = 0; i < 2; i++) {
        ((s32 **)data_ov016_021de550)[i] = DWCi_CEINlSetExCell(0, data_ov016_021da084[kind][i], 1);
        DWCi_CELLlSetPriority(((void **)data_ov016_021de550)[i], -1, 1);
    }
    func_ov016_021d5118(((s32 *)data_ov016_021de550)[0], -1, x0, y);
    func_ov016_021d5118(((s32 *)data_ov016_021de550)[1], -1, x1, y);
}

void DWCi_FOCUSlDisp4(u32 x0, u32 x1, u32 y0, u32 y1) {
    s32 i;
    s32 j;

    i = 6;
    DWCi_FOCUSlDelete();
    j = 0;
    do {
        *(s32 **)((s32)(u8 *)data_ov016_021de550 + j * 4) = DWCi_CEINlSetExCell(0, i, 1);
        DWCi_CELLlSetPriority(*(void **)((s32)(u8 *)data_ov016_021de550 + j * 4), -1, 1);
        j = j + 1;
        i = i + 1;
    } while (j < 4);
    func_ov016_021d5118(*(s32 *)data_ov016_021de550, -1, x0, y0);
    func_ov016_021d5118(*(s32 *)((s32)(u8 *)data_ov016_021de550 + 4), -1, x1, y0);
    func_ov016_021d5118(*(s32 *)((s32)(u8 *)data_ov016_021de550 + 8), -1, x0, y1);
    func_ov016_021d5118(*(s32 *)((s32)(u8 *)data_ov016_021de550 + 0xc), -1, x1, y1);
}

void DWCi_FOCUSlDelete(void) {
    s32 *ptr;
    s32 i;

    i = 0;
    do {
        ptr = *(s32 **)(data_ov016_021de550 + i * 4);
        if (ptr != NULL) {
            DWCi_CELLlDelete(ptr);
            *(u32 *)(data_ov016_021de550 + i * 4) = 0;
        }
        i = i + 1;
    } while (i < 4);
}
