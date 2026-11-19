#include "global.h"

#include "error_handling.h"
#include "overlay_93_thumb_1.h"
#include "unk_02014DA0.h"
#include "field/overlay_01_021FB878.h"
#include "font.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "overlay_manager.h"
#include "pm_string.h"
#include "sprite_system.h"
#include "camera.h"
#include "gf_gfx_loader.h"
#include "palette.h"
#include "render_window.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

s32 ov93_02261EB8(void *, SpriteManager *, void *, void *, String *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
s32 ov93_02261FC8(void *);
ManagedSprite *ov93_02262230(UnkStruct_ov117_02261280_t *);
s32 ov93_02262250(void *);
ManagedSprite *ov93_02262344(UnkStruct_ov117_02261280_t *);
struct OverlayManager;
struct TextOBJ;
struct GF3DVramMan;
struct MsgData;
struct ParticleSystem;
struct MessageFormat;
struct PlayerProfile;
struct UnkStruct_02012744_t;
struct UnkStruct_ov114_02025CAD4_t;
struct UnkStruct_ov117_02261280_t;
struct UnkStruct_ov117_022626B0_t;
struct UnkStruct_ov66_0222DFF8_t;
void ov93_02261FB0(UnkStruct_ov117_02265EB0 *param0);
void ov93_02262310(UnkStruct_ov117_02261280_t *param0);
void ov93_02262338(UnkStruct_ov117_02261280_t *param0, ManagedSprite *param1);
void ov93_02262368(UnkStruct_ov117_02261280_t *param0, ManagedSprite *param1);
enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};
void ov93_0225D9E8(UnkStruct_ov117_02261280_t *param0);

void ov93_022609E0(UnkStruct_ov117_02261280_t *param0);
void ov93_02260CF8(UnkStruct_ov117_02261280_t *param0);
void ov93_02260E1C(UnkStruct_ov117_02261280_t *param0);
void ov93_022610B0(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_0226504C *param1);
void ov93_02261164(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_022653F4 *param1);
void ov93_02261BBC(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02265C3C *param1);
void ov93_0225DA40(UnkStruct_ov117_02261280_t *param0, NARC *param1);
void ov93_02260A14(UnkStruct_ov117_02261280_t *param0);
void ov93_0226114C(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_0226504C *param1);
void ov93_022612E0(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_022653F4 *param1);
void ov93_02261C3C(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02265C3C *param1);
void ov93_0225DAF8(UnkStruct_ov117_02261280_t *param0);
void ov93_0225DBC4(void);
s32 ov93_02260BB0(UnkStruct_ov117_02261280_t *);
s32 ov93_02260BF0(void *);
void ov93_0225DD28(void);

u32 ov93_0225D744(u32 param0, BOOL param1) {
    NNSGfdTexKey v0;

    v0 = NNS_GfdAllocTexVram(param0, param1, 0);
    GF_ASSERT(v0 != NNS_GFD_ALLOC_ERROR_TEXKEY);
    sub_02015354(v0);

    return NNS_GfdGetTexKeyAddr(v0);
}

u32 ov93_0225D768(u32 param0, BOOL param1) {
    NNSGfdPlttKey v0;
    u32 v1;

    v0 = NNS_GfdAllocPlttVram(param0, param1, NNS_GFD_ALLOC_FROM_LOW);

    if (v0 == NNS_GFD_ALLOC_ERROR_PLTTKEY) {
        GF_ASSERT(FALSE);
    }

    sub_02015394(v0);

    v1 = NNS_GfdGetPlttKeyAddr(v0);

    return v1;
}

