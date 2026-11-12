// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov265/ddemo_obj.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/arc.h"
#include "constants/pokemon.h"
#include "constants/trainer_classes.h"
#include "demo/ddemo_main.h"
#include "demo/ddemo_obj.h"
#include "demo/dendou_demo.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bmp.h"
#include "gfl/clact.h"
#include "gfl/double3Ddisp.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/random.h"
#include "gfl/str.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/gx.h"
#include "pml/poke_graphic.h"
#include "pml/poke_party.h"
#include "save/player_info.h"
#include "save/playtime.h"
#include "system/bmp_oam.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/wordset.h"
#include "text/system/btl_main_2.h"

// The Hall of Fame entry demo's actors, their resources and the texts drawn on sprites (overlay 265). The file's name
// is a guess, after the Hall of Fame PC's dpc_obj.c

// An actor: its setup, its resources (DDEMO_CHR_*, DDEMO_PAL_* and DDEMO_CEL_*) and its surface
typedef struct {
    ClActorSetup setup;
    u32 chr;
    u32 pal;
    u32 cel;
    u16 unk14;
    u16 surface;
} DDemoObjSetup;

#define RES_NONE 0xffffffff

// The texts' color
#define FONT_COLOR PRINT_COLOR(15, 2, 0)

static void func_ov180_021b73b0(DDemoWork *wk);
static void func_ov180_021b74c8(DDemoWork *wk);
static void func_ov180_021b7510(DDemoWork *wk, u32 id);
static void func_ov180_021b7530(DDemoWork *wk, u32 id);
static void func_ov180_021b7550(DDemoWork *wk, u32 id);
static void func_ov180_021b756c(DDemoWork *wk);
static ClActor *func_ov180_021b75a4(DDemoWork *wk, const DDemoObjSetup *setup);
static void func_ov180_021b75e8(DDemoWork *wk, u32 id);
static void func_ov180_021b7604(DDemoWork *wk);
static void func_ov180_021b7798(DDemoWork *wk, u32 id);
static void func_ov180_021b77c4(DDemoWork *wk);
static void func_ov180_021b77dc(DDemoWork *wk, u32 id, Font *font, StrBuf *str, int x, int y);
static void func_ov180_021b7814(DDemoWork *wk);
static void func_ov180_021b79f4(DDemoWork *wk);
static void func_ov180_021b7c78(DDemoWork *wk);

// The palette file of each type, for the plates
// pokebw2's sDDemoTypePalettes
static const u32 data_ov180_021b82d4[] = {
#include "asset/ddemo_obj_data_ov180_021b82d4.inc"
};

