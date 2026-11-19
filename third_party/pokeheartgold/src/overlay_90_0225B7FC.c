#include "global.h"
#include "sprite_resources_header.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "yes_no_prompt.h"
#include "overlay_90.h"
#include "unk_02005D10.h"
#include "sprite.h"
#include "overlay_07_bg_scroll_context.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_90_unk_struct_ov114_0225_d084.h"
#include "overlay_90_unk_struct_ov114_0225dac0.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "player_data.h"
#include "overlay_91_unk_struct_ov66_02230dbc.h"
#include "overlay_90_unk_struct_ov114_0225c76c.h"
#include "overlay_90_unk_struct_ov114_0225c9a8.h"
#include "msgdata.h"
#include "overlay_90_unk_struct_ov114_0225cdb4.h"
#include "overlay_90_unk_struct_ov114_0225ccd0.h"
#include "overlay_90_unk_struct_ov114_0225cfcc.h"
#include "overlay_90_unk_struct_ov114_02260284.h"
#include "overlay_90_unk_struct_ov114_0225cef0.h"
#include "overlay_90_unk_struct_ov114_0225d338.h"
#include "overlay_90_unk_struct_ov114_0225e0f8.h"
#include "overlay_90_unk_struct_ov114_0225dffc.h"
#include "overlay_90_unk_struct_ov114_0225f270.h"
#include "overlay_90_unk_struct_ov114_0225f578.h"
#include "overlay_90_unk_struct_ov114_0225_e854_t.h"

struct BufferManager;

struct MsgData;

struct SpriteList;

struct SpriteResourceCollection;

struct MessageFormat;

struct PlayerProfile;

struct UnkStruct_ov114_0225E854_t;

struct UnkStruct_ov66_0222DFF8_t;

struct YesNoPrompt;

typedef struct UnkStruct_ov114_0225E854_t UnkStruct_ov114_0225E854;

u32 ov90_0225888C(const UnkStruct_ov114_0225C76C *param0, u32 param1);

BOOL ov90_022588A4(const UnkStruct_ov114_0225C76C *param0, u32 param1);

enum PlazaMinigame {
    UnkEnum_ov66_022324D0_00,
    UnkEnum_ov66_022324D0_01,
    UnkEnum_ov66_022324D0_02,
    UnkEnum_ov66_022324D0_03,
    UnkEnum_ov66_022324D0_04,
    UnkEnum_ov66_022324D0_05,
    UnkEnum_ov66_022324D0_06,
    UnkEnum_ov66_022324D0_07,
    UnkEnum_ov66_022324D0_08,
    UnkEnum_ov66_022324D0_09,
    UnkEnum_ov66_022324D0_10,
    UnkEnum_ov66_022324D0_11,
    UnkEnum_ov66_022324D0_12 = UnkEnum_ov66_022324D0_02 + 1,
};

void ov90_02259BCC(UnkStruct_ov114_0225DAC0 *param0, u32 param1, u32 param2, NNSG2dScreenData *param3, UnkStruct_ov114_0225CEF0 *param4, u32 param5, u32 param6, UnkStruct_ov114_0225CFCC *param7, UnkStruct_ov114_0225CDB4 *param8, u32 heapID);

void ov90_02259D50(UnkStruct_ov114_0225DAC0 *param0, UnkStruct_ov114_0225D084 *param1, const PlayerProfile *param2, s16 param3, s16 param4, u32 param5);

void ov90_02259DAC(UnkStruct_ov114_0225DAC0 *param0, UnkStruct_ov114_0225D084 *param1, BOOL param2, u32 param3, u32 param4, s16 param5);

void ov90_02259E18(UnkStruct_ov114_0225DAC0 *param0, u32 param1);

void ov90_02259E38(UnkStruct_ov114_0225DAC0 *param0, UnkStruct_ov114_0225D084 *param1, u32 param2, s16 param3, s16 param4, u32 param5);

void ov90_02259E8C(UnkStruct_ov114_0225DAC0 *param0, UnkStruct_ov114_0225CEF0 *param1);

void ov90_02259EE0(UnkStruct_ov114_0225DAC0 *param0, s16 param1, s16 param2);

extern const s16 ov90_0225C1F0[4];

void ov90_0225B7FC(UnkStruct_ov114_0225E854 *param0, u32 param1, u32 param2);

BOOL ov90_02259F7C(UnkStruct_ov114_0225DAC0 *param0);
void ov90_0225A134(UnkStruct_ov114_0225DFFC *param0, const UnkStruct_ov114_0225DAC0 *param1);
BOOL ov90_0225B8F0(UnkStruct_ov114_0225E854 *param0);

void ov90_0225B954(UnkStruct_ov114_0225E854 *arg0);

void ov90_0225B7FC(UnkStruct_ov114_0225E854 *param0, u32 param1, u32 param2) {
    BOOL v0;
    u32 v1;
    u32 v2;

    ov90_02259BCC(&param0->unk_2C0[param1], param0->unk_0C.unk_08, param0->unk_07, param0->unk_2BC[0], &param0->unk_4C, param1, param0->unk_0C.unk_09, &param0->unk_A0, param0->unk_248, param0->heapID);

    v2 = ov90_0225888C(&param0->unk_0C, param1);
    v0 = ov90_022588A4(&param0->unk_0C, v2);

    ov90_02259D50(&param0->unk_2C0[param1], &param0->unk_58, param0->unk_34.unk_08[param1], 8, 0, v0);
    ov90_02259DAC(&param0->unk_2C0[param1], &param0->unk_58, param0->unk_05, param0->unk_34.unk_00[param1], param0->unk_34.unk_04[param1], 8);

    if (param0->unk_07 == UnkEnum_ov66_022324D0_00) {
        v1 = 5;
    } else {
        v1 = 6;
    }

    ov90_02259E38(&param0->unk_2C0[param1], &param0->unk_58, param0->unk_1C.unk_00[param1], 176 + (-ov90_0225C1F0[4 - (param2 + 1)]), 0, v1);
    ov90_02259E8C(&param0->unk_2C0[param1], &param0->unk_4C);
    ov90_02259E18(&param0->unk_2C0[param1], param2);
    ov90_02259EE0(&param0->unk_2C0[param1], ov90_0225C1F0[param2], 0);
}

BOOL ov90_0225B8F0(UnkStruct_ov114_0225E854 *param0) {
    int v0;
    BOOL v1;
    BOOL v2 = 1;

    for (v0 = 0; v0 < param0->unk_0C.unk_08; v0++) {
        if (param0->unk_2C0[v0].unk_10 != NULL) {
            v1 = ov90_02259F7C(&param0->unk_2C0[v0]);
            ov90_0225A134(&param0->unk_3A0, &param0->unk_2C0[v0]);

            if (v1 == 0) {
                v2 = 0;
            }
        } else {
            v2 = 0;
        }
    }

    return v2;
}

void ov90_0225B954(UnkStruct_ov114_0225E854 *arg0) {
    if (arg0->unk_6C4 == 0) {
        PlayFanfare(0x4BFU);
        arg0->unk_6C4 = 1;
    }
}

s32 ov90_0225B978(UnkStruct_ov114_0225E854 *arg0) {
    if ((arg0->unk_6C4 == 1) && (IsFanfarePlaying() == 0)) {
        arg0->unk_6C4 = 0;
    }
    if (arg0->unk_6C4 != 1) {
        return 1;
    }
    return 0;
}
