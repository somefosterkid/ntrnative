#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "gf_3d_vramman.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "options.h"
#include "player_data.h"
#include "render_window.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"
#include "unk_0203A3B0.h"
#include "vram_transfer_manager.h"
#include "overlay_49_unk_struct_ov70_0225_e4_ec.h"

void ov49_0225A854(void);

extern GraphicsModes ov49_02269724;

extern s32 ov49_02269734;

extern ObjCharTransferTemplate ov49_02269744;

extern GraphicsBanks ov49_022697CC;

extern BgTemplate ov49_0226981C;

void ov49_0225A5EC(void *arg0, SaveData *arg1, enum HeapID arg2);

void ov49_0225A5EC(void *arg0, SaveData *arg1, enum HeapID arg2) {
    s32 sp18;
    SaveData *sp14;
    BgTemplate *var_r7;
    GfGfxResType var_r7_2;
    s32 *var_r4;
    s32 temp_r0;
    u8 temp_r4;
    struct SpriteResourceCollection **var_r4_2;

    sp14 = arg1;
    *(s16 *)REG_BLDCNT_ADDR = 0;
    *(s16 *)REG_DB_BLDCNT_ADDR = 0;
    GF_CreateVramTransferManager(0x20U, arg2);
    GfGfx_SetBanks(&ov49_022697CC);
    gSystem.screensFlipped = 0;
    GfGfx_SwapDisplay();
    BG_SetMaskColor(0U, 0U);
    SetBothScreensModesAndDisable(&ov49_02269724);
    *(BgConfig **)arg0 = BgConfig_Alloc(arg2);
    var_r7 = &ov49_0226981C;
    var_r4 = &ov49_02269734;
    sp18 = 0;
    do {
        InitBgFromTemplate(*(BgConfig **)arg0, (u8)*var_r4, var_r7, 0U);
        BG_ClearCharDataRange((u8)*var_r4, 0x20U, 0U, arg2);
        BgClearTilemapBufferAndCommit(*(BgConfig **)arg0, (u8)*var_r4);
        var_r7 = (void *)((u8 *)var_r7 + (0x1C));
        var_r4 = (s32 *)((u8 *)var_r4 + 4);
        temp_r0 = sp18 + 1;
        sp18 = temp_r0;
    } while (temp_r0 < 4);
    temp_r4 = (u8)Options_GetFrame(Save_PlayerData_GetOptionsAddr(sp14));
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_5_OFFSET, arg2);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_4_OFFSET, arg2);
    LoadUserFrameGfx1(*(BgConfig **)arg0, GF_BG_LYR_SUB_0_F, 0x55U, 3U, 0U, arg2);
    LoadUserFrameGfx2(*(BgConfig **)arg0, GF_BG_LYR_SUB_0_F, 1U, 1U, (u8)temp_r4, arg2);
    LoadMapSignpostFrameAndGraphic(*(BgConfig **)arg0, 1U, 0x1FU, 2U, 3U, 0U, arg2);
    GfGfxLoader_GXLoadPal(NARC_a_2_0_7, 0x5A, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_2_OFFSET, 0x20U, arg2);
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 0x7E, 0, 0x1F, 0, 0x7E, 0, 0x1F, arg2);
    ObjCharTransfer_InitEx(&ov49_02269744, GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_64K);
    ObjPlttTransfer_Init(0x18, arg2);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
    G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_32K);
    G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);
    sub_0203A880();
    ((UnkStruct_ov70_0225E4EC *)arg0)->unk_04 = G2dRenderer_Init(0x18, &((UnkStruct_ov70_0225E4EC *)arg0)->unk_08, arg2);
    G2dRenderer_SetSubSurfaceCoords(&((UnkStruct_ov70_0225E4EC *)arg0)->unk_08, 0, FX32_CONST(256));
    var_r7_2 = GF_GFX_RES_TYPE_CHAR;
    var_r4_2 = (struct SpriteResourceCollection **)arg0;
    do {
        *(GF_2DGfxResMan **)((u8 *)var_r4_2 + offsetof(UnkStruct_ov70_0225E4EC, unk_194)) = Create2DGfxResObjMan(0x18, var_r7_2, arg2);
        var_r4_2 += 1;
        var_r7_2 += 1;
    } while (var_r7_2 < 4);
    ((UnkStruct_ov70_0225E4EC *)arg0)->unk_1A4 = sub_02020654(0x18, arg2);
    GfGfx_EngineATogglePlanes(0x10U, 1U);
    GfGfx_EngineBTogglePlanes(0x10U, 1U);
    *((GF3DVramMan **)&((UnkStruct_ov70_0225E4EC *)arg0)->unk_1A8) = GF_3DVramMan_Create(arg2, 0, 2, 0, 4, ov49_0225A854);
}