// The actors. The order they are declared in gives the order the game has them in
// pokebw2's sDDemoObjPokeFront
static const DDemoObjSetup data_ov180_021b8174 = {
    { -48, 144, 0, 10, 0 }, DDEMO_CHR_POKE_FRONT, DDEMO_PAL_POKE_FRONT, DDEMO_CEL_POKE_FRONT, 0, CLACT_SURFACE_MAIN,
};
// pokebw2's sDDemoObjPokeBack
static const DDemoObjSetup data_ov180_021b818c = {
    { 304, 144, 0, 10, 0 }, DDEMO_CHR_POKE_BACK, DDEMO_PAL_POKE_BACK, DDEMO_CEL_POKE_BACK, 0, CLACT_SURFACE_MAIN,
};
// pokebw2's sDDemoObjTrainerSub
static const DDemoObjSetup data_ov180_021b8264 = {
    { 88, -48, 0, 10, 1 }, DDEMO_CHR_TRAINER_SUB, DDEMO_PAL_TRAINER_SUB, DDEMO_CEL_TRAINER_SUB, 0, CLACT_SURFACE_SUB,
};
// pokebw2's sDDemoObjTrainerMain
static const DDemoObjSetup data_ov180_021b824c = {
    { 88, -304, 0, 10, 1 }, DDEMO_CHR_TRAINER_MAIN, DDEMO_PAL_TRAINER_MAIN, DDEMO_CEL_TRAINER_MAIN, 0,
    CLACT_SURFACE_MAIN,
};
// pokebw2's sDDemoObjConfettiSub
static const DDemoObjSetup data_ov180_021b8234 = {
    { 0, 0, 4, 8, 0 }, DDEMO_CHR_OBJ_SUB, DDEMO_PAL_OBJ_SUB, DDEMO_CEL_OBJ_SUB, 0, CLACT_SURFACE_SUB,
};
// pokebw2's sDDemoObjConfettiMain
static const DDemoObjSetup data_ov180_021b821c = {
    { 0, 0, 4, 8, 0 }, DDEMO_CHR_OBJ, DDEMO_PAL_OBJ, DDEMO_CEL_OBJ, 0, CLACT_SURFACE_MAIN,
};
// pokebw2's sDDemoObjPlateUpper
static const DDemoObjSetup data_ov180_021b8204 = {
    { 128, 24, 2, 10, 0 }, DDEMO_CHR_OBJ, DDEMO_PAL_OBJ, DDEMO_CEL_OBJ, 0, CLACT_SURFACE_MAIN,
};
// pokebw2's sDDemoObjPlateLower
static const DDemoObjSetup data_ov180_021b81ec = {
    { 128, 168, 2, 10, 0 }, DDEMO_CHR_OBJ, DDEMO_PAL_OBJ, DDEMO_CEL_OBJ, 0, CLACT_SURFACE_MAIN,
};
// pokebw2's sDDemoObjPlateName
static const DDemoObjSetup data_ov180_021b81d4 = {
    { 264, 168, 1, 10, 0 }, DDEMO_CHR_OBJ, DDEMO_PAL_OBJ, DDEMO_CEL_OBJ, 0, CLACT_SURFACE_MAIN,
};
// pokebw2's sDDemoObjPlateTitle
static const DDemoObjSetup data_ov180_021b81bc = {
    { -8, 24, 0, 10, 0 }, DDEMO_CHR_OBJ, DDEMO_PAL_OBJ, DDEMO_CEL_OBJ, 0, CLACT_SURFACE_MAIN,
};
// pokebw2's sDDemoObjFlash
static const DDemoObjSetup data_ov180_021b81a4 = {
    { 48, 144, 6, 8, 0 }, DDEMO_CHR_OBJ, DDEMO_PAL_OBJ, DDEMO_CEL_OBJ, 0, CLACT_SURFACE_MAIN,
};

