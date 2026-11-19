#include "global.h"
#include "sprite_resources_header.h"

#include "overlay_90.h"
#include "bg_window.h"
#include "error_handling.h"
#include "math_util.h"
#include "pm_string.h"
#include "save.h"
#include "unk_0200A090.h"
#include "overlay_90_unk_struct_ov114_0225_e1_a4.h"
#include "overlay_90_unk_struct_ov114_0225_d084.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "player_data.h"
#include "overlay_91_unk_struct_ov66_02230dbc.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_07_bg_scroll_context.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_90_unk_struct_ov114_0225dac0.h"
#include "overlay_90_unk_struct_ov114_0225dffc.h"
#include "overlay_90_unk_struct_ov114_0225cdb4.h"
#include "overlay_90_unk_struct_ov114_0225ccd0.h"
#include "overlay_90_unk_struct_ov114_0225cfcc.h"
#include "overlay_90_unk_struct_ov114_0225d338.h"
#include "overlay_90_unk_struct_ov114_0225e0f8.h"

void ov90_022590C0(s32);

void ov90_0225917C(s32);

void ov90_0225A198(s32);

struct BufferManager;
struct MsgData;
struct SpriteList;
struct SpriteResourceCollection;
struct MessageFormat;
struct PlayerProfile;
struct UnkStruct_ov114_0225D678_t;
struct UnkStruct_ov66_0222DFF8_t;
typedef struct UnkStruct_ov114_0225D678_t UnkStruct_ov114_0225D678;
struct UnkStruct_ov114_0225D678_t {
    u8 unk_00;
    u8 unk_01;
    u16 heapID;
    u16 unk_04;
    u8 unk_06;
    u8 unk_07;
    UnkStruct_ov114_0225C76C unk_08;
    UnkStruct_ov114_0225E0F8 unk_18;
    UnkStruct_ov114_0225CEF0 unk_30;
    UnkStruct_ov114_0225D084 unk_3C;
    UnkStruct_ov114_0225D338 unk_4C;
    UnkStruct_ov114_0225CFCC unk_84;
    UnkStruct_ov114_0225CCD0 unk_214;
    UnkStruct_ov114_0225CDB4 *unk_22C;
    UnkStruct_ov114_0225E1A4 unk_230;
    void *unk_260;
    NNSG2dScreenData *unk_264;
    UnkStruct_ov114_0225DAC0 unk_268[4];
    UnkStruct_ov114_0225DFFC unk_348;
    SysTask *unk_650;
    SysTask *unk_654;
};
BOOL ov90_02259F7C(UnkStruct_ov114_0225DAC0 *param0);
void ov90_0225A134(UnkStruct_ov114_0225DFFC *param0, const UnkStruct_ov114_0225DAC0 *param1);
BOOL ov90_02259B68(UnkStruct_ov114_0225D678 *param0);

void ov90_02259B18(u32 param_1, s32 param_2) {
    ov90_022590C0(param_2 + 0x30);
    ov90_0225917C(param_2 + 0x84);
    ov90_0225A198(param_2 + 0x2e4);
    return;
}

int ov90_02259B38(const u8 *param0, u32 param1) {
    u32 v0;
    u32 v1;
    int v2;

    v0 = MTRandom() % 100;
    v1 = 0;

    for (v2 = 0; v2 < param1; v2++) {
        v1 += param0[v2];

        if (v1 > v0) {
            return v2;
        }
    }

    GF_ASSERT(FALSE);
    return param1 - 1;
}

BOOL ov90_02259B68(UnkStruct_ov114_0225D678 *param0) {
    int v0;
    BOOL v1;
    BOOL v2 = 1;

    for (v0 = 0; v0 < param0->unk_08.unk_08; v0++) {
        if (param0->unk_268[v0].unk_10 != NULL) {
            v1 = ov90_02259F7C(&param0->unk_268[v0]);
            ov90_0225A134(&param0->unk_348, &param0->unk_268[v0]);

            if (v1 == 0) {
                v2 = 0;
            }
        } else {
            v2 = 0;
        }
    }

    return v2;
}
