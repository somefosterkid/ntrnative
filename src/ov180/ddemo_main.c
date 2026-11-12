// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov265/ddemo_main.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/arc.h"
#include "constants/pokemon.h"
#include "constants/sound.h"
#include "constants/species.h"
#include "constants/text_banks.h"
#include "demo/ddemo_main.h"
#include "demo/dendou_demo.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/calctool.h"
#include "gfl/double3Ddisp.h"
#include "gfl/g3d.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/particle.h"
#include "gfl/sound.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/os.h"
#include "pml/poke_party.h"
#include "system/gf_font.h"
#include "system/mcss.h"
#include "system/wordset.h"

// The Hall of Fame entry demo's set-up: the display, the texts, the sound, the team, the 3D of both scenes, the
// particles of the first and the Pokémon sprite of the second (overlay 265). The file's name is a guess, after the
// Hall of Fame PC's dpc_main.c

static void func_ov180_021b6194(TCB *tcb, void *data);
static void func_ov180_021b61b4(TCB *tcb, void *data);
static void func_ov180_021b65fc(void);
static void func_ov180_021b6844(DDemoWork *wk, ParticleSystem *ptc, BOOL sub);
static void func_ov180_021b69e8(uintptr_t param, fx32 frame);

// The first scene's banks, with 3D on both screens, and the second's
// pokebw2's sDDemoVramConfig1st
static const BGSysVRAMConfig data_ov180_021b8060 = {
    GX_VRAM_BG_32_FG,
    GX_VRAM_BGEXTPLTT_NONE,
    GX_VRAM_SUB_BG_NONE,
    GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B,
    GX_VRAM_OBJEXTPLTT_NONE,
    GX_VRAM_SUB_OBJ_NONE,
    GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_0_A,
    GX_VRAM_E,
    GX_OBJVRAMMODE_CHAR_1D_128K,
    GX_OBJVRAMMODE_CHAR_1D_128K,
};

// pokebw2's sDDemoVramConfig2nd
static const BGSysVRAMConfig data_ov180_021b8090 = {
    GX_VRAM_BG_128_D,
    GX_VRAM_BGEXTPLTT_23_G,
    GX_VRAM_SUB_BG_128_C,
    GX_VRAM_SUB_BGEXTPLTT_0123_H,
    GX_VRAM_OBJ_128_B,
    GX_VRAM_OBJEXTPLTT_NONE,
    GX_VRAM_SUB_OBJ_16_I,
    GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_0_A,
    GX_VRAM_E,
    GX_OBJVRAMMODE_CHAR_1D_128K,
    GX_OBJVRAMMODE_CHAR_1D_32K,
};

// pokebw2's sDDemoPreloadSE
static const u32 data_ov180_021b8018[] = { SEQ_SE_ROTATION_B };

// pokebw2's sDDemoLights
static const Light data_ov180_021b8040[] = {
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
    { { 0, -FX16_ONE, 0 }, GX_RGB(16, 16, 16) },
};

// The camera of the second scene's Pokémon
// pokebw2's sDDemoCameraUp
static const VecFx32 data_ov180_021b8028 = { 0, FX32_ONE, 0 };
// pokebw2's sDDemoCameraPos
static const VecFx32 data_ov180_021b8034 = { 0, FX32_CONST(30), FX32_CONST(3000) };
// pokebw2's sDDemoCameraTarget
static const VecFx32 data_ov180_021b801c = { 0, FX32_CONST(30), 0 };

// The particle file of each type
// pokebw2's sDDemoTypeParticles
static const u32 data_ov180_021b8108[] = {
#include "asset/ddemo_main_data_ov180_021b8108.inc"
};

// pokebw2's DDemoMain_InitVBlank1st
void func_ov180_021b6158(DDemoWork *wk) {
    wk->vblankTask = GFL_VBlankTCBAdd(func_ov180_021b6194, wk, 0);
}

