#include "global.h"

#include "list_menu_2d.h"
#include "message_format.h"
#include "overlay_85.h"
#include "unk_02005D10.h"
#include "unk_02034354.h"
#include "overlay_85_ov109_021D5140.h"
#include "unk_struct_0209_bdf8.h"

s32 ov85_021E943C(void *);

extern WindowTemplate ov85_021EA8D8;

s32 ov85_021E993C(UnkStruct_ov109_021D5140 *arg0, s32 arg1) {
    arg0->unk_394 = Std_CreateYesNoMenu(arg0->unk_14, &ov85_021EA8D8, 0x1FU, 0xBU, HEAP_ID_102);
    arg0->unk_3B8 = 0xC;
    ov85_021E943C(arg0);
    return arg1;
}

s32 ov85_021E9FD0(void);

s32 ov85_021EA39C(void *, s32);

s32 sub_02096D4C(void *, s32, s32 *, s32);

s32 ov85_021E996C(UnkStruct_ov109_021D5140 *arg0, s32 arg1) {
    s32 v0;
    s32 v1;
    UnkStruct_0209BDF8 *v2;

    v2 = arg0->unk_10;
    if ((v2->unk_2C != ov85_021E9FD0()) || ((s32)v2->unk_30 != 0)) {
        if (0xC3 & gSystem.newKeys) {
            PlaySE(0x5F2U);
        }
        ov85_021E943C(arg0);
        return arg1;
    }
    v1 = Handle2dMenuInput_DeleteOnFinish(arg0->unk_394, HEAP_ID_102);
    if (v1 != -1) {
        if (v1 == -2) {
            v0 = 0;
            arg0->unk_3B8 = 0;
            sub_02096D4C(arg0->unk_10, 7, &v0, 1);
            ov85_021EA39C(arg0, 1);
        } else {
            arg0->unk_3B8 = 0xD;
            sub_02096D4C(arg0->unk_10, 3, NULL, 0);
            BufferPlayersName(arg0->unk_34, 0U, sub_02034818(0U));
        }
        arg0->unk_394 = NULL;
    }
    ov85_021E943C(arg0);
    return arg1;
}
