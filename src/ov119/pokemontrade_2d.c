// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov194/pokemontrade_2d.c, ported by tools/port_file.py.
#include "types.h"
#include "app/ui/ui_scene.h"
#include "app/ui/touchbar.h"
#include "app/pokemon_trade_local.h"
#include "constants/arc.h"
#include "constants/items.h"
#include "constants/pokemon.h"
#include "constants/sound.h"
#include "constants/species.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/g3d.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "gfl/ui.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/fx/fx_cp.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/memorymap.h"
#include "nitro/os.h"
#include "pml/item.h"
#include "pml/personal.h"
#include "pml/poke_party.h"
#include "save/box.h"
#include "system/app_menu_common.h"
#include "system/bmp_winframe.h"
#include "system/gf_font.h"
#include "system/palanm.h"
#include "system/printsys.h"

// The trade's 2D: the backgrounds of both screens, the strip of boxes on the lower screen with the Pokémon icons in
// it, the touch bar, and the sprites of the panels

// The strip: the party's two columns of three, then each box's six columns of five, each column 24 pixels wide. On
// the background, the party takes 12 tiles and each box 20
#define PARTY_COLUMNS 2
#define BOX_COLUMNS 6
#define COLUMN_ROWS 5
#define PARTY_TILES 12
#define BOX_TILES 20
// The most boxes there can be
#define MAX_BOXES 24
// The columns of icons that are set up at once, enough to cover the screen
#define ICON_COLUMNS 12

// The screen files of the strip's backgrounds: where the cells start, after the file's, the block's and the screen's
// headers, and the screens' width, in tiles
#define SCREEN_CELLS (0x24 / 2)
#define SCREEN_WIDTH 32

static u8 func_ov119_021e3d04(BoxPkm *pkm, HeapID heapId);
void func_ov119_021e3d88(PokemonTradeWork *wk, int box, u8 *colors);
void func_ov119_021e3dbc(PokemonTradeWork *wk, PokeParty *party, u8 *colors);
u16 func_ov119_021e41a0(int x, int y, PokemonTradeWork *wk, BOOL marked);
BOOL func_ov119_021e421c(PokemonTradeWork *wk, int x, int y);
void func_ov119_021e4324(PokemonTradeWork *wk, u32 chars);
void func_ov119_021e43dc(PokemonTradeWork *wk);
void func_ov119_021e4810(PokemonTradeWork *wk, int index);
void func_ov119_021e496c(int column, int row, ClActorPos *pos);
BoxPkm *func_ov119_021e4984(BoxSaveAccessor *boxes, int column, int row, PokemonTradeWork *wk, BOOL *isParty);
TradeBoxEntry *func_ov119_021e49dc(int column, int row, PokemonTradeWork *wk);
BOOL func_ov119_021e4a84(PokemonTradeWork *wk, int species);
void func_ov119_021e4ad0(PokemonTradeWork *wk, int column, int row, BoxPkm *pkm, BOOL isParty, int index);
void func_ov119_021e4b00(PokemonTradeWork *wk, BoxSaveAccessor *boxes, int column, int index, BOOL async);
void func_ov119_021e4f28(PokemonTradeWork *wk);
int func_ov119_021e5158(int column);
void func_ov119_021e5164(PokemonTradeWork *wk);
void func_ov119_021e6008(PokemonTradeWork *wk, int side, PartyPkm *pkm);
static void func_ov119_021e7220(TCB *tcb, void *data);
static void func_ov119_021e7234(TCB *tcb, void *data);
void func_ov119_021e7248(TradeCurve *curve, VecFx32 *start, VecFx32 *control1, VecFx32 *control2, VecFx32 *end,
                                int frames);
static BOOL func_ov119_021e729c(TradeCurve *curve);
void func_ov119_021e5520(void);
void func_ov119_021e6ab0(PokemonTradeWork *wk);
void func_ov119_021e6fc8(uintptr_t param, fx32 frame);
void func_ov119_021e7098(const PokemonTradeWork *wk, u32 vram, u32 paletteMask);
void func_ov119_021e7160(PokemonTradeWork *wk, BOOL dim, u16 vram, u32 paletteMask);

// The cell actor systems of the trade demo, which has only a few sprites, and of the trade
// pokebw2's sDemoClActSetup
static const ClActSysSetup data_ov119_021e7ce4 = { 0, 0, 0, 512, 4, 124, 4, 124, 0, 5, 5, 5, 5, 16, 16 };
// pokebw2's sClActSetup
static const ClActSysSetup data_ov119_021e7cc8 = { 0, 0, 0, 512, 4, 124, 4, 124, 0, 128, 128, 128, 128, 16, 16 };

// pokebw2's sLights
static const Light data_ov119_021e7d00[4] = {
    { { 0, -4096, 0 }, GX_RGB(16, 16, 16) },
    { { 0, 4096, 0 }, GX_RGB(16, 16, 16) },
    { { 0, -4096, 0 }, GX_RGB(16, 16, 16) },
    { { 0, -4096, 0 }, GX_RGB(16, 16, 16) },
};

// pokebw2's sVRAMConfig
static const BGSysVRAMConfig data_ov119_021e7d40 = {
    GX_VRAM_BG_64_E,   GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,        GX_VRAM_SUB_BGEXTPLTT_0123_H,
    GX_VRAM_OBJ_16_G,  GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_128_D,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_01_AB, GX_VRAM_TEXPLTT_0_F,     GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K,
};

// The colour of a Pokémon's slot, an index into the box palette: none for an empty slot, the species' colour, or the
// colour of an egg, Manaphy's being different
static u8 func_ov119_021e3d04(BoxPkm *pkm, HeapID heapId) {
    u16 color = 0;
    BOOL encrypted = PML_PkmDecrypt(pkm);
    u32 species = PML_PkmGetParam(pkm, PKM_PARAM_SPECIES, NULL);

    if (PML_PkmGetParam(pkm, PKM_PARAM_SPECIES_VALID, NULL)) {
        if (!PML_PkmGetParam(pkm, PKM_PARAM_IS_EGG, NULL)) {
            u16 form = PML_PkmGetParam(pkm, PKM_PARAM_FORM, NULL);
            void *personal = PML_PersonalLoad(species, form, heapId);
            color = PML_PersonalGetParam(personal, 33);
            PML_PersonalFree(personal);
        } else {
            color = COLOR_BLUE;
            if (species != SPECIES_MANAPHY) {
                color = COLOR_WHITE;
            }
        }
        color++;
    }
    PML_PkmReEncrypt(pkm, encrypted);
    return color;
}

void func_ov119_021e3d88(PokemonTradeWork *wk, int box, u8 *colors) {
    u8 i;

    for (i = 0; i < 30; i++) {
        colors[i] = func_ov119_021e3d04(BoxSaveAccessor_GetPkm(wk->boxes, box, i), wk->heapId);
    }
}



void func_ov119_021e3dbc(PokemonTradeWork *wk, PokeParty *party, u8 *colors) {
    int count = PokeParty_GetPkmCount(party);
    u8 i;

    for (i = 0; i < 6; i++) {
        if (count > i) {
            colors[i] = func_ov119_021e3d04(func_020185f0(PokeParty_GetPkm(party, i)), wk->heapId);
        } else {
            colors[i] = 0;
        }
    }
}

BOOL func_ov119_021e3e00(PokemonTradeWork *wk, int box) {
    while (box < wk->unk630 + 1) {
        if (box == wk->unk630) {
            func_ov119_021e3dbc(wk, wk->party, wk->unk2E0);
            return TRUE;
        }
        func_ov119_021e3d88(wk, box, wk->unk10[box]);
        break;
    }
    return FALSE;
}


void func_ov119_021e3e40(PokemonTradeWork *wk) {
    ClActSys_Create(&data_ov119_021e7ce4, &data_ov119_021e7d40, wk->heapId);
}

void func_ov119_021e3e60(PokemonTradeWork *wk) {
    ClActSys_Create(&data_ov119_021e7cc8, &data_ov119_021e7d40, wk->heapId);
}

void func_ov119_021e3e80(PokemonTradeWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(0x66, wk->heapId);
    u32 fileId;

    if (func_ov119_021d8aa8(wk)) {
        fileId = 4;
    } else {
        fileId = 11;
    }
    GFL_G2DIOLoadArcNCLRDefault(arc, 9, 0, 0, 0, wk->heapId);
    if (wk->unk86C == 0) {
        wk->unk86C = GFL_BGSysLoadArcNCGRDynamic(arc, 10, 2, 0, FALSE, wk->heapId);
    }
    GFL_G2DIOLoadNSCRSync(arc, fileId, 2, 0, CHAR_POS(wk->unk86C), 0, FALSE, wk->heapId);
    GFL_ArcToolFree(arc);
}


void func_ov119_021e3f08(PokemonTradeWork *wk) {
    if (wk->unk86C != 0) {
        GFL_BGSysFreeCharMemory(2, CHAR_POS(wk->unk86C), CHAR_SIZE(wk->unk86C));
    }
    wk->unk86C = 0;
}

void func_ov119_021e3f30(PokemonTradeWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(0x66, wk->heapId);

    GFL_G2DIOLoadNSCRSync(arc, 12, 4, 0, CHAR_POS(wk->unk870), 0, FALSE, wk->heapId);
    GFL_ArcToolFree(arc);
}



void func_ov119_021e3f70(PokemonTradeWork *wk) {
}

void func_ov119_021e3f74(PokemonTradeWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(0x66, wk->heapId);

    GFL_G2DIOLoadArcNCLRDefault(arc, 17, 4, 0, 0xc0, wk->heapId);
    wk->unk874 = GFL_BGSysLoadArcNCGRDynamic(arc, 13, 7, 0, FALSE, wk->heapId);
    wk->unk870 = GFL_BGSysLoadArcNCGRDynamic(arc, 13, 5, 0, FALSE, wk->heapId);
    GFL_ArcToolFree(arc);
    func_ov119_021e4324(wk, wk->unk874);
}


void func_ov119_021e3fe4(PokemonTradeWork *wk) {
    wk->cursorImage = LoadCursorImageEndOfHeap(6, 15, 0, wk->heapId);
}

void func_ov119_021e4000(PokemonTradeWork *wk) {
    func_ov119_021e43dc(wk);
    if (wk->unk870 != 0) {
        GFL_BGSysFreeCharMemory(5, CHAR_POS(wk->unk870), CHAR_SIZE(wk->unk870));
        wk->unk870 = 0;
    }
    if (wk->unk874 != 0) {
        GFL_BGSysFreeCharMemory(7, CHAR_POS(wk->unk874), CHAR_SIZE(wk->unk874));
        wk->unk874 = 0;
    }
    if (wk->cursorImage != 0) {
        GFL_BGSysFreeCharMemory(6, CHAR_POS(wk->cursorImage), CHAR_SIZE(wk->cursorImage));
        wk->cursorImage = 0;
    }
}



// Loads the common UI sprites once
void func_ov119_021e4068(PokemonTradeWork *wk) {
    if (wk->objRes[TRADE_OBJRES_PLTT_COMMON] == 0) {
        ArcTool *arc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), wk->heapId);

        wk->objRes[TRADE_OBJRES_CHAR_COMMON] = func_0204a6c8(arc, func_02027618(), FALSE, CLACT_VRAM_SUB, wk->heapId);
        wk->objRes[TRADE_OBJRES_PLTT_COMMON] = func_0204ac18(arc, func_02027614(), CLACT_VRAM_SUB, 0, 0, 3, wk->heapId);
        wk->objRes[TRADE_OBJRES_CELL_COMMON] = func_0204af28(arc, func_0202761c(2), func_02027620(2), wk->heapId);
        GFL_ArcToolFree(arc);
    }
}