// pokebw2's DDemoMain_InitVBlank2nd
void func_ov180_021b6170(DDemoWork *wk) {
    wk->vblankTask = GFL_VBlankTCBAdd(func_ov180_021b61b4, wk, 0);
}

// pokebw2's DDemoMain_ExitVBlank
void func_ov180_021b6188(DDemoWork *wk) {
    GFL_TCBRemove(wk->vblankTask);
}

// pokebw2's DDemoMain_VBlank1st
static void func_ov180_021b6194(TCB *tcb, void *data) {
    func_0204a648();
    GFL_G3DDual3DExecVBlank();
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

// pokebw2's DDemoMain_VBlank2nd
static void func_ov180_021b61b4(TCB *tcb, void *data) {
    GFL_BGSysUpdate();
    func_0204a648();
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

// pokebw2's DDemoMain_InitVram
void func_ov180_021b61d4(u32 scene) {
    GFL_BGSysInitVRAM(GX_VRAM_LCDC_NONE);
    if (scene == 0) {
        GFL_BGSysSetVRAMBanks(&data_ov180_021b8060);
    } else {
        GFL_BGSysSetVRAMBanks(&data_ov180_021b8090);
    }
}

// pokebw2's DDemoMain_GetVramConfig
const BGSysVRAMConfig *func_ov180_021b61fc(u32 scene) {
    if (scene == 0) {
        return &data_ov180_021b8060;
    }
    return &data_ov180_021b8090;
}

// pokebw2's DDemoMain_InitBg
void func_ov180_021b6210(void) {
    GFL_BGSysCreate(HEAPID_DENDOU_DEMO);
}

// pokebw2's DDemoMain_ExitBg
void func_ov180_021b621c(void) {
    GFL_BGSysFree();
}

// pokebw2's DDemoMain_InitBgMode
void func_ov180_021b6224(void) {
    BGSysLCDConfig config = { GX_DISPMODE_GRAPHICS, GX_BGMODE_3, GX_BGMODE_3, GX_BG0_AS_3D };

    GFL_BGSysSetLCDConfig(&config);
    GFL_BGSysResetStdPalette(0, GX_RGB(0, 0, 0));
    GFL_BGSysResetStdPalette(BGSYS_BG_SUB, GX_RGB(0, 0, 0));
}

// The second scene's backgrounds: one 1024x1024 affine picture across both screens
// pokebw2's DDemoMain_InitBg2nd
void func_ov180_021b6254(void) {
    BGSetup setup = {
        0,
        0,
        0x2000,
        0,
        BGRES_512x512,
        GX_BG_COLORMODE_256,
        GX_BG_SCRBASE_0x2000,
        GX_BG_CHARBASE_0x08000,
        0x10000,
        GX_BG_EXTPLTT_23,
        1,
        GX_BG_AREAOVER_XLU,
        FALSE,
    };
    MtxFx22 mtx;

    GFL_BGSysCreateBG(3, &setup, BGMODE_EXTENDED);
    GFL_BGSysCreateBG(BGSYS_BG_SUB + 3, &setup, BGMODE_EXTENDED);
    MAT2_SetScaleRot(&mtx, 0, FX32_ONE, FX32_ONE, 0);
    G2_SetBG3Affine(&mtx, 0, 0, 0, 0);
    G2S_SetBG3Affine(&mtx, 0, 0, 0, 0);
    GFL_BGSysMoveBGReq(3, BG_MOVE_SET_X, 160);
    GFL_BGSysMoveBGReq(3, BG_MOVE_SET_Y, 112);
    GFL_BGSysMoveBGReq(BGSYS_BG_SUB + 3, BG_MOVE_SET_X, 160);
    GFL_BGSysMoveBGReq(BGSYS_BG_SUB + 3, BG_MOVE_SET_Y, -152);
    GFL_BGSysAdjustBGOriginReq(3, 9, 160);
    GFL_BGSysAdjustBGOriginReq(3, 12, 160);
    GFL_BGSysAdjustBGOriginReq(BGSYS_BG_SUB + 3, 9, 160);
    GFL_BGSysAdjustBGOriginReq(BGSYS_BG_SUB + 3, 12, 160);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG3, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG3, TRUE);
    GFL_BGSysResetStdPalette(0, GX_RGB(0, 0, 0));
    GFL_BGSysResetStdPalette(BGSYS_BG_SUB, GX_RGB(0, 0, 0));
}

// pokebw2's DDemoMain_ExitBg2nd
void func_ov180_021b6340(void) {
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG3, FALSE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG3, FALSE);
    GFL_BGSysReleaseBG(BGSYS_BG_SUB + 3);
    GFL_BGSysReleaseBG(3);
}