// Sets up the actors, on the main screen alone for the first scene
// pokebw2's DDemoObj_Init
void func_ov180_021b7318(DDemoWork *wk, u32 scene) {
    if (scene == 0) {
        ClActSysSetup setup = { 0, 0, 0, 512, 4, 124, 0, 0, 0, 222, 7, 222, 0, 16, 0 };

        ClActSys_Create(&setup, func_ov180_021b61fc(scene), HEAPID_DENDOU_DEMO);
    } else {
        ClActSysSetup setup = { 0, 0, 0, 512, 4, 124, 4, 124, 0, 222, 7, 222, 0, 16, 16 };

        ClActSys_Create(&setup, func_ov180_021b61fc(scene), HEAPID_DENDOU_DEMO);
    }
    wk->clUnit = func_0204b100(DDEMOOBJ_COUNT + 216, 0, HEAPID_DENDOU_DEMO);
    wk->bmpOam = func_02025538(HEAPID_DENDOU_DEMO, wk->clUnit);
    func_ov180_021b74c8(wk);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

// pokebw2's DDemoObj_Exit
void func_ov180_021b7390(DDemoWork *wk) {
    func_02025588(wk->bmpOam);
    func_0204b1cc(wk->clUnit);
    func_0204a5ac();
}

// pokebw2's DDemoObj_Animate
static void func_ov180_021b73b0(DDemoWork *wk) {
    u32 i;

    for (i = 0; i < DDEMOOBJ_COUNT; i++) {
        if (wk->clact[i] != NULL) {
            if (func_0204b944(wk->clact[i]) != TRUE) {
                func_0204b8d4(wk->clact[i], FX32_ONE);
            }
        }
    }
    func_0204a600();
}

// pokebw2's DDemoObj_SetPos
void func_ov180_021b73e4(DDemoWork *wk, u32 id, s16 x, s16 y) {
    ClActorPos pos;

    pos.x = x;
    pos.y = y;
    func_0204b404(wk->clact[id], &pos, CLACT_SURFACE_MAIN);
}

// pokebw2's DDemoObj_GetPos
void func_ov180_021b7408(DDemoWork *wk, u32 id, s16 *x, s16 *y) {
    ClActorPos pos;

    func_0204b454(wk->clact[id], &pos, CLACT_SURFACE_MAIN);
    *x = pos.x;
    *y = pos.y;
}

// pokebw2's DDemoObj_Move
void func_ov180_021b7438(DDemoWork *wk, u32 id, s16 x, s16 y) {
    s16 posX, posY;

    func_ov180_021b7408(wk, id, &posX, &posY);
    posX += x;
    posY += y;
    func_ov180_021b73e4(wk, id, posX, posY);
}

// pokebw2's DDemoObj_SetVisible
void func_ov180_021b746c(DDemoWork *wk, u32 id, BOOL visible) {
    if (wk->clact[id] != NULL) {
        func_0204b3dc(wk->clact[id], visible);
    }
}

// pokebw2's DDemoObj_SetAnime
void func_ov180_021b7488(DDemoWork *wk, u32 id, u32 sequence) {
    func_0204b8c4(wk->clact[id], 0);
    func_0204b878(wk->clact[id], sequence);
    func_0204b92c(wk->clact[id], TRUE);
}

// pokebw2's DDemoObj_IsAnimating
BOOL func_ov180_021b74b4(DDemoWork *wk, u32 id) {
    return func_0204b98c(wk->clact[id]);
}

// pokebw2's DDemoObj_InitResources
static void func_ov180_021b74c8(DDemoWork *wk) {
    u32 i;

    for (i = 0; i < DDEMO_CHR_COUNT; i++) {
        wk->chrRes[i] = RES_NONE;
    }
    for (i = 0; i < DDEMO_PAL_COUNT; i++) {
        wk->palRes[i] = RES_NONE;
    }
    for (i = 0; i < DDEMO_CEL_COUNT; i++) {
        wk->celRes[i] = RES_NONE;
    }
}

// pokebw2's DDemoObj_FreeChars
static void func_ov180_021b7510(DDemoWork *wk, u32 id) {
    if (wk->chrRes[id] != RES_NONE) {
        func_0204a8d4(wk->chrRes[id]);
        wk->chrRes[id] = RES_NONE;
    }
}

// pokebw2's DDemoObj_FreePalette
static void func_ov180_021b7530(DDemoWork *wk, u32 id) {
    if (wk->palRes[id] != RES_NONE) {
        func_0204ada4(wk->palRes[id]);
        wk->palRes[id] = RES_NONE;
    }
}

// pokebw2's DDemoObj_FreeCellAnims
static void func_ov180_021b7550(DDemoWork *wk, u32 id) {
    if (wk->celRes[id] != RES_NONE) {
        func_0204afd8(wk->celRes[id]);
        wk->celRes[id] = RES_NONE;
    }
}

// pokebw2's DDemoObj_FreeResources
static void func_ov180_021b756c(DDemoWork *wk) {
    u32 i;

    for (i = 0; i < DDEMO_CHR_COUNT; i++) {
        func_ov180_021b7510(wk, i);
    }
    for (i = 0; i < DDEMO_PAL_COUNT; i++) {
        func_ov180_021b7530(wk, i);
    }
    for (i = 0; i < DDEMO_CEL_COUNT; i++) {
        func_ov180_021b7550(wk, i);
    }
}

// pokebw2's DDemoObj_Add
static ClActor *func_ov180_021b75a4(DDemoWork *wk, const DDemoObjSetup *setup) {
    return func_0204b294(wk->clUnit, wk->chrRes[setup->chr], wk->palRes[setup->pal], wk->celRes[setup->cel],
                         &setup->setup, setup->surface, HEAPID_DENDOU_DEMO);
}

// pokebw2's DDemoObj_Delete
static void func_ov180_021b75e8(DDemoWork *wk, u32 id) {
    if (wk->clact[id] != NULL) {
        func_0204b3b4(wk->clact[id]);
        wk->clact[id] = NULL;
    }
}

// pokebw2's DDemoObj_DeleteAll
static void func_ov180_021b7604(DDemoWork *wk) {
    u32 i;

    for (i = 0; i < DDEMOOBJ_COUNT; i++) {
        func_ov180_021b75e8(wk, i);
    }
}

// pokebw2's DDemoObj_AddPoke
void func_ov180_021b761c(DDemoWork *wk) {
    ArcTool *arc;
    BoxPkm *pkm;
    BOOL wasEncrypted;

    if (wk->clact[DDEMOOBJ_POKE_FRONT] != NULL) {
        func_ov180_021b75e8(wk, DDEMOOBJ_POKE_FRONT);
        func_ov180_021b7510(wk, DDEMO_CHR_POKE_FRONT);
        func_ov180_021b7530(wk, DDEMO_PAL_POKE_FRONT);
        func_ov180_021b7550(wk, DDEMO_CEL_POKE_FRONT);
    }
    if (wk->clact[DDEMOOBJ_POKE_BACK] != NULL) {
        func_ov180_021b75e8(wk, DDEMOOBJ_POKE_BACK);
        func_ov180_021b7510(wk, DDEMO_CHR_POKE_BACK);
        func_ov180_021b7530(wk, DDEMO_PAL_POKE_BACK);
        func_ov180_021b7550(wk, DDEMO_CEL_POKE_BACK);
    }
    arc = MakePokeGraArcHandle(HEAPID_DENDOU_DEMO);
    pkm = func_020185f0(wk->mons[wk->monIndex]);
    wasEncrypted = PML_PkmDecrypt(pkm);
    wk->chrRes[DDEMO_CHR_POKE_FRONT] =
        func_0202d934(arc, pkm, 0, CLACT_VRAM_MAIN, HEAPID_DENDOU_DEMO);
    wk->palRes[DDEMO_PAL_POKE_FRONT] =
        func_0202d8d0(arc, pkm, 0, CLACT_VRAM_MAIN, 0x60, HEAPID_DENDOU_DEMO);
    wk->celRes[DDEMO_CEL_POKE_FRONT] =
        func_0202d9a4(pkm, 0, 2, CLACT_VRAM_MAIN, HEAPID_DENDOU_DEMO);
    wk->chrRes[DDEMO_CHR_POKE_BACK] = func_0202d934(arc, pkm, 1, CLACT_VRAM_MAIN, HEAPID_DENDOU_DEMO);
    wk->palRes[DDEMO_PAL_POKE_BACK] =
        func_0202d8d0(arc, pkm, 1, CLACT_VRAM_MAIN, 0x80, HEAPID_DENDOU_DEMO);
    wk->celRes[DDEMO_CEL_POKE_BACK] =
        func_0202d9a4(pkm, 1, 2, CLACT_VRAM_MAIN, HEAPID_DENDOU_DEMO);
    PML_PkmReEncrypt(pkm, wasEncrypted);
    GFL_ArcToolFree(arc);
    wk->clact[DDEMOOBJ_POKE_FRONT] = func_ov180_021b75a4(wk, &data_ov180_021b8174);
    wk->clact[DDEMOOBJ_POKE_BACK] = func_ov180_021b75a4(wk, &data_ov180_021b818c);
}

// pokebw2's DDemoObj_LoadTypePalette
void func_ov180_021b7750(DDemoWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_DENDOU_DEMO, HEAPID_TAIL(HEAPID_DENDOU_DEMO));
    NNSG2dPaletteData *palette;
    void *file = GFL_G2DIOReadNCLRArc(arc, data_ov180_021b82d4[wk->type], &palette, HEAPID_DENDOU_DEMO);

    func_0204adec(wk->palRes[DDEMO_PAL_OBJ], palette, 1);
    GFL_HeapFree(file);
    GFL_ArcToolFree(arc);
}