void func_ov119_021e40ec(PokemonTradeWork *wk, int side, int page) {
    u32 screens[] = { 27, 28 };
    ArcTool *arc = GFL_ArcSysCreateFileHandle(0x66, wk->heapId);

    GFL_G2DIOLoadNSCRSync(arc, screens[page], 4, 0, CHAR_POS(wk->unk870), 0, FALSE, wk->heapId);
    if (side) {
        GFL_BGSysMoveBG(4, 0, 128);
    } else {
        GFL_BGSysMoveBG(4, 0, 0);
    }
    func_0204b3dc(wk->actors[2], FALSE);
    GFL_ArcToolFree(arc);
    GFL_BGSysSetBGEnabled(4, TRUE);
}


void func_ov119_021e4174(PokemonTradeWork *wk) {
    GFL_BGSysSetBGEnabled(4, FALSE);
    GFL_BGSysMoveBG(4, 0, 0);
    func_ov119_021e6700(wk);
    GFL_BGSysClearScr(4);
    GFL_BGSysQueueScrLoad(4);
}

u16 func_ov119_021e41a0(int x, int y, PokemonTradeWork *wk, BOOL marked) {
    if (x >= wk->unk638) {
        x -= wk->unk638;
    }
    if (x < PARTY_TILES) {
        if (marked) {
            return wk->unk1174[3][SCREEN_CELLS + y * SCREEN_WIDTH + x];
        }
        return wk->unk1174[1][SCREEN_CELLS + y * SCREEN_WIDTH + x];
    }
    x = (x - PARTY_TILES) % BOX_TILES;
    if (marked) {
        return wk->unk1174[2][SCREEN_CELLS + y * SCREEN_WIDTH + x];
    }
    return wk->unk1174[0][SCREEN_CELLS + y * SCREEN_WIDTH + x];
}