// pokebw2's DDemoMain_LoadBg
void func_ov180_021b6360(void) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_DENDOU_DEMO, HEAPID_DENDOU_DEMO);

    GFL_G2DIOLoadArcNCLRDefault(arc, NARC_DDEMO_BG_NCLR, PALTYPE_MAIN_BG_EX, 0x6000, 0x20, HEAPID_DENDOU_DEMO);
    GFL_G2DIOLoadArcNCLRDefault(arc, NARC_DDEMO_BG_NCLR, PALTYPE_SUB_BG_EX, 0x6000, 0x20, HEAPID_DENDOU_DEMO);
    GFL_BGSysLoadArcNCGRStatic(arc, NARC_DDEMO_BG_NCGR, 3, 0, 0, FALSE, HEAPID_DENDOU_DEMO);
    GFL_BGSysLoadArcNCGRStatic(arc, NARC_DDEMO_BG_NCGR, BGSYS_BG_SUB + 3, 0, 0, FALSE, HEAPID_DENDOU_DEMO);
    loadBGScrToVramByFileNoReserveNegAlign(arc, NARC_DDEMO_BG_NSCR, 3, 0, 0, FALSE, HEAPID_DENDOU_DEMO);
    loadBGScrToVramByFileNoReserveNegAlign(arc, NARC_DDEMO_BG_NSCR, BGSYS_BG_SUB + 3, 0, 0, FALSE, HEAPID_DENDOU_DEMO);
    GFL_ArcToolFree(arc);
}

// Blends the 3D of the first scene over the actors and the backdrop
// pokebw2's DDemoMain_SetBlendAlpha
void func_ov180_021b63e8(void) {
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG0,
                     GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_BD,
                     6, 10);
    G2S_SetBlendAlpha(GX_BLEND_PLANEMASK_BG0,
                      GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_BD,
                      6, 10);
}

// pokebw2's DDemoMain_InitMsg
void func_ov180_021b6418(DDemoWork *wk) {
    wk->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_BTL_MAIN_2, HEAPID_DENDOU_DEMO);
    wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, HEAPID_DENDOU_DEMO);
    wk->fontSmall = GFL_FontCreate(ARCID_FONT, 3, 0, FALSE, HEAPID_DENDOU_DEMO);
    wk->wordSet = GFL_WordSetSystemCreateDefault(HEAPID_DENDOU_DEMO);
    wk->strbuf = GFL_StrBufCreate(0x400, HEAPID_DENDOU_DEMO);
}

// pokebw2's DDemoMain_ExitMsg
void func_ov180_021b6474(DDemoWork *wk) {
    GFL_StrBufFree(wk->strbuf);
    GFL_WordSetSystemFree(wk->wordSet);
    GFL_FontFree(wk->fontSmall);
    GFL_FontFree(wk->font);
    GFL_MsgDataFree(wk->msgData);
}

// pokebw2's DDemoMain_InitSound
void func_ov180_021b64ac(DDemoWork *wk) {
    wk->seHandle = func_02005a98(data_ov180_021b8018, NELEMS(data_ov180_021b8018));
    GFL_SndBGMPlay(SEQ_BGM_E_DENDOUIRI, 0xffff);
}

