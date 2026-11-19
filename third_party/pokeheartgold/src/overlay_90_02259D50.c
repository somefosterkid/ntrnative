#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "pm_string.h"
#include "sprite.h"
#include "overlay_90_unk_struct_ov114_0225_d084.h"
#include "player_data.h"
#include "msgdata.h"
#include "overlay_90_unk_struct_ov114_0225cbf0.h"
#include "overlay_90_unk_struct_ov114_0225dac0.h"

struct MsgData;

struct MessageFormat;

struct PlayerProfile;

typedef u32 TextColor;

void ov90_02259200(UnkStruct_ov114_0225D084 *param0, const PlayerProfile *param1);

void ov90_022592A8(UnkStruct_ov114_0225D084 *param0, u32 param1, Window *param2, u8 param3, u8 param4);

void ov90_02259350(UnkStruct_ov114_0225D084 *param0, u32 param1, Window *param2, u8 param3, u8 param4, TextColor param5);

void ov90_02259D50(UnkStruct_ov114_0225DAC0 *param0, UnkStruct_ov114_0225D084 *param1, const PlayerProfile *param2, s16 param3, s16 param4, u32 param5);

void ov90_02259E18(void *arg0, s32 arg1);

void ov90_02259D50(UnkStruct_ov114_0225DAC0 *param0, UnkStruct_ov114_0225D084 *param1, const PlayerProfile *param2, s16 param3, s16 param4, u32 param5) {
    ov90_02259200(param1, param2);

    if (param5 == 1) {
        ov90_02259350(param1, 10, &param0->unk_00, param3, param4, ((TextColor)(((5 & 0xFF) << 16) | ((6 & 0xFF) << 8) | ((0 & 0xFF) << 0))));
    } else {
        ov90_022592A8(param1, 10, &param0->unk_00, param3, param4);
    }
}

void ov90_022591F4(UnkStruct_ov114_0225D084 *param0);

void ov90_02259210(UnkStruct_ov114_0225D084 *param0, u32 param1);

void ov90_02259220(UnkStruct_ov114_0225D084 *param0, u32 param1, u32 param2);

void ov90_02259DAC(UnkStruct_ov114_0225DAC0 *param0, UnkStruct_ov114_0225D084 *param1, BOOL param2, u32 param3, u32 param4, s16 param5);

void ov90_02259DAC(UnkStruct_ov114_0225DAC0 *param0, UnkStruct_ov114_0225D084 *param1, BOOL param2, u32 param3, u32 param4, s16 param5) {
    if (param2) {
        if (param3 != 0) {
            ov90_022591F4(param1);
            ov90_02259210(param1, param3);
            ov90_022592A8(param1, 11, &param0->unk_00, param5, 16);
        }
    } else {
        if (param3 != 0) {
            ov90_022591F4(param1);
            ov90_02259220(param1, param3, param4);
            ov90_022592A8(param1, 12, &param0->unk_00, param5, 16);
        }
    }
}

void ov90_02259E18(void *arg0, s32 arg1) {
    Sprite_SetDrawFlag(*(Sprite **)((u8 *)arg0 + 0x14), 1);
    Sprite_SetAnimCtrlSeq(*(Sprite **)((u8 *)arg0 + 0x14), arg1 + 4);
    Sprite_SetAnimActiveFlag(*(Sprite **)((u8 *)arg0 + 0x14), 1);
}
