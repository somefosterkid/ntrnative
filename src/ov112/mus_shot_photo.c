// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov209/mus_shot_photo.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/musical/mus_item_draw.h"
#include "app/musical/mus_poke_draw.h"
#include "app/musical/mus_shot_photo.h"
#include "app/musical/sta_act_bg.h"
#include "app/musical/sta_act_light.h"
#include "app/musical/sta_act_poke.h"
#include "constants/arc.h"
#include "constants/text_banks.h"
#include "field/musical.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/blact.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/g3d.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/str.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "save/save_control.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/wordset.h"
#include "text/system/btl_pokeparam_2.h"
#include "app/musical/musical_system.h"
#include "save/musical_savedata.h"

// Overlay 209's mus_shot_photo.c: the photo of a musical's finale on the main screen, the four Pokémon in the props
// they wore, under spotlights, with the date and the program's title

struct MusShotPhoto {
    HeapID heapId;
    MusicalShot *shot;
    MusicalPoke *pokes[4];
    G3DCamera *camera;
    BlActScene *blact;
    MusPokeDrawSys *pokeDraw;
    MusItemDrawSys *itemDraw;
    StaActBg *bg;
    StaActPokeSys *staPoke;
    StaActPoke *pokeActs[4];
    StaActLightSys *lightSys;
    StaActLight *lights[4];
    BmpWin *dateWin;
    BmpWin *titleWin;
    Font *dateFont;
    Font *titleFont;
};

static void func_ov112_021f3ca4(MusShotPhoto *photo);
static void func_ov112_021f3eac(const BGSetup *setup, u8 bg, u8 mode);
static void func_ov112_021f3ed0(MusShotPhoto *photo);
static void func_ov112_021f3f00(MusShotPhoto *photo);
static fx32 func_ov112_021f40d0(MusShotPhoto *photo, u8 pos, u16 species);
static void func_ov112_021f40f4(MusShotPhoto *photo);
static void func_ov112_021f4280(MusShotPhoto *photo);

static inline fx32 GetPokeX(u8 pos) {
    fx32 xs[4] = { FX32_CONST(160), FX32_CONST(224), FX32_CONST(288), FX32_CONST(352) };

    return xs[pos];
}

static const BGSetup data_ov112_021f81f4 = {
    0,
    0,
    0x1000,
    0,
    BGRES_512x256,
    GX_BG_COLORMODE_16,
    GX_BG_SCRBASE_0x7000,
    GX_BG_CHARBASE_0x10000,
    0x8000,
    GX_BG_EXTPLTT_23,
    2,
    GX_BG_AREAOVER_XLU,
    FALSE,
};

static const VecFx32 data_ov112_021f81b8 = { 0, FX32_ONE, 0 };
static const VecFx32 data_ov112_021f81a0 = { FX32_CONST(8), 0, 0 };
static const VecFx32 data_ov112_021f81ac = { FX32_CONST(8), 0, FX32_CONST(301) };
static const GXRgb data_ov112_021f81c4[8] = { 0 };

static const BGSetup data_ov112_021f81d4 = {
    0,
    0,
    0x2000,
    0,
    BGRES_512x512,
    GX_BG_COLORMODE_16,
    GX_BG_SCRBASE_0x5000,
    GX_BG_CHARBASE_0x08000,
    0x8000,
    GX_BG_EXTPLTT_01,
    1,
    GX_BG_AREAOVER_REPEAT,
    FALSE,
};

static const BGSetup data_ov112_021f8214 = {
    0,
    0,
    0x800,
    0,
    BGRES_256x256,
    GX_BG_COLORMODE_16,
    GX_BG_SCRBASE_0x4800,
    GX_BG_CHARBASE_0x00000,
    0x5000,
    GX_BG_EXTPLTT_01,
    0,
    GX_BG_AREAOVER_XLU,
    FALSE,
};