// pokebw2's DDemoMain_ExitSound
void func_ov180_021b64d0(DDemoWork *wk) {
    func_02005b04(wk->seHandle);
}

// Lists the team's Pokémon that are not Eggs
// pokebw2's DDemoMain_GetPokeMax
void func_ov180_021b64dc(DDemoWork *wk) {
    u32 i;

    wk->monCount = 0;
    for (i = 0; i < PokeParty_GetPkmCount(wk->param->party); i++) {
        PartyPkm *pkm = PokeParty_GetPkm(wk->param->party, i);
        BOOL wasEncrypted = PokeParty_DecryptPkm(pkm);

        if (PokeParty_GetParam(pkm, PKM_PARAM_IS_EGG, NULL) == FALSE) {
            wk->mons[wk->monCount] = pkm;
            wk->monCount++;
        }
        PokeParty_EncryptPkm(pkm, wasEncrypted);
    }
}

// pokebw2's DDemoMain_GetPokeData
void func_ov180_021b654c(DDemoWork *wk) {
    PartyPkm *pkm = wk->mons[wk->monIndex];
    BOOL wasEncrypted = PokeParty_DecryptPkm(pkm);

    wk->species = PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL);
    wk->type = PokeParty_GetParam(pkm, PKM_PARAM_TYPE1, NULL);
    wk->form = PokeParty_GetParam(pkm, PKM_PARAM_FORM, NULL);
    PokeParty_EncryptPkm(pkm, wasEncrypted);
}

// pokebw2's DDemoMain_LoadPokeVoice
void func_ov180_021b65a0(DDemoWork *wk) {
    if (wk->species == SPECIES_CHATOT) {
        PokeVoiceChatterInfo chatter;

        PokeVoice_CreateChatterInfo(&chatter);
        wk->voice = PokeVoice_Load(wk->species, wk->form, 64, FALSE, 0, 0, FALSE, &chatter);
    } else {
        wk->voice = PokeVoice_Load(wk->species, wk->form, 64, FALSE, 0, 0, FALSE, NULL);
    }
}

// The 3D system calls this once it is set up
// pokebw2's DDemoMain_Setup3D
static void func_ov180_021b65fc(void) {
    u32 i;

    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG0, TRUE);
    G2_SetBG0Priority(0);
    G3X_SetShading(GX_SHADING_HIGHLIGHT);
    G3X_AntiAlias(FALSE);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(TRUE);
    G3X_EdgeMarking(FALSE);
    G3X_SetFog(FALSE, 0, 0, 0);
    G3X_SetClearColor(GX_RGB(0, 0, 0), 0, 0x7fff, 63, FALSE);
    G3_ViewPort(0, 0, 255, 191);
    for (i = 0; i < NELEMS(data_ov180_021b8040); i++) {
        GFL_G3DSysLightSet(i, &data_ov180_021b8040[i]);
    }
    GFL_G3DSysSetSwapBufferParams(GX_SORTMODE_MANUAL, GX_BUFFERMODE_Z);
}

// pokebw2's DDemoMain_Init3D
void func_ov180_021b66a0(DDemoWork *wk) {
    GFL_G3DSysCreate(FALSE, 1, FALSE, 1, 0x1000, HEAPID_DENDOU_DEMO, func_ov180_021b65fc);
    wk->g3dMgr = GFL_G3DMgrCreate(32, 32, HEAPID_DENDOU_DEMO);
}

// pokebw2's DDemoMain_Exit3D
void func_ov180_021b66d4(DDemoWork *wk) {
    GFL_G3DMgrFree(wk->g3dMgr);
    GFL_G3DSysFree();
}

// Draws the second scene's Pokémon
// pokebw2's DDemoMain_Main3D
void func_ov180_021b66e4(DDemoWork *wk) {
    GFL_G3DSysReset();
    GFL_G3DSysMtxViewFlush();
    func_02014c08(wk->mcssSys);
    func_02014d2c(wk->mcssSys);
    GFL_G3DSysReqSwapBuffers();
}