BOOL func_ov119_021e421c(PokemonTradeWork *wk, int x, int y) {
    int i, j;

    for (i = 0; i < ICON_COLUMNS; i++) {
        for (j = 0; j < COLUMN_ROWS; j++) {
            if (wk->unkEBC[i][j]) {
                int left = wk->unkDCC[i][j].x - 5;
                int top = wk->unkDCC[i][j].y - 8;

                if (left < 0) {
                    left -= 8;
                }
                left /= 8;
                top /= 8;
                if (left <= x && left + 3 > x && top <= y && top + 3 > y) {
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

void func_ov119_021e42b4(PokemonTradeWork *wk) {
    int scroll = wk->unk107C / 8;
    int x, y;

    func_ov119_021e4dbc(wk);
    for (y = 0; y < 24; y++) {
        for (x = 0; x < 52; x++) {
            BOOL marked = func_ov119_021e421c(wk, x, y);
            u16 tile = func_ov119_021e41a0(scroll + x, y, wk, marked);

            func_02042fb0(7, x, y, tile);
        }
    }
    wk->unk108C = wk->unk107C % 8;
    wk->unk1084 = 1;
    GFL_BGSysQueueScrLoad(7);
}



void func_ov119_021e4324(PokemonTradeWork *wk, u32 chars) {
    int i;

    wk->unk1174[0] = GFL_ArcSysReadHeapNew(0x66, 14, wk->heapId);
    wk->unk1174[1] = GFL_ArcSysReadHeapNew(0x66, 15, wk->heapId);
    wk->unk1174[2] = GFL_ArcSysReadHeapNew(0x66, 25, wk->heapId);
    wk->unk1174[3] = GFL_ArcSysReadHeapNew(0x66, 26, wk->heapId);
    for (i = 0; i < SCREEN_WIDTH * 24; i++) {
        wk->unk1174[0][SCREEN_CELLS + i] += CHAR_POS(chars);
        wk->unk1174[1][SCREEN_CELLS + i] += CHAR_POS(chars);
        wk->unk1174[2][SCREEN_CELLS + i] += CHAR_POS(chars);
        wk->unk1174[3][SCREEN_CELLS + i] += CHAR_POS(chars);
    }
    wk->unk107C = wk->unk63C - 80;
    func_ov119_021e42b4(wk);
}


void func_ov119_021e43dc(PokemonTradeWork *wk) {
    if (wk->unk1174[0] != NULL) {
        GFL_HeapFree(wk->unk1174[0]);
        wk->unk1174[0] = NULL;
        GFL_HeapFree(wk->unk1174[1]);
        wk->unk1174[1] = NULL;
        GFL_HeapFree(wk->unk1174[2]);
        wk->unk1174[2] = NULL;
        GFL_HeapFree(wk->unk1174[3]);
        wk->unk1174[3] = NULL;
    }
}

void func_ov119_021e4420(PokemonTradeWork *wk) {
    int i;

    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 4, 0x1c0, 0x20, wk->heapId);
    for (i = 0; i < wk->unk630 + 1; i++) {
        if (wk->unk6B8[i] == NULL) {
            wk->unk6B8[i] = BmpWin_CreateDynamic(5, 0, 0, 15, 2, 14, FALSE);
        }
        if (i == wk->unk630) {
            GFL_MsgDataLoadStrbuf(wk->msgData, 16, wk->drawStr);
        } else {
            loadBoxNameToStrbuf(wk->boxes, i, wk->drawStr);
        }
        GFL_TextRndUpdateColorIndexLUT(15, 2, 0);
        GFL_BitmapFill(BmpWin_GetBitmap(wk->unk6B8[i]), 0);
        GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(wk->unk6B8[i]), 0, 1, wk->drawStr, wk->font);
        BmpWin_FlushChar(wk->unk6B8[i]);
        func_02045788(wk->unk6B8[i], 0);
    }
}



void func_ov119_021e4570(PokemonTradeWork *wk) {
    int i;

    for (i = 0; i < 0x19; i++) {
        if (wk->unk6B8[i] != NULL) {
            BmpWin_Free(wk->unk6B8[i]);
            wk->unk6B8[i] = NULL;
        }
    }
}

void func_ov119_021e4598(PokemonTradeWork *wk) {
    int scroll = wk->unk107C / 8;
    int x, y, i, box;

    for (y = 0; y < 2; y++) {
        for (x = 0; x < 64; x++) {
            func_02042fb0(5, x, y, 0);
        }
    }
    if (wk->unk107C < 96) {
        box = wk->unk630;
    } else {
        box = (wk->unk107C - 96) / 160;
    }
    for (i = 0; i < 3; i++) {
        int name = box + i;

        if (name > wk->unk630) {
            name = name - wk->unk630 - 1;
        } else if (name < 0) {
            name = name + wk->unk630 + 1;
        }
        if (name == wk->unk630) {
            x = wk->unk638 - scroll;
            if (scroll > x) {
                scroll -= wk->unk638;
            } else {
                x = -scroll;
            }
        } else {
            x = name * 20 + 12 - scroll;
        }
        if (x >= -10 && x <= 33) {
            func_02045780(wk->unk6B8[name], x + 1);
            BmpWin_FlushMap(wk->unk6B8[name]);
        }
    }
    GFL_BGSysQueueScrLoad(5);
}

void func_ov119_021e467c(PokemonTradeWork *wk) {
    int i, j;
    ArcTool *arc;
    ClActorSetup setup;

    arc = GFL_ArcSysCreateFileHandle(ARCID_POKEICON, wk->heapId);
    wk->objRes[TRADE_OBJRES_PLTT_ICON] = func_0204acec(arc, func_0201bc3c(), CLACT_VRAM_SUB, 0x60, wk->heapId);
    wk->objRes[TRADE_OBJRES_CELL_ICON] = func_0204af28(arc, func_0201bc78(), getOBJTileMapping_MainEng(), wk->heapId);
    for (i = 0; i < ICON_COLUMNS; i++) {
        for (j = 0; j < COLUMN_ROWS; j++) {
            setup.x = 0;
            setup.y = 0;
            setup.sequence = 1;
            setup.priority = 16;
            setup.bgPriority = 3;
            wk->unk9CC[i][j] = func_0204a6c8(arc, 0x51b, FALSE, CLACT_VRAM_SUB, wk->heapId);
            wk->unkAC0[i][j] = func_0204b294(wk->clactUnit, wk->unk9CC[i][j], wk->objRes[TRADE_OBJRES_PLTT_ICON],
                                             wk->objRes[TRADE_OBJRES_CELL_ICON], &setup, CLACT_VRAM_SUB, wk->heapId);
            setup.sequence = 0;
            setup.priority = 15;
            wk->unkBB0[i][j] = func_0204b294(wk->clactUnit, wk->objRes[TRADE_OBJRES_CHAR_SUB],
                                             wk->objRes[TRADE_OBJRES_PLTT_SUB], wk->objRes[TRADE_OBJRES_CELL_SUB],
                                             &setup, CLACT_VRAM_SUB, wk->heapId);
            func_0204b92c(wk->unkAC0[i][j], FALSE);
            func_0204b3dc(wk->unkAC0[i][j], FALSE);
            func_0204b3dc(wk->unkBB0[i][j], FALSE);
            func_0204b92c(wk->unkBB0[i][j], TRUE);
        }
    }
    GFL_ArcToolFree(arc);
}



void func_ov119_021e4810(PokemonTradeWork *wk, int index) {
    int i;

    for (i = 0; i < COLUMN_ROWS; i++) {
        if (wk->unkAC0[index][i] != NULL) {
            func_0204b3b4(wk->unkAC0[index][i]);
            wk->unkAC0[index][i] = NULL;
        }
        if (wk->unkBB0[index][i] != NULL) {
            func_0204b3b4(wk->unkBB0[index][i]);
            wk->unkBB0[index][i] = NULL;
        }
        if (wk->unk9CC[index][i] != 0) {
            func_0204a8d4(wk->unk9CC[index][i]);
            wk->unk9CC[index][i] = 0;
        }
    }
}

void func_ov119_021e4888(PokemonTradeWork *wk) {
    int i;

    func_ov119_021e69a4(wk);
    for (i = 0; i < ICON_COLUMNS; i++) {
        func_ov119_021e4810(wk, i);
    }
    if (wk->objRes[TRADE_OBJRES_PLTT_ICON] != 0) {
        func_0204ada4(wk->objRes[TRADE_OBJRES_PLTT_ICON]);
        wk->objRes[TRADE_OBJRES_PLTT_ICON] = 0;
    }
    if (wk->objRes[TRADE_OBJRES_CELL_ICON] != 0) {
        func_0204afd8(wk->objRes[TRADE_OBJRES_CELL_ICON]);
        wk->objRes[TRADE_OBJRES_CELL_ICON] = 0;
    }
    if (wk->unk1184 != NULL) {
        GFL_HeapFree(wk->unk1184);
    }
    wk->unk1184 = NULL;
}

void func_ov119_021e48e0(PokemonTradeWork *wk) {
    int i;

    func_ov119_021e7058(wk);
    func_ov119_021e1ca8(wk);
    for (i = 0; i < 10; i++) {
        if (wk->actors[i] != NULL) {
            func_0204b3b4(wk->actors[i]);
            wk->actors[i] = NULL;
        }
    }
    for (i = 0; i < 6; i++) {
        if (wk->objRes[i] != 0) {
            func_0204ada4(wk->objRes[i]);
            wk->objRes[i] = 0;
        }
    }
    for (; i < 12; i++) {
        if (wk->objRes[i] != 0) {
            func_0204a8d4(wk->objRes[i]);
            wk->objRes[i] = 0;
        }
    }
    for (; i < 18; i++) {
        if (wk->objRes[i] != 0) {
            func_0204afd8(wk->objRes[i]);
            wk->objRes[i] = 0;
        }
    }
}





void func_ov119_021e496c(int column, int row, ClActorPos *pos) {
    // A column's width and a row's height, and where the first row is: their reads fold, but they are the first
    // bytes of the file's .rodata
    static const u8 width = 24;
    static const u8 height = 24;
    static const u8 top = 72;

    pos->x = (column + 1) * width;
    pos->y = row * height + top;
}

BoxPkm *func_ov119_021e4984(BoxSaveAccessor *boxes, int column, int row, PokemonTradeWork *wk, BOOL *isParty) {
    if (column >= PARTY_COLUMNS) {
        int box = func_ov119_021d8af4(column, wk);
        int slot = func_ov119_021d8b0c(column, row);

        *isParty = FALSE;
        return func_ov119_021d8dd0(boxes, box, slot, wk);
    }
    if (row < 3) {
        *isParty = TRUE;
        return func_ov119_021d8dd0(boxes, wk->unk630, column + row * 2, wk);
    }
    return NULL;
}


TradeBoxEntry *func_ov119_021e49dc(int column, int row, PokemonTradeWork *wk) {
    if (column >= PARTY_COLUMNS) {
        int box = func_ov119_021d8af4(column, wk);
        int slot = func_ov119_021d8b0c(column, row);

        return func_ov119_021d8e24(box, slot, wk);
    }
    if (row < 3) {
        return func_ov119_021d8e24(wk->unk630, column + row * 2, wk);
    }
    return NULL;
}

void func_ov119_021e4a1c(PokemonTradeWork *wk) {
    int i, j;

    for (i = 0; i < COLUMN_ROWS; i++) {
        for (j = 0; j < ICON_COLUMNS; j++) {
            if (func_0204b3f4(wk->unkBB0[j][i])) {
                ClActorPos pos;

                func_0204b454(wk->unkBB0[j][i], &pos, CLACT_VRAM_SUB);
                wk->touchX = pos.x;
                wk->unkF58 = pos.y;
            }
        }
    }
}



// The initial of each species' English name, from 0 for A, which the search by initial looks for. The two after the
// last species are the egg's
// pokebw2's sSpeciesInitials
static u8 data_ov119_021e82a4[] = {
#include "asset/pokemontrade_2d_data_ov119_021e82a4.inc"
};

BOOL func_ov119_021e4a84(PokemonTradeWork *wk, int species) {
    if (wk->unk800 != 0 && wk->unk800 - 1 == data_ov119_021e82a4[species]) {
        return TRUE;
    }
    return FALSE;
}


BOOL func_ov119_021e4aa4(u32 species, u32 initial) {
    if (initial == data_ov119_021e82a4[species]) {
        return TRUE;
    }
    return FALSE;
}

// Greys an icon out or not
void func_ov119_021e4ab8(PokemonTradeWork *wk, ClActor *icon, BOOL grey) {
    func_0204b688(icon, grey ? 1 : 0);
}

void func_ov119_021e4ad0(PokemonTradeWork *wk, int column, int row, BoxPkm *pkm, BOOL isParty, int index) {
    BOOL grey = FALSE;

    if (func_ov119_021df614(wk, column, row)) {
        grey = TRUE;
    }
    func_ov119_021e4ab8(wk, wk->unkAC0[index][row], grey);
}

// Where an image's characters are in a screen's OBJ VRAM, as NNS_G2dGetImageLocation(), inlined
static inline u32 ImageProxy_GetLocation(const NNSG2dImageProxy *proxy, int type) {
    return proxy->vramLocation.baseAddrOfVram[type];
}

void func_ov119_021e4b00(PokemonTradeWork *wk, BoxSaveAccessor *boxes, int column, int index, BOOL async) {
    int j;

    if (column == wk->unkF74 && !func_020355c4()) {
        func_0204b3dc(wk->unkBB0[index][wk->cursorRow], TRUE);
    }
    for (j = 0; j < COLUMN_ROWS; j++) {
        BoxPkm *pkm;
        TradeBoxEntry *entry;
        BOOL isParty;
        NNSG2dImageProxy proxy;
        ClActorPos pos, screenPos;
        int species, form, sex;

        pkm = func_ov119_021e4984(boxes, column, j, wk, &isParty);
        entry = func_ov119_021e49dc(column, j, wk);
        if (entry == NULL) {
            wk->unkD90[index][j] = 0xff;
            continue;
        }
        if (entry->species == SPECIES_NONE) {
            wk->unkD90[index][j] = 0xff;
            continue;
        }
        wk->unkD90[index][j] = column;
        form = entry->form;
        species = entry->species;
        sex = entry->sex;
        if (species == wk->iconSpecies[index][j] && form == wk->iconForms[index][j] && sex == wk->iconSexes[index][j]) {
            func_0204b3dc(wk->unkAC0[index][j], TRUE);
            func_ov119_021e4ad0(wk, column, j, pkm, isParty, index);
        } else if (species != wk->iconSpecies[index][j] || (species == wk->iconSpecies[index][j] && form != wk->iconForms[index][j])
                   || (species == wk->iconSpecies[index][j] && sex != wk->iconSexes[index][j])) {
            int box, slot;

            wk->iconSpecies[index][j] = species;
            wk->iconForms[index][j] = form;
            wk->iconSexes[index][j] = sex;
            func_ov119_021e496c(column, j, &pos);
            func_0204b7c8(wk->unkAC0[index][j], &proxy);
            box = func_ov119_021d8af4(column, wk);
            slot = func_ov119_021d8b0c(column, j);
            slot += box * 30;
            func_0204b454(wk->unkAC0[index][j], &screenPos, CLACT_VRAM_SUB);
            if (async == TRUE && screenPos.x >= -16 && screenPos.x <= 272) {
                if (!NNS_GfdRegisterNewVramTransferTask(35, ImageProxy_GetLocation(&proxy, NNS_G2D_VRAM_TYPE_2DSUB), (u8 *)wk->unk1184 + slot * 0x200, 0x200)) {
                    MI_CpuCopy8((u8 *)wk->unk1184 + slot * 0x200, (void *)((uintptr_t)G2S_GetOBJCharPtr() + ImageProxy_GetLocation(&proxy, NNS_G2D_VRAM_TYPE_2DSUB)), 0x200);
                }
            } else {
                MI_CpuCopy8((u8 *)wk->unk1184 + slot * 0x200, (void *)((uintptr_t)G2S_GetOBJCharPtr() + ImageProxy_GetLocation(&proxy, NNS_G2D_VRAM_TYPE_2DSUB)), 0x200);
            }
            func_ov119_021e4ad0(wk, column, j, pkm, isParty, index);
            func_0204b6f4(wk->unkAC0[index][j], func_0201bbe8(pkm), 1);
            func_0204b92c(wk->unkAC0[index][j], FALSE);
            func_0204b3dc(wk->unkAC0[index][j], TRUE);
        }
        if (!PML_PkmGetParam(pkm, PKM_PARAM_IS_EGG, NULL) && func_ov119_021e4a84(wk, species)) {
            wk->unkEBC[index][j] = TRUE;
        }
    }
}


int func_ov119_021e4dbc(PokemonTradeWork *wk) {
    int x = wk->unk107C;
    int column;

    if (x < 48) {
        column = 0;
    } else if (x < 96) {
        column = 1;
    } else {
        int box, offset;
        int boxesX = x - 96;

        box = boxesX / 160;
        offset = boxesX - box * 160;
        column = box * BOX_COLUMNS + PARTY_COLUMNS;
        if (offset >= 8) {
            if (offset >= 152) {
                column += BOX_COLUMNS - 1;
            } else {
                column += (offset - 8) / 24;
            }
        }
    }
    return column;
}

BOOL func_ov119_021e4e0c(PokemonTradeWork *wk, int *column) {
    int first = func_ov119_021e4dbc(wk);
    int last = first + 10;

    *column = first + 2;
    if (first + 2 >= wk->unk634) {
        *column = first + 2 - wk->unk634;
    }
    if (last >= wk->unk634) {
        int cursor = wk->unkF74;

        if (cursor < 20) {
            cursor = cursor + 1 + wk->unk634;
        }
        if (first <= cursor && cursor <= last) {
            return TRUE;
        }
    } else if (first <= wk->unkF74 && wk->unkF74 <= last) {
        return TRUE;
    }
    return FALSE;
}

void func_ov119_021e4e64(BoxSaveAccessor *boxes, PokemonTradeWork *wk, BOOL async) {
    int i, j;
    int column = func_ov119_021e4dbc(wk);

    if (wk->firstColumn != column) {
        wk->firstColumn = column;
        for (i = 0; i < ICON_COLUMNS; i++) {
            for (j = 0; j < COLUMN_ROWS; j++) {
                func_0204b3dc(wk->unkBB0[i][j], FALSE);
                wk->unkEBC[i][j] = FALSE;
            }
        }
        wk->unk1090 = column;
        func_ov119_021e5164(wk);
        for (i = 0; i < ICON_COLUMNS; i++) {
            func_ov119_021e4b00(wk, boxes, column, func_ov119_021e5158(wk->firstColumn + i), async);
            column++;
            if (column >= wk->unk634) {
                column = 0;
            }
        }
    }
    func_ov119_021e4f28(wk);
}


void func_ov119_021e4f28(PokemonTradeWork *wk) {
    int i, j;
    int wrap = 0;
    int column = wk->firstColumn;

    for (i = 0; i < ICON_COLUMNS; i++) {
        int index = func_ov119_021e5158(wk->firstColumn + i);
        int boxX;

        if (column >= wk->unk634) {
            column -= wk->unk634;
            wrap = wk->unk63C;
        }
        j = 0;
        boxX = (column - PARTY_COLUMNS) / BOX_COLUMNS * 160;
        boxX += (column - PARTY_COLUMNS) % BOX_COLUMNS * 24 + 20;
        boxX += 96;
        for (; j < COLUMN_ROWS; j++) {
            int y = j * 24 + 32;
            int x;
            ClActorPos pos, markPos;

            if (column == 0) {
                x = 28;
            } else if (column == 1) {
                x = 60;
            } else {
                x = boxX;
            }
            pos.x = wrap + (x - wk->unk107C);
            pos.y = y;
            markPos = pos;
            if (func_ov119_021df6b4(wk, column, j) && func_020355c4() == TRUE) {
                pos.y -= 8;
                func_0204b800(wk->unkAC0[index][j], 11);
            } else {
                func_0204b800(wk->unkAC0[index][j], 28 - i);
            }
            func_0204b404(wk->unkAC0[index][j], &pos, CLACT_VRAM_SUB);
            if (wk->unkEBC[index][j]) {
                wk->unkDCC[index][j] = markPos;
            }
            pos.y = y + 3;
            func_0204b404(wk->unkBB0[index][j], &pos, CLACT_VRAM_SUB);
        }
        column++;
    }
}


void func_ov119_021e5098(PokemonTradeWork *wk, int box) {
    ArcTool *arc;
    int i, slot;
    void *file;
    NNSG2dCharacterData *chars;

    if (box == 0) {
        wk->unk1184 = GFL_HeapAllocate(wk->heapId, (MAX_BOXES * 30 + 6) * 0x200, FALSE, "pokemontrade_2d.c", 1291);
    }
    arc = GFL_ArcSysCreateFileHandle(ARCID_POKEICON, wk->heapId);
    while (box < MAX_BOXES + 1) {
        slot = box * 30;
        for (i = 0; i < 30; i++) {
            BoxPkm *pkm;
            u32 icon;

            if (box == MAX_BOXES && i == 6) {
                break;
            }
            pkm = func_ov119_021d8dd0(wk->boxes, box, i, wk);
            if (pkm != NULL) {
                icon = func_0201ba68(pkm);
            } else {
                icon = PokeParty_GetIconIndex(0, 0, 0, FALSE);
            }
            file = GFL_G2DIOReadBGNCGRArc(arc, icon, FALSE, &chars, wk->heapId);
            MI_CpuCopy8(chars->pRawData, (u8 *)wk->unk1184 + slot * 0x200, 0x200);
            slot++;
            GFL_HeapFree(file);
        }
        break;
    }
    GFL_ArcToolFree(arc);
}





// The icon column that a column of the strip is set up in
int func_ov119_021e5158(int column) {
    return column % ICON_COLUMNS;
}

void func_ov119_021e5164(PokemonTradeWork *wk) {
    int i, j;

    for (i = 0; i < ICON_COLUMNS; i++) {
        for (j = 0; j < COLUMN_ROWS; j++) {
            func_0204b3dc(wk->unkAC0[i][j], FALSE);
        }
    }
}

ClActor *func_ov119_021e51a4(PokemonTradeWork *wk, int x, int y, int *box, int *slot, int *column, int *row, int *index) {
    int i, j;
    int iconColumn;

    for (i = 0; i < ICON_COLUMNS; i++) {
        for (j = 0; j < COLUMN_ROWS; j++) {
            ClActorPos pos;

            func_0204b454(wk->unkAC0[i][j], &pos, CLACT_VRAM_SUB);
            if (pos.x <= x && x < pos.x + 24 && pos.y <= y && y < pos.y + 24) {
                if (box != NULL) {
                    int slotNum;

                    iconColumn = wk->unkD90[i][j];
                    if (iconColumn == 0xff) {
                        return NULL;
                    }
                    slotNum = func_ov119_021d8b0c(iconColumn, j);
                    if (slotNum != -1) {
                        *box = func_ov119_021d8af4(iconColumn, wk);
                        *slot = func_ov119_021d8b0c(iconColumn, j);
                    } else {
                        return NULL;
                    }
                }
                if (column != NULL) {
                    *column = iconColumn;
                    *row = j;
                    *index = i;
                }
                return wk->unkAC0[i][j];
            }
        }
    }
    return NULL;
}

void func_ov119_021e5284(PokemonTradeWork *wk, BOOL other) {
    ArcTool *arc;

    func_ov119_021e57e8(1);
    wk->unk11EC = TRUE;
    {
        BGSetup setup = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xe800, GX_BG_CHARBASE_0x10000, 0x8000, GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE };

        GFL_BGSysCreateBG(6, &setup, BGMODE_TEXT);
        GFL_BGSysFillChar(6, 0, 1, 0);
        GFL_BGSysFillScrArea(6, 0, 0, 0, 32, 24, BGSYS_FILL_TILE_PALETTE);
        GFL_BGSysLoadScr(6);
    }
    {
        BGSetup setup = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_256, GX_BG_SCRBASE_0xf000, GX_BG_CHARBASE_0x00000, 0x8000, GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE };

        GFL_BGSysCreateBG(7, &setup, BGMODE_EXTENDED);
    }
    if (!other) {
        arc = GFL_ArcSysCreateFileHandle(0x67, wk->heapId);
        GFL_G2DIOLoadArcNCLRDefault(arc, 5, 6, 0x6000, 0, wk->heapId);
        GFL_BGSysLoadArcNCGRStatic(arc, 4, 7, 0, 0, FALSE, wk->heapId);
        loadBGScrToVramByFileNoReserveNegAlign(arc, 6, 7, 0, 0, FALSE, wk->heapId);
        GFL_BGSysMoveBG(7, BG_MOVE_SET_X, 0);
        GFL_ArcToolFree(arc);
    } else {
        arc = GFL_ArcSysCreateFileHandle(0x68, wk->heapId);
        GFL_G2DIOLoadArcNCLRDefault(arc, 1, 6, 0x6000, 0, wk->heapId);
        GFL_BGSysLoadArcNCGRStatic(arc, 0, 7, 0, 0, FALSE, wk->heapId);
        loadBGScrToVramByFileNoReserveNegAlign(arc, 2, 7, 0, 0, FALSE, wk->heapId);
        GFL_BGSysMoveBG(7, BG_MOVE_SET_X, 0);
        GFL_ArcToolFree(arc);
    }
}




void func_ov119_021e53cc(PokemonTradeWork *wk) {
    if (wk->unk11EC) {
        GFL_BGSysFreeFilledChar(6, 1, 0);
        GFL_BGSysReleaseBG(6);
        GFL_BGSysReleaseBG(7);
        wk->unk11EC = FALSE;
    }
}


// Sets up the VRAM and the cell actor system, with every BG hidden
void func_ov119_021e53f8(PokemonTradeWork *wk) {
    GFL_BGSysSetVRAMBanks(&data_ov119_021e7d40);
    ClActSys_Create(&data_ov119_021e7cc8, &data_ov119_021e7d40, wk->heapId);
    func_ov119_021e57e8(0);
    GFL_BGSysSetEnabledBGsA(0);
    GFL_BGSysSetEnabledBGsB(0);
}

// Sets up the upper screen's BGs 1 to 3, over the 3D of BG 0
void func_ov119_021e5430(PokemonTradeWork *wk) {
    {
        BGSetup setup = { 0,
                          0,
                          0x800,
                          0,
                          BGRES_256x256,
                          GX_BG_COLORMODE_16,
                          GX_BG_SCRBASE_0xe000,
                          GX_BG_CHARBASE_0x00000,
                          0x8000,
                          GX_BG_EXTPLTT_01,
                          0,
                          GX_BG_AREAOVER_XLU,
                          FALSE };

        GFL_BGSysCreateBG(3, &setup, BGMODE_TEXT);
        GFL_BGSysFillChar(3, 0, 1, 0);
        GFL_BGSysFillScrArea(3, 0, 0, 0, 32, 24, BGSYS_FILL_TILE_PALETTE);
        GFL_BGSysLoadScr(3);
    }
    {
        BGSetup setup = { 0,
                          0,
                          0x800,
                          0,
                          BGRES_256x256,
                          GX_BG_COLORMODE_16,
                          GX_BG_SCRBASE_0xf000,
                          GX_BG_CHARBASE_0x08000,
                          0x8000,
                          GX_BG_EXTPLTT_01,
                          3,
                          GX_BG_AREAOVER_XLU,
                          FALSE };

        GFL_BGSysCreateBG(2, &setup, BGMODE_TEXT);
        GFL_BGSysFillChar(2, 0, 1, 0);
        GFL_BGSysFillScrArea(2, 0, 0, 0, 32, 24, BGSYS_FILL_TILE_PALETTE);
        GFL_BGSysLoadScr(2);
    }
    {
        BGSetup setup = { 0,
                          0,
                          0x800,
                          0,
                          BGRES_256x256,
                          GX_BG_COLORMODE_16,
                          GX_BG_SCRBASE_0xf800,
                          GX_BG_CHARBASE_0x08000,
                          0x8000,
                          GX_BG_EXTPLTT_01,
                          1,
                          GX_BG_AREAOVER_XLU,
                          FALSE };

        GFL_BGSysCreateBG(1, &setup, BGMODE_TEXT);
        GFL_BGSysFillScrArea(1, 0, 0, 0, 32, 24, BGSYS_FILL_TILE_PALETTE);
        GFL_BGSysLoadScr(1);
    }
    GFL_BGSysSet3DBGPriority(0);
    GFL_BGSysSetBGEnabled(0, TRUE);
    GFL_BGSysSetBGPriority(0, 0);
}

// The 3D system calls this once it is set up
void func_ov119_021e5520(void) {
    u32 i;

    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG0, TRUE);
    G3X_SetShading(GX_SHADING_HIGHLIGHT);
    G3X_AntiAlias(TRUE);
    G3X_AlphaBlend(TRUE);
    G3X_EdgeMarking(FALSE);
    G3X_SetFog(TRUE, 0, 0, 0);
    G3X_SetClearColor(GX_RGB(0, 0, 0), 0, 0x7fff, 63, FALSE);
    G3_ViewPort(0, 0, 255, 191);
    for (i = 0; i < NELEMS(data_ov119_021e7d00); i++) {
        GFL_G3DSysLightSet(i, &data_ov119_021e7d00[i]);
    }
    G2_SetBG0Priority(2);
}

