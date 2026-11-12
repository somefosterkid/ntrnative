// libdwcutil's list.c (Black's ov016 0x021d6d44-0x021d6e14): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order. Named
// dwcutil_list.c here: dsd takes each file name once, and lib/nnsys/src/fnd/list.c has list.c.
#include "dwcutil.h"

u32 DWCi_LSTlForm(void) {
    u32 *ptr;

    ptr = DWCi_HEAPlAllocEx(0x10, 4);
    *ptr = 0;
    ptr[1] = (u32)(ptr + 2);
    ptr[2] = (u32)ptr;
    ptr[3] = 0;
    return (u32)ptr;
}

void DWCi_LSTlDelete(u32 list) {
    DWCi_HEAPlFree_(&list);
}

void DWCi_LSTlErase(s32 *ob) {
    u32 mask;

    mask = OS_DisableIrqMask(1);
    *(s32 *)(*ob + 4) = ob[1];
    *(s32 *)ob[1] = *ob;
    ob[1] = 0;
    *ob = 0;
    OS_EnableIrqMask(mask);
}

void DWCi_LSTlInsert(s32 *pos, s32 *ob) {
    u32 mask;

    mask = OS_DisableIrqMask(1);
    *(s32 **)(*pos + 4) = ob;
    *ob = *pos;
    ob[1] = (s32)pos;
    *pos = (s32)ob;
    OS_EnableIrqMask(mask);
}

void DWCi_LSTlPushBack(s32 list, s32 *ob) {
    DWCi_LSTlInsert((s32 *)(list + 8), ob);
}

void DWCi_LSTlPushFront(s32 list, s32 *ob) {
    DWCi_LSTlInsert(*(s32 **)(list + 4), ob);
}