// Draws the first scene's particles for the screen whose turn it is
// pokebw2's DDemoMain_MainDouble3D
void func_ov180_021b6704(DDemoWork *wk) {
    GFL_G3DSysReset();
    GFL_G3DSysMtxViewFlush();
    if (GFL_G3DDual3DGetRenderDisp() == TRUE) {
        func_ov180_021b6844(wk, wk->ptc[DDEMO_PTC_TYPE], TRUE);
        func_ov180_021b6844(wk, wk->ptc[DDEMO_PTC_NAME], TRUE);
    } else {
        func_ov180_021b6844(wk, wk->ptc[DDEMO_PTC_TYPE], FALSE);
        func_ov180_021b6844(wk, wk->ptc[DDEMO_PTC_NAME], FALSE);
    }
    func_02050be8();
    GFL_G3DSysReqSwapBuffers();
    GFL_G3DDual3DRaiseNeedRenderFlag();
}

// pokebw2's DDemoMain_InitDouble3D
void func_ov180_021b6750(void) {
    GFL_G3DDual3DInit(HEAPID_DENDOU_DEMO);
}

// pokebw2's DDemoMain_ExitDouble3D
void func_ov180_021b675c(void) {
    GFL_G3DDual3DFree();
}

// pokebw2's DDemoMain_InitParticle
void func_ov180_021b6764(void) {
    func_02050b80(HEAPID_DENDOU_DEMO);
}

// pokebw2's DDemoMain_ExitParticle
void func_ov180_021b6770(void) {
    func_02050ed4();
}

// pokebw2's DDemoMain_CreateTypeParticle
void func_ov180_021b6778(DDemoWork *wk) {
    wk->ptc[DDEMO_PTC_TYPE] = func_02050c04(wk->ptcWork[DDEMO_PTC_TYPE], DDEMO_PTC_WORK_SIZE, TRUE, HEAPID_DENDOU_DEMO);
    func_02051230(wk->ptc[DDEMO_PTC_TYPE],
                  func_0205121c(ARCID_DENDOU_DEMO, data_ov180_021b8108[wk->type], HEAPID_DENDOU_DEMO), TRUE, NULL);
}

// pokebw2's DDemoMain_DeleteTypeParticle
void func_ov180_021b67b4(DDemoWork *wk) {
    func_02050db8(wk->ptc[DDEMO_PTC_TYPE]);
    wk->ptc[DDEMO_PTC_TYPE] = NULL;
}

// pokebw2's DDemoMain_SetTypeParticle
void func_ov180_021b67c4(DDemoWork *wk, u32 id) {
    VecFx32 pos = { 0, 0, 0x19a };

    func_020515b4(wk->ptc[DDEMO_PTC_TYPE], id, &pos);
}

// pokebw2's DDemoMain_CreateNameParticle
void func_ov180_021b67ec(DDemoWork *wk) {
    wk->ptc[DDEMO_PTC_NAME] = func_02050c04(wk->ptcWork[DDEMO_PTC_NAME], DDEMO_PTC_WORK_SIZE, TRUE, HEAPID_DENDOU_DEMO);
    func_02051230(wk->ptc[DDEMO_PTC_NAME], func_0205121c(ARCID_DENDOU_DEMO, NARC_DDEMO_NAME_SPA, HEAPID_DENDOU_DEMO),
                  TRUE, NULL);
}

// pokebw2's DDemoMain_DeleteNameParticle
void func_ov180_021b681c(DDemoWork *wk) {
    func_02050db8(wk->ptc[DDEMO_PTC_NAME]);
    wk->ptc[DDEMO_PTC_NAME] = NULL;
}

// pokebw2's DDemoMain_SetNameParticle
void func_ov180_021b682c(DDemoWork *wk, u32 id) {
    VecFx32 pos = { 0, 0, 0 };

    func_020515b4(wk->ptc[DDEMO_PTC_NAME], id, &pos);
}