// Sets up the 3D system and the upper screen's camera
void func_ov119_021e55bc(PokemonTradeWork *wk) {
    // Nothing reads it, but Black's .rodata has these 16 zero bytes after the camera's vectors (pokebw2's guess of
    // its type and name: the eight colors that G3X_SetEdgeColorTable takes)
    static const GXRgb sEdgeColors[8] = { 0 };
    // The camera: where it is, what it looks at, and which way is up. Nothing reads the up either: the camera is
    // given the local copy below
    static const VecFx32 sCameraPos = { 0, 0, FX32_CONST(200) };
    static const VecFx32 sCameraTarget = { 0, 0, 0 };
    static const VecFx32 sCameraUp = { 0, FX32_ONE, 0 };

    GFL_G3DSysCreate(FALSE, 2, FALSE, 1, 0, HEAPID_TAIL(wk->heapId), func_ov119_021e5520);
    {
        HeapID heapId = HEAPID_TAIL(wk->heapId);
        VecFx32 up = { 0, FX32_ONE, 0 };

        wk->unk810 = GFL_G3DCameraCreate(G3DCAM_PROJECTION_PERSPECTIVE, FX_SinIdx(DEG_TO_IDX(20)),
                                         FX_CosIdx(DEG_TO_IDX(20)), FX32_CONST(4.0 / 3.0), 0, FX32_ONE, FX32_CONST(1024), 0,
                                         &sCameraPos, &up, &sCameraTarget, heapId);
    }
    GFL_G3DCameraFlush(wk->unk810);
    G3X_EdgeMarking(FALSE);
    GFL_G3DSysSetSwapBufferParams(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
}

void func_ov119_021e5680(PokemonTradeWork *wk) {
    {
        BGSetup setup = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xe000, GX_BG_CHARBASE_0x10000, 0x10000, GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, FALSE };

        GFL_BGSysCreateBG(4, &setup, BGMODE_TEXT);
        GFL_BGSysFillScrArea(4, 0, 0, 0, 32, 24, BGSYS_FILL_TILE_PALETTE);
    }
    {
        BGSetup setup = { 0, 0, 0x1000, 0, BGRES_512x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xc000, GX_BG_CHARBASE_0x10000, 0x10000, GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE };

        GFL_BGSysCreateBG(5, &setup, BGMODE_TEXT);
        GFL_BGSysFillChar(5, 0, 1, 0);
        GFL_BGSysFillScrArea(5, 0, 0, 0, 32, 24, BGSYS_FILL_TILE_PALETTE);
        GFL_BGSysLoadScr(5);
    }
    {
        BGSetup setup = { 0, 0, 0x1000, 0, BGRES_512x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf000, GX_BG_CHARBASE_0x08000, 0x8000, GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE };

        GFL_BGSysCreateBG(7, &setup, BGMODE_TEXT);
        GFL_BGSysFillScrArea(7, 0, 0, 0, 64, 24, BGSYS_FILL_TILE_PALETTE);
        GFL_BGSysLoadScr(7);
    }
    {
        BGSetup setup = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xe800, GX_BG_CHARBASE_0x00000, 0x8000, GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE };

        GFL_BGSysCreateBG(6, &setup, BGMODE_TEXT);
        GFL_BGSysFillChar(6, 0, 1, 0);
        GFL_BGSysFillScrArea(6, 0, 0, 0, 32, 24, BGSYS_FILL_TILE_PALETTE);
        GFL_BGSysLoadScr(6);
    }
    wk->unk11EB = TRUE;
}



void func_ov119_021e57a4(PokemonTradeWork *wk) {
    if (wk->unk11EB == TRUE) {
        GFL_BGSysFreeFilledChar(4, 1, 0);
        GFL_BGSysFreeFilledChar(6, 1, 0);
        GFL_BGSysReleaseBG(4);
        GFL_BGSysReleaseBG(5);
        GFL_BGSysReleaseBG(6);
        GFL_BGSysReleaseBG(7);
        wk->unk11EB = FALSE;
    }
}