// pokebw2's DDemoObj_DeleteFontOam
static void func_ov180_021b7798(DDemoWork *wk, u32 id) {
    if (wk->fontOam[id].oam != NULL) {
        func_0202570c(wk->fontOam[id].oam);
        GFL_BitmapFree(wk->fontOam[id].bmp);
        wk->fontOam[id].oam = NULL;
    }
}

// pokebw2's DDemoObj_DeleteFontOamAll
static void func_ov180_021b77c4(DDemoWork *wk) {
    u32 i;

    for (i = 0; i < DDEMO_FOAM_COUNT; i++) {
        func_ov180_021b7798(wk, i);
    }
}

// pokebw2's DDemoObj_PrintFontOam
static void func_ov180_021b77dc(DDemoWork *wk, u32 id, Font *font, StrBuf *str, int x, int y) {
    GFL_TextRendererDrawToBitmapEx(wk->fontOam[id].bmp, x, y, str, font, FONT_COLOR);
    func_020257d0(wk->fontOam[id].oam);
}

// The first scene's texts: the title on its plate, and the Pokémon's, which func_ov180_021b78e0 draws
// pokebw2's DDemoObj_InitFontOam1st
static void func_ov180_021b7814(DDemoWork *wk) {
    BmpOamActorSetup setup;
    DDemoFontOam *fontOam;
    StrBuf *str;

    setup.palette = wk->palRes[DDEMO_PAL_FONT];
    setup.paletteOffset = 0;
    setup.priority = 4;
    setup.bgPriority = 0;
    setup.surface = 0xffff;
    setup.vramType = CLACT_VRAM_MAIN;

    fontOam = &wk->fontOam[DDEMO_FOAM_TITLE];
    fontOam->bmp = GFL_BitmapCreate(22, 2, BGSYS_TILE_SIZE_16, HEAPID_DENDOU_DEMO);
    setup.bitmap = fontOam->bmp;
    setup.x = -184;
    setup.y = 16;
    wk->fontOam[DDEMO_FOAM_TITLE].oam = func_020255a0(wk->bmpOam, &setup);
    GFL_BitmapFill(wk->fontOam[DDEMO_FOAM_TITLE].bmp, 0);
    str = GFL_MsgDataLoadStrbufNew(wk->msgData, BtlMain2_Text_000);
    func_ov180_021b77dc(wk, DDEMO_FOAM_TITLE, wk->font, str, 12, 0);
    GFL_StrBufFree(str);

    fontOam = &wk->fontOam[DDEMO_FOAM_NAME];
    fontOam->bmp = GFL_BitmapCreate(22, 2, BGSYS_TILE_SIZE_16, HEAPID_DENDOU_DEMO);
    setup.bitmap = fontOam->bmp;
    setup.x = 264;
    setup.y = 160;
    wk->fontOam[DDEMO_FOAM_NAME].oam = func_020255a0(wk->bmpOam, &setup);
}

