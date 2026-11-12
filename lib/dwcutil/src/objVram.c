// libdwcutil's objVram.c (Black's ov016 0x021d6f9c-0x021d7200): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

static s32 data_ov016_021de6ac;

typedef struct VramNode {
    struct VramNode *prev;
    struct VramNode *next;
    u16 addr;
    u16 size;
} VramNode;

typedef struct VramGroup {
    u8 buf[0x180];
    VramNode first;
    VramNode last;
    u32 list;
    u16 *queue;
} VramGroup;


void DWCi_OVRlInit(void) {
    s32 i;

    data_ov016_021de6ac = (s32)DWCi_HEAPlAllocEx(0x340, 4);
    for (i = 0; i < 2; i++) {
        ((VramGroup *)data_ov016_021de6ac)[i].queue =
            (u16 *)DWCi_QUElFormEx(0x20, (s32)&((VramGroup *)data_ov016_021de6ac)[i], 0xc);
        ((VramGroup *)data_ov016_021de6ac)[i].list = DWCi_LSTlForm();
        ((VramGroup *)data_ov016_021de6ac)[i].first.addr = 0x300;
        ((VramGroup *)data_ov016_021de6ac)[i].last.addr = 0x400;
        DWCi_LSTlPushFront(((VramGroup *)data_ov016_021de6ac)[i].list,
                           (s32 *)&((VramGroup *)data_ov016_021de6ac)[i].first);
        DWCi_LSTlPushBack(((VramGroup *)data_ov016_021de6ac)[i].list,
                          (s32 *)&((VramGroup *)data_ov016_021de6ac)[i].last);
    }
}

s32 *func_ov016_021d705c(s32 lcd, s32 size, s32 head, u32 *addr) {
    VramNode *node;
    VramNode *cur;
    VramNode *adj;
    s32 num;
    s32 pos;
    OSIrqMask mask;

    node = (VramNode *)DWCi_QUElPopBack(((VramGroup *)data_ov016_021de6ac)[lcd].queue);
    num = ((size + 3) & ~3) >> 2;
    node->size = num;
    mask = OS_DisableIrqMask(1);
    if (head != 0) {
        cur = &((VramGroup *)data_ov016_021de6ac)[lcd].first;
        if (cur != &((VramGroup *)data_ov016_021de6ac)[lcd].last) {
            do {
                adj = cur->next;
                pos = cur->addr + cur->size;
                if (pos + num <= adj->addr) {
                    node->addr = pos;
                    DWCi_LSTlInsert((s32 *)adj, (s32 *)node);
                    break;
                }
                cur = adj;
            } while (cur != &((VramGroup *)data_ov016_021de6ac)[lcd].last);
        }
        if (cur == &((VramGroup *)data_ov016_021de6ac)[lcd].last) {
            OS_Terminate();
        }
    } else {
        cur = &((VramGroup *)data_ov016_021de6ac)[lcd].last;
        if (cur != &((VramGroup *)data_ov016_021de6ac)[lcd].first) {
            do {
                adj = cur->prev;
                pos = cur->addr - num;
                if (pos >= adj->addr + adj->size) {
                    node->addr = pos;
                    DWCi_LSTlInsert((s32 *)cur, (s32 *)node);
                    break;
                }
                cur = adj;
            } while (cur != &((VramGroup *)data_ov016_021de6ac)[lcd].first);
        }
        if (cur == &((VramGroup *)data_ov016_021de6ac)[lcd].first) {
            OS_Terminate();
        }
    }
    *addr = node->addr;
    OS_EnableIrqMask(mask);
    return (s32 *)node;
}


void DWCi_OVRlFree(s32 *ovram) {
    s32 flag;

    flag = 0;
    DWCi_LSTlErase(ovram);
    if ((u32) ovram >= (u32) (data_ov016_021de6ac + 0x1A0)) {
        flag = 1;
    }
    DWCi_QUElPushBack(*(u16 **)((u8 *)(((flag * 0x1A0) + data_ov016_021de6ac)) + 0x19C), (u32) ovram);
}