// The trade's display, and the trade demo's, whose lower screen shows a bitmap BG
// pokebw2's sLCDConfigs
static const BGSysLCDConfig data_ov119_021e7d20[] = {
    { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_3D },
    { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_3, GX_BG0_AS_3D },
};

// Sets the display up for the trade, or for the trade demo
void func_ov119_021e57e8(int config) {
    GFL_BGSysSetLCDConfig(&data_ov119_021e7d20[config]);
}

// Loads the sprites of the upper screen once
void func_ov119_021e57fc(PokemonTradeWork *wk) {
    if (wk->objRes[TRADE_OBJRES_CHAR_MAIN] == 0) {
        ArcTool *arc = GFL_ArcSysCreateFileHandle(0x66, wk->heapId);

        wk->objRes[TRADE_OBJRES_CHAR_MAIN] = func_0204a6c8(arc, 21, FALSE, CLACT_VRAM_MAIN, wk->heapId);
        wk->objRes[TRADE_OBJRES_PLTT_MAIN] = func_0204ac18(arc, 18, CLACT_VRAM_MAIN, 0xc0, 0, 6, wk->heapId);
        wk->objRes[TRADE_OBJRES_CELL_MAIN] = func_0204af28(arc, 20, 19, wk->heapId);
        GFL_ArcToolFree(arc);
    }
}

void func_ov119_021e5868(PokemonTradeWork *wk) {
    if (wk->objRes[TRADE_OBJRES_CHAR_MAIN] != 0) {
        func_0204a8d4(wk->objRes[TRADE_OBJRES_CHAR_MAIN]);
        func_0204ada4(wk->objRes[TRADE_OBJRES_PLTT_MAIN]);
        func_0204afd8(wk->objRes[TRADE_OBJRES_CELL_MAIN]);
        wk->objRes[TRADE_OBJRES_CHAR_MAIN] = 0;
        wk->objRes[TRADE_OBJRES_PLTT_MAIN] = 0;
        wk->objRes[TRADE_OBJRES_CELL_MAIN] = 0;
    }
}

void func_ov119_021e58a0(PokemonTradeWork *wk) {
    ClActorSetup setup;
    ArcTool *arc = GFL_ArcSysCreateFileHandle(0x66, wk->heapId);

    wk->objRes[TRADE_OBJRES_CHAR_SUB] = func_0204a6c8(arc, 24, FALSE, CLACT_VRAM_SUB, wk->heapId);
    wk->objRes[TRADE_OBJRES_PLTT_SUB] = func_0204ac18(arc, 18, CLACT_VRAM_SUB, 0xc0, 0, 6, wk->heapId);
    wk->objRes[TRADE_OBJRES_CELL_SUB] = func_0204af28(arc, 23, 22, wk->heapId);
    GFL_ArcToolFree(arc);
    setup.x = 128;
    setup.y = 180;
    setup.sequence = 2;
    setup.priority = 14;
    setup.bgPriority = 0;
    wk->actors[2] = func_0204b294(wk->clactUnit, wk->objRes[TRADE_OBJRES_CHAR_SUB], wk->objRes[TRADE_OBJRES_PLTT_SUB],
                                  wk->objRes[TRADE_OBJRES_CELL_SUB], &setup, CLACT_VRAM_SUB, wk->heapId);
    func_0204b92c(wk->actors[2], FALSE);
    func_0204b3dc(wk->actors[2], TRUE);
}


void func_ov119_021e5958(PokemonTradeWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(0x66, wk->heapId);

    GFL_G2DIOLoadNSCRSync(arc, 29, 2, 0, CHAR_POS(wk->unk86C), 0, FALSE, wk->heapId);
    GFL_ArcToolFree(arc);
}

void func_ov119_021e5998(PokemonTradeWork *wk) {
    ClActorSetup setup;
    ClActorSetup iconSetup;
    ArcTool *arc;
    BoxPkm *pkm0, *pkm1;

    setup.x = 128;
    setup.y = 16;
    setup.sequence = 19;
    setup.priority = 14;
    setup.bgPriority = 1;
    wk->actors[3] = func_0204b294(wk->clactUnit, wk->objRes[TRADE_OBJRES_CHAR_SUB], wk->objRes[TRADE_OBJRES_PLTT_SUB],
                                  wk->objRes[TRADE_OBJRES_CELL_SUB], &setup, CLACT_VRAM_SUB, wk->heapId);
    func_0204b92c(wk->actors[3], TRUE);
    func_0204b3dc(wk->actors[3], TRUE);
    setup.x = 96;
    setup.y = 16;
    setup.sequence = 0;
    setup.priority = 11;
    setup.bgPriority = 1;
    wk->actors[4] = func_0204b294(wk->clactUnit, wk->objRes[TRADE_OBJRES_CHAR_SUB], wk->objRes[TRADE_OBJRES_PLTT_SUB],
                                  wk->objRes[TRADE_OBJRES_CELL_SUB], &setup, CLACT_VRAM_SUB, wk->heapId);
    func_0204b92c(wk->actors[4], TRUE);
    if (func_020355c4()) {
        func_0204b3dc(wk->actors[4], FALSE);
    }

    arc = GFL_ArcSysCreateFileHandle(ARCID_POKEICON, wk->heapId);
    pkm0 = func_0201d624(func_ov119_021d8fa4(wk, 0));
    pkm1 = func_0201d624(func_ov119_021d8fa4(wk, 1));
    wk->objRes[TRADE_OBJRES_CHAR_ICON0] = func_0204a6c8(arc, func_0201ba68(pkm0), FALSE, CLACT_VRAM_SUB, wk->heapId);
    wk->objRes[TRADE_OBJRES_CHAR_ICON1] = func_0204a6c8(arc, func_0201ba68(pkm1), FALSE, CLACT_VRAM_SUB, wk->heapId);
    iconSetup.x = 96;
    iconSetup.y = 12;
    iconSetup.sequence = 0;
    iconSetup.priority = 12;
    iconSetup.bgPriority = 1;
    wk->actors[5] = func_0204b294(wk->clactUnit, wk->objRes[TRADE_OBJRES_CHAR_ICON0], wk->objRes[TRADE_OBJRES_PLTT_ICON],
                                  wk->objRes[TRADE_OBJRES_CELL_ICON], &iconSetup, CLACT_VRAM_SUB, wk->heapId);
    func_0204b92c(wk->actors[5], FALSE);
    func_0204b3dc(wk->actors[5], TRUE);
    func_0204b6f4(wk->actors[5], func_0201bbe8(pkm0), 1);
    iconSetup.x = 160;
    wk->actors[6] = func_0204b294(wk->clactUnit, wk->objRes[TRADE_OBJRES_CHAR_ICON1], wk->objRes[TRADE_OBJRES_PLTT_ICON],
                                  wk->objRes[TRADE_OBJRES_CELL_ICON], &iconSetup, CLACT_VRAM_SUB, wk->heapId);
    func_0204b92c(wk->actors[6], FALSE);
    func_0204b3dc(wk->actors[6], TRUE);
    func_0204b6f4(wk->actors[6], func_0201bbe8(pkm1), 1);
    GFL_ArcToolFree(arc);
}


void func_ov119_021e5b6c(PokemonTradeWork *wk, int side, BOOL level) {
    ClActorPos pos;

    if (level) {
        pos.x = 96;
        pos.y = 12;
        func_0204b404(wk->actors[5], &pos, CLACT_VRAM_SUB);
        pos.x = 160;
        pos.y = 12;
        func_0204b404(wk->actors[6], &pos, CLACT_VRAM_SUB);
    } else {
        pos.x = 96;
        pos.y = side == 0 ? 8 : 12;
        func_0204b404(wk->actors[5], &pos, CLACT_VRAM_SUB);
        pos.x = 160;
        pos.y = side != 0 ? 8 : 12;
        func_0204b404(wk->actors[6], &pos, CLACT_VRAM_SUB);
    }
}

void func_ov119_021e5be4(PokemonTradeWork *wk) {
    if (wk->actors[3] != NULL) {
        func_0204b3b4(wk->actors[3]);
    }
    if (wk->actors[4] != NULL) {
        func_0204b3b4(wk->actors[4]);
    }
    if (wk->actors[5] != NULL) {
        func_0204b3b4(wk->actors[5]);
    }
    if (wk->actors[6] != NULL) {
        func_0204b3b4(wk->actors[6]);
    }
    wk->actors[3] = NULL;
    wk->actors[4] = NULL;
    wk->actors[5] = NULL;
    wk->actors[6] = NULL;
    if (wk->objRes[TRADE_OBJRES_CHAR_ICON0] != 0) {
        func_0204a8d4(wk->objRes[TRADE_OBJRES_CHAR_ICON0]);
    }
    if (wk->objRes[TRADE_OBJRES_CHAR_ICON1] != 0) {
        func_0204a8d4(wk->objRes[TRADE_OBJRES_CHAR_ICON1]);
    }
    wk->objRes[TRADE_OBJRES_CHAR_ICON0] = 0;
    wk->objRes[TRADE_OBJRES_CHAR_ICON1] = 0;
}

// Where the windows of the letters of the search by initial go, in tiles
// pokebw2's sInitialWindowPos
static u32 data_ov119_021e81d4[][2] = {
#include "asset/pokemontrade_2d_data_ov119_021e81d4.inc"
};

void func_ov119_021e5c64(PokemonTradeWork *wk) {
    ArcTool *arc;
    u32 i;

    GFL_BGSysSetBGEnabled(4, FALSE);
    GFL_BGSysSetBGEnabled(6, FALSE);
    arc = GFL_ArcSysCreateFileHandle(0x66, wk->heapId);
    GFL_G2DIOLoadNSCRSync(arc, 16, 4, 0, CHAR_POS(wk->unk870), 0, FALSE, wk->heapId);
    GFL_BGSysQueueScrLoad(4);
    GFL_BGSysMoveBG(4, BG_MOVE_SET_X, -4);
    GFL_BGSysMoveBG(6, BG_MOVE_SET_X, -4);
    GFL_ArcToolFree(arc);
    GFL_TextRndUpdateColorIndexLUT(1, 2, 15);
    for (i = 0; i < NELEMS(data_ov119_021e81d4); i++) {
        wk->unk750[i] = BmpWin_CreateDynamic(6, data_ov119_021e81d4[i][0], data_ov119_021e81d4[i][1], 2, 2, 14, FALSE);
        GFL_MsgDataLoadStrbuf(wk->msgData, 53 + i, wk->drawStr);
        GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(wk->unk750[i]), 0, 0, wk->drawStr, wk->font);
        BmpWin_FlushMap(wk->unk750[i]);
        BmpWin_FlushChar(wk->unk750[i]);
    }
    GFL_BGSysQueueScrLoad(6);
    GFL_BGSysSetBGEnabled(4, TRUE);
    GFL_BGSysSetBGEnabled(6, TRUE);
}


void func_ov119_021e5d84(PokemonTradeWork *wk) {
    u32 i;

    GFL_BGSysSetBGEnabled(4, FALSE);
    GFL_BGSysMoveBG(4, BG_MOVE_SET_X, 0);
    GFL_BGSysMoveBG(6, BG_MOVE_SET_X, 0);
    if (wk->actors[2] != NULL) {
        func_0204b3dc(wk->actors[2], TRUE);
    }
    if (wk->actors[7] != NULL) {
        func_0204b3dc(wk->actors[7], TRUE);
    }
    for (i = 0; i < 0x1a; i++) {
        if (wk->unk750[i] != NULL) {
            func_020456f8(wk->unk750[i]);
            func_0201f7a8(wk->unk750[i], 2);
            BmpWin_Free(wk->unk750[i]);
            wk->unk750[i] = NULL;
        }
    }
    GFL_BGSysQueueScrLoad(6);
}