// Draws the name and level of the Pokémon being shown
// pokebw2's DDemoObj_PrintPokeInfo
void func_ov180_021b78e0(DDemoWork *wk) {
    BoxPkm *pkm = func_020185f0(wk->mons[wk->monIndex]);
    BOOL wasEncrypted = PML_PkmDecrypt(pkm);
    StrBuf *str;
    u32 width;

    GFL_BitmapFill(wk->fontOam[DDEMO_FOAM_NAME].bmp, 0);

    str = GFL_MsgDataLoadStrbufNew(wk->msgData, BtlMain2_Text_001);
    loadBoxPokemonNameToStrbuf(wk->wordSet, 0, pkm);
    GFL_WordSetFormatStrbuf(wk->wordSet, wk->strbuf, str);
    func_ov180_021b77dc(wk, DDEMO_FOAM_NAME, wk->font, wk->strbuf, 8, 0);
    GFL_StrBufFree(str);

    str = GFL_MsgDataLoadStrbufNew(wk->msgData, BtlMain2_Text_002);
    width = GFL_FontGetBlockWidth(str, wk->font, 0);
    func_ov180_021b77dc(wk, DDEMO_FOAM_NAME, wk->font, str, 112, 0);
    GFL_StrBufFree(str);

    str = GFL_MsgDataLoadStrbufNew(wk->msgData, BtlMain2_Text_003);
    WordSetNumber(wk->wordSet, 0, PML_PkmGetParam(pkm, PKM_PARAM_LEVEL, NULL), 3, 0, TRUE);
    GFL_WordSetFormatStrbuf(wk->wordSet, wk->strbuf, str);
    func_ov180_021b77dc(wk, DDEMO_FOAM_NAME, wk->font, wk->strbuf, 112 + width, 0);
    GFL_StrBufFree(str);

    PML_PkmReEncrypt(pkm, wasEncrypted);
}