// pokebw2's MusShotPhoto_Create
MusShotPhoto *func_ov112_021f3af0(MusicalShot *shot, HeapID heapId) {
    u8 i, j;
    MusShotPhoto *photo = GFL_HeapAllocate(heapId, sizeof(MusShotPhoto), TRUE, "mus_shot_photo.c", 133);

    photo->heapId = heapId;
    photo->shot = shot;
    for (i = 0; i < 4; i++) {
        photo->pokes[i] = func_ov112_021f67e0(shot->pokes[i].species, shot->pokes[i].sex, shot->pokes[i].form,
                                              shot->pokes[i].rare, shot->pokes[i].personality, heapId);
        for (j = 0; j < 8; j++) {
            u8 equip = shot->pokes[i].equips[j].unk4;

            if (equip != 10) {
                photo->pokes[i]->equips[equip].itemId = shot->pokes[i].equips[j].itemId;
                photo->pokes[i]->equips[equip].unk2 = shot->pokes[i].equips[j].unk2;
                photo->pokes[i]->equips[equip].slot = j;
            }
        }
    }
    func_ov112_021f3ca4(photo);
    func_ov112_021f3f00(photo);
    func_ov112_021f40f4(photo);
    photo->bg = func_ov112_021f6f24(photo->heapId, NULL);
    func_ov112_021f6fb0(photo->bg, photo->shot->unk0_0);
    func_ov112_021f75dc(photo->staPoke, 0x80);
    func_ov112_021f7098(photo->bg, 0x80);
    return photo;
}

// pokebw2's MusShotPhoto_Delete
void func_ov112_021f3bf0(MusShotPhoto *photo) {
    u8 i;

    func_ov112_021f6c7c(photo->lightSys);
    func_ov112_021f6f4c(photo->bg);
    func_ov112_021f7120(photo->staPoke);
    func_ov112_021f51b0(photo->pokeDraw);
    func_ov112_021f4948(photo->itemDraw);
    BmpWin_Free(photo->titleWin);
    BmpWin_Free(photo->dateWin);
    GFL_FontFree(photo->titleFont);
    GFL_FontFree(photo->dateFont);
    func_ov112_021f3ed0(photo);
    for (i = 0; i < 4; i++) {
        GFL_HeapFree(photo->pokes[i]);
    }
    GFL_HeapFree(photo);
}

// pokebw2's MusShotPhoto_Main
void func_ov112_021f3c50(MusShotPhoto *photo) {
    func_ov112_021f7160(photo->staPoke);
    func_ov112_021f51e8(photo->pokeDraw);
    func_ov112_021f6cb0(photo->lightSys);
    GFL_G3DSysReset();
    GFL_G3DSysMtxViewFlush();
    func_ov112_021f6e74(photo->lightSys);
    func_ov112_021f75a0(photo->staPoke);
    func_ov112_021f51f4(photo->pokeDraw);
    func_ov112_021f718c(photo->staPoke);
    func_ov112_021f6f8c(photo->bg);
    BlActScene_Draw(photo->blact, photo->camera, NULL);
    func_ov112_021f4280(photo);
    GFL_G3DSysReqSwapBuffers();
}