void func_ov119_021e5dfc(PokemonTradeWork *wk, int side, PartyPkm *pkm) {
    ResSprite *sprite = &wk->infoIcons[0];
    UIObjResSetup setup;
    int x, y;
    BOOL mail = FALSE;
    u32 item = PokeParty_GetParam(pkm, PKM_PARAM_ITEM, NULL);

    if (PML_ItemIsMail(item)) {
        mail = TRUE;
    }
    if (sprite->actor != NULL) {
        if (item == ITEM_NONE) {
            func_0204b3dc(sprite->actor, FALSE);
            return;
        }
        func_0204b878(sprite->actor, mail);
        func_0204b3dc(sprite->actor, TRUE);
        return;
    }
    if (item == ITEM_NONE) {
        return;
    }
    switch (side) {
    case 0:
    case 1:
        x = side * 128 + 16;
        y = 132;
        setup.vramType = CLACT_VRAM_SUB;
        setup.paletteOffset = 12;
        break;
    case 2:
        x = 32;
        y = 172;
        setup.vramType = CLACT_VRAM_MAIN;
        setup.paletteOffset = 6;
        break;
    }
    setup.flags = 0;
    setup.arcId = getUINarcIdx();
    setup.paletteFile = func_02027694();
    setup.charFile = func_02027698();
    setup.cellFile = func_0202769c(2);
    setup.animFile = func_020276a0(2);
    setup.paletteStart = 0;
    setup.paletteCount = 1;
    func_ov067_021b6208(&sprite->res, &setup, wk->clactUnit, wk->heapId);
    sprite->actor = func_ov067_021b629c(&sprite->res, wk->clactUnit, x, y, mail, wk->heapId);
}





void func_ov119_021e5ef8(ResSprite *sprite) {
    if (sprite->actor != NULL) {
        func_0204b3b4(sprite->actor);
        func_ov067_021b6284(&sprite->res);
        sprite->actor = NULL;
    }
}

void func_ov119_021e5f14(PokemonTradeWork *wk, int side, BOOL summary, PartyPkm *pkm) {
    ResSprite *sprite = &wk->infoIcons[1];
    UIObjResSetup setup;
    BOOL pokerus = pokerusDuration(pkm);

    if (sprite->actor != NULL) {
        if (!pokerus) {
            func_0204b3dc(sprite->actor, FALSE);
        } else {
            func_0204b3dc(sprite->actor, TRUE);
        }
        return;
    }
    if (!pokerus) {
        if (!summary) {
            func_ov119_021e6008(wk, side, pkm);
        }
        return;
    }
    if (summary) {
        setup.vramType = CLACT_VRAM_MAIN;
        setup.paletteOffset = 0;
    } else {
        setup.vramType = CLACT_VRAM_SUB;
        setup.paletteOffset = 11;
    }
    setup.flags = 0;
    setup.arcId = getUINarcIdx();
    setup.paletteFile = func_020276b4();
    setup.charFile = func_020276b8();
    setup.cellFile = func_020276bc(2);
    setup.animFile = func_020276c0(2);
    setup.paletteStart = 0;
    setup.paletteCount = 1;
    func_ov067_021b6208(&sprite->res, &setup, wk->clactUnit, wk->heapId);
    if (summary) {
        sprite->actor = func_ov067_021b629c(&sprite->res, wk->clactUnit, 244, 104, 0, wk->heapId);
    } else {
        sprite->actor = func_ov067_021b629c(&sprite->res, wk->clactUnit, side * 128 + 108, 24, 0, wk->heapId);
    }
}

void func_ov119_021e6008(PokemonTradeWork *wk, int side, PartyPkm *pkm) {
    ResSprite *sprite = &wk->infoIcons[2];
    UIObjResSetup setup;
    BOOL cured = pokeHasPkrs(pkm);

    if (cured) {
        if (sprite->actor != NULL) {
            if (!cured) {
                func_0204b3dc(sprite->actor, FALSE);
            } else {
                func_0204b3dc(sprite->actor, TRUE);
            }
            return;
        }
        setup.vramType = CLACT_VRAM_SUB;
        setup.paletteOffset = 14;
        setup.flags = 0;
        setup.arcId = getUINarcIdx();
        setup.paletteFile = func_02027748();
        setup.charFile = func_0202774c(2);
        setup.cellFile = func_02027750(2);
        setup.animFile = func_02027754(2);
        setup.paletteStart = 0;
        setup.paletteCount = 1;
        func_ov067_021b6208(&sprite->res, &setup, wk->clactUnit, wk->heapId);
        sprite->actor = func_ov067_021b629c(&sprite->res, wk->clactUnit, side * 128 + 108, 20, 13, wk->heapId);
    }
}

void func_ov119_021e60bc(PokemonTradeWork *wk, PartyPkm *pkm, BOOL isEgg) {
    u32 marks = PokeParty_GetParam(pkm, PKM_PARAM_MARKINGS, NULL);
    UIObjResSetup setup;
    u32 xs[] = { 25, 26, 27, 28, 29, 30, 20, 21 };
    int anims[][2] = { { 0, 1 }, { 2, 3 }, { 4, 5 }, { 6, 7 }, { 8, 9 }, { 10, 11 }, { 12, -1 }, { 13, -1 } };
    long i;

    setup.vramType = CLACT_VRAM_MAIN;
    setup.flags = 0;
    setup.arcId = getUINarcIdx();
    setup.paletteFile = func_02027748();
    setup.charFile = func_0202774c(2);
    setup.cellFile = func_02027750(2);
    setup.animFile = func_02027754(2);
    setup.paletteOffset = 13;
    setup.paletteStart = 0;
    setup.paletteCount = 1;
    if (!isEgg && PokeParty_IsRare(pkm)) {
        marks |= 1 << 6;
    }
    if (pokeHasPkrs(pkm)) {
        marks |= 1 << 7;
    }
    if (!wk->markIcons.loaded) {
        func_ov067_021b6208(&wk->markIcons.res, &setup, wk->clactUnit, wk->heapId);
        wk->markIcons.loaded = TRUE;
    }
    for (i = 0; i < 8; i++) {
        ClActor *icon = wk->markIcons.icons[i];
        int anim;

        if (marks & (1 << i)) {
            anim = anims[i][0];
        } else {
            anim = anims[i][1];
        }
        if (icon != NULL) {
            if (anim == -1) {
                func_0204b3dc(icon, FALSE);
            } else if (isEgg && i == 6) {
                func_0204b3dc(icon, FALSE);
            } else {
                func_0204b878(icon, anim);
                func_0204b3dc(icon, TRUE);
            }
        } else {
            wk->markIcons.icons[i] = func_ov067_021b629c(&wk->markIcons.res, wk->clactUnit, xs[i] * 8, 101,
                                                                anim == -1 ? anims[i][0] : anim, wk->heapId);
            icon = wk->markIcons.icons[i];
            if (anim == -1) {
                func_0204b3dc(icon, FALSE);
            } else if (isEgg && i == 6) {
                func_0204b3dc(icon, FALSE);
            }
        }
    }
}




void func_ov119_021e625c(PokemonTradeWork *wk) {
    TradeMarkIcons *marks = &wk->markIcons;
    int i;

    for (i = 0; i < 8; i++) {
        if (marks->icons[i] != NULL) {
            func_0204b3b4(marks->icons[i]);
            marks->icons[i] = NULL;
        }
    }
    if (marks->loaded) {
        func_ov067_021b6284(&marks->res);
        marks->loaded = FALSE;
    }
}


// Loads the box palette into a BG palette slot, with the colours of the UI's last four after it
void func_ov119_021e6294(PokemonTradeWork *wk, u32 palette, u32 type) {
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, type, palette * 32, 32, wk->heapId);
    GFL_G2DIOLoadNCLR(getUINarcIdx(), 31, type, 0x1c, palette * 32 + 0x1c, 4, wk->heapId);
}

void func_ov119_021e62d4(PokemonTradeWork *wk, int side, int index) {
    if (wk->negoIcons[side][index] != NULL) {
        func_0204b3b4(wk->negoIcons[side][index]);
        wk->negoIcons[side][index] = NULL;
        func_0204a8d4(wk->negoIconChars[side][index]);
    }
}


// The negotiation's icon positions, the main screen's and the sub screen's, by side * 3 + index
static const int data_ov119_021e7d70[6][2] = {
#include "asset/pokemontrade_2d_data_ov119_021e7d70.inc"
};
static const int data_ov119_021e7da0[6][2] = {
#include "asset/pokemontrade_2d_data_ov119_021e7da0.inc"
};

void func_ov119_021e6308(PokemonTradeWork *wk, int side, BOOL visible) {
    int i;

    for (i = 0; i < 3; i++) {
        if (wk->negoIcons[side][i] != NULL) {
            func_0204b3dc(wk->negoIcons[side][i], visible);
        }
    }
}

void func_ov119_021e6334(PokemonTradeWork *wk, int side, int index, PartyPkm *pkm, BOOL onMain, BOOL visible) {
    BoxPkm *boxPkm = func_020185f0(pkm);
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_POKEICON, wk->heapId);
    ClActorSetup setup;
    u32 vramType;

    func_ov119_021e62d4(wk, side, index);
    if (onMain) {
        setup.x = data_ov119_021e7d70[side * 3 + index][0];
        setup.y = data_ov119_021e7d70[side * 3 + index][1];
        vramType = CLACT_VRAM_MAIN;
    } else {
        setup.x = data_ov119_021e7da0[side * 3 + index][0];
        setup.y = data_ov119_021e7da0[side * 3 + index][1];
        vramType = CLACT_VRAM_SUB;
    }
    setup.sequence = 1;
    setup.priority = 16;
    setup.bgPriority = 1;
    wk->negoIconChars[side][index] = func_0204a6c8(arc, func_0201ba68(boxPkm), FALSE, vramType, wk->heapId);
    wk->negoIcons[side][index] = func_0204b294(wk->clactUnit, wk->negoIconChars[side][index], wk->objRes[TRADE_OBJRES_PLTT_NEGO], wk->objRes[TRADE_OBJRES_CELL_NEGO], &setup, vramType, wk->heapId);
    func_0204b6f4(wk->negoIcons[side][index], func_0201bbe8(boxPkm), 1);
    func_0204b92c(wk->negoIcons[side][index], FALSE);
    func_0204b3dc(wk->negoIcons[side][index], visible);
    GFL_ArcToolFree(arc);
}

void func_ov119_021e6440(PokemonTradeWork *wk, int side, int index) {
    int i, j;
    ClActorPos pos;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            if (wk->negoIcons[i][j] != NULL) {
                pos.x = data_ov119_021e7da0[i * 3 + j][0];
                pos.y = data_ov119_021e7da0[i * 3 + j][1];
                if (i == side && j == index) {
                    pos.y -= 4;
                }
                func_0204b404(wk->negoIcons[i][j], &pos, CLACT_VRAM_SUB);
            }
        }
    }
}




void func_ov119_021e64b8(PokemonTradeWork *wk) {
    int j, i;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            func_ov119_021e62d4(wk, i, j);
        }
    }
}

void func_ov119_021e64dc(PokemonTradeWork *wk, int column, int row, int x, int y, BoxPkm *pkm) {
    ClActorSetup setup;

    setup.x = x;
    setup.y = y;
    setup.sequence = 1;
    setup.priority = 0;
    setup.bgPriority = 0;
    wk->actors[9] = func_0204b294(wk->clactUnit, wk->unk9CC[column][row], wk->objRes[TRADE_OBJRES_PLTT_ICON],
                               wk->objRes[TRADE_OBJRES_CELL_ICON], &setup, CLACT_VRAM_SUB, wk->heapId);
    func_0204b6f4(wk->actors[9], func_0201bbe8(pkm), 1);
}

