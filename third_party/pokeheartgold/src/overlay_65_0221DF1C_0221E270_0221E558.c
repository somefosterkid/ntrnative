#include "global.h"

#include "constants/heap.h"

#include "list_menu_items.h"
#include "msgdata.h"
#include "pm_string.h"
#include "unk_0202C034.h"

void *ov65_0221FCA0(void *, s32, void *, void *);

s32 sub_0202C2DC(WiFiList *, s32);

void ov65_0221E474();

u32 ov65_0221E558(u8 *param_1);

u32 ov65_0221E558(u8 *param_1) {
    s32 iVar1;
    ListMenuItem *pLVar2;
    String *string;
    s32 iVar3;
    u16 *src;
    u8 *uVar4;
    s32 a1;

    iVar1 = sub_0202C2F8(*(WiFiList **)(param_1 + 0x36a0));
    pLVar2 = ListMenuItems_New(iVar1 + 1, HEAP_ID_26);
    *(ListMenuItem **)(param_1 + 0x3688) = pLVar2;
    string = String_New(100, HEAP_ID_26);
    a1 = 0;
    do {
        iVar3 = sub_0202C2DC(*(void **)(param_1 + 0x36a0), a1);
        if (iVar3 != 0) {
            src = sub_0202C254(*(WiFiList **)(param_1 + 0x36a0), a1);
            CopyU16ArrayToString(string, src);
            ListMenuItems_AddItem(*(ListMenuItem **)(param_1 + 0x3688), string, a1);
        }
        a1 = a1 + 1;
    } while (a1 < 0x20);
    ListMenuItems_AppendFromMsgData(*(ListMenuItem **)(param_1 + 0x3688), *(MsgData **)(param_1 + 0x3684), 0xb, -2);
    String_Delete(string);
    uVar4 = ov65_0221FCA0(*(u8 **)(param_1 + 0x3688), iVar1, param_1 + 0x3690, *(u8 **)(param_1 + 0x180));
    *(u8 **)(param_1 + 0x368c) = uVar4;
    *(void (**)())(param_1 + 0x2220) = ov65_0221E474;
    return 0;
}