void ov93_0225D78C(UnkStruct_ov117_02261280_t *arg0, NARC *narc) {
    String *v3;
    s32 v2;
    String *string;
    NARC *nARC;
    String *v4;
    s32 v5;
    s32 v6;
    void *v8;
    void *v7;

    nARC = narc;
    SpriteSystem_LoadPaletteBufferFromOpenNarc(arg0->unk_8C, PLTTBUF_MAIN_OBJ, arg0->unk_24, arg0->unk_28, narc, 0x1B, 0, 1, 1, 0x2713);
    v4 = NewString_ReadMsgData(arg0->unk_80, 2);
    v6 = ((u32)(0x1E - FontID_String_GetWidth(FONT_SYSTEM, v4, 0U)) >> 1) + 6;
    ov93_02261EB8(arg0->unk_2C, arg0->unk_28, arg0->unk_90, &arg0->unk_15A8.unk_F4, v4, 0, 0xE0F00, 0, 0x2713, v6 + 0x24, 0xA8, 0, 1, 0xC, 2);
    String_Delete(v4);
    string = NewString_ReadMsgData(arg0->unk_80, 0);
    v3 = NewString_ReadMsgData(arg0->unk_80, 1);
    v2 = 0;
    v8 = arg0->unk_15A8.unk_04;
    v7 = &arg0->unk_15A8.unk_04[0][1];
    do {
        ov93_02261EB8(arg0->unk_2C, arg0->unk_28, arg0->unk_90, v8, string, 0, 0xE0F00, 0, 0x2713, v6, 0xA8, 0, 3, 0xC, 0xA);
        ov93_02261EB8(arg0->unk_2C, arg0->unk_28, arg0->unk_90, v7, v3, 0, 0xE0F00, 0, 0x2713, v6, 0xA8, 0, 3, 0xC, 0xA);
        v6 += 6;
        v8 += 0x28;
        v5 = v2 + 1;
        v7 += 0x28;
        v2 = v5;
    } while (v5 < 6);
    String_Delete(string);
    String_Delete(v3);
    ov93_02261FC8(&arg0->unk_15A8);
    ov93_02262250(arg0);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(arg0->unk_8C, PLTTBUF_MAIN_OBJ, arg0->unk_24, arg0->unk_28, nARC, 0x1B, 0, 1, 1, 0x2714);
    SpriteSystem_LoadCharResObjFromOpenNarc(arg0->unk_24, arg0->unk_28, nARC, 0x18, 0, 1, 0x2712);
    SpriteSystem_LoadCellResObjFromOpenNarc(arg0->unk_24, arg0->unk_28, nARC, 0x1A, 0, 0x2712);
    SpriteSystem_LoadAnimResObjFromOpenNarc(arg0->unk_24, arg0->unk_28, nARC, 0x19, 0, 0x2712);
    arg0->unk_15A8.unk_00 = ov93_02262230(arg0);
    SpriteSystem_LoadPaletteBuffer(arg0->unk_8C, PLTTBUF_MAIN_OBJ, arg0->unk_24, arg0->unk_28, NARC_a_1_9_8, 0x14, 0, 1, 1, 0x2712);
    SpriteSystem_LoadCharResObj(arg0->unk_24, arg0->unk_28, NARC_a_1_9_8, 0x15, 0, 1, 0x2711);
    SpriteSystem_LoadCellResObj(arg0->unk_24, arg0->unk_28, NARC_a_1_9_8, 0x16, 0, 0x2711);
    SpriteSystem_LoadAnimResObj(arg0->unk_24, arg0->unk_28, NARC_a_1_9_8, 0x17, 0, 0x2711);
    arg0->unk_174C.unk_00 = ov93_02262344(arg0);
}

void ov93_0225D9E8(UnkStruct_ov117_02261280_t *param0) {
    int v0;

    for (v0 = 0; v0 < 6; v0++) {
        ov93_02261FB0(&param0->unk_15A8.unk_04[v0][0]);
        ov93_02261FB0(&param0->unk_15A8.unk_04[v0][1]);
    }

    ov93_02261FB0(&param0->unk_15A8.unk_F4);
    ov93_02262310(param0);
    ov93_02262338(param0, param0->unk_15A8.unk_00);
    ov93_02262368(param0, param0->unk_174C.unk_00);
}

void ov93_0225DA40(UnkStruct_ov117_02261280_t *param0, NARC *param1) {
    SpriteSystem_LoadPaletteBufferFromOpenNarc(param0->unk_8C, PLTTBUF_SUB_OBJ, param0->unk_24, param0->unk_28, param1, 23, FALSE, 9, NNS_G2D_VRAM_TYPE_2DSUB, 10006);
    SpriteSystem_LoadCharResObjFromOpenNarc(param0->unk_24, param0->unk_28, param1, 20, FALSE, NNS_G2D_VRAM_TYPE_2DSUB, 10004);
    SpriteSystem_LoadCellResObjFromOpenNarc(param0->unk_24, param0->unk_28, param1, 22, FALSE, 10004);
    SpriteSystem_LoadAnimResObjFromOpenNarc(param0->unk_24, param0->unk_28, param1, 21, FALSE, 10004);

    ov93_022609E0(param0);
    ov93_022610B0(param0, &param0->unk_1458);
    ov93_02261164(param0, &param0->unk_1468);
    ov93_02261BBC(param0, &param0->unk_1560);
    ov93_02260CF8(param0);
    ov93_02260E1C(param0);
}