void func_ov119_021e6544(PokemonTradeWork *wk) {
    ClActorPos pos;
    VecFx32 points[4];
    fx32 x, y, dx, dy, length;

    func_0204b454(wk->actors[9], &pos, CLACT_VRAM_SUB);
    dx = (pos.x - wk->unkF6C.x) * FX32_ONE;
    dy = (pos.y - wk->unkF6C.y) * FX32_ONE;
    points[0].x = pos.x * FX32_ONE;
    points[0].y = pos.y * FX32_ONE;
    points[0].z = 0;
    points[1].x = (pos.x * 2 - wk->unkF6C.x) * FX32_ONE;
    points[1].y = (pos.y * 2 - wk->unkF6C.y) * FX32_ONE;
    points[1].z = 0;
    points[3].x = FX32_CONST(28);
    points[3].y = 0;
    points[3].z = 0;
    x = points[0].x - points[3].x;
    y = points[0].y / 2;
    length = FX_Sqrt(FX_Mul(x, x) + FX_Mul(y, y)) / 15;
    points[2].x = x + FX_Mul(dx, length);
    points[2].y = y + FX_Mul(dy, length);
    points[2].z = 0;
    func_ov119_021e7248(&wk->curve, &points[0], &points[1], &points[2], &points[3], 19);
    wk->unk11DC = 21;
}

BOOL func_ov119_021e665c(PokemonTradeWork *wk) {
    ClActorPos pos;

    if (wk->unk11DC != 0) {
        wk->unk11DC--;
        if (wk->unk11DC == 0) {
            return TRUE;
        }
        func_0204b454(wk->actors[9], &pos, CLACT_VRAM_SUB);
        if (pos.y < 0) {
            wk->unk11DC = 1;
        }
        if (wk->unk11DC == 1) {
            func_ov119_021e66e8(wk);
        } else {
            func_ov119_021e729c(&wk->curve);
            pos.x = wk->curve.pos.x / FX32_ONE;
            pos.y = wk->curve.pos.y / FX32_ONE;
            func_0204b404(wk->actors[9], &pos, CLACT_VRAM_SUB);
        }
    }
    return FALSE;
}




void func_ov119_021e66e8(PokemonTradeWork *wk) {
    if (wk->actors[9] != NULL) {
        func_0204b3b4(wk->actors[9]);
        wk->actors[9] = NULL;
    }
}


void func_ov119_021e6700(PokemonTradeWork *wk) {
    G2S_SetBlendAlpha(0, GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3, 8, 8);
}

void func_ov119_021e6718(PokemonTradeWork *wk, int index) {
    ClActorSetup setup;

    if (wk->unk10BC[index] == NULL) {
        setup.x = index * 24 + 8;
        setup.y = 144;
        setup.sequence = index + 11;
        setup.priority = 50;
        setup.bgPriority = 1;
        wk->unk10BC[index] = func_0204b294(wk->clactUnit, wk->objRes[TRADE_OBJRES_CHAR_SUB],
                                           wk->objRes[TRADE_OBJRES_PLTT_SUB], wk->objRes[TRADE_OBJRES_CELL_SUB], &setup,
                                           CLACT_VRAM_SUB, wk->heapId);
    }
    func_0204b92c(wk->unk10BC[index], TRUE);
    func_0204b3dc(wk->unk10BC[index], TRUE);
}


void func_ov119_021e6790(PokemonTradeWork *wk, int index, BOOL visible) {
    func_0204b3dc(wk->unk10BC[index], visible);
}

void func_ov119_021e67a8(PokemonTradeWork *wk, int index) {
    if (wk->unk10BC[index] != NULL) {
        func_0204b3b4(wk->unk10BC[index]);
        wk->unk10BC[index] = NULL;
    }
}

void func_ov119_021e67c4(PokemonTradeWork *wk, u32 index) {
    func_0204b878(wk->unk10BC[index], index + 15);
}

void func_ov119_021e67e0(PokemonTradeWork *wk, u32 index, int side) {
    ClActorSetup setup;
    int no;
    ClActor **clActors;

    setup.x = side * 206 + 8;
    setup.y = 160;
    setup.sequence = 4;
    setup.priority = 1;
    setup.bgPriority = 0;
    no = side * 2;
    if (wk->unk10AC[no] == NULL) {
        wk->unk10AC[no] = func_0204b294(wk->clactUnit, wk->objRes[TRADE_OBJRES_CHAR_MAIN],
                                        wk->objRes[TRADE_OBJRES_PLTT_MAIN], wk->objRes[TRADE_OBJRES_CELL_MAIN],
                                        &setup, CLACT_VRAM_MAIN, wk->heapId);
        clActors = wk->unk10AC;
        func_0204b92c(clActors[no], TRUE);
        func_0204b3dc(wk->unk10AC[no], TRUE);
    } else {
        func_0204b3dc(wk->unk10AC[no], TRUE);
    }
    no++;
    if (wk->unk10AC[no] == NULL) {
        setup.priority = 0;
        setup.sequence = index;
        wk->unk10AC[no] = func_0204b294(wk->clactUnit, wk->objRes[TRADE_OBJRES_CHAR_MAIN],
                                        wk->objRes[TRADE_OBJRES_PLTT_MAIN], wk->objRes[TRADE_OBJRES_CELL_MAIN],
                                        &setup, CLACT_VRAM_MAIN, wk->heapId);
        func_0204b92c(wk->unk10AC[no], TRUE);
        func_0204b3dc(wk->unk10AC[no], TRUE);
    } else {
        func_0204b878((clActors = wk->unk10AC)[no], index);
    }
}


void func_ov119_021e68b4(PokemonTradeWork *wk, int side) {
    if (wk->unk10AC[side * 2 + 1] != NULL) {
        func_0204b3b4(wk->unk10AC[side * 2 + 1]);
        wk->unk10AC[side * 2 + 1] = NULL;
    }
    if (wk->unk10AC[side * 2] != NULL) {
        func_0204b3b4(wk->unk10AC[side * 2]);
        wk->unk10AC[side * 2] = NULL;
    }
}

void func_ov119_021e68f4(PokemonTradeWork *wk, int cursor) {
    ClActorPos positions[] = {
        { 64, 48 }, { 64, 96 }, { 64, 144 }, { 192, 48 }, { 192, 96 }, { 192, 144 },
    };
    ClActorSetup setup;

    if (cursor == -1) {
        if (wk->unkFD4 != NULL) {
            func_0204b3dc(wk->unkFD4, FALSE);
        }
        return;
    }
    if (wk->unkFD4 == NULL) {
        setup.x = 64;
        setup.y = 48;
        setup.sequence = 23;
        setup.priority = 0;
        setup.bgPriority = 1;
        wk->unkFD4 = func_0204b294(wk->clactUnit, wk->objRes[TRADE_OBJRES_CHAR_SUB], wk->objRes[TRADE_OBJRES_PLTT_SUB],
                                   wk->objRes[TRADE_OBJRES_CELL_SUB], &setup, CLACT_VRAM_SUB, wk->heapId);
    }
    func_0204b92c(wk->unkFD4, TRUE);
    func_0204b3dc(wk->unkFD4, TRUE);
    func_0204b404(wk->unkFD4, &positions[cursor], CLACT_VRAM_SUB);
}





void func_ov119_021e69a4(PokemonTradeWork *wk) {
    if (wk->unkFD4 != NULL) {
        func_0204b3b4(wk->unkFD4);
        wk->unkFD4 = NULL;
    }
}

BOOL func_ov119_021e69c0(PokemonTradeWork *wk) {
    // A 4 of the file's .rodata, though its reads fold
    static const int speed = 4;
    ClActorPos pos, leftPos, rightPos;
    int i;

    if (wk->unk11DE < 128) {
        wk->unk11DE += speed;
    } else {
        return TRUE;
    }
    for (i = 0; i < 8; i++) {
        func_0204b454(wk->negoPanels[i], &pos, CLACT_VRAM_SUB);
        if (i / 4) {
            pos.x += speed;
        } else {
            pos.x -= speed;
        }
        func_0204b404(wk->negoPanels[i], &pos, CLACT_VRAM_SUB);
    }
    for (i = 0; i < 3; i++) {
        if (wk->negoIcons[0][i] != NULL) {
            func_0204b454(wk->negoIcons[0][i], &leftPos, CLACT_VRAM_SUB);
            leftPos.x -= speed;
            func_0204b404(wk->negoIcons[0][i], &leftPos, CLACT_VRAM_SUB);
        }
    }
    for (i = 0; i < 3; i++) {
        if (wk->negoIcons[1][i] != NULL) {
            func_0204b454(wk->negoIcons[1][i], &rightPos, CLACT_VRAM_SUB);
            rightPos.x += speed;
            func_0204b404(wk->negoIcons[1][i], &rightPos, CLACT_VRAM_SUB);
        }
    }
    GFL_BGSysMoveBG(5, BG_MOVE_SET_X, -wk->unk11DE);
    GFL_BGSysMoveBG(4, BG_MOVE_SET_X, wk->unk11DE - 128);
    return FALSE;
}


void func_ov119_021e6ab0(PokemonTradeWork *wk) {
    ClActorPos pos, leftPos, rightPos;
    int i;

    for (i = 0; i < 8; i++) {
        func_0204b454(wk->negoPanels[i], &pos, CLACT_VRAM_SUB);
        if (i / 4) {
            pos.x -= 128;
            func_0204b84c(wk->negoPanels[i], 3);
        } else {
            pos.x += 128;
        }
        func_0204b404(wk->negoPanels[i], &pos, CLACT_VRAM_SUB);
    }
    for (i = 0; i < 3; i++) {
        if (wk->negoIcons[0][i] != NULL) {
            func_0204b454(wk->negoIcons[0][i], &leftPos, CLACT_VRAM_SUB);
            leftPos.x += 128;
            func_0204b404(wk->negoIcons[0][i], &leftPos, CLACT_VRAM_SUB);
        }
    }
    for (i = 0; i < 3; i++) {
        if (wk->negoIcons[1][i] != NULL) {
            func_0204b454(wk->negoIcons[1][i], &rightPos, CLACT_VRAM_SUB);
            rightPos.x -= 128;
            func_0204b404(wk->negoIcons[1][i], &rightPos, CLACT_VRAM_SUB);
            func_0204b84c(wk->negoIcons[1][i], 3);
        }
    }
    GFL_BGSysSetBGPriority(4, 2);
    GFL_BGSysSetBGPriority(5, 3);
}

void func_ov119_021e6b90(PokemonTradeWork *wk) {
    ArcTool *arc;

    wk->cursorImage = LoadCursorImageEndOfHeap(6, 15, 0, wk->heapId);
    arc = GFL_ArcSysCreateFileHandle(0x66, wk->heapId);
    wk->unk870 = GFL_BGSysLoadArcNCGRDynamic(arc, 13, 5, 0, FALSE, wk->heapId);
    GFL_G2DIOLoadNSCRSync(arc, 7, 4, 0, CHAR_POS(wk->unk870), 0, FALSE, wk->heapId);
    GFL_G2DIOLoadNSCRSync(arc, 7, 5, 0, CHAR_POS(wk->unk870), 0, FALSE, wk->heapId);
    {
        BGSetup setup = { 0, 0, 0x1000, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf000, GX_BG_CHARBASE_0x10000, 0x10000, GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE };

        GFL_BGSysReleaseBG(7);
        GFL_BGSysCreateBG(7, &setup, BGMODE_TEXT);
    }
    GFL_G2DIOLoadNSCRSync(arc, 8, 7, 0, CHAR_POS(wk->unk870), 0, FALSE, wk->heapId);
    GFL_BGSysMoveBG(4, BG_MOVE_SET_X, -128);
    GFL_BGSysMoveBG(7, BG_MOVE_SET_X, 0);
    GFL_BGSysSetBGPriority(4, 3);
    GFL_G2DIOLoadNSCRSync(arc, 11, 2, 0, CHAR_POS(wk->unk86C), 0, FALSE, wk->heapId);
    GFL_ArcToolFree(arc);
    wk->unk11DE = 0;
    func_ov119_021e6ab0(wk);
    GFL_BGSysSetEnabledBGsB(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3 | GX_PLANEMASK_OBJ);
}

