// libdwcutil's icon.c (Black's ov016 0x021bd744-0x021bd8d8): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

static const u8 data_ov016_021da090[8] = { 24, 23, 22, 21, 95, 94, 93, 92 };
static void *data_ov016_021de558;
void taskRenew(void);

void DWCi_ICONlInit(s32 type) {
    s32 val;
    u8 *ptr;
    s32 *ptr2;
    s32 *ptr3;
    void *ptr4;

    if (data_ov016_021de558 != NULL) {
        return;
    }
    ptr4 = DWCi_HEAPlAllocEx(0xCU, 4);
    ptr = (s32 *)((u8 *)(data_ov016_021da090) + type * 4);
    val = *ptr;
    data_ov016_021de558 = ptr4;
    ptr = (u8 *)((u8 *)ptr4) + 8;
    *(s8 *)((u8 *)((u8 *)ptr4) + 8) = (s8)type;
    *(s32 **)data_ov016_021de558 = DWCi_CEINlSetExObj(NULL, (s32)val);
    ptr3 = *(s32 **)data_ov016_021de558;
    *ptr3 = (*ptr3 & 0xFE00FF00) | 0xE50026;
    ptr2 = *(s32 **)data_ov016_021de558;
    *(u16 *)((u8 *)ptr2 + 4) = (u16)((*(u16 *)((u8 *)ptr2 + 4) & ~0xC00) | 0x800);
    *(s32 **)((u8 *)((u8 *)data_ov016_021de558) + 4) = DWCi_TSKlForm(0, (s32)taskRenew, 0, 0x78U);
}

void DWCi_ICONlEnd(void) {
    if (data_ov016_021de558 == NULL) {
        return;
    }
    DWCi_TSKlDeleteEx(0, *(s32 **)((u8 *)data_ov016_021de558 + 4));
    DWCi_OBJlDelete(*(u32 **)data_ov016_021de558);
    DWCi_HEAPlFree_((u32 *)&data_ov016_021de558);
}


void taskRenew(void)
{
    BOOL irq;
    WMLinkLevel level;
    u32 *b;

    level = WM_LINK_LEVEL_0;
    irq = OS_DisableIrq();
    if (WM_GetAllowedChannel() != 0x8000) {
        level = WM_GetLinkLevel();
    }
    OS_RestoreIrq(irq);
    b = *(u32 **)data_ov016_021de558;
    func_ov016_021d52d4(0, (data_ov016_021da090 + *(s8 *)((u8 *)data_ov016_021de558 + 8) * 4)[level], b);
    b = *(u32 **)data_ov016_021de558;
    *b = (*b & 0xfe00ff00) | 0xe50026;
    *(u16 *)((u8 *)*(u32 **)data_ov016_021de558 + 4) = (*(u16 *)((u8 *)*(u32 **)data_ov016_021de558 + 4) & ~0xc00) | 0x800;
}