void ov93_0225DAF8(UnkStruct_ov117_02261280_t *param0) {
    ov93_02260A14(param0);
    ov93_0226114C(param0, &param0->unk_1458);
    ov93_022612E0(param0, &param0->unk_1468);
    ov93_02261C3C(param0, &param0->unk_1560);
}

void ov93_0225DB2C(UnkStruct_ov117_02261280_t *arg0, NARC *narc) {
    PaletteData_LoadNarc(arg0->unk_8C, NARC_a_1_9_9, 4, HEAP_ID_117, PLTTBUF_MAIN_BG, 0U, 0U);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 2, arg0->unk_2C, GF_BG_LYR_MAIN_3, 0U, 0U, 0, HEAP_ID_117);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 3, arg0->unk_2C, GF_BG_LYR_MAIN_3, 0U, 0U, 0, HEAP_ID_117);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 0, arg0->unk_2C, GF_BG_LYR_SUB_1_F, 0U, 0U, 0, HEAP_ID_117);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 1, arg0->unk_2C, GF_BG_LYR_SUB_1_F, 0U, 0U, 0, HEAP_ID_117);
    PaletteData_FillPaletteInBuffer(arg0->unk_8C, PLTTBUF_MAIN_BG, PLTTSEL_BOTH, 0U, 0U, 1U);
}

void ov93_0225DBC4(void) {
}

void ov93_0225DBC8(UnkStruct_ov117_02261280_t *arg0, NARC *narc) {
    u8 v0;

    PaletteData_LoadNarc(arg0->unk_8C, NARC_a_1_9_9, 5, HEAP_ID_117, PLTTBUF_SUB_BG, 0xA0U, 0U);
    PaletteData_LoadFromNarc(arg0->unk_8C, NARC_a_1_9_9, 6, HEAP_ID_117, PLTTBUF_SUB_BG, 0x140U, 0x60U, 0x60U);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 0xD, arg0->unk_2C, GF_BG_LYR_SUB_2, 0U, 0U, 0, HEAP_ID_117);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0xE, arg0->unk_2C, GF_BG_LYR_SUB_2, 0U, 0U, 0, HEAP_ID_117);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 0x12, arg0->unk_2C, GF_BG_LYR_SUB_1, 0U, 0U, 0, HEAP_ID_117);
    v0 = arg0->unk_00->unk_30;
    switch (v0) {
    case 2:
        GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0xF, arg0->unk_2C, GF_BG_LYR_SUB_1, 0U, 0U, 0, HEAP_ID_117);
        break;
    case 3:
        GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0x10, arg0->unk_2C, GF_BG_LYR_SUB_1, 0U, 0U, 0, HEAP_ID_117);
        break;
    default:
        GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0x11, arg0->unk_2C, GF_BG_LYR_SUB_1, 0U, 0U, 0, HEAP_ID_117);
        break;
    }
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 0x12, arg0->unk_2C, GF_BG_LYR_SUB_2_F, 0U, 0U, 0, HEAP_ID_117);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0x13, arg0->unk_2C, GF_BG_LYR_SUB_2_F, 0U, 0U, 0, HEAP_ID_117);
    ov93_02260BF0(arg0);
    ov93_02260BB0(arg0);
    LoadUserFrameGfx1(arg0->unk_2C, GF_BG_LYR_SUB_2_F, 0x350U, 6U, 0U, HEAP_ID_117);
    PaletteData_LoadPaletteSlotFromHardware(arg0->unk_8C, PLTTBUF_SUB_BG, 0x60U, 0x20U);
    PaletteData_FillPaletteInBuffer(arg0->unk_8C, PLTTBUF_SUB_BG, PLTTSEL_BOTH, 0U, 0U, 1U);
}

void ov93_0225DD28(void) {
}
