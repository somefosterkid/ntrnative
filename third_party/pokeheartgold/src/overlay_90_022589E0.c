#include "global.h"
#include "sprite_resources_header.h"

#include "overlay_90.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "heap.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "unk_0200A090.h"
#include "vram_transfer_manager.h"
#include "yes_no_prompt.h"
#include "unk_02034354.h"
#include "sprite.h"
#include "overlay_07_bg_scroll_context.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_90_unk_struct_ov114_0225_d084.h"
#include "overlay_90_unk_struct_ov114_0225dac0.h"
#include "overlay_90_unk_struct_ov114_0225_e1_a4.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
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

void *ov90_0225A6B4(void *, void *, s32, s32);

struct BufferManager;
struct MsgData;
struct SpriteList;
struct SpriteResourceCollection;
struct MessageFormat;
struct PlayerProfile;
struct UnkStruct_ov114_0225D678_t;
struct UnkStruct_ov114_0225E854_t;
struct UnkStruct_ov66_0222DFF8_t;
struct YesNoPrompt;
typedef struct UnkStruct_ov114_0225E854_t UnkStruct_ov114_0225E854;
void ov90_02258E10(UnkStruct_ov114_0225CCD0 *param0);
void ov90_02259084(UnkStruct_ov114_0225CEF0 *param0);
void ov90_02259158(UnkStruct_ov114_0225CFCC *param0);
void ov90_022591D4(UnkStruct_ov114_0225D084 *param0);
void ov90_02259434(UnkStruct_ov114_0225D338 *param0);
void ov90_02259EAC(UnkStruct_ov114_0225DAC0 *param0);
void ov90_0225A108(UnkStruct_ov114_0225DFFC *param0);
void ov90_0225A960(struct UnkStruct_ov114_0225E854_t *param0);
void ov90_0225B380(UnkStruct_ov114_0225F270 *param0);
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
void ov90_0225B53C(UnkStruct_ov114_0225F270 *param0, u32 param1, BOOL param2, u32 param3);
void ov90_02258A04(UnkStruct_ov114_0225E854 *param0);
BOOL ov90_02258AA0(const UnkStruct_ov114_0225E854 *param0);
BOOL ov90_02258AA4(const UnkStruct_ov114_0225E854 *param0);
void ov90_02258AA8(UnkStruct_ov114_0225C9A8 *param0, u32 param1);
void ov90_02258B24(UnkStruct_ov114_0225D678 *param0, int param1);
void ov90_02258B2C(UnkStruct_ov114_0225E854 *param0, u32 param1, BOOL param2);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov90_0225B594(UnkStruct_ov114_0225F270 *param0, s8 param1);
#else
void ov90_0225B594(UnkStruct_ov114_0225F270 *param0, BOOL param1);
#endif
int sub_020390C4(void);
u8 sub_02039264(void);
int sub_02037454(void);
BOOL sub_020392A0(void);
void sub_0203986C(void);
void ov90_02258B44(UnkStruct_ov114_0225E854 *param0, BOOL param1);
BOOL ov90_02258B54(UnkStruct_ov114_0225C76C *param0);
BOOL ov90_02258B98(const UnkStruct_ov114_0225C76C *param0);

void ov90_022589E0(void *arg0, void *arg1, s32 arg2) {
    ov90_0225A6B4(arg0, arg1, 0, arg2);
}

void ov90_022589EC(void *arg0, void *arg1, s32 arg2) {
    ov90_0225A6B4(arg0, arg1, 1, arg2);
}

void ov90_022589F8(void *arg0, void *arg1, s32 arg2) {
    ov90_0225A6B4(arg0, arg1, 2, arg2);
}

void ov90_02258A04(UnkStruct_ov114_0225E854 *param0) {
    SysTask_Destroy(param0->unk_6A8);
    SysTask_Destroy(param0->unk_6AC);

    ov90_0225A108(&param0->unk_3A0);

    {
        int v0;

        for (v0 = 0; v0 < param0->unk_0C.unk_08; v0++) {
            ov90_02259EAC(&param0->unk_2C0[v0]);
        }
    }

    ov90_0225B380(&param0->unk_24C);
    ov90_0225A960(param0);
    ov90_02259434(&param0->unk_68);
    ov90_022591D4(&param0->unk_58);
    ov90_02258E10(&param0->unk_230);
    ov90_02259158(&param0->unk_A0);
    ov90_02259084(&param0->unk_4C);

    G2_BlendNone();
    G2S_BlendNone();

    GF_DestroyVramTransferManager();
    Heap_Free(param0);
}

BOOL ov90_02258AA0(const UnkStruct_ov114_0225E854 *param0) {
    return param0->unk_06;
}

BOOL ov90_02258AA4(const UnkStruct_ov114_0225E854 *param0) {
    return param0->unk_08;
}

void ov90_02258AA8(UnkStruct_ov114_0225C9A8 *param0, u32 param1) {
    u8 v0[4];
    int v1, v2;

    for (v1 = 0; v1 < param1; v1++) {
        for (v2 = v1; v2 > 0; v2--) {
            if (param0->unk_00[v1] <= param0->unk_00[v0[v2 - 1]]) {
                break;
            } else {
                v0[v2] = v0[v2 - 1];
            }
        }

        v0[v2] = v1;
    }

    {
        for (v1 = 0; v1 < param1; v1++) {
            (void)0;
        }
    }

    {
        u32 v3;

        for (v1 = 0; v1 < param1; v1++) {
            v3 = v1;

            if (v1 > 0) {
                if (param0->unk_00[v0[v1]] == param0->unk_00[v0[v1 - 1]]) {
                    v3 = param0->unk_10[v0[v1 - 1]];
                }
            }

            param0->unk_10[v0[v1]] = v3;
        }
    }
}

void ov90_02258B24(UnkStruct_ov114_0225D678 *param0, int param1) {
    param0->unk_06 = param1;
    param0->unk_07 = 1;
}

void ov90_02258B2C(UnkStruct_ov114_0225E854 *param0, u32 param1, BOOL param2) {
    if (param0->unk_0C.unk_09 == 0) {
        ov90_0225B53C(&param0->unk_24C, param1, param2, param0->unk_0C.unk_08);
    }
}

void ov90_02258B44(UnkStruct_ov114_0225E854 *param0, BOOL param1) {
    ov90_0225B594(&param0->unk_24C, param1);
}

BOOL ov90_02258B54(UnkStruct_ov114_0225C76C *param0) {
    BOOL v0 = 0;

    if (param0->unk_0C != 0L) {
        if (param0->unk_08 != sub_02037454()) {
            v0 = 1;
        }

        if (sub_02039264() == 1) {
            v0 = 1;
        }

        if (sub_020390C4() >= 2) {
            v0 = 1;
        }

        if (v0 == 1) {
            param0->unk_0C->unk_1C = 1;
        }

        if (param0->unk_0C->unk_1C == 1) {
            v0 = 1;
        }
    }

    return v0;
}

BOOL ov90_02258B98(const UnkStruct_ov114_0225C76C *param0) {
    ((param0->unk_0C != 0L) ? (void)0 : GF_AssertFail());
    ((param0->unk_0C->unk_1C == 1) ? (void)0 : GF_AssertFail());

    if (sub_02034420() == 1) {
        sub_020343E4();
        sub_0203986C();
    } else {
        if (sub_020392A0() == 1) {
            return 1;
        }
    }

    return 0;
}