// The second scene's texts, hidden: the congratulations, and the trainer's name, ID and play time
// pokebw2's DDemoObj_InitFontOam2nd
static void func_ov180_021b79f4(DDemoWork *wk) {
    BmpOamActorSetup setup;
    DDemoFontOam *fontOam;
    StrBuf *str;
    u32 width;

    setup.palette = wk->palRes[DDEMO_PAL_FONT];
    setup.paletteOffset = 0;
    setup.priority = 4;
    setup.bgPriority = 0;
    setup.surface = 0xffff;
    setup.vramType = CLACT_VRAM_MAIN;

    fontOam = &wk->fontOam[DDEMO_FOAM_TITLE_2ND];
    fontOam->bmp = GFL_BitmapCreate(32, 2, BGSYS_TILE_SIZE_16, HEAPID_DENDOU_DEMO);
    setup.bitmap = fontOam->bmp;
    setup.x = 0;
    setup.y = 16;
    wk->fontOam[DDEMO_FOAM_TITLE_2ND].oam = func_020255a0(wk->bmpOam, &setup);
    func_02025774(wk->fontOam[DDEMO_FOAM_TITLE_2ND].oam, FALSE);
    GFL_BitmapFill(wk->fontOam[DDEMO_FOAM_TITLE_2ND].bmp, 0);

    str = GFL_MsgDataLoadStrbufNew(wk->msgData, BtlMain2_Text_004);
    width = GFL_FontGetBlockWidth(str, wk->font, 0);
    func_ov180_021b77dc(wk, DDEMO_FOAM_TITLE_2ND, wk->font, str, (256 - width) / 2, 0);
    GFL_StrBufFree(str);

    fontOam = &wk->fontOam[DDEMO_FOAM_PLAYER];
    fontOam->bmp = GFL_BitmapCreate(32, 2, BGSYS_TILE_SIZE_16, HEAPID_DENDOU_DEMO);
    setup.bitmap = fontOam->bmp;
    setup.x = 0;
    setup.y = 160;
    wk->fontOam[DDEMO_FOAM_PLAYER].oam = func_020255a0(wk->bmpOam, &setup);
    func_02025774(wk->fontOam[DDEMO_FOAM_PLAYER].oam, FALSE);
    GFL_BitmapFill(wk->fontOam[DDEMO_FOAM_PLAYER].bmp, 0);

    str = GFL_MsgDataLoadStrbufNew(wk->msgData, BtlMain2_Text_005);
    copyVarForText(wk->wordSet, 0, wk->param->playerInfo);
    GFL_WordSetFormatStrbuf(wk->wordSet, wk->strbuf, str);
    func_ov180_021b77dc(wk, DDEMO_FOAM_PLAYER, wk->font, wk->strbuf, 8, 0);
    GFL_StrBufFree(str);

    str = GFL_MsgDataLoadStrbufNew(wk->msgData, BtlMain2_Text_006);
    WordSetNumber(wk->wordSet, 0, getTrainerID(wk->param->playerInfo), 5, 2, TRUE);
    GFL_WordSetFormatStrbuf(wk->wordSet, wk->strbuf, str);
    func_ov180_021b77dc(wk, DDEMO_FOAM_PLAYER, wk->font, wk->strbuf, 92, 0);
    GFL_StrBufFree(str);

    str = GFL_MsgDataLoadStrbufNew(wk->msgData, BtlMain2_Text_007);
    WordSetNumber(wk->wordSet, 0, func_0200864c(wk->param->playTime), 3, 0, TRUE);
    WordSetNumber(wk->wordSet, 1, func_02008650(wk->param->playTime), 2, 2, TRUE);
    GFL_WordSetFormatStrbuf(wk->wordSet, wk->strbuf, str);
    func_ov180_021b77dc(wk, DDEMO_FOAM_PLAYER, wk->font, wk->strbuf, 200, 0);
    GFL_StrBufFree(str);
}

// pokebw2's DDemoObj_MoveFontOam
void func_ov180_021b7c08(DDemoWork *wk) {
    s16 x, y;

    func_ov180_021b7408(wk, DDEMOOBJ_PLATE_TITLE, &x, &y);
    func_0202590c(wk->fontOam[DDEMO_FOAM_TITLE].oam, x - 176, y - 8);
    func_ov180_021b7408(wk, DDEMOOBJ_PLATE_NAME, &x, &y);
    func_0202590c(wk->fontOam[DDEMO_FOAM_NAME].oam, x, y - 8);
}

// pokebw2's DDemoObj_SetFontOamVisible
void func_ov180_021b7c60(DDemoWork *wk, u32 id, BOOL visible) {
    func_02025774(wk->fontOam[id].oam, visible);
}

