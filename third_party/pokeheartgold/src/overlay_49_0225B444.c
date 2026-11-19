#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_planes.h"
#include "overlay_49.h"
#include "player_data.h"
#include "overlay_49_unk_struct_ov70_0225_dee8_t.h"

typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;

void ov49_0225BB10(UnkStruct_ov70_0225F350 *, UnkStruct_ov70_0225E4EC *);
void ov49_0225BBA8(UnkStruct_ov70_0225FA84 *, UnkStruct_ov70_0225E9C8 *, UnkStruct_ov70_0225E4EC *);
void ov49_0225C8A8(UnkStruct_ov70_02260744 *);
void ov49_0225B4E4(UnkStruct_ov70_0225F350 *param_1, UnkStruct_ov70_0225E9C8 *param_2, UnkStruct_ov70_0225E4EC *param_3);

void ov49_0225B444(UnkStruct_ov70_0225DEE8 *arg0) {
    arg0->unk_43C = 1;
}

s32 ov49_0225BABC(UnkStruct_ov70_0225F350 *, UnkStruct_ov70_0225E4EC *, NARC *, u32, enum HeapID);

s32 ov49_0225BB84(UnkStruct_ov70_0225FA84 *, UnkStruct_ov70_0225E4EC *, NARC *, enum HeapID);

s32 ov49_0225C844(UnkStruct_ov70_02260744 *, UnkStruct_ov70_0225E4EC *, NARC *, enum HeapID);

void ov49_0225B450(UnkStruct_ov70_0225F350 *arg0, const UnkStruct_ov70_02263910 *arg1, UnkStruct_ov70_0225E4EC *arg2, PlayerProfile *arg3, enum HeapID arg4);

void ov49_0225B450(UnkStruct_ov70_0225F350 *arg0, const UnkStruct_ov70_02263910 *arg1, UnkStruct_ov70_0225E4EC *arg2, PlayerProfile *profile, enum HeapID heapID) {
    u32 v0;

    v0 = PlayerProfile_GetTrainerGender(profile);
    arg0->unk_14C = NARC_New(NARC_a_2_0_7, heapID);
    ov49_0225BABC(arg0, arg2, arg0->unk_14C, v0, heapID);
    ov49_0225BB84(&arg0->unk_08, arg2, arg0->unk_14C, heapID);
    ov49_0225C844(&arg0->unk_114, arg2, arg0->unk_14C, heapID);
    arg0->unk_02 = 0;
    arg0->unk_01 = 0;
    if (arg1->unk_06 == 0) {
        arg0->unk_00 = 0;
        BG_SetMaskColor(4U, 0U);
        GfGfx_EngineBTogglePlanes(1U, 0U);
        GfGfx_EngineBTogglePlanes(2U, 0U);
        GfGfx_EngineBTogglePlanes(4U, 0U);
        GfGfx_EngineBTogglePlanes(8U, 0U);
        return;
    }
    arg0->unk_00 = 5;
}

void ov49_0225B4E4(UnkStruct_ov70_0225F350 *param_1, UnkStruct_ov70_0225E9C8 *param_2, UnkStruct_ov70_0225E4EC *param_3) {
    ov49_0225C8A8(&param_1->unk_114);
    ov49_0225BBA8(&param_1->unk_08, param_2, param_3);
    ov49_0225BB10(param_1, param_3);
    NARC_Delete(param_1->unk_14C);
    return;
}