// Gives a particle system the half of the view that a screen shows: the particles are laid out over both screens
// pokebw2's DDemoMain_SetParticleCamera
static void func_ov180_021b6844(DDemoWork *wk, ParticleSystem *ptc, BOOL sub) {
    G3DCameraProjection projection;

    if (ptc == NULL) {
        return;
    }
    if (sub == TRUE) {
        projection.param1 = FX32_CONST(4);
        projection.param2 = -FX32_CONST(4);
    } else {
        projection.param1 = FX32_CONST(14);
        projection.param2 = FX32_CONST(6);
    }
    projection.param3 = -0x5548;
    projection.param4 = 0x5548;
    projection.type = G3DCAM_PROJECTION_ORTHO;
    projection.far = FX32_CONST(1024);
    projection.near = 0;
    projection.ndcRangeOverride = 0;
    func_02051758(ptc);
    func_02051648(ptc, &projection, 0x2000, NULL, NULL, NULL, HEAPID_DENDOU_DEMO);
}

// pokebw2's DDemoMain_InitMcss
void func_ov180_021b68a8(DDemoWork *wk) {
    wk->mcssSys = func_02014b08(1, HEAPID_DENDOU_DEMO);
    func_02016020(wk->mcssSys, 0);
    func_02015bf0(wk->mcssSys);
    wk->camera = GFL_G3DCameraCreate(G3DCAM_PROJECTION_ORTHO, FX32_CONST(96), -FX32_CONST(96), -FX32_CONST(128),
                                     FX32_CONST(128), FX32_ONE, FX32_CONST(1024), 0, &data_ov180_021b8034,
                                     &data_ov180_021b801c, &data_ov180_021b8028, HEAPID_DENDOU_DEMO);
    GFL_G3DCameraFlush(wk->camera);
}

// pokebw2's DDemoMain_ExitMcss
void func_ov180_021b6914(DDemoWork *wk) {
    func_ov180_021b6984(wk);
    GFL_G3DCameraFree(wk->camera);
    func_02014ba8(wk->mcssSys);
}

// Adds the sprite of the Pokémon on show at the screen's right side, held still
// pokebw2's DDemoMain_AddMcss
void func_ov180_021b692c(DDemoWork *wk) {
    MCSSLoadInfo info;
    VecFx32 scale = { FX32_CONST(16), FX32_CONST(16), FX32_ONE };

    func_02017100(wk->mons[wk->monIndex], &info, 0);
    wk->mcss = func_020159c8(wk->mcssSys, FX32_CONST(176), -FX32_CONST(48), 0, &info);
    func_02015cac(wk->mcss, &scale);
    func_02015e04(wk->mcss);
}

// pokebw2's DDemoMain_DelMcss
void func_ov180_021b6984(DDemoWork *wk) {
    if (wk->mcss != NULL) {
        func_02015ba4(wk->mcssSys, wk->mcss);
        wk->mcss = NULL;
    }
}

// pokebw2's DDemoMain_MoveMcss
void func_ov180_021b699c(DDemoWork *wk, s32 move) {
    VecFx32 pos;

    func_02015c30(wk->mcss, &pos);
    pos.x += FX32_CONST(move);
    func_02015c48(wk->mcss, &pos);
}

// pokebw2's DDemoMain_McssCallBack
static void func_ov180_021b69e8(uintptr_t param, fx32 frame) {
    DDemoWork *wk = (DDemoWork *)param;

    func_02015e04(wk->mcss);
    wk->mcssAnmEnd = TRUE;
}

// Has the Pokémon's animation stop at its end, which sets mcssAnmEnd
// pokebw2's DDemoMain_SetMcssCallBack
void func_ov180_021b69f8(DDemoWork *wk) {
    wk->mcssAnmEnd = FALSE;
    func_02015f2c(wk->mcss, (uintptr_t)wk, func_ov180_021b69e8, 0);
}
