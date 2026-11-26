#include "global.h"

#include "overlay_45_wifi_plaza_api.h"
#include "gf_gfx_planes.h"
#include "unk_02026E30.h"
#include "overlay_45_thumb.h"
#include "bg_window.h"
#include "options.h"
#include "player_data.h"
#include "pm_string.h"
#include "overlay_49_unk_struct_ov70_0225_dee8_t.h"

extern u16 ov49_02269754;

void ov49_0225A854(void);

void ov49_0225A854(void) {
    VecFx16 sp4;

    GfGfx_EngineATogglePlanes(1U, 1U);
    reg_G2_BG0CNT = (u16) (1 | (reg_G2_BG0CNT & ~3));
    reg_G3X_DISP3DCNT = (u16) (reg_G3X_DISP3DCNT & 0xFFFFCFFD);
    reg_G3X_DISP3DCNT = (u16) (0x10 | (reg_G3X_DISP3DCNT & 0xFFFFCFFF));
    reg_G3X_DISP3DCNT = (u16) (0xCFFB & reg_G3X_DISP3DCNT);
    reg_G3X_DISP3DCNT = (u16) (8 | (reg_G3X_DISP3DCNT & 0xFFFFCFFF));
    reg_G3X_DISP3DCNT = (u16) (0x20 | (reg_G3X_DISP3DCNT & 0xFFFFCFFF));
    G3X_SetEdgeColorTable(&ov49_02269754);
    G3X_SetFog(0, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
    G3X_SetClearColor(0x6B5AU, 0, 0x7FFF, 0x3F, 0);
    reg_G3_VIEWPORT = 0xBFFF0000;
    NNS_G3dGlbLightVector(GX_LIGHTID_0, 0xFFFFF805, 0xFFFFF224, 0x6E);
    sp4.x = 0;
    sp4.y = 0;
    sp4.z = 0x1000;
    VEC_Fx16Normalize(&sp4, &sp4);
    NNS_G3dGlbLightVector(GX_LIGHTID_1, sp4.x, sp4.y, sp4.z);
    sp4.x = 0;
    sp4.y = 0;
    sp4.z = 0x1000;
    VEC_Fx16Normalize(&sp4, &sp4);
    NNS_G3dGlbLightVector(GX_LIGHTID_2, sp4.x, sp4.y, sp4.z);
    NNS_G3dGlbLightColor(GX_LIGHTID_2, 0x7FFFU);
    sp4.x = 0;
    sp4.y = 0;
    sp4.z = 0x1000;
    VEC_Fx16Normalize(&sp4, &sp4);
    NNS_G3dGlbLightVector(GX_LIGHTID_3, sp4.x, sp4.y, sp4.z);
    NNS_G3dGlbLightColor(GX_LIGHTID_3, 0x7FFFU);
}

typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;

s32 ov49_02258B44(void *);

s32 ov49_02258B5C(void *);

void ov49_0225AA70(UnkStruct_ov70_0225DEE8 *arg0);

s32 ov49_0225B518(UnkStruct_ov70_0225F350 *, UnkStruct_ov70_0225DEE8 *, s32, s32);

s32 ov49_0225E318(void *);

s32 ov49_0225EF24(void *);

s32 ov49_0225EF30(void *);

s32 ov49_02265378(void *);

s32 ov49_02268870(void *);

void ov49_0225A98C(UnkStruct_ov70_0225DEE8 *arg0);

void ov49_0225A98C(UnkStruct_ov70_0225DEE8 *arg0) {
    s32 var_r2;

    ov49_02258B44(arg0->unk_444);
    if ((arg0->unk_04_4 == 0) && (arg0->unk_06 == 0)) {
        ov49_0225EF24(arg0->unk_454);
    }
    ov49_0225E318(arg0->unk_448);
    if ((arg0->unk_04_4 == 0) && (arg0->unk_06 == 0)) {
        ov49_02258B5C(arg0->unk_444);
        ov49_0225EF30(arg0->unk_454);
        ov49_02268870(arg0->unk_440);
    }
    ov49_0225AA70(arg0);
    if (arg0->unk_03 == 0) {
        if ((arg0->unk_04_4 == 1) || (arg0->unk_06 == 1) || (arg0->unk_00 == 1)) {
            var_r2 = 1;
        } else {
            var_r2 = 0;
        }
        ov49_0225B518(&arg0->unk_1E8, arg0, var_r2, 0x77);
    }
    ov49_02265378(arg0->unk_438);
}

s32 ov49_02258BD4(void *);

s32 ov49_0225A7C0(UnkStruct_ov70_0225E4EC *);

s32 ov49_0225CBF4(void *);

s32 ov49_0225E3A0(void *);

s32 ov49_02265398(void *);

void ov49_0225AA2C(UnkStruct_ov70_0225DEE8 *arg0);

void ov49_0225AA2C(UnkStruct_ov70_0225DEE8 *arg0) {
    Thunk_G3X_Reset();
    ov49_0225CBF4(arg0->unk_450);
    ov49_0225E3A0(arg0->unk_448);
    ov49_02258BD4(arg0->unk_444);
    ov49_02265398(arg0->unk_438);
    RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    ov49_0225A7C0(&arg0->unk_3C);
}


s32 ov45_0222F294(s32);

s32 ov45_0222F314(s32);

s32 ov49_0225E8C4(void *, s32, s32, s32, s32, s32);

void ov49_0225AA70(UnkStruct_ov70_0225DEE8 *arg0) {
    s32 sp8;
    s32 temp_r0;
    s32 var_r4;
    s32 var_r5;

    sp8 = ov45_0222A394(arg0->unk_34);
    var_r4 = 0;
    do {
        temp_r0 = ov45_0222F274(var_r4);
        if (temp_r0 == 1) {
            if (ov45_0222F294(var_r4) == 0) {
                var_r5 = 1;
            } else {
                var_r5 = 0;
            }
        } else {
            var_r5 = 0;
        }
        ov49_0225E8C4(arg0->unk_448, var_r4, ov45_0222F314(var_r4), temp_r0, var_r5, sp8);
        var_r4 += 1;
    } while (var_r4 < 3);
}

void ov49_0225AAC8(UnkStruct_ov70_0225E9C8 *arg0, UnkStruct_ov70_0225E4EC *arg1, SaveData *arg2, enum HeapID arg3);

void ov49_0225AAC8(UnkStruct_ov70_0225E9C8 *arg0, UnkStruct_ov70_0225E4EC *arg1, SaveData *arg2, enum HeapID arg3) {
    AddWindowParameterized(arg1->unk_00, &arg0->unk_00, 1U, 2U, 0x13U, 0x1BU, 4U, 4U, 0x5EU);
    FillWindowPixelBuffer(&arg0->unk_00, 0xFU);
    arg0->unk_18 = String_New(0x180U, arg3);
    arg0->unk_14 = (s32) Options_GetTextFrameDelay(Save_PlayerData_GetOptionsAddr(arg2));
}