void func_ov119_021e6cb8(PokemonTradeWork *wk) {
    ClActorPos positions[] = {
        { 96, 32 }, { 104, 64 }, { 104, 112 }, { 104, 160 }, { 224, 32 }, { 232, 64 }, { 232, 112 }, { 232, 160 },
    };
    ClActorSetup setup;
    ArcTool *arc = GFL_ArcSysCreateFileHandle(0x66, wk->heapId);
    int i;

    wk->negoPanelPltt = func_0204ac18(arc, 0, CLACT_VRAM_SUB, 0x1c0, 0, 1, wk->heapId);
    wk->negoPanelCells = func_0204af28(arc, 2, 1, wk->heapId);
    for (i = 0; i < 8; i++) {
        wk->negoBitmaps[i] = GFL_BitmapCreate(16, 6, 32, wk->heapId);
        wk->negoPanelChars[i] = func_0204a6c8(arc, 3, FALSE, CLACT_VRAM_SUB, wk->heapId);
    }
    for (i = 0; i < 8; i++) {
        setup.x = positions[i].x;
        setup.y = positions[i].y;
        setup.sequence = 0;
        setup.priority = 0;
        setup.bgPriority = 1;
        wk->negoPanels[i] = func_0204b294(wk->clactUnit, wk->negoPanelChars[i], wk->negoPanelPltt, wk->negoPanelCells,
                                          &setup, CLACT_VRAM_SUB, wk->heapId);
        func_0204b3dc(wk->negoPanels[i], TRUE);
    }
    GFL_ArcToolFree(arc);
}

void func_ov119_021e6dec(PokemonTradeWork *wk) {
    int i;

    for (i = 0; i < 8; i++) {
        if (wk->negoPanels[i] != NULL) {
            func_0204b3b4(wk->negoPanels[i]);
            wk->negoPanels[i] = NULL;
            GFL_BitmapFree(wk->negoBitmaps[i]);
            wk->negoBitmaps[i] = NULL;
            func_0204a8d4(wk->negoPanelChars[i]);
            wk->negoPanelChars[i] = 0;
        }
    }
    if (wk->negoPanelPltt != 0) {
        func_0204ada4(wk->negoPanelPltt);
        wk->negoPanelPltt = 0;
    }
    if (wk->negoPanelPltt != 0) {
        func_0204afd8(wk->negoPanelChars[2]);
        wk->negoPanelPltt = 0;
    }
}

void func_ov119_021e6e7c(PokemonTradeWork *wk) {
    u8 order[] = { 0, 2, 4, 6, 1, 3, 5, 7 };
    int i, j;

    for (i = 0; i < 8; i++) {
        u32 dest = func_0204abbc(wk->negoPanelChars[i], TRUE);
        u8 *pixels = GFL_BitmapGetPixelData(wk->negoBitmaps[i]);

        DC_FlushRange(pixels, GFL_BitmapCalcPixelDataSize(wk->negoBitmaps[i]));
        for (j = 0; j < 8; j++) {
            GXS_LoadOBJ(pixels + order[j] * 0x100, dest, 0x100);
            dest += 0x100;
        }
    }
}






void func_ov119_021e6f0c(PokemonTradeWork *wk, int side, PartyPkm *pkm) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(0x66, wk->heapId);
    NNSG2dScreenData *screen;
    void *file;
    u16 *cells;
    u32 i;

    if (side == 0) {
        file = GFL_G2DIOReadNSCRArc(arc, 5, FALSE, &screen, wk->heapId);
    } else {
        file = GFL_G2DIOReadNSCRArc(arc, 6, FALSE, &screen, wk->heapId);
    }
    cells = (u16 *)screen->rawData;
    for (i = 0; i < screen->szByte / 2; i++) {
        cells[i] += CHAR_POS(wk->unk86C);
    }
    if (side == 0) {
        GFL_BGSysLoadScrAreaAll(1, cells, 0, 0, 16, 24);
    } else {
        GFL_BGSysLoadScrAreaAll(1, screen->rawData, 16, 0, 16, 24);
    }
    GFL_HeapFree(file);
    GFL_BGSysSetBGPriority(3, 2);
    GFL_BGSysSetBGPriority(2, 3);
    GFL_BGSysSetBGPriority(1, 1);
    GFL_BGSysSetBGPriority(0, 0);
    GFL_ArcToolFree(arc);
    GFL_BGSysQueueScrLoad(1);
}




void func_ov119_021e6fc8(uintptr_t param, fx32 frame) {
    PokemonTradeWork *wk = (PokemonTradeWork *)param;

    wk->unk11F5 = TRUE;
}

void func_ov119_021e6fd4(PokemonTradeWork *wk, u8 x, u8 y) {
    ClActorSetup setup;
    ClActorCallback callback;

    setup.x = x + 12;
    setup.y = y + 12;
    setup.sequence = 24;
    setup.priority = 0;
    setup.bgPriority = 0;
    func_ov119_021e7058(wk);
    wk->unkF24 = func_0204b294(wk->clactUnit, wk->objRes[TRADE_OBJRES_CHAR_SUB], wk->objRes[TRADE_OBJRES_PLTT_SUB],
                               wk->objRes[TRADE_OBJRES_CELL_SUB], &setup, CLACT_VRAM_SUB, wk->heapId);
    func_0204b3dc(wk->unkF24, TRUE);
    func_0204b92c(wk->unkF24, TRUE);
    wk->unk11F5 = FALSE;
    callback.type = CLACT_CALLBACK_LAST_FRAME;
    callback.param = (uintptr_t)wk;
    callback.func = func_ov119_021e6fc8;
    func_0204ba00(wk->unkF24, &callback);
}


void func_ov119_021e7058(PokemonTradeWork *wk) {
    if (wk->unkF24 != NULL) {
        func_0204b3b4(wk->unkF24);
        wk->unkF24 = NULL;
        wk->unk11F5 = FALSE;
    }
}

BOOL func_ov119_021e707c(PokemonTradeWork *wk) {
    if (wk->unk11F5) {
        func_ov119_021e7058(wk);
        return FALSE;
    }
    return TRUE;
}




// Dims the palettes of a standard palette memory (PALFADE_VRAM_*) in paletteMask
void func_ov119_021e7098(const PokemonTradeWork *wk, u32 vram, u32 paletteMask) {
    PaletteFade *fade = func_02021518(wk->heapId);
    u8 *colors;
    int i;

    func_0202155c(fade, vram, 0x200, wk->heapId);
    func_020216d4(fade, vram, 0, 0x200);
    func_02021fd8(fade, vram, 0, 0x100, 6, 0);
    colors = (u8 *)func_02021730(fade, vram);
    for (i = 0; i < 16; i++) {
        if (paletteMask & (1 << i)) {
            DC_FlushRange(colors + i * 32, 32);
            switch (vram) {
            case PALFADE_VRAM_SUB_OBJ:
                GXS_LoadOBJPltt(colors + i * 32, i * 32, 32);
                break;
            case PALFADE_VRAM_SUB_BG:
                GXS_LoadBGPltt(colors + i * 32, i * 32, 32);
                break;
            case PALFADE_VRAM_MAIN_OBJ:
                GX_LoadOBJPltt(colors + i * 32, i * 32, 32);
                break;
            case PALFADE_VRAM_MAIN_BG:
                GX_LoadBGPltt(colors + i * 32, i * 32, 32);
                break;
            }
        }
    }
    func_020215a0(fade, vram);
    func_02021540(fade);
}

void func_ov119_021e7160(PokemonTradeWork *wk, BOOL dim, u16 vram, u32 paletteMask) {
    if (dim) {
        if (vram == PALFADE_VRAM_SUB_OBJ) {
            MI_CpuCopy8((void *)HW_DB_OBJ_PLTT, wk->savedObjPalette, 0x200);
        } else if (vram == PALFADE_VRAM_SUB_BG) {
            MI_CpuCopy8((void *)HW_DB_BG_PLTT, wk->savedBGPalette, 0x200);
        }
        func_ov119_021e7098(wk, vram, paletteMask);
    } else if (vram == PALFADE_VRAM_SUB_OBJ) {
        DC_FlushRange(wk->savedObjPalette, 0x200);
        GXS_LoadOBJPltt(wk->savedObjPalette, 0, 0x200);
    } else if (vram == PALFADE_VRAM_SUB_BG) {
        DC_FlushRange(wk->savedBGPalette, 0x200);
        GXS_LoadBGPltt(wk->savedBGPalette, 0, 0x200);
    }
}


// Dims the lower screen's OBJ palettes but the first, or brings them back
void func_ov119_021e71e0(PokemonTradeWork *wk, BOOL dim) {
    func_ov119_021e7160(wk, dim, PALFADE_VRAM_SUB_OBJ, 0xfffe);
}

// Sets the planes of the main screen shown, in the next V-blank
void func_ov119_021e71f0(PokemonTradeWork *wk, int planes) {
    GFL_TCBMgrAddTask(GFL_VBlankGetTCBMgr(), func_ov119_021e7220, (void *)(intptr_t)planes, 10);
}

// Sets the planes of the sub screen shown, in the next V-blank
void func_ov119_021e7208(PokemonTradeWork *wk, int planes) {
    GFL_TCBMgrAddTask(GFL_VBlankGetTCBMgr(), func_ov119_021e7234, (void *)(intptr_t)planes, 10);
}

static void func_ov119_021e7220(TCB *tcb, void *data) {
    GFL_BGSysSetEnabledBGsA((int)(intptr_t)data);
    GFL_TCBRemove(tcb);
}

static void func_ov119_021e7234(TCB *tcb, void *data) {
    GFL_BGSysSetEnabledBGsB((int)(intptr_t)data);
    GFL_TCBRemove(tcb);
}

// Starts a point along the curve from start to end, over frames
void func_ov119_021e7248(TradeCurve *curve, VecFx32 *start, VecFx32 *control1, VecFx32 *control2, VecFx32 *end,
                         int frames) {
    curve->pos = *start;
    curve->points[0] = *start;
    curve->points[1] = *control1;
    curve->points[2] = *control2;
    curve->points[3] = *end;
    curve->frame = 0;
    curve->frames = frames;
}

// Moves the point to the curve's next frame; TRUE once it is at the end
static BOOL func_ov119_021e729c(TradeCurve *curve) {
    fx32 t, t2, s, s2, b0, b1, b2, b3;

    if (curve->frame < curve->frames - 1) {
        t = FX_Div(FX32_CONST(curve->frame), FX32_CONST(curve->frames));
        t2 = FX_Mul(t, t);
        s = FX32_ONE - t;
        s2 = FX_Mul(s, s);
        b1 = FX_Mul(3 * t, s2);
        b0 = FX_Mul(s2, s);
        b2 = FX_Mul(3 * t2, s);
        b3 = FX_Mul(t2, t);
        curve->pos.x = FX_Mul(curve->points[0].x, b0) + FX_Mul(curve->points[1].x, b1) +
                       FX_Mul(curve->points[2].x, b2) + FX_Mul(curve->points[3].x, b3);
        curve->pos.y = FX_Mul(curve->points[0].y, b0) + FX_Mul(curve->points[1].y, b1) +
                       FX_Mul(curve->points[2].y, b2) + FX_Mul(curve->points[3].y, b3);
        curve->pos.z = FX_Mul(curve->points[0].z, b0) + FX_Mul(curve->points[1].z, b1) +
                       FX_Mul(curve->points[2].z, b2) + FX_Mul(curve->points[3].z, b3);
        curve->frame++;
        return FALSE;
    }
    curve->pos = curve->points[3];
    return TRUE;
}