// pokebw2's MusShotPhoto_InitGraphics
static void func_ov112_021f3ca4(MusShotPhoto *photo) {
    ArcTool *arc;

    GFL_G3DSysCreate(FALSE, 2, FALSE, 4, 0, photo->heapId, NULL);
    GFL_BGSysSet3DBGPriority(3);
    GFL_G3DSysSetSwapBufferParams(0, 1);
    photo->camera = GFL_G3DCameraCreate(G3DCAM_PROJECTION_ORTHO, FX32_CONST(12), 0, 0, FX32_CONST(16), FX32_ONE,
                                        FX32_CONST(1000), 0, &data_ov112_021f81ac, &data_ov112_021f81b8,
                                        &data_ov112_021f81a0, photo->heapId);
    GFL_G3DCameraFlush(photo->camera);
    G3X_SetEdgeColorTable(data_ov112_021f81c4);
    G3X_EdgeMarking(FALSE);
    G3X_AntiAlias(TRUE);
    G3X_AlphaBlend(TRUE);
    GFL_G3DSysSetSwapBufferParams(0, 0);
    {
        BlActSceneSetup setup = { 128, 128, { FX32_ONE, FX32_ONE, FX32_ONE }, 0, 0, 0, 0, 63, 0 };
        VecFx32 scale = { FX32_CONST(4), FX32_CONST(4), FX32_ONE };

        photo->blact = BlActScene_Create(&setup, photo->heapId);
        BlActScene_SetScale(photo->blact, &scale);
    }
    func_ov112_021f3eac(&data_ov112_021f8214, 1, 0);
    func_ov112_021f3eac(&data_ov112_021f81d4, 2, 0);
    func_ov112_021f3eac(&data_ov112_021f81f4, 3, 0);
    GFL_BGSysMoveBG(2, BG_MOVE_SET_Y, 224);
    arc = GFL_ArcSysCreateFileHandle(ARCID_MUSICAL, photo->heapId);
    GFL_G2DIOLoadArcNCLR(arc, 3, 0, 0, 0, 0x20, photo->heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 9, 2, 0, 0, FALSE, photo->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 15, 2, 0, 0, FALSE, photo->heapId);
    GFL_G2DIOLoadArcNCLR(arc, 2, 0, 0x20, 0x20, 0x20, photo->heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 8, 3, 0, 0, FALSE, photo->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 14, 3, 0, 0, FALSE, photo->heapId);
    GFL_BGSysLoadScr(2);
    GFL_BGSysLoadScr(3);
    GFL_ArcToolFree(arc);
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG3, GX_BLEND_PLANEMASK_BG0, 0, 10);
    GX_SetVisibleWnd(GX_WNDMASK_OW);
    G2_SetWndOBJInsidePlane(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ, TRUE);
    G2_SetWndOutsidePlane(GX_PLANEMASK_ALL, TRUE);
}

// pokebw2's MusShotPhoto_InitBG
static void func_ov112_021f3eac(const BGSetup *setup, u8 bg, u8 mode) {
    GFL_BGSysCreateBG(bg, setup, mode);
    GFL_BGSysSetBGEnabled(bg, TRUE);
    GFL_BGSysClearBG(bg);
    GFL_BGSysLoadScr(bg);
}

// pokebw2's MusShotPhoto_ExitGraphics
static void func_ov112_021f3ed0(MusShotPhoto *photo) {
    GFL_BGSysReleaseBG(3);
    GFL_BGSysReleaseBG(1);
    GFL_BGSysReleaseBG(2);
    func_0204ed24(photo->blact);
    GFL_G3DCameraFree(photo->camera);
    GFL_G3DSysFree();
    GFL_BGSysReleaseBG(0);
}

// pokebw2's MusShotPhoto_InitPokes
static void func_ov112_021f3f00(MusShotPhoto *photo) {
    VecFx32 pos = { FX32_CONST(64), FX32_CONST(155), FX32_CONST(170) };
    VecFx32 lightPos = { FX32_CONST(64), FX32_CONST(128), FX32_CONST(200) };
    fx32 z = FX32_CONST(80);
    fx32 zs[4];
    u32 tops;
    u8 mask;
    u8 i;
    u8 bit;

    photo->pokeDraw = func_ov112_021f5164(photo->heapId);
    func_ov112_021f5438(photo->pokeDraw, FX32_CONST(32));
    photo->itemDraw = func_ov112_021f48c0(photo->blact, 36, photo->heapId);
    photo->staPoke = func_ov112_021f709c(photo->heapId, NULL, photo->pokeDraw, photo->itemDraw, photo->blact);
    for (i = 0; i < 4; i++) {
        photo->pokeActs[i] = NULL;
    }
    for (i = 0; i < 4; i++) {
        photo->pokeActs[i] = func_ov112_021f760c(photo->staPoke, photo->pokes[i]);
        func_ov112_021f80b0(photo->staPoke, photo->pokeActs[i]);
        func_ov112_021f8154(photo->staPoke, photo->pokeActs[i], TRUE);
    }

    // The Pokémon that got the most points stand in front
    tops = photo->shot->tops;
    bit = 1;
    for (i = 0; i < 4; i++) {
        if (!(tops & bit)) {
            zs[i] = z;
            z += FX32_CONST(30);
        }
        bit <<= 1;
    }
    mask = 1;
    for (i = 0; i < 4; i++) {
        if (tops & mask) {
            zs[i] = z;
            z += FX32_CONST(30);
        }
        mask <<= 1;
    }

    mask = 1;
    photo->lightSys = func_ov112_021f6bd8(photo->heapId, NULL);
    for (i = 0; i < 4; i++) {
        lightPos.x = func_ov112_021f40d0(photo, i, photo->pokes[i]->species);
        if (mask & photo->shot->tops) {
            VecFx32 offset = { 0, FX32_CONST(-35), 0 };

            pos.x = func_ov112_021f40d0(photo, i, photo->pokes[i]->species);
            pos.y = FX32_CONST(155);
            pos.z = zs[i];
            func_ov112_021f8054(photo->staPoke, photo->pokeActs[i], &pos);
            func_ov112_021f8084(photo->staPoke, photo->pokeActs[i], &offset);
            lightPos.y = FX32_CONST(88);
        } else {
            pos.x = func_ov112_021f40d0(photo, i, photo->pokes[i]->species);
            pos.y = FX32_CONST(155);
            pos.z = zs[i];
            func_ov112_021f8054(photo->staPoke, photo->pokeActs[i], &pos);
            lightPos.y = FX32_CONST(128);
        }
        photo->lights[i] = func_ov112_021f6e78(photo->lightSys, 1);
        func_ov112_021f6ef4(photo->lightSys, photo->lights[i], &lightPos);
        mask <<= 1;
    }
}