// Loads the main screen's actors' graphics and the font's palette
// pokebw2's DDemoObj_LoadResources
static void func_ov180_021b7c78(DDemoWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_DENDOU_DEMO, HEAPID_TAIL(HEAPID_DENDOU_DEMO));

    wk->chrRes[DDEMO_CHR_OBJ] = func_0204a6c8(arc, NARC_DDEMO_OBJ_1ST_NCGR, FALSE, CLACT_VRAM_MAIN, HEAPID_DENDOU_DEMO);
    wk->palRes[DDEMO_PAL_OBJ] = func_0204abf0(arc, NARC_DDEMO_OBJ_NCLR, CLACT_VRAM_MAIN, 0, HEAPID_DENDOU_DEMO);
    wk->celRes[DDEMO_CEL_OBJ] =
        func_0204af28(arc, NARC_DDEMO_OBJ_1ST_NCER, NARC_DDEMO_OBJ_1ST_NANR, HEAPID_DENDOU_DEMO);
    GFL_ArcToolFree(arc);

    arc = GFL_ArcSysCreateFileHandle(ARCID_FONT, HEAPID_TAIL(HEAPID_DENDOU_DEMO));
    wk->palRes[DDEMO_PAL_FONT] = func_0204acec(arc, 5, CLACT_VRAM_MAIN, 0xa0, HEAPID_DENDOU_DEMO);
    GFL_ArcToolFree(arc);
}

// pokebw2's DDemoObj_Init1st
void func_ov180_021b7cf0(DDemoWork *wk) {
    func_ov180_021b7c78(wk);
    func_ov180_021b7750(wk);
    wk->clact[DDEMOOBJ_PLATE_TITLE] = func_ov180_021b75a4(wk, &data_ov180_021b81bc);
    wk->clact[DDEMOOBJ_PLATE_NAME] = func_ov180_021b75a4(wk, &data_ov180_021b81d4);
    wk->clact[DDEMOOBJ_FLASH] = func_ov180_021b75a4(wk, &data_ov180_021b81a4);
    func_ov180_021b746c(wk, DDEMOOBJ_FLASH, FALSE);
    func_ov180_021b7814(wk);
}

// pokebw2's DDemoObj_Exit1st
void func_ov180_021b7d44(DDemoWork *wk) {
    func_ov180_021b77c4(wk);
    func_ov180_021b7604(wk);
    func_ov180_021b756c(wk);
}

// Shows the actors on the frames that draw the main screen
// pokebw2's DDemoObj_Main1st
void func_ov180_021b7d5c(DDemoWork *wk) {
    if (GFL_G3DDual3DGetRenderDisp() == TRUE) {
        GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, FALSE);
    } else {
        GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    }
    func_ov180_021b73b0(wk);
}

