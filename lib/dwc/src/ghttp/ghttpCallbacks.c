// Black's code at ov114 0x021c0244-0x021c0368. Split out of ghttpEncryption.c by the files of pokebw2's overlay 189
// (the same functions in the same order); the name is that file's.

#include <string.h>
#include <stdio.h>
#include "types.h"
#include "dwc/dwc.h"
#include "dwc/ext.h"
#include "dwc/nhttp.h"
#include "dwc/base.h"
#include "gfl/std.h"
#include "nitro/os.h"
#include "nitro/std.h"
#include "nitrowifi/cps.h"
#include "nitrowifi/soc.h"
#include "field/unity_tower.h"
#include "save/player_info.h"
#include "struct_decls.h"
#include "system/str_tool.h"
#include "gs/nonport.h"
#include "gs/darray.h"
#include "nitro/os.h"
#include "gfl/blact.h"
#include "dwc/ghttp_internal.h"

static char data_ov114_021ce598[0x14] = "Socket Error: %d\n";
#pragma thumb off
void func_ov114_021c0244(s32 *param_1) {
    s32 (*func)(s32, s32, s32, s32, s32);
    s32 result;
    s32 val;

    if (*(s32 *)((u8 *)param_1 + 0x40) != 0) {
        gsDebugFormat(8, 0, 2, data_ov114_021ce598, *(s32 *)((u8 *)param_1 + 0x54));
    }
    func = *(s32 (**)(s32, s32, s32, s32, s32))((u8 *)param_1 + 0x48);
    if (func == NULL) {
        return;
    }
    if (*(s32 *)((u8 *)param_1 + 0xC) == 0) {
        val = *(s32 *)((u8 *)param_1 + 0xEC);
    } else {
        val = 0;
    }
    result = func(*(s32 *)((u8 *)param_1 + 4), *(s32 *)((u8 *)param_1 + 0x40), val, *(s32 *)((u8 *)param_1 + 0x128), *(s32 *)((u8 *)param_1 + 0x4C));
    if (val == 0) {
        return;
    }
    if (result == 0) {
        *(s32 *)((u8 *)param_1 + 0x104) = 1;
    }
}

void func_ov114_021c02c8(s32 *param_1, s32 param_2, s32 param_3) {
    if (param_1[17] == 0) {
        return;
    }
    ((void (*)(s32, s32, s32, s32, s32, s32, s32))param_1[17])(param_1[1], param_1[4], param_2, param_3, param_1[74], param_1[75], param_1[19]);
}

void func_ov114_021c0318(s32 *param_1) {
    s32 len;

    if (param_1[94] == 0) {
        return;
    }
    len = ArrayLength((void *)param_1[90]);
    ((void (*)(s32, s32, s32, s32, s32, s32))param_1[94])(param_1[1], param_1[92], param_1[93], param_1[91], len, param_1[19]);
}