// pokebw2's MusShotPhoto_GetPokeX
static fx32 func_ov112_021f40d0(MusShotPhoto *photo, u8 pos, u16 species) {
    return GetPokeX(pos);
}

// pokebw2's MusShotPhoto_InitText
static void func_ov112_021f40f4(MusShotPhoto *photo) {
    StrBuf *str;
    WordSet *wordSet;
    MsgData *msgData;
    StrBuf *format;
    StrBuf *title;

    photo->dateWin = BmpWin_CreateDynamic(1, 24, 22, 8, 2, 10, TRUE);
    photo->titleWin = BmpWin_CreateDynamic(1, 1, 1, 30, 2, 10, TRUE);
    GFL_BitmapFill(BmpWin_GetBitmap(photo->dateWin), 0);
    GFL_BitmapFill(BmpWin_GetBitmap(photo->titleWin), 0);
    photo->dateFont = GFL_FontCreate(ARCID_FONT, 3, 0, FALSE, photo->heapId);
    photo->titleFont = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, photo->heapId);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 0, 0x140, 0x20, photo->heapId);

    str = GFL_StrBufCreate(128, photo->heapId);
    wordSet = GFL_WordSetSystemCreateDefault(photo->heapId);
    msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_BTL_POKEPARAM_2, photo->heapId);
    format = GFL_MsgDataLoadStrbufNew(msgData, BtlPokeparam2_Text_000);
    WordSetNumber(wordSet, 0, photo->shot->year, 2, 2, TRUE);
    WordSetNumber(wordSet, 1, photo->shot->month, 2, 2, TRUE);
    WordSetNumber(wordSet, 2, photo->shot->day, 2, 2, TRUE);
    GFL_WordSetFormatStrbuf(wordSet, str, format);
    GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(photo->dateWin), 0, 0, str, photo->dateFont, PRINT_COLOR(3, 11, 0));
    GFL_StrBufFree(str);
    GFL_StrBufFree(format);
    GFL_WordSetSystemFree(wordSet);
    GFL_MsgDataFree(msgData);

    title = GFL_StrBufCreate(0x25, photo->heapId);
    GFL_StrBufLoadString(title, photo->shot->title);
    GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(photo->titleWin), 0, 0, title, photo->titleFont,
                                   PRINT_COLOR(15, 2, 0));
    GFL_StrBufFree(title);
    BmpWin_FlushChar(photo->dateWin);
    BmpWin_FlushMap(photo->dateWin);
    BmpWin_FlushChar(photo->titleWin);
    BmpWin_FlushMap(photo->titleWin);
    GFL_BGSysLoadScr(1);
}

// pokebw2's MusShotPhoto_Debug
static void func_ov112_021f4280(MusShotPhoto *photo) {
}
