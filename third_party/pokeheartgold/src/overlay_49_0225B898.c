#include "global.h"

#include "error_handling.h"
#include "bg_window.h"
#include "filesystem.h"
#include "list_menu.h"
#include "pm_string.h"
#include "touch_hitbox_controller.h"
#include "unk_0200A090.h"
#include "overlay_49_unk_struct_ov70_0225_e9_c8.h"
#include "overlay_49_unk_struct_ov70_0225_e4_ec.h"
#include "overlay_45_wifi_plaza_system.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_49_unk_struct_ov70_0225f208.h"
#include "overlay_49_unk_struct_ov70_02260744.h"
#include "overlay_49_unk_struct_ov70_0225fa84.h"
#include "overlay_49_unk_struct_ov70_0225f350.h"

u8 ov49_0225B898(const UnkStruct_ov70_0225F350 *arg0);

struct G3DPipelineBuffers;
struct MsgData;
struct SpriteList;
struct SpriteResourceCollection;
struct MessageFormat;
struct TouchHitboxController;
struct WiFiPlazaProfile;
typedef struct G3DPipelineBuffers G3DPipelineBuffers;
void ov49_0225B99C(UnkStruct_ov70_0225F350 *param0, UnkStruct_ov70_0225E9C8 *param1, UnkStruct_ov70_0225E4EC *param2);
void ov49_0225BBD0(UnkStruct_ov70_0225F350 *param0, UnkStruct_ov70_0225E9C8 *param1, UnkStruct_ov70_0225FA84 *param2, UnkStruct_ov70_0225E4EC *param3, UnkStruct_ov70_0225F208 *param4, BOOL param5, NARC *param6, u32 heapID, const WiFiPlazaProfile *param8, const WiFiPlazaProfile *param9, BOOL param10, BOOL param11, const UnkStruct_ov66_02230914 *param12, BOOL param13);
void ov49_0225B944(UnkStruct_ov70_0225F350 *param0, UnkStruct_ov70_0225E9C8 *param1, UnkStruct_ov70_0225E4EC *param2, UnkStruct_ov70_0225F208 *param3, BOOL param4, u32 heapID, const WiFiPlazaProfile *param6, const WiFiPlazaProfile *param7, BOOL param8, BOOL param9, const UnkStruct_ov66_02230914 *param10, BOOL param11);

u8 ov49_0225B898(const UnkStruct_ov70_0225F350 *arg0) {
    return arg0->unk_00;
}

void ov49_0225B89C(UnkStruct_ov70_0225F350 *arg0, s8 arg1, s16 arg2);

void ov49_0225B89C(UnkStruct_ov70_0225F350 *arg0, s8 arg1, s16 arg2) {
    arg0->unk_02 = 2;
    arg0->unk_03 = arg1;
    arg0->unk_04 = arg2;
}

s32 ov49_0225BFC4(UnkStruct_ov70_0225FA84 *, UnkStruct_ov70_0225E4EC *, NARC *, s32, u32);

void ov49_0225B8A8(UnkStruct_ov70_0225F350 *arg0, UnkStruct_ov70_0225E4EC *arg1, u32 arg2, s32 arg3);

void ov49_0225B8A8(UnkStruct_ov70_0225F350 *arg0, UnkStruct_ov70_0225E4EC *arg1, u32 arg2, s32 arg3) {
    if (arg2 >= 0x1BU) {
        GF_AssertFail();
    }
    if (arg0->unk_01 != 2) {
        GF_AssertFail();
    }
    if (arg0->unk_01 == 2) {
        ov49_0225BFC4(&arg0->unk_08, arg1, arg0->unk_14C, arg3, arg2);
    }
}

void ov49_0225B8E0(UnkStruct_ov70_0225F350 *arg0);

void ov49_0225B8E0(UnkStruct_ov70_0225F350 *arg0) {
    if (arg0->unk_01 != 1) {
        arg0->unk_02 = 1;
    }
}

void ov49_0225B8EC(UnkStruct_ov70_0225F350 *arg0);

void ov49_0225B8EC(UnkStruct_ov70_0225F350 *arg0) {
    if (arg0->unk_01 != 3) {
        arg0->unk_02 = 3;
    }
}

u8 ov49_0225B8F8(UnkStruct_ov70_0225F350 *arg0);

u8 ov49_0225B8F8(UnkStruct_ov70_0225F350 *arg0) {
    return arg0->unk_03;
}

s32 ov49_0225B8FC(UnkStruct_ov70_0225F350 *arg0);

s32 ov49_0225B8FC(UnkStruct_ov70_0225F350 *arg0) {
    if (arg0->unk_02 == 2) {
        return 1;
    }
    if (arg0->unk_01 == 2) {
        return 1;
    }
    return 0;
}

s32 ov49_0225B914(UnkStruct_ov70_0225F350 *arg0);

s32 ov49_0225B914(UnkStruct_ov70_0225F350 *arg0) {
    if ((arg0->unk_02 == 0) && (arg0->unk_01 == 2)) {
        return 1;
    }
    return 0;
}

s32 ov49_0225BFEC(const UnkStruct_ov70_0225FA84 *);

s32 ov49_0225B928(UnkStruct_ov70_0225F350 *arg0);

s32 ov49_0225B928(UnkStruct_ov70_0225F350 *arg0) {
    return ov49_0225BFEC(&arg0->unk_08);
}

s32 ov49_0225B934(UnkStruct_ov70_0225F350 *arg0);

s32 ov49_0225B934(UnkStruct_ov70_0225F350 *arg0) {
    if (arg0->unk_02 != 0) {
        return 1;
    }
    return 0;
}

void ov49_0225B944(UnkStruct_ov70_0225F350 *param0, UnkStruct_ov70_0225E9C8 *param1, UnkStruct_ov70_0225E4EC *param2, UnkStruct_ov70_0225F208 *param3, BOOL param4, u32 heapID, const WiFiPlazaProfile *param6, const WiFiPlazaProfile *param7, BOOL param8, BOOL param9, const UnkStruct_ov66_02230914 *param10, BOOL param11) {
    if (param0->unk_01 == 2) {
        ov49_0225B99C(param0, param1, param2);
    }

    ov49_0225BBD0(param0, param1, &param0->unk_08, param2, param3, param4, param0->unk_14C, heapID, param6, param7, param8, param9, param10, param11);
    param0->unk_01 = 2;
}