// The second scene's actors, on a renderer of their own whose sub screen is 512 below the main one
// pokebw2's DDemoObj_Init2nd
void func_ov180_021b7d80(DDemoWork *wk) {
    ArcTool *arc;
    u32 trainerClass;

    func_ov180_021b7c78(wk);
    {
        ClActSurfaceSetup surfaces[] = {
            { 0, 0, 256, 192, CLACT_VRAM_MAIN, TRUE },
            { 0, 512, 256, 192, CLACT_VRAM_SUB, TRUE },
        };

        wk->renderer = func_0204b034(surfaces, NELEMS(surfaces), HEAPID_DENDOU_DEMO);
    }
    func_0204b0f4(wk->renderer, TRUE);
    func_0204b258(wk->clUnit, wk->renderer);
    wk->clact[DDEMOOBJ_PLATE_UPPER] = func_ov180_021b75a4(wk, &data_ov180_021b8204);
    wk->clact[DDEMOOBJ_PLATE_LOWER] = func_ov180_021b75a4(wk, &data_ov180_021b81ec);
    func_ov180_021b746c(wk, DDEMOOBJ_PLATE_UPPER, FALSE);
    func_ov180_021b746c(wk, DDEMOOBJ_PLATE_LOWER, FALSE);

    arc = GFL_ArcSysCreateFileHandle(ARCID_DENDOU_DEMO, HEAPID_TAIL(HEAPID_DENDOU_DEMO));
    wk->chrRes[DDEMO_CHR_OBJ_SUB] =
        func_0204a6c8(arc, NARC_DDEMO_OBJ_2ND_NCGR, FALSE, CLACT_VRAM_SUB, HEAPID_DENDOU_DEMO);
    wk->palRes[DDEMO_PAL_OBJ_SUB] = func_0204abf0(arc, NARC_DDEMO_OBJ_NCLR, CLACT_VRAM_SUB, 0, HEAPID_DENDOU_DEMO);
    wk->celRes[DDEMO_CEL_OBJ_SUB] =
        func_0204af28(arc, NARC_DDEMO_OBJ_2ND_NCER, NARC_DDEMO_OBJ_2ND_NANR, HEAPID_DENDOU_DEMO);
    GFL_ArcToolFree(arc);

    trainerClass = TRAINER_CLASS_PKMN_TRAINER_ROSA;
    if (getTrainerGender(wk->param->playerInfo) == GENDER_MALE) {
        trainerClass = TRAINER_CLASS_PKMN_TRAINER_NATE;
    }
    arc = func_0202da04(HEAPID_TAIL(HEAPID_DENDOU_DEMO));
    wk->chrRes[DDEMO_CHR_TRAINER_MAIN] = func_0202da48(arc, trainerClass, CLACT_VRAM_MAIN, HEAPID_DENDOU_DEMO);
    wk->palRes[DDEMO_PAL_TRAINER_MAIN] =
        func_0202da18(arc, trainerClass, CLACT_VRAM_MAIN, 0xc0, HEAPID_DENDOU_DEMO);
    wk->celRes[DDEMO_CEL_TRAINER_MAIN] = func_0202da6c(trainerClass, 2, CLACT_VRAM_MAIN, HEAPID_DENDOU_DEMO);
    wk->chrRes[DDEMO_CHR_TRAINER_SUB] = func_0202da48(arc, trainerClass, CLACT_VRAM_SUB, HEAPID_DENDOU_DEMO);
    wk->palRes[DDEMO_PAL_TRAINER_SUB] =
        func_0202da18(arc, trainerClass, CLACT_VRAM_SUB, 0x60, HEAPID_DENDOU_DEMO);
    wk->celRes[DDEMO_CEL_TRAINER_SUB] = func_0202da6c(trainerClass, 0, CLACT_VRAM_SUB, HEAPID_DENDOU_DEMO);
    GFL_ArcToolFree(arc);
    wk->clact[DDEMOOBJ_TRAINER_MAIN] = func_ov180_021b75a4(wk, &data_ov180_021b824c);
    wk->clact[DDEMOOBJ_TRAINER_SUB] = func_ov180_021b75a4(wk, &data_ov180_021b8264);
    func_ov180_021b79f4(wk);
}

// pokebw2's DDemoObj_Exit2nd
void func_ov180_021b7f0c(DDemoWork *wk) {
    func_ov180_021b77c4(wk);
    func_ov180_021b7604(wk);
    func_0204b084(wk->renderer);
    func_ov180_021b756c(wk);
}

// pokebw2's DDemoObj_Main2nd
void func_ov180_021b7f30(DDemoWork *wk) {
    func_ov180_021b73b0(wk);
}

// pokebw2's DDemoObj_MainConfetti
void func_ov180_021b7f38(DDemoWork *wk) {
    s16 x, y;
    u16 i;
    u16 rand;

    for (i = DDEMOOBJ_CONFETTI; i < DDEMOOBJ_COUNT; i++) {
        if (wk->clact[i] != NULL) {
            if (func_ov180_021b74b4(wk, i) != TRUE) {
                func_ov180_021b75e8(wk, i);
            }
        }
    }
    rand = GFL_RandomLC(4);
    if (rand < 2) {
        for (i = DDEMOOBJ_CONFETTI; i < DDEMOOBJ_COUNT; i++) {
            if (wk->clact[i] == NULL) {
                DDemoObjSetup setup;

                x = GFL_RandomLC(256);
                y = GFL_RandomLC(216);
                if (y < 192) {
                    setup = data_ov180_021b821c;
                    setup.setup.x = x;
                    setup.setup.y = y;
                } else {
                    setup = data_ov180_021b8234;
                    setup.setup.x = x;
                    setup.setup.y = y - 24;
                }
                wk->clact[i] = func_ov180_021b75a4(wk, &setup);
                func_ov180_021b7488(wk, i, rand + 4);
                break;
            }
        }
    }
}
