#include "global.h"
struct UnkStruct_ov5_021EAE78;

#include "constants/heap.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "options.h"
#include "player_data.h"
#include "render_window.h"
#include "save.h"
#include "unk_0202C034.h"
#include "unk_02037C94.h"
#include "bag.h"
#include "message_format.h"
#include "text.h"
#include "unk_02034354.h"
#include "unk_02035900.h"
#include "overlay_01_unk_struct_ov5_021_eae78.h"

u32 ov01_021F6CFC(u8 *param_1);

s32 ov01_021F71C4(void *, s32);
s32 sub_0202C2DC(WiFiList *, s32);
s32 ov01_021F6D78(void *arg0);
s32 ov01_021F6E44(struct UnkStruct_ov5_021EAE78 *arg0);

u32 ov01_021F6CFC(u8 *param_1) {
    WiFiList *a0;
    s32 sVar1;
    Options *options;
    u32 uVar2;

    a0 = sub_0202C6F4(((UnkStruct_ov5_021EAE78 *)param_1)->saveData);
    sub_0202C23C(a0, 0);
    sVar1 = sub_0203A1C4(((UnkStruct_ov5_021EAE78 *)param_1)->saveData, (s32 *)(param_1 + 0x48), HEAP_ID_FIELD1);
    if (sVar1 == 0) {
        return 1;
    }
    *((u32 *)&((UnkStruct_ov5_021EAE78 *)param_1)->unk_6C[7]) = 0;
    options = Save_PlayerData_GetOptionsAddr(((UnkStruct_ov5_021EAE78 *)param_1)->saveData);
    uVar2 = Options_GetFrame(options);
    LoadUserFrameGfx2(*(BgConfig **)(((u8 *)((UnkStruct_ov5_021EAE78 *)param_1)->fieldSystem) + 8), GF_BG_LYR_MAIN_3, 0x1e2, 0xa, (u8)uVar2, HEAP_ID_FIELD1);
    LoadUserFrameGfx1(*(BgConfig **)(((u8 *)((UnkStruct_ov5_021EAE78 *)param_1)->fieldSystem) + 8), GF_BG_LYR_MAIN_3, 0x3d9, 0xb, 0, HEAP_ID_FIELD1);
    *((u32 *)&((UnkStruct_ov5_021EAE78 *)param_1)->unk_44) = 1;
    return 0;
}

s32 ov01_021F6D78(void *arg0) {
    WiFiList *wiFiList;
    s32 v0;
    s32 v2;
    int *v1;

    *((s32 *)&((UnkStruct_ov5_021EAE78 *)arg0)->unk_6C[7]) = -1;
    v0 = 0;
    if (sub_02037454() > 0) {
        v1 = (int *)arg0;
loop_2:
        if (*(s32 *)((u8 *)v1 + offsetof(UnkStruct_ov5_021EAE78, unk_48)) == 2) {
            *((s32 *)&((UnkStruct_ov5_021EAE78 *)arg0)->unk_6C[7]) = v0;
            *(s32 *)((u8 *)(arg0 + (v0 * 4)) + 0x48) = 0;
        } else {
            v1 += 1;
            v0 += 1;
            if (v0 < sub_02037454()) {
                goto loop_2;
            }
        }
    }
    if (*((s32 *)&((UnkStruct_ov5_021EAE78 *)arg0)->unk_6C[7]) == -1) {
        return 1;
    }
    if (Bag_HasItem(Save_Bag_Get(((UnkStruct_ov5_021EAE78 *)arg0)->saveData), 0x1B5U, 1U, HEAP_ID_FIELD1) == 1) {
        BufferPlayersName(((MessageFormat *)((UnkStruct_ov5_021EAE78 *)arg0)->unk_38), 0U, sub_02034818((u32) *((s32 *)&((UnkStruct_ov5_021EAE78 *)arg0)->unk_6C[7])));
        ov01_021F71C4(arg0, 0x3E);
        *((s32 *)&((UnkStruct_ov5_021EAE78 *)arg0)->unk_44) = 2;
        return 0;
    }
    wiFiList = sub_0202C6F4(((UnkStruct_ov5_021EAE78 *)arg0)->saveData);
    v2 = 0;
loop_10:
    if (sub_0202C2DC(wiFiList, v2) == 0) {
        sub_0203A280(((UnkStruct_ov5_021EAE78 *)arg0)->saveData, *((s32 *)&((UnkStruct_ov5_021EAE78 *)arg0)->unk_6C[7]), v2, HEAP_ID_FIELD1, 0U);
    } else {
        v2 += 1;
        if (v2 < 0x20) {
            goto loop_10;
        }
    }
    *((s32 *)&((UnkStruct_ov5_021EAE78 *)arg0)->unk_44) = 1;
    return 0;
}

s32 ov01_021F6E44(UnkStruct_ov5_021EAE78 *arg0) {
    if (TextPrinterCheckActive((u8) *((s32 *)&arg0->unk_40)) == 0) {
        *((s16 *)&arg0->unk_98) = 0;
        *((s32 *)&arg0->unk_44) = 3;
    }
    return 0;
}
