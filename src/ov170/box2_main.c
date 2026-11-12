// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov255/box2_main.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/bag.h"
#include "app/box2_bgwfrm.h"
#include "app/box2_bmp.h"
#include "app/box2_main.h"
#include "app/box2_obj.h"
#include "app/box2_seq.h"
#include "app/box2_ui.h"
#include "app/box_search.h"
#include "app/name_entry.h"
#include "app/p_status.h"
#include "constants/arc.h"
#include "constants/items.h"
#include "constants/pokemon.h"
#include "constants/sound.h"
#include "constants/species.h"
#include "constants/text_banks.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/overlay.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/math.h"
#include "nitro/os.h"
#include "pml/item.h"
#include "pml/personal.h"
#include "pml/poke_party.h"
#include "save/box.h"
#include "save/chatter.h"
#include "save/dream_world.h"
#include "save/pokedex.h"
#include "save/save_control.h"
#include "system/app_menu_common.h"
#include "system/app_taskmenu.h"
#include "system/bgwinfrm.h"
#include "system/bmp_winframe.h"
#include "system/cursor_move.h"
#include "system/game_data.h"
#include "system/gf_font.h"
#include "system/palanm.h"
#include "system/printsys.h"
#include "system/wordset.h"
#include "text/system/btl_pokeparam_box_main_save_init.h"

// The PC box's graphics setup, the moves of Pokémon between the boxes and the party, and the frames and buttons that
// slide in and out. Names are ours

void func_ov170_021dc970(TCB *tcb, void *work);
void func_ov170_021dd330(Box2SysWork *syswk, Box2PokeMoveData *box, Box2PokeMoveData *party);
void func_ov170_021dd39c(Box2SysWork *syswk, Box2PokeMoveData *move);
void func_ov170_021dd3dc(Box2SysWork *syswk, Box2PokeMoveData *party);
void func_ov170_021dd414(Box2SysWork *syswk, Box2PokeMoveData *party);
void func_ov170_021dd42c(Box2SysWork *syswk, Box2PokeMoveData *party);
static void func_ov170_021dd6e8(Box2SysWork *syswk, BoxPkm *pkm, u32 forme);
void func_ov170_021dd810(Box2SysWork *syswk, u32 putPos, u32 getPos, u32 tray);
static void func_ov170_021de8b4(Box2SysWork *syswk, u32 setPos, u32 putPos, u32 mvMode, BOOL hand);
void func_ov170_021de980(Box2SysWork *syswk, u32 mvMode);
void func_ov170_021dea3c(Box2SysWork *syswk, u32 setPos, u32 putPos, u32 mvMode);
void func_ov170_021dea4c(Box2SysWork *syswk, u32 putPos, u32 mvMode);
BOOL func_ov170_021dea6c(Box2SysWork *syswk, BOOL hand);
BOOL func_ov170_021deafc(Box2SysWork *syswk);
BOOL func_ov170_021deb08(Box2SysWork *syswk);
static void func_ov170_021deb40(Box2SysWork *syswk, u32 wallpaper, u32 offset);
static void func_ov170_021deb70(Box2SysWork *syswk, u32 wallpaper, u32 palette);
static void func_ov170_021deb90(Box2SysWork *syswk, u32 wallpaper, u32 x, u32 charOffset, u32 palette);
static void func_ov170_021dec64(Box2SysWork *syswk, u32 wallpaper, u32 x, u32 charOffset, u32 palette);
static Box2PokeInfo *func_ov170_021dedd8(BoxPkm *pkm);
static void func_ov170_021def58(Box2PokeInfo *info);
static void func_ov170_021def60(Box2SysWork *syswk, Box2PokeInfo *info);
static void func_ov170_021df0d4(Box2SysWork *syswk, u32 base, u32 mark);
static void func_ov170_021df128(Box2AppWork *app);
static void func_ov170_021df144(Box2AppWork *app, Box2PokeInfo *info);
BOOL func_ov170_021df1bc(int x, int y, const Box2Area *area);
u32 func_ov170_021df1dc(s16 x, s16 y);
u32 func_ov170_021df228(s16 x, s16 y, const Box2Area *areas);
u32 func_ov170_021df254(Box2SysWork *syswk, s16 x, s16 y);

// MWCC sorts static data by size, unstably, so these are declared in the order that lays them out as the ROM has
// them
// pokebw2's sVramBanks
static const BGSysVRAMConfig data_ov170_021ef7cc = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,        GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_128_D,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_NONE,  GX_VRAM_TEXPLTT_NONE,    GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K,
};

// Where the party's Pokémon are, with the party's frame on the right and on the left
// pokebw2's sPartyPokeAreaLeft
const Box2Area data_ov170_021ef79c[6] = {
    { 178, 209, 58, 81 },  { 214, 245, 66, 89 },   { 178, 209, 90, 113 },
    { 214, 245, 98, 121 }, { 178, 209, 122, 145 }, { 214, 245, 130, 153 },
};
// pokebw2's sPartyPokeAreaRight
const Box2Area data_ov170_021ef7b4[6] = {
    { 26, 57, 58, 81 },  { 62, 93, 66, 89 },   { 26, 57, 90, 113 },
    { 62, 93, 98, 121 }, { 26, 57, 122, 145 }, { 62, 93, 130, 153 },
};

// Where the tray's Pokémon are on the lower screen
// pokebw2's sTrayPokeArea
static const Box2Area data_ov170_021ef798 = { 8, 159, 40, 159 };

// pokebw2's Box2Main_InitVBlank
void func_ov170_021dc94c(Box2SysWork *syswk) {
    syswk->app->vtask = GFL_VBlankTCBAdd(func_ov170_021dc970, syswk, 0);
}

// pokebw2's Box2Main_ExitVBlank
void func_ov170_021dc964(Box2SysWork *syswk) {
    GFL_TCBRemove(syswk->app->vtask);
}

void func_ov170_021dc970(TCB *tcb, void *work) {
    Box2SysWork *syswk = work;

    func_ov170_021eab04(syswk->app);
    GFL_BGSysUpdate();
    func_0204a648();
    func_02021d50(syswk->app->palFade);
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}


// pokebw2's Box2Main_VFuncSet
void func_ov170_021dc9a0(Box2AppWork *app, Box2VFunc func) {
    app->vfunk.seq = 0;
    app->vfunk.cnt = 0;
    app->vfunk.func = func;
    app->vfunk.freq = NULL;
}

// pokebw2's Box2Main_VFuncReq
void func_ov170_021dc9b4(Box2AppWork *app, Box2VFunc func) {
    app->vfunk.freq = func;
}

// pokebw2's Box2Main_VFuncReqSet
void func_ov170_021dc9b8(Box2AppWork *app) {
    func_ov170_021dc9a0(app, app->vfunk.freq);
}

// pokebw2's Box2Main_InitVramBanks
void func_ov170_021dc9c4(void) {
    GFL_BGSysInitVRAM(0);
    GFL_BGSysSetVRAMBanks(&data_ov170_021ef7cc);
}

// pokebw2's Box2Main_GetVramBanks
const BGSysVRAMConfig *func_ov170_021dc9d8(void) {
    return &data_ov170_021ef7cc;
}

// pokebw2's Box2Main_InitBg
void func_ov170_021dc9e0(Box2SysWork *syswk) {
    GFL_BGSysCreate(HEAPID_BOX2_APP);

    {
        BGSysLCDConfig config = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D };
        GFL_BGSysSetLCDConfig(&config);
    }

    // Main screen
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xf800,
            GX_BG_CHARBASE_0x18000,
            0x8000,
            GX_BG_EXTPLTT_01,
            0,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(0, &setup, BGMODE_TEXT);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xf000,
            GX_BG_CHARBASE_0x10000,
            0x8000,
            GX_BG_EXTPLTT_01,
            1,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(1, &setup, BGMODE_TEXT);
        GFL_BGSysClearScr(1);
        GFL_BGSysClearCharCore(1, 0x20, 0, HEAPID_BOX2_APP);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xe800,
            GX_BG_CHARBASE_0x08000,
            0x8000,
            GX_BG_EXTPLTT_01,
            2,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(2, &setup, BGMODE_TEXT);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x1000,
            0,
            BGRES_512x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xd800,
            GX_BG_CHARBASE_0x00000,
            0x8000,
            GX_BG_EXTPLTT_01,
            3,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(3, &setup, BGMODE_TEXT);
    }

    // Sub screen
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xf800,
            GX_BG_CHARBASE_0x10000,
            0x8000,
            GX_BG_EXTPLTT_01,
            1,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(4, &setup, BGMODE_TEXT);
        GFL_BGSysClearScr(4);
        GFL_BGSysClearCharCore(4, 0x20, 0, HEAPID_BOX2_APP);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x1000,
            0,
            BGRES_256x512,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xe800,
            GX_BG_CHARBASE_0x00000,
            0x8000,
            GX_BG_EXTPLTT_01,
            2,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(5, &setup, BGMODE_TEXT);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xe000,
            GX_BG_CHARBASE_0x18000,
            0x8000,
            GX_BG_EXTPLTT_01,
            0,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(6, &setup, BGMODE_TEXT);
        GFL_BGSysClearScr(6);
        GFL_BGSysClearCharCore(6, 0x20, 0, HEAPID_BOX2_APP);
    }

    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2, TRUE);
}

// pokebw2's Box2Main_ExitBg
void func_ov170_021dcb48(Box2SysWork *syswk) {
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3, FALSE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2, FALSE);
    GFL_BGSysReleaseBG(6);
    GFL_BGSysReleaseBG(5);
    GFL_BGSysReleaseBG(4);
    GFL_BGSysReleaseBG(3);
    GFL_BGSysReleaseBG(2);
    GFL_BGSysReleaseBG(1);
    GFL_BGSysReleaseBG(0);
    GFL_BGSysFree();
}

// pokebw2's Box2Main_LoadBgGraphics
void func_ov170_021dcb8c(Box2SysWork *syswk) {
    ArcTool *arc;
    ArcTool *uiArc;
    u32 charOffset;
    u16 *src;
    u16 *dst;
    u32 i;

    arc = GFL_ArcSysCreateFileHandle(ARCID_BOX2, HEAPID_BOX2_APP);
    GFL_BGSysLoadArcNCGRStatic(arc, 12, 1, 0, 0, TRUE, HEAPID_BOX2_APP);
    GFL_BGSysLoadArcNCGRDynamic(arc, 3, 2, 0, TRUE, HEAPID_BOX2_APP);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 2, 2, 0, 0, TRUE, HEAPID_BOX2_APP);
    GFL_BGSysLoadArcNCGRStatic(arc, 1, 3, 0, 0, TRUE, HEAPID_BOX2_APP);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 0, 3, 0, 0, TRUE, HEAPID_BOX2_APP);
    GFL_G2DIOLoadArcNCLRDefault(arc, 4, 0, 0, 0x80, HEAPID_BOX2_APP);
    GFL_BGSysLoadArcNCGRStatic(arc, 62, 1, 0x3e8, 0x300, TRUE, HEAPID_BOX2_APP);
    GFL_G2DIOLoadArcNCLRDefault(arc, 63, 0, 0x180, 0x40, HEAPID_BOX2_APP);

    {
        NNSG2dPaletteData *palette;
        void *buf = GFL_G2DIOReadNCLR(getUINarcIdx(), 31, &palette, HEAPID_TAIL(HEAPID_BOX2_APP));
        u8 *colors = palette->pRawData;
        u32 offset = 0x194;

        GFL_BGSysUploadStdPalette(0, colors + 0x34, 4, offset);
        colors += 0x14;
        offset += 0x20;
        GFL_BGSysUploadStdPalette(0, colors, 4, offset);
        GFL_HeapFree(buf);
    }

    GFL_BGSysLoadArcNCGRStatic(arc, 6, 5, 0, 0, TRUE, HEAPID_BOX2_APP);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 5, 5, 0, 0, TRUE, HEAPID_BOX2_APP);
    GFL_BGSysLoadArcNCGRStatic(arc, 6, 6, 0, 0, TRUE, HEAPID_BOX2_APP);
    GFL_G2DIOLoadArcNCLRDefault(arc, 7, 4, 0, 0, HEAPID_BOX2_APP);
    GFL_ArcToolFree(arc);

    // The bar at the bottom of the main screen, which BG 2 copies
    uiArc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), HEAPID_BOX2_APP);
    GFL_BGSysLoadArcNCGRDynamic(uiArc, func_02027628(), 0, 0, FALSE, HEAPID_BOX2_APP);
    GFL_G2DIOLoadArcNCLRDefault(uiArc, func_02027624(), 0, 0xe0, 0x20, HEAPID_BOX2_APP);
    loadBGScrToVramByFileNoReserveNegAlign(uiArc, func_0202762c(), 0, 0, 0, FALSE, HEAPID_BOX2_APP);
    GFL_BGSysSetScrPaletteNo(0, 0, 21, 32, 3, 7);
    GFL_BGSysQueueScrLoad(0);
    charOffset = GFL_BGSysLoadArcNCGRDynamic(uiArc, func_02027628(), 2, 0, FALSE, HEAPID_BOX2_APP);
    GFL_ArcToolFree(uiArc);

    src = GFL_BGSysIsScrHeapExists(0);
    dst = GFL_BGSysIsScrHeapExists(2);
    for (i = 0; i < 32 * 3; i++) {
        dst[21 * 32 + i] = src[21 * 32 + i] + charOffset;
    }
    GFL_BGSysQueueScrLoad(2);

    syswk->app->cursorChars = LoadCursorImageEndOfHeap(0, 10, 0, HEAPID_BOX2_APP);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 0, 0x160, 0x20, HEAPID_BOX2_APP);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 4, 0x1e0, 0x20, HEAPID_BOX2_APP);
}

void func_ov170_021dcdb8(Box2SysWork *syswk) {
    syswk->app->palFade = func_02021518(HEAPID_BOX2_APP);
    func_0202155c(syswk->app->palFade, 0, 0x200, HEAPID_BOX2_APP);
}


void func_ov170_021dcdd8(Box2SysWork *syswk) {
    func_020215a0(syswk->app->palFade, 0);
    func_02021540(syswk->app->palFade);
}


// pokebw2's Box2Main_SetBlendAlpha
void func_ov170_021dcdf0(BOOL enabled) {
    if (enabled == TRUE) {
        G2_SetBlendAlpha(GX_BLEND_PLANEMASK_NONE,
                         GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_BD, 6, 10);
    } else {
        G2_BlendNone();
    }
}

void func_ov170_021dce14(Box2SysWork *syswk) {
    syswk->app->msgData = GFL_MsgSysLoadData(FALSE, 2, 9, HEAPID_BOX2_APP);
    syswk->app->font = GFL_FontCreate(0x17, 0, 1, FALSE, HEAPID_BOX2_APP);
    syswk->app->smallFont = GFL_FontCreate(0x17, 3, 0, FALSE, HEAPID_BOX2_APP);
    syswk->app->wordSet = GFL_WordSetSystemCreateDefault(HEAPID_BOX2_APP);
    syswk->app->printQueue = func_0201c450(0x800, HEAPID_BOX2_APP);
    syswk->app->expandBuf = GFL_StrBufCreate(0x400, HEAPID_BOX2_APP);
}


void func_ov170_021dce80(Box2SysWork *syswk) {
    GFL_StrBufFree(syswk->app->expandBuf);
    func_0201c4c0(syswk->app->printQueue);
    GFL_WordSetSystemFree(syswk->app->wordSet);
    GFL_FontFree(syswk->app->smallFont);
    GFL_FontFree(syswk->app->font);
    GFL_MsgDataFree(syswk->app->msgData);
}

void func_ov170_021dcec0(Box2SysWork *syswk) {
    syswk->app->yesNoItems[0].str = GFL_MsgDataLoadStrbufNew(syswk->app->msgData, 0x6e);
    syswk->app->yesNoItems[0].color = 0x39e3;
    syswk->app->yesNoItems[0].type = 0;
    syswk->app->yesNoItems[1].str = GFL_MsgDataLoadStrbufNew(syswk->app->msgData, 0x6f);
    syswk->app->yesNoItems[1].color = 0x39e3;
    syswk->app->yesNoItems[1].type = 0;
    syswk->app->yesNoRes = func_02027f60(0, 8, syswk->app->font, syswk->app->printQueue, HEAPID_BOX2_APP);
}


void func_ov170_021dcf30(Box2SysWork *syswk) {
    func_02027fd8(syswk->app->yesNoRes);
    GFL_StrBufFree(syswk->app->yesNoItems[1].str);
    GFL_StrBufFree(syswk->app->yesNoItems[0].str);
}

void func_ov170_021dcf58(Box2SysWork *syswk, u32 pos) {
    AppTaskMenuInit init;

    init.heapId = HEAPID_BOX2_APP;
    init.itemCount = 2;
    init.items = syswk->app->yesNoItems;
    init.posType = 1;
    init.x = 0x20;
    init.y = 0x12;
    init.width = 8;
    init.height = 3;
    if (func_020260f0(syswk->app->cursorMove) == TRUE) {
        func_020355d8(FALSE);
    } else {
        func_020355d8(TRUE);
    }
    syswk->app->yesNoMenu = func_02027778(&init, syswk->app->yesNoRes);
    func_02027cfc(syswk->app->yesNoMenu, pos);
}

BOOL func_ov170_021dcfc0(Box2SysWork *syswk) {
    Box2ButtonAnm *bawk = &syswk->app->bawk;

    switch (bawk->seq) {
    case 0:
        if (bawk->mode == BOX2_BTN_ANM_MODE_OBJ) {
            func_ov170_021ebfcc(syswk->app, bawk->id, bawk->pal1);
        } else {
            GFL_BGSysSetScrPaletteNo(bawk->id, bawk->px, bawk->py, bawk->sx, bawk->sy, bawk->pal1);
            GFL_BGSysQueueScrLoad(bawk->id);
        }
        bawk->seq++;
        break;
    case 1:
        if (bawk->mode == BOX2_BTN_ANM_MODE_OBJ) {
            if (func_ov170_021ebfec(syswk->app, bawk->id) == FALSE) {
                return FALSE;
            }
        } else {
            bawk->cnt++;
            if (bawk->cnt == 4) {
                GFL_BGSysSetScrPaletteNo(bawk->id, bawk->px, bawk->py, bawk->sx, bawk->sy, bawk->pal2);
                GFL_BGSysQueueScrLoad(bawk->id);
                bawk->cnt = 0;
                bawk->seq++;
            }
        }
        break;
    case 2:
        bawk->cnt++;
        if (bawk->cnt == 2) {
            return FALSE;
        }
        break;
    }
    return TRUE;
}


void func_ov170_021dd090(Box2SysWork *syswk, u32 index) {
    s8 x;
    s8 y;
    u16 width;
    u16 height;

    func_0202cf98(syswk->app->bgWinFrame, index, &x, &y);
    func_0202cfc0(syswk->app->bgWinFrame, index, &width, &height);
    syswk->app->bawk.mode = 1;
    syswk->app->bawk.id = func_0202cf8c(syswk->app->bgWinFrame, index);
    syswk->app->bawk.pal1 = 0xD;
    syswk->app->bawk.pal2 = 0xC;
    syswk->app->bawk.seq = 0;
    syswk->app->bawk.cnt = 0;
    syswk->app->bawk.px = x;
    syswk->app->bawk.py = y;
    syswk->app->bawk.sx = width;
    syswk->app->bawk.sy = height;
}


void func_ov170_021dd14c(Box2SysWork *syswk) {
    func_ov170_021ee038(syswk, syswk->unk1F, syswk->unk20, syswk->unk21);
    func_ov170_021ef660(syswk->app->bgWinFrame);
}


// pokebw2's Box2Main_ScrollTray
void func_ov170_021dd16c(Box2SysWork *syswk, BOOL right) {
    if (right == FALSE) {
        if (syswk->tray == 0) {
            syswk->tray = syswk->trayMax - 1;
        } else {
            syswk->tray = syswk->tray - 1;
        }
        func_ov170_021ebfa8(syswk->app, 0, 2);
    } else {
        if (syswk->tray == syswk->trayMax - 1) {
            syswk->tray = 0;
        } else {
            syswk->tray = syswk->tray + 1;
        }
        func_ov170_021ebfa8(syswk->app, 1, 4);
    }
    func_ov170_021ec35c(syswk, syswk->tray);
    func_ov170_021dec98(syswk, func_ov170_021dedcc(syswk, syswk->tray), right);
}

void func_ov170_021dd1c4(Box2SysWork *syswk, u32 a1) {
    syswk->trayScroll = func_ov170_021deb14(syswk, a1);
    func_ov170_021edecc(syswk, a1);
    func_ov170_021ee008(syswk, 0, 0);
}

void func_ov170_021dd1e8(Box2SysWork *syswk) {
    func_ov170_021ed904(syswk);
    func_ov170_021ed9b4(syswk->app);
    func_ov170_021edf5c(syswk);
    func_ov170_021eb85c(syswk, 0);
    func_ov170_021ef738(syswk->app->bgWinFrame);
}

// Whether the way from one tray to another is shorter going right
// pokebw2's Box2Main_IsTrayScrollRight
BOOL func_ov170_021dd210(Box2SysWork *syswk, u32 from, u32 to) {
    u32 diff = MATH_ABS((s32)(from - to));

    if (from > to) {
        if (diff < syswk->trayMax / 2) {
            return FALSE;
        }
        return TRUE;
    }
    if (diff >= syswk->trayMax / 2) {
        return FALSE;
    }
    return TRUE;
}

void func_ov170_021dd238(Box2SysWork *syswk) {
    func_020355d8(FALSE);
    if (func_020260f0(syswk->app->cursorMove) == FALSE) {
        func_020260f4(syswk->app->cursorMove, TRUE);
    }
}


// pokebw2's Box2Main_GetPokeParam
u32 func_ov170_021dd25c(Box2SysWork *syswk, u16 pos, u16 tray, u32 param, void *buf) {
    if (pos >= BOX2_PARTY_POS) {
        if (pos >= BOX2_PARTY_POS) {
            pos -= BOX2_PARTY_POS;
        }
        if (PokeParty_GetPkmCount(syswk->param->party) > pos) {
            return PokeParty_GetParam(PokeParty_GetPkm(syswk->param->party, pos), param, buf);
        }
    } else if (pos != BOX2_GET_NONE) {
        BoxPkm *pkm = BoxSaveAccessor_GetPkm(syswk->param->boxes, tray, pos);
        if (pkm != NULL) {
            return PML_PkmGetParam(pkm, param, buf);
        }
    }
    return 0;
}

// pokebw2's Box2Main_SetPokeParam
void func_ov170_021dd2b4(Box2SysWork *syswk, u32 pos, u32 tray, u32 param, u32 value) {
    BoxPkm *pkm = func_ov170_021dd2d0(syswk, tray, pos);

    if (pkm != NULL) {
        PML_PkmSetParam(pkm, param, value);
    }
}

// pokebw2's Box2Main_GetBoxPkm
BoxPkm *func_ov170_021dd2d0(Box2SysWork *syswk, u32 tray, u32 pos) {
    if (tray == BOX2_GET_NONE || pos >= BOX2_PARTY_POS) {
        if (pos >= BOX2_PARTY_POS) {
            pos -= BOX2_PARTY_POS;
        }
        if (PokeParty_GetPkmCount(syswk->param->party) > pos) {
            return func_020185f0(PokeParty_GetPkm(syswk->param->party, pos));
        }
    } else if (pos != BOX2_GET_NONE) {
        return BoxSaveAccessor_GetPkm(syswk->param->boxes, tray, pos);
    }
    return NULL;
}

// pokebw2's Box2Main_ClearPokeData
void func_ov170_021dd314(Box2SysWork *syswk, u32 tray, u32 pos) {
    if (pos < BOX2_PARTY_POS) {
        BoxSaveAccessor_ClearPkm(syswk->param->boxes, tray, pos);
    } else {
        PokeParty_RemovePkm(syswk->param->party, pos - BOX2_PARTY_POS);
    }
}

// pokebw2's PokeDataSwapBoxParty
void func_ov170_021dd330(Box2SysWork *syswk, Box2PokeMoveData *box, Box2PokeMoveData *party) {
    u32 tray;
    PartyPkm *pkm;
    u16 index;

    if (box->iconPos == BOX2_BOXLIST_POS) {
        tray = syswk->getTray;
    } else {
        tray = syswk->tray;
    }
    pkm = boxPkmRegenToPartyPkm(func_ov170_021dd2d0(syswk, tray, box->dfPos), HEAPID_TAIL(HEAPID_BOX2_APP));
    index = party->dfPos - BOX2_PARTY_POS;
    BoxSaveAccessor_SetPkm(syswk->param->boxes, tray, box->dfPos,
                           func_0201d624(PokeParty_GetPkm(syswk->param->party, index)));
    copyPkmIntoPartyBlk(syswk->param->party, index, pkm);
    GFL_HeapFree(pkm);
    func_ov170_021edab0(syswk, tray);
}

void func_ov170_021dd39c(Box2SysWork *syswk, Box2PokeMoveData *move) {
    PartyPkm *pkm;

    pkm = boxPkmRegenToPartyPkm(func_ov170_021dd2d0(syswk, syswk->getTray, move->dfPos), HEAPID_TAIL(HEAPID_BOX2_APP));
    PokeParty_AddPkm(syswk->param->party, pkm);
    func_ov170_021dd314(syswk, syswk->getTray, move->dfPos);
    GFL_HeapFree(pkm);
    func_ov170_021edab0(syswk, syswk->getTray);
}


void func_ov170_021dd3dc(Box2SysWork *syswk, Box2PokeMoveData *party) {
    PartyPkm *pkm = PokeParty_GetPkm(syswk->param->party, party->dfPos - BOX2_PARTY_POS);

    BoxSaveAccessor_SetPkm(syswk->param->boxes, syswk->tray, party->mvPos, func_0201d624(pkm));
    func_ov170_021dd314(syswk, syswk->tray, party->dfPos);
    func_ov170_021edab0(syswk, syswk->tray);
}


// pokebw2's PokeDataSwapParty
void func_ov170_021dd414(Box2SysWork *syswk, Box2PokeMoveData *party) {
    PokeParty_SwapPkms(syswk->param->party, party->dfPos - BOX2_PARTY_POS, party->mvPos - BOX2_PARTY_POS,
                       HEAPID_BOX2_APP);
}

// pokebw2's PokeDataMoveParty
void func_ov170_021dd42c(Box2SysWork *syswk, Box2PokeMoveData *party) {
    u32 index;

    for (index = party->dfPos - BOX2_PARTY_POS; index < PokeParty_GetPkmCount(syswk->param->party) - 1; index++) {
        PokeParty_SwapPkms(syswk->param->party, index, index + 1, HEAPID_BOX2_APP);
    }
}

void func_ov170_021dd464(Box2SysWork *syswk, u32 pos, u32 slot, u32 box) {
    BoxPkm *pkm = func_ov170_021dd2d0(syswk, pos, slot);
    u32 newBox = func_ov170_021dd684(syswk, box);

    BoxSaveAccessor_InsertPkmCore(syswk->param->boxes, newBox, pkm);
    BoxSaveAccessor_ClearPkm(syswk->param->boxes, pos, slot);
    func_ov170_021edab0(syswk, pos);
    func_ov170_021edb34(syswk, pos);
    func_ov170_021edab0(syswk, newBox);
    func_ov170_021edb34(syswk, newBox);
}

void func_ov170_021dd4b8(Box2SysWork *syswk, u32 pos, u32 newPos) {
    PartyPkm *pkm;
    u32 box;

    box = func_ov170_021dd684(syswk, newPos);
    pkm = PokeParty_GetPkm(syswk->param->party, pos - BOX2_PARTY_POS);

    if (PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL) == SPECIES_SHAYMIN) {
        PokeParty_ChangeForme(pkm, 0);
        func_ov170_021dd8a8(syswk, pos);
        PokeDex_RegistPkm(GameData_GetPokedex(syswk->param->gameData), pkm);
    }
    BoxSaveAccessor_InsertPkmCore(syswk->param->boxes, box, func_0201d624(pkm));
    func_ov170_021dd314(syswk, syswk->tray, pos);
    func_ov170_021edab0(syswk, box);
    func_ov170_021edb34(syswk, box);
}

void func_ov170_021dd530(Box2SysWork *syswk) {
    Box2PokeMoveWork *work = syswk->app->vfunk.work;
    u16 count;

    if (work->getPos == BOX2_GET_NONE) {
        return;
    }
    count = PokeParty_GetPkmCount(syswk->param->party);
    if (work->putPos >= BOX2_BOXLIST_POS) {
        if (work->getPos < BOX2_PARTY_POS) {
            func_ov170_021dd464(syswk, syswk->getTray, work->getPos, work->putPos - BOX2_BOXLIST_POS);
        } else {
            func_ov170_021dd4b8(syswk, work->getPos, work->putPos - BOX2_BOXLIST_POS);
        }
        return;
    }
    if (work->getPos < BOX2_PARTY_POS) {
        if (work->putPos < BOX2_PARTY_POS) {
            BoxSaveAccessor_SwapPkms(syswk->param->boxes, syswk->getTray, work->getPos, syswk->tray, work->putPos);
            func_ov170_021edab0(syswk, syswk->tray);
            func_ov170_021edb34(syswk, syswk->tray);
            if (syswk->tray != syswk->getTray) {
                func_ov170_021edab0(syswk, syswk->getTray);
                func_ov170_021edb34(syswk, syswk->getTray);
            }
        } else if (work->putPos - BOX2_PARTY_POS < count) {
            func_ov170_021dd330(syswk, &work->data[0], &work->data[1]);
            func_ov170_021dd810(syswk, work->putPos, work->getPos, syswk->getTray);
        } else {
            func_ov170_021dd39c(syswk, &work->data[0]);
            func_ov170_021dd810(syswk, work->putPos, work->getPos, syswk->getTray);
        }
    } else if (work->putPos < BOX2_PARTY_POS) {
        if (func_ov170_021dd25c(syswk, work->putPos, syswk->tray, PKM_PARAM_SPECIES_VALID, NULL) != 0) {
            func_ov170_021dd330(syswk, &work->data[1], &work->data[0]);
            func_ov170_021dd810(syswk, work->putPos, work->getPos, syswk->tray);
        } else {
            func_ov170_021dd3dc(syswk, &work->data[work->getPos - BOX2_PARTY_POS]);
            func_ov170_021dd810(syswk, work->putPos, work->getPos, syswk->tray);
        }
    } else if (work->putPos - BOX2_PARTY_POS < count) {
        func_ov170_021dd414(syswk, &work->data[0]);
    } else {
        func_ov170_021dd42c(syswk, &work->data[work->getPos - BOX2_PARTY_POS]);
    }
}


// pokebw2's Box2Main_GetScrolledTray
u32 func_ov170_021dd684(Box2SysWork *syswk, u32 tray) {
    tray += syswk->trayScroll;
    if (tray >= syswk->trayMax) {
        tray -= syswk->trayMax;
    }
    return tray;
}

BOOL func_ov170_021dd694(Box2SysWork *syswk, u32 pos) {
    u32 i;
    PartyPkm *pkm;

    for (i = 0; i < PokeParty_GetPkmCount(syswk->param->party); i++) {
        if (i != pos) {
            pkm = PokeParty_GetPkm(syswk->param->party, i);
            if (PokeParty_GetParam(pkm, PKM_PARAM_IS_EGG, NULL) == 0 && PokeParty_GetParam(pkm, PKM_PARAM_HP, NULL) != 0) {
                return TRUE;
            }
        }
    }
    return FALSE;
}


// Changes a Pokémon's forme, and records the new forme as seen
// pokebw2's PokeChangeForme
static void func_ov170_021dd6e8(Box2SysWork *syswk, BoxPkm *pkm, u32 forme) {
    PartyPkm *partyPkm;

    PML_PkmChangeForme(pkm, forme);
    partyPkm = boxPkmRegenToPartyPkm(pkm, HEAPID_BOX2_APP);
    PokeDex_RegistPkm(GameData_GetPokedex(syswk->param->gameData), partyPkm);
    GFL_HeapFree(partyPkm);
}

// Changes the forme of Arceus, Genesect and Giratina to the one their item gives, and returns whether it changed
// pokebw2's Box2Main_PokeItemFormChange
BOOL func_ov170_021dd718(Box2SysWork *syswk, BoxPkm *pkm) {
    u16 species = PML_PkmGetParam(pkm, PKM_PARAM_SPECIES, NULL);
    u16 item;
    u16 form;
    u16 newForm;

    if (species == SPECIES_ARCEUS) {
        item = PML_PkmGetParam(pkm, PKM_PARAM_ITEM, NULL);
        form = PML_PkmGetParam(pkm, PKM_PARAM_FORM, NULL);
        newForm = _getTypeForPlate(item);
        if (form != newForm) {
            func_ov170_021dd6e8(syswk, pkm, newForm);
        }
        if (form != newForm) {
            return TRUE;
        }
    } else if (species == SPECIES_GENESECT) {
        item = PML_PkmGetParam(pkm, PKM_PARAM_ITEM, NULL);
        form = PML_PkmGetParam(pkm, PKM_PARAM_FORM, NULL);
        newForm = func_02019d00(item);
        if (form != newForm) {
            func_ov170_021dd6e8(syswk, pkm, newForm);
        }
        if (form != newForm) {
            return TRUE;
        }
    } else if (species == SPECIES_GIRATINA) {
        item = PML_PkmGetParam(pkm, PKM_PARAM_ITEM, NULL);
        form = PML_PkmGetParam(pkm, PKM_PARAM_FORM, NULL);
        newForm = form;
        if (form == 0) {
            if (item == ITEM_GRISEOUS_ORB) {
                func_ov170_021dd6e8(syswk, pkm, 1);
                newForm = 1;
            }
        } else if (item != ITEM_GRISEOUS_ORB) {
            func_ov170_021dd6e8(syswk, pkm, 0);
            newForm = 0;
        }
        if (form != newForm) {
            return TRUE;
        }
    }
    return FALSE;
}

void func_ov170_021dd810(Box2SysWork *syswk, u32 putPos, u32 getPos, u32 tray) {
    if (putPos < BOX2_PARTY_POS && getPos < BOX2_PARTY_POS) {
        return;
    }
    if (putPos >= BOX2_PARTY_POS && getPos >= BOX2_PARTY_POS) {
        return;
    }
    if (putPos >= BOX2_PARTY_POS) {
        putPos = getPos;
    }
    if (func_ov170_021dd25c(syswk, putPos, tray, PKM_PARAM_SPECIES, NULL) != SPECIES_SHAYMIN) {
        return;
    }
    if (func_ov170_021dd25c(syswk, putPos, tray, PKM_PARAM_FORM, NULL) == 0) {
        return;
    }
    func_ov170_021dd6e8(syswk, func_ov170_021dd2d0(syswk, tray, putPos), 0);
    func_ov170_021dd8a8(syswk, putPos);
    if (tray == syswk->tray) {
        func_ov170_021ec48c(syswk, tray, putPos, syswk->app->pokeIconId[putPos]);
    }
    if (syswk->pos == putPos) {
        func_ov170_021defec(syswk, putPos);
    }
}


// pokebw2's Box2Main_RecalcPartyStats
void func_ov170_021dd8a8(Box2SysWork *syswk, u32 pos) {
    PartyPkm *pkm;
    BOOL encrypted;

    if (pos >= BOX2_PARTY_POS) {
        pkm = PokeParty_GetPkm(syswk->param->party, pos - BOX2_PARTY_POS);
        encrypted = PokeParty_DecryptPkm(pkm);
        PokeParty_RecalcStats(pkm);
        PokeParty_EncryptPkm(pkm, encrypted);
    }
}

// pokebw2's Box2Main_InitDexData
void func_ov170_021dd8d0(Box2SysWork *syswk) {
    syswk->app->regionalDex = PML_PersonalLoadRegionalDexTable(HEAPID_BOX2_APP, 0);
    syswk->app->nationalDex = PokeDex_IsNationalObtained(GameData_GetPokedex(syswk->param->gameData));
}

// pokebw2's Box2Main_ExitDexData
void func_ov170_021dd8f8(Box2SysWork *syswk) {
    GFL_HeapFree(syswk->app->regionalDex);
}

// pokebw2's Box2Main_IsSpeciesFlagged
BOOL func_ov170_021dd908(Box2SysWork *syswk, u32 pos) {
    u16 species = func_ov170_021dd25c(syswk, pos, syswk->tray, PKM_PARAM_SPECIES, NULL);

    return func_020092ac(syswk->param->unk20, species);
}

BOOL func_ov170_021dd930(Box2SysWork *syswk) {
    Box2PokeMoveWork *work = syswk->app->vfunk.work;
    Box2PokeMoveData *data;
    u32 i;
    u32 id;
    s16 x, y;

    if (work->cnt == 8) {
        for (i = 0; i < 7; i++) {
            data = &work->data[i];
            if (data->flag != 0) {
                func_ov170_021ec7b8(syswk->app, data->iconPos, data->mvPos, 1);
                id = syswk->app->pokeIconId[data->iconPos];
                if (data->mvPos < BOX2_PARTY_POS) {
                    func_ov170_021ec548(data->mvPos, &x, &y, syswk->unk1A);
                } else if (data->mvPos < BOX2_BOXLIST_POS) {
                    func_ov170_021ec548(data->mvPos, &x, &y, syswk->unk1A);
                    if (syswk->unk1A == 2 && func_ov170_021ef4a4(syswk->app->bgWinFrame) == FALSE) {
                        y += 0xc0;
                    }
                } else {
                    if (data->iconPos >= BOX2_PARTY_POS) {
                        func_ov170_021ec548(BOX2_BOXLIST_POS, &x, &y, syswk->unk1A);
                        if (syswk->unk1A == 2 && func_ov170_021ef4a4(syswk->app->bgWinFrame) == FALSE) {
                            y += 0xc0;
                        }
                    } else {
                        func_ov170_021ec548(data->iconPos, &x, &y, syswk->unk1A);
                    }
                    func_ov170_021ec000(syswk->app, id, FALSE);
                }
                func_ov170_021ec08c(syswk->app, id, x, y, 0);
                if (data->flag == 2) {
                    func_ov170_021ec000(syswk->app, id, FALSE);
                }
            }
        }
        func_ov170_021ed75c(syswk, syswk->pos);
        func_ov170_021ed7c4(syswk->app, BOX2_BOXLIST_POS);
        return FALSE;
    }
    work->cnt++;
    for (i = 0; i < 7; i++) {
        data = &work->data[i];
        if (data->flag != 0) {
            id = syswk->app->pokeIconId[data->iconPos];
            x = data->dx + data->vx * ((work->cnt * data->mx) >> 16);
            y = data->dy + data->vy * ((work->cnt * data->my) >> 16);
            func_ov170_021ec08c(syswk->app, id, x, y, 0);
            func_ov170_021ec768(syswk->app, data->iconPos, 0);
        }
    }
    func_ov170_021ed75c(syswk, syswk->pos);
    func_ov170_021ed7c4(syswk->app, BOX2_BOXLIST_POS);
    return TRUE;
}

BOOL func_ov170_021ddae8(Box2SysWork *syswk) {
    Box2PokeMoveWork *work = syswk->app->vfunk.work;
    Box2PokeMoveData *data;
    u32 i;
    u32 id;
    s16 x, y;

    if (work->cnt == 8) {
        for (i = 0; i < 7; i++) {
            data = &work->data[i];
            if (data->flag != 0) {
                id = syswk->app->pokeIconId[data->iconPos];
                func_ov170_021ec548(data->mvPos, &x, &y, syswk->unk1A);
                if (data->mvPos >= BOX2_PARTY_POS) {
                    y += 0x90;
                }
                func_ov170_021ec08c(syswk->app, id, x, y, 0);
            }
        }
        func_ov170_021ed7c4(syswk->app, BOX2_BOXLIST_POS);
        return FALSE;
    }
    work->cnt++;
    for (i = 0; i < 7; i++) {
        data = &work->data[i];
        if (data->flag != 0) {
            id = syswk->app->pokeIconId[data->iconPos];
            x = data->dx + data->vx * ((work->cnt * data->mx) >> 16);
            y = data->dy + data->vy * ((work->cnt * data->my) >> 16);
            func_ov170_021ec08c(syswk->app, id, x, y, 0);
            func_ov170_021ec768(syswk->app, data->iconPos, 0);
            break;
        }
    }
    func_ov170_021ed75c(syswk, syswk->pos);
    func_ov170_021ed7c4(syswk->app, BOX2_BOXLIST_POS);
    return TRUE;
}

BOOL func_ov170_021ddc00(Box2SysWork *syswk, u32 getPos, u32 putPos) {
    u32 exists;
    u32 box;

    if (putPos >= BOX2_BOXLIST_POS) {
        box = func_ov170_021dd684(syswk, putPos - BOX2_BOXLIST_POS);
        if (box == syswk->tray || howManyPokesInGeneralAreInBox(syswk->param->boxes, box) == BOX2_TRAY_POKE_MAX) {
            syswk->app->moveErr = BOX2_MOVE_ERR_BOX_FULL;
            return FALSE;
        }
        if (getPos >= BOX2_PARTY_POS) {
            if (func_ov170_021dd694(syswk, getPos - BOX2_PARTY_POS) == FALSE) {
                syswk->app->moveErr = BOX2_MOVE_ERR_LAST_BATTLER;
                return FALSE;
            }
            if (PML_ItemIsMail(PokeParty_GetParam(PokeParty_GetPkm(syswk->param->party, getPos - BOX2_PARTY_POS), PKM_PARAM_ITEM, NULL)) == TRUE) {
                syswk->app->moveErr = BOX2_MOVE_ERR_MAIL;
                return FALSE;
            }
        }
        exists = 0;
    } else {
        exists = func_ov170_021dd25c(syswk, putPos, syswk->tray, PKM_PARAM_SPECIES_VALID, NULL);
    }
    if (syswk->param->mode == 4) {
        if (syswk->param->unk14 == 1) {
            syswk->app->moveErr = 5;
            return FALSE;
        }
        if (getPos < BOX2_PARTY_POS && putPos >= BOX2_PARTY_POS && putPos < BOX2_BOXLIST_POS) {
            if (func_ov170_021dd25c(syswk, getPos, syswk->getTray, PKM_PARAM_IS_EGG, NULL) != 0) {
                syswk->app->moveErr = 4;
                return FALSE;
            }
        }
        if (getPos >= BOX2_PARTY_POS && getPos < BOX2_BOXLIST_POS && putPos < BOX2_PARTY_POS && exists != 0) {
            if (func_ov170_021dd25c(syswk, putPos, syswk->tray, PKM_PARAM_IS_EGG, NULL) != 0) {
                syswk->app->moveErr = 4;
                return FALSE;
            }
        }
    }
    if (getPos >= BOX2_PARTY_POS) {
        if (func_ov170_021dd694(syswk, getPos - BOX2_PARTY_POS) == FALSE) {
            if (exists == 0) {
                if ((putPos < BOX2_PARTY_POS || putPos >= BOX2_BOXLIST_POS) && syswk->param->mode != 4) {
                    syswk->app->moveErr = BOX2_MOVE_ERR_LAST_BATTLER;
                    return FALSE;
                }
            } else if (func_ov170_021dd25c(syswk, putPos, syswk->tray, PKM_PARAM_IS_EGG, NULL) != 0 && putPos < BOX2_PARTY_POS && syswk->param->mode != 4) {
                syswk->app->moveErr = BOX2_MOVE_ERR_LAST_BATTLER;
                return FALSE;
            }
        }
        if (putPos < BOX2_PARTY_POS) {
            if (PML_ItemIsMail(PokeParty_GetParam(PokeParty_GetPkm(syswk->param->party, getPos - BOX2_PARTY_POS), PKM_PARAM_ITEM, NULL)) == TRUE) {
                syswk->app->moveErr = BOX2_MOVE_ERR_MAIL;
                return FALSE;
            }
        }
    } else if (putPos >= BOX2_PARTY_POS && exists != 0) {
        if (PML_ItemIsMail(PokeParty_GetParam(PokeParty_GetPkm(syswk->param->party, putPos - BOX2_PARTY_POS), PKM_PARAM_ITEM, NULL)) == TRUE) {
            syswk->app->moveErr = BOX2_MOVE_ERR_MAIL;
            return FALSE;
        }
        if (func_ov170_021dd25c(syswk, getPos, syswk->getTray, PKM_PARAM_IS_EGG, NULL) != 0 && func_ov170_021dd694(syswk, putPos - BOX2_PARTY_POS) == FALSE && syswk->param->mode != 4) {
            syswk->app->moveErr = BOX2_MOVE_ERR_LAST_BATTLER;
            return FALSE;
        }
    }
    syswk->app->moveErr = BOX2_MOVE_ERR_NONE;
    return TRUE;
}


BOOL func_ov170_021dde20(Box2SysWork *syswk, u32 pos, u32 slot) {
    PartyPkm *pkm;
    u32 item;

    if (func_ov170_021dd25c(syswk, slot, syswk->tray, 0xA9, NULL) != 0) {
        return FALSE;
    }
    if (func_ov170_021dd694(syswk, pos - BOX2_PARTY_POS) == FALSE) {
        return FALSE;
    }
    pkm = PokeParty_GetPkm(syswk->param->party, pos - BOX2_PARTY_POS);
    item = PokeParty_GetParam(pkm, PKM_PARAM_ITEM, NULL);
    return PML_ItemIsMail(item) != TRUE;
}


// Whether the items of the Pokémon at getPos and putPos can be swapped
// pokebw2's Box2Main_PokeItemMoveCheck
BOOL func_ov170_021dde80(Box2SysWork *syswk, u32 getPos, u32 putPos) {
    if (func_ov170_021dd25c(syswk, putPos, syswk->tray, PKM_PARAM_SPECIES_VALID, NULL) == 0) {
        return FALSE;
    }
    if (func_ov170_021dd25c(syswk, putPos, syswk->tray, PKM_PARAM_IS_EGG, NULL) != 0) {
        return FALSE;
    }
    if (PML_ItemIsMail(func_ov170_021dd25c(syswk, getPos, syswk->tray, PKM_PARAM_ITEM, NULL)) == TRUE) {
        return FALSE;
    }
    return PML_ItemIsMail(func_ov170_021dd25c(syswk, putPos, syswk->tray, PKM_PARAM_ITEM, NULL)) == TRUE ? FALSE
                                                                                                           : TRUE;
}

void func_ov170_021ddefc(Box2SysWork *syswk, Box2PokeMoveData *move) {
    s16 x, y;

    func_ov170_021ec0b0(syswk->app, syswk->app->pokeIconId[move->iconPos], &move->dx, &move->dy, 0);
    if (move->flag != 2) {
        if (move->mvPos < BOX2_BOXLIST_POS) {
            func_ov170_021ec548(move->mvPos, &x, &y, syswk->unk1A);
        } else {
            func_ov170_021eda70(syswk->app, move->mvPos - BOX2_BOXLIST_POS, &x, &y);
        }
    } else {
        if (syswk->tray > syswk->getTray) {
            func_ov170_021ec0b0(syswk->app, 0, &x, &y, 0);
        } else {
            func_ov170_021ec0b0(syswk->app, 1, &x, &y, 0);
        }
    }
    if (move->dx <= x) {
        move->vx = 1;
        move->mx = (s32)((x - move->dx) << 16) / 8;
    } else {
        move->vx = -1;
        move->mx = (s32)((move->dx - x) << 16) / 8;
    }
    if (move->dy <= y) {
        move->vy = 1;
        move->my = (s32)((y - move->dy) << 16) / 8;
    } else {
        move->vy = -1;
        move->my = (s32)((move->dy - y) << 16) / 8;
    }
}

void func_ov170_021ddfd4(Box2SysWork *syswk, Box2PokeMoveData *data) {
    s16 x, y;
    u8 id = syswk->app->pokeIconId[data->mvPos];

    func_ov170_021ec0b0(syswk->app, syswk->app->pokeIconId[data->iconPos], &data->dx, &data->dy, 0);
    func_ov170_021ec0b0(syswk->app, id, &x, &y, 0);
    if (data->dx <= x) {
        data->vx = 1;
        data->mx = (s32)((x - data->dx) << 16) / 8;
    } else {
        data->vx = -1;
        data->mx = (s32)((data->dx - x) << 16) / 8;
    }
    if (data->dy <= y) {
        data->vy = 1;
        data->my = (s32)((y - data->dy) << 16) / 8;
    } else {
        data->vy = -1;
        data->my = (s32)((data->dy - y) << 16) / 8;
    }
}

void func_ov170_021de07c(Box2SysWork *syswk, Box2PokeMoveData *data) {
    s16 x, y;

    func_ov170_021ec0b0(syswk->app, syswk->app->pokeIconId[data->iconPos], &data->dx, &data->dy, 0);
    func_ov170_021ec548(data->mvPos, &x, &y, syswk->unk1A);
    y += 0xc0;
    if (data->dx <= x) {
        data->vx = 1;
        data->mx = (s32)((x - data->dx) << 16) / 8;
    } else {
        data->vx = -1;
        data->mx = (s32)((data->dx - x) << 16) / 8;
    }
    if (data->dy <= y) {
        data->vy = 1;
        data->my = (s32)((y - data->dy) << 16) / 8;
    } else {
        data->vy = -1;
        data->my = (s32)((data->dy - y) << 16) / 8;
    }
}


void func_ov170_021de120(Box2SysWork *syswk, u32 dfPos, u32 mvPos) {
    Box2PokeMoveData *work = syswk->app->vfunk.work;

    work[0].iconPos = 0x24;
    work[0].mvPos = mvPos;
    work[0].dfPos = dfPos;
    work[0].flag = 1;
    func_ov170_021ddefc(syswk, &work[0]);

    work[1].iconPos = mvPos;
    work[1].mvPos = dfPos;
    work[1].dfPos = mvPos;
    work[1].flag = 1;
    func_ov170_021ddefc(syswk, &work[1]);
}

BOOL func_ov170_021de158(Box2SysWork *syswk, u32 pos, u32 putPos) {
    Box2PokeMoveWork *work = syswk->app->vfunk.work;
    Box2PokeMoveData *data = work->data;
    u32 i;
    u32 count;

    for (i = 0; i < 7; i++) {
        data[i].iconPos = 0;
        data[i].mvPos = 0;
        data[i].dfPos = 0;
        data[i].flag = 0;
    }
    work->setPos = putPos;
    if (putPos == BOX2_GET_NONE) {
        work->getPos = BOX2_GET_NONE;
        work->putPos = BOX2_GET_NONE;
        data[0].iconPos = BOX2_BOXLIST_POS;
        data[0].mvPos = pos;
        data[0].dfPos = pos;
        data[0].flag = 1;
        if (syswk->tray == syswk->getTray) {
            data[1].iconPos = pos;
            data[1].mvPos = pos;
            data[1].dfPos = pos;
            data[1].flag = 1;
        }
        func_ov170_021ddefc(syswk, &data[0]);
        return FALSE;
    }
    if ((pos == putPos && (pos >= BOX2_PARTY_POS || syswk->tray == syswk->getTray)) ||
        !func_ov170_021ddc00(syswk, pos, putPos)) {
        work->getPos = BOX2_GET_NONE;
        work->putPos = BOX2_GET_NONE;
        data[0].iconPos = BOX2_BOXLIST_POS;
        data[0].mvPos = pos;
        data[0].dfPos = pos;
        data[0].flag = 1;
        data[1].iconPos = pos;
        data[1].mvPos = pos;
        data[1].dfPos = pos;
        data[1].flag = 1;
        func_ov170_021ddefc(syswk, &data[0]);
        return FALSE;
    }
    work->getPos = pos;
    work->putPos = putPos;
    count = PokeParty_GetPkmCount(syswk->param->party);
    if (putPos >= BOX2_BOXLIST_POS) {
        data[0].iconPos = BOX2_BOXLIST_POS;
        data[0].mvPos = putPos;
        data[0].dfPos = BOX2_BOXLIST_POS;
        data[0].flag = 1;
        func_ov170_021ddefc(syswk, &data[0]);
        if (pos >= BOX2_PARTY_POS) {
            for (i = pos - (BOX2_PARTY_POS - 1); i < count; i++) {
                data[i].iconPos = i + BOX2_PARTY_POS;
                data[i].mvPos = i + (BOX2_PARTY_POS - 1);
                data[i].dfPos = i + BOX2_PARTY_POS;
                data[i].flag = 1;
                func_ov170_021de07c(syswk, &data[i]);
            }
            data[count].iconPos = pos;
            data[count].mvPos = count + (BOX2_PARTY_POS - 1);
            data[count].dfPos = pos;
            data[count].flag = 1;
            func_ov170_021de07c(syswk, &data[count]);
        }
        return TRUE;
    }
    if (syswk->tray != syswk->getTray && pos < BOX2_PARTY_POS) {
        if (putPos >= BOX2_PARTY_POS && putPos > count + BOX2_PARTY_POS) {
            putPos = count + BOX2_PARTY_POS;
        }
        data[0].iconPos = BOX2_BOXLIST_POS;
        data[0].mvPos = putPos;
        data[0].dfPos = pos;
        data[0].flag = 1;
        data[1].iconPos = putPos;
        data[1].mvPos = putPos;
        data[1].dfPos = putPos;
        data[1].flag = 2;
        func_ov170_021ddefc(syswk, &data[0]);
        func_ov170_021ddefc(syswk, &data[1]);
        return TRUE;
    }
    if (pos < BOX2_PARTY_POS) {
        if (putPos < BOX2_PARTY_POS) {
            func_ov170_021de120(syswk, pos, putPos);
        } else if (putPos - BOX2_PARTY_POS < count) {
            func_ov170_021de120(syswk, pos, putPos);
        } else {
            func_ov170_021de120(syswk, pos, count + BOX2_PARTY_POS);
        }
    } else if (putPos < BOX2_PARTY_POS) {
        if (func_ov170_021dd25c(syswk, putPos, syswk->tray, PKM_PARAM_SPECIES_VALID, NULL) != 0) {
            func_ov170_021de120(syswk, pos, putPos);
        } else {
            u32 get = pos - BOX2_PARTY_POS;

            for (i = get + 1; i < count; i++) {
                data[i].iconPos = i + BOX2_PARTY_POS;
                data[i].mvPos = i + (BOX2_PARTY_POS - 1);
                data[i].dfPos = i + BOX2_PARTY_POS;
                data[i].flag = 1;
                func_ov170_021ddefc(syswk, &data[i]);
            }
            data[get].iconPos = BOX2_BOXLIST_POS;
            data[get].mvPos = putPos;
            data[get].dfPos = pos;
            data[get].flag = 1;
            func_ov170_021ddefc(syswk, &data[get]);
            data[count].iconPos = putPos;
            data[count].mvPos = count + (BOX2_PARTY_POS - 1);
            data[count].dfPos = putPos;
            data[count].flag = 1;
            func_ov170_021ddefc(syswk, &data[count]);
        }
    } else {
        if (putPos - BOX2_PARTY_POS < count) {
            func_ov170_021de120(syswk, pos, putPos);
        } else {
            u32 get = pos - BOX2_PARTY_POS;

            for (i = get + 1; i < count; i++) {
                data[i].iconPos = i + BOX2_PARTY_POS;
                data[i].mvPos = i + (BOX2_PARTY_POS - 1);
                data[i].dfPos = i + BOX2_PARTY_POS;
                data[i].flag = 1;
                func_ov170_021ddefc(syswk, &data[i]);
            }
            data[get].iconPos = BOX2_BOXLIST_POS;
            data[get].mvPos = count + (BOX2_PARTY_POS - 1);
            data[get].dfPos = pos;
            data[get].flag = 1;
            func_ov170_021ddefc(syswk, &data[get]);
        }
    }
    return TRUE;
}


void func_ov170_021de428(Box2SysWork *syswk, u32 getPos, u32 putPos) {
    Box2PokeMoveWork *work = syswk->app->vfunk.work;
    u32 count;
    u32 i;
    u32 index;

    for (i = 0; i < 7; i++) {
        work->data[i].iconPos = 0;
        work->data[i].mvPos = 0;
        work->data[i].dfPos = 0;
        work->data[i].flag = 0;
    }
    if (putPos == BOX2_GET_NONE || func_ov170_021dde20(syswk, getPos, putPos) == FALSE) {
        work->getPos = BOX2_GET_NONE;
        work->putPos = BOX2_GET_NONE;
        work->data[0].iconPos = BOX2_BOXLIST_POS;
        work->data[0].mvPos = getPos;
        work->data[0].dfPos = getPos;
        work->data[0].flag = 1;
        work->data[1].iconPos = getPos;
        work->data[1].mvPos = getPos;
        work->data[1].dfPos = getPos;
        work->data[1].flag = 1;
        func_ov170_021ddefc(syswk, &work->data[0]);
        return;
    }
    work->getPos = getPos;
    work->putPos = putPos;
    count = PokeParty_GetPkmCount(syswk->param->party);
    if (putPos < BOX2_PARTY_POS) {
        index = getPos - BOX2_PARTY_POS;
        for (i = index + 1; i < count; i++) {
            work->data[i].iconPos = i + BOX2_PARTY_POS;
            work->data[i].mvPos = i + BOX2_PARTY_POS - 1;
            work->data[i].dfPos = i + BOX2_PARTY_POS;
            work->data[i].flag = 1;
            func_ov170_021ddfd4(syswk, &work->data[i]);
        }
        work->data[index].iconPos = BOX2_BOXLIST_POS;
        work->data[index].mvPos = putPos;
        work->data[index].dfPos = getPos;
        work->data[index].flag = 1;
        func_ov170_021ddfd4(syswk, &work->data[index]);
        work->data[count].iconPos = putPos;
        work->data[count].mvPos = count + BOX2_PARTY_POS - 1;
        work->data[count].dfPos = putPos;
        work->data[count].flag = 1;
        func_ov170_021ddfd4(syswk, &work->data[count]);
    }
}

static inline void SetMoveData(Box2PokeMoveData *data, u16 iconPos, u16 mvPos, u16 dfPos) {
    data->iconPos = iconPos;
    data->mvPos = mvPos;
    data->dfPos = dfPos;
    data->flag = 1;
}

void func_ov170_021de530(Box2SysWork *syswk, u32 pos, u32 putPos) {
    Box2PokeMoveWork *work = syswk->app->vfunk.work;
    u32 i;

    for (i = 0; i < 7; i++) {
        work->data[i].iconPos = 0;
        work->data[i].mvPos = 0;
        work->data[i].dfPos = 0;
        work->data[i].flag = 0;
    }
    if (putPos == BOX2_GET_NONE || func_ov170_021dd25c(syswk, putPos, syswk->tray, PKM_PARAM_SPECIES_VALID, NULL) != 0) {
        work->getPos = BOX2_GET_NONE;
        work->putPos = BOX2_GET_NONE;
        SetMoveData(&work->data[0], BOX2_BOXLIST_POS, pos, pos);
        SetMoveData(&work->data[1], pos, pos, pos);
        func_ov170_021ddefc(syswk, &work->data[0]);
    } else {
        work->getPos = pos;
        work->putPos = putPos;
        func_ov170_021de120(syswk, pos, PokeParty_GetPkmCount(syswk->param->party) + BOX2_PARTY_POS);
    }
}


u16 func_ov170_021de5c8(Box2SysWork *syswk, u16 iconPos) {
    Box2PokeMoveData *work = syswk->app->vfunk.work;
    u32 i;

    for (i = 0; i < 7; i++) {
        if (work[i].flag != 0 && work[i].iconPos == iconPos) {
            return work[i].mvPos;
        }
    }
    return syswk->pos;
}

void func_ov170_021de5f8(Box2SysWork *syswk, u32 pos) {
    Box2PokeMoveData *work = syswk->app->vfunk.work;
    u32 idx = pos - BOX2_PARTY_POS;
    u32 i;

    for (i = 0; i < 6; i++) {
        work[i].iconPos = i + BOX2_PARTY_POS;
        work[i].mvPos = i + BOX2_PARTY_POS;
        work[i].dfPos = i + BOX2_PARTY_POS;
        work[i].flag = 0;
    }
    work[i].flag = 0;

    for (i = idx + 1; i < 6; i++) {
        work[i].mvPos = i + 0x1D;
        work[i].flag = 1;
        func_ov170_021ddefc(syswk, &work[i]);
    }
    work[idx].mvPos = 0x23;
    work[idx].flag = 1;
    func_ov170_021ddefc(syswk, &work[idx]);
}

void func_ov170_021de66c(Box2SysWork *syswk, Box2PokeMoveData *work) {
    u8 icons[7];
    u16 pos = 0xFFFF;
    u16 i;

    for (i = 0; i < 7; i++) {
        if (work[i].flag != 0) {
            icons[i] = syswk->app->pokeIconId[work[i].dfPos];
        }
    }
    for (i = 0; i < 7; i++) {
        if (work[i].flag != 0) {
            if (work[i].mvPos <= 0x24) {
                syswk->app->pokeIconId[work[i].mvPos] = icons[i];
                if (work[i].iconPos == 0x24) {
                    pos = work[i].mvPos;
                }
            } else {
                syswk->app->pokeIconId[0x24] = icons[i];
                if (work[i].iconPos == 0x24) {
                    pos = 0x24;
                }
            }
        }
    }
    if (pos != 0xFFFF) {
        func_ov170_021ecbd8(syswk->app, pos);
    }
    for (i = 0; i < 0x24; i++) {
        func_ov170_021ec768(syswk->app, i, 1);
    }
}


u32 func_ov170_021de720(u16 move) {
    return 0xFFFFFFFF;
}

void func_ov170_021de728(Box2SysWork *syswk) {
    Box2PokeFreeWork *work;
    BoxPkm *pkm;
    u32 i;
    u32 waza;

    syswk->app->unkA190 = GFL_HeapAllocate(HEAPID_TAIL(HEAPID_BOX2_APP), sizeof(Box2PokeFreeWork), FALSE, "box2_main.c", 0xa70);
    work = syswk->app->unkA190;
    work->cap = syswk->app->unk9E74[syswk->app->pokeIconId[syswk->pos]];
    work->checkCnt = 0;
    work->checkFlag = 0;
    pkm = func_ov170_021dd2d0(syswk, syswk->tray, syswk->pos);
    for (i = 0; i < 4; i++) {
        waza = func_ov170_021de720(PML_PkmGetParam(pkm, PKM_PARAM_MOVE1 + i, NULL));
        if (waza != 0xFFFFFFFF) {
            work->checkFlag |= (1 << waza);
            return;
        }
    }
}


void func_ov170_021de7b4(Box2SysWork *syswk) {
    GFL_HeapFree(syswk->app->unkA190);
}

BOOL func_ov170_021de7c8(Box2SysWork *syswk) {
    Box2PokeFreeWork *work = syswk->app->unkA190;
    u16 pos = work->checkCnt;
    u32 n;
    u32 i;
    BoxPkm *pkm;

    if (pos == BOX2_TRAY_MAX * BOX2_TRAY_POKE_MAX + 6) {
        return FALSE;
    }
    for (n = 0; n < 15; n++) {
        if (pos < BOX2_TRAY_MAX * BOX2_TRAY_POKE_MAX) {
            int tray = pos / BOX2_TRAY_POKE_MAX;
            int slot = pos % BOX2_TRAY_POKE_MAX;

            if (tray == syswk->tray && slot == syswk->pos) {
                pkm = NULL;
            } else {
                pkm = func_ov170_021dd2d0(syswk, tray, slot);
            }
        } else if (pos - BOX2_TRAY_MAX * BOX2_TRAY_POKE_MAX == syswk->pos - BOX2_PARTY_POS) {
            pkm = NULL;
        } else {
            pkm = func_ov170_021dd2d0(syswk, BOX2_GET_NONE, pos - BOX2_TRAY_MAX * BOX2_TRAY_POKE_MAX);
        }
        if (pkm != NULL && PML_PkmGetParam(pkm, PKM_PARAM_SPECIES_VALID, NULL) != 0) {
            for (i = 0; i < 4; i++) {
                u32 waza = func_ov170_021de720(PML_PkmGetParam(pkm, PKM_PARAM_MOVE1 + i, NULL));
                if (waza != 0xFFFFFFFF) {
                    work->checkFlag = work->checkFlag & (0xFF ^ (1 << waza));
                }
            }
        }
        work->checkCnt++;
        pos = work->checkCnt;
        if (pos == BOX2_TRAY_MAX * BOX2_TRAY_POKE_MAX + 6) {
            return FALSE;
        }
    }
    return TRUE;
}


// Records whether Chatot is still in the party, which keeps its recorded cry
// pokebw2's Box2Main_UpdateChatter
void func_ov170_021de898(Box2SysWork *syswk) {
    Box2Param *param = syswk->param;

    if (param->mode == 4 || param->mode == 5) {
        return;
    }
    checkChatotInParty(getChatterDataAddress(param->gameData), param->party);
}

// Starts moving the item icon to the position pos, from where it is
// pokebw2's ItemIconMoveMakeCore
void func_ov170_021de8b4(Box2SysWork *syswk, u32 setPos, u32 putPos, u32 mvMode, BOOL hand) {
    Box2ItemMoveWork *work = syswk->app->vfunk.work;
    s16 cx, cy;
    s16 px, py;
    int diff;

    func_ov170_021ec0b0(syswk->app, 0xA, &cx, &cy, 0);
    func_ov170_021ec548(putPos, &px, &py, mvMode);
    if (hand == TRUE) {
        px += 8;
        py += 8;
    } else {
        py += 4;
    }
    work->putPos = putPos;
    work->setPos = setPos;
    work->cnt = 0;
    work->mvMode = mvMode;
    if (cx > px) {
        work->mvX = 1;
        diff = cx - px;
    } else {
        work->mvX = 0;
        diff = px - cx;
    }
    work->mx = (diff << 8) / BOX2_ITEMMOVE_CNT;
    if (cy > py) {
        work->mvY = 1;
        diff = cy - py;
    } else {
        work->mvY = 0;
        diff = py - cy;
    }
    work->my = (diff << 8) / BOX2_ITEMMOVE_CNT;
    work->nowX = cx << 8;
    work->nowY = cy << 8;
}

// Starts moving the item icon to the tray scroll arrow
// pokebw2's Box2Main_ItemIconMoveMakeScroll
void func_ov170_021de980(Box2SysWork *syswk, u32 mvMode) {
    Box2ItemMoveWork *work = syswk->app->vfunk.work;
    s16 cx, cy;
    s16 px, py;
    int diff;

    func_ov170_021ec0b0(syswk->app, BOX2_ACTOR_ITEM_ICON, &cx, &cy, 0);
    if (syswk->tray > syswk->getTray) {
        func_ov170_021ec0b0(syswk->app, 0, &px, &py, 0);
    } else {
        func_ov170_021ec0b0(syswk->app, 1, &px, &py, 0);
    }
    work->putPos = BOX2_GET_NONE;
    work->setPos = BOX2_GET_NONE;
    work->cnt = 0;
    work->mvMode = mvMode;
    if (cx > px) {
        work->mvX = 1;
        diff = cx - px;
    } else {
        work->mvX = 0;
        diff = px - cx;
    }
    work->mx = (diff << 8) / BOX2_ITEMMOVE_CNT;
    if (cy > py) {
        work->mvY = 1;
        diff = cy - py;
    } else {
        work->mvY = 0;
        diff = py - cy;
    }
    work->my = (diff << 8) / BOX2_ITEMMOVE_CNT;
    work->nowX = cx << 8;
    work->nowY = cy << 8;
}

// pokebw2's Box2Main_ItemIconMoveMakeHand
void func_ov170_021dea3c(Box2SysWork *syswk, u32 setPos, u32 putPos, u32 mvMode) {
    func_ov170_021de8b4(syswk, setPos, putPos, mvMode, TRUE);
}

void func_ov170_021dea4c(Box2SysWork *syswk, u32 putPos, u32 mvMode) {
    func_ov170_021de8b4(syswk, syswk->app->pokePutKey, putPos, mvMode, FALSE);
}


// pokebw2's ItemIconMoveMain
BOOL func_ov170_021dea6c(Box2SysWork *syswk, BOOL hand) {
    Box2ItemMoveWork *work = syswk->app->vfunk.work;

    if (work->cnt == BOX2_ITEMMOVE_CNT) {
        if (work->putPos != BOX2_GET_NONE) {
            if (hand == TRUE) {
                func_ov170_021ed164(syswk->app, work->putPos, work->mvMode);
            } else {
                func_ov170_021ed198(syswk->app, work->putPos, work->mvMode);
            }
        }
        func_ov170_021ed310(syswk->app);
        return FALSE;
    }
    if (work->mvX == 0) {
        work->nowX += work->mx;
    } else {
        work->nowX -= work->mx;
    }
    if (work->mvY == 0) {
        work->nowY += work->my;
    } else {
        work->nowY -= work->my;
    }
    func_ov170_021ed14c(syswk->app, (s16)(work->nowX >> 8), (s16)(work->nowY >> 8));
    func_ov170_021ed310(syswk->app);
    work->cnt++;
    return TRUE;
}

// pokebw2's Box2Main_VFuncItemIconMoveHand
BOOL func_ov170_021deafc(Box2SysWork *syswk) {
    return func_ov170_021dea6c(syswk, TRUE);
}

// pokebw2's Box2Main_VFuncItemIconMove
BOOL func_ov170_021deb08(Box2SysWork *syswk) {
    return func_ov170_021dea6c(syswk, FALSE);
}

// Moves the box list's scroll by mv boxes, around the boxes that are open
// pokebw2's Box2Main_GetTrayScroll
u8 func_ov170_021deb14(Box2SysWork *syswk, s8 mv) {
    s8 pos = (s8)syswk->trayScroll + mv;

    if (pos < 0) {
        pos = pos + (s8)syswk->trayMax;
    } else if (pos >= syswk->trayMax) {
        pos = pos - (s8)syswk->trayMax;
    }
    return pos;
}

// pokebw2's WallCharLoad
void func_ov170_021deb40(Box2SysWork *syswk, u32 wallpaper, u32 offset) {
    NNSG2dCharacterData *chr;
    void *buf = GFL_G2DIOReadBGNCGR(ARCID_BOX2, wallpaper + 14, TRUE, &chr, HEAPID_BOX2_APP);

    GFL_BGSysLoadChar(3, chr->pRawData, chr->szByte, offset);
    GFL_HeapFree(buf);
}

// pokebw2's WallPaletteLoad
void func_ov170_021deb70(Box2SysWork *syswk, u32 wallpaper, u32 palette) {
    GFL_BGSysLoadNCLRDefault(ARCID_BOX2, wallpaper + 38, 0, palette * 32, 0x20, HEAPID_BOX2_APP);
}

// pokebw2's WallScreenLoad
static void func_ov170_021deb90(Box2SysWork *syswk, u32 wallpaper, u32 x, u32 charOffset, u32 palette) {
    NNSG2dScreenData *scr;
    void *buf;
    u16 *raw;
    u32 px;
    u8 y, i;
    u16 tile;

    buf = GFL_G2DIOReadNSCR(ARCID_BOX2, 13, TRUE, &scr, HEAPID_BOX2_APP);
    raw = (u16 *)scr->rawData;
    for (y = 0; y < 20; y++) {
        u16 *row = &raw[y * 21];
        px = x;
        for (i = 0; i < 21; i++) {
            tile = charOffset + ((palette << 12) + (row[i] & 0xfff));
            GFL_BGSysLoadScrAreaAll(3, &tile, px, y + 1, 1, 1);
            px++;
            if (px >= 64) {
                px = 0;
            }
        }
    }
    GFL_HeapFree(buf);
    GFL_BGSysFillScrArea(3, 0xe287, px, 0, 2, 20, 17);
    GFL_BGSysFillScrArea(3, 0xe287 + 41, px, 5, 2, 1, 17);
}

// pokebw2's WallGraSet
static void func_ov170_021dec64(Box2SysWork *syswk, u32 wallpaper, u32 x, u32 charOffset, u32 palette) {
    func_ov170_021deb40(syswk, wallpaper, charOffset);
    func_ov170_021deb70(syswk, wallpaper, palette);
    func_ov170_021deb90(syswk, wallpaper, x, charOffset, palette);
    GFL_BGSysQueueScrLoad(3);
}

void func_ov170_021dec98(Box2SysWork *syswk, u32 wallpaper, u32 dir) {
    u32 charOffset;
    u32 palette;
    u32 frame;

    if (dir == BOX2_TRAY_SCROLL_L) {
        syswk->app->wallPx -= 0x17;
        if (syswk->app->wallPx < 0) {
            syswk->app->wallPx += 0x40;
        }
    } else if (dir == BOX2_TRAY_SCROLL_R) {
        syswk->app->wallPx += 0x17;
        if (syswk->app->wallPx >= 0x40) {
            syswk->app->wallPx -= 0x40;
        }
    }
    if (syswk->app->wallArea == 0) {
        charOffset = 0x25C;
        palette = 14;
    } else {
        charOffset = 0xB8;
        palette = 15;
    }
    syswk->app->wallArea ^= 1;
    func_ov170_021dec64(syswk, wallpaper, syswk->app->wallPx, charOffset, palette);
    if (dir == BOX2_TRAY_SCROLL_NONE) {
        func_ov170_021eb7e8(syswk, syswk->tray, 7);
        func_ov170_021ee224(syswk->app, 8, FALSE);
    } else {
        frame = 7;
        if (func_ov170_021ee234(syswk->app, 7) == TRUE) {
            frame = 8;
        }
        func_ov170_021eb7e8(syswk, syswk->tray, frame);
        func_ov170_021ee244(syswk->app, frame, dir);
        func_ov170_021ee224(syswk->app, frame, TRUE);
    }
}

void func_ov170_021ded58(Box2SysWork *syswk, u32 wallpaper) {
    u32 charOffset;
    u32 palette;

    if (syswk->app->wallArea == 0) {
        charOffset = 0x25C;
        palette = 14;
    } else {
        charOffset = 0xB8;
        palette = 15;
    }
    syswk->app->wallArea ^= 1;
    func_ov170_021deb40(syswk, wallpaper, charOffset);
    func_02021640(syswk->app->palFade, ARCID_BOX2, wallpaper + 0x26, HEAPID_BOX2_APP, 0, 0x20, palette << 4);
    func_ov170_021deb90(syswk, wallpaper, syswk->app->wallPx, charOffset, palette);
    GFL_BGSysQueueScrLoad(3);
}


// pokebw2's Box2Main_GetWallPaperNumber
u32 func_ov170_021dedcc(Box2SysWork *syswk, u32 tray) {
    return getBoxNumFromIdx(syswk->param->boxes, tray);
}

// Reads the data of a Pokémon that the upper screen shows; NULL if there is none
// pokebw2's PokeInfoDataMake
static Box2PokeInfo *func_ov170_021dedd8(BoxPkm *pkm) {
    Box2PokeInfo *info;
    u32 i;

    if (PML_PkmGetParam(pkm, PKM_PARAM_SPECIES_VALID, NULL) != 0) {
        info = GFL_HeapAllocate(HEAPID_BOX2_APP, sizeof(Box2PokeInfo), FALSE, "box2_main.c", 3282);
        info->pkm = pkm;
        info->species = PML_PkmGetParam(pkm, PKM_PARAM_SPECIES, NULL);
        info->item = PML_PkmGetParam(pkm, PKM_PARAM_ITEM, NULL);
        info->pid = PML_PkmGetParam(pkm, PKM_PARAM_PID, NULL);
        info->type1 = PML_PkmGetParam(pkm, PKM_PARAM_TYPE1, NULL);
        info->type2 = PML_PkmGetParam(pkm, PKM_PARAM_TYPE2, NULL);
        info->ability = PML_PkmGetParam(pkm, PKM_PARAM_ABILITY, NULL);
        info->nature = PML_PkmGetNature(pkm);
        info->mark = PML_PkmGetParam(pkm, PKM_PARAM_MARKINGS, NULL);
        info->level = PML_PkmGetParam(pkm, PKM_PARAM_LEVEL, NULL);
        info->egg = PML_PkmGetParam(pkm, PKM_PARAM_IS_EGG, NULL);
        if (info->egg == 0) {
            info->sex = PML_PkmGetSex(pkm);
        } else {
            info->sex = 0;
        }
        if (PML_PkmIsRare(pkm) == TRUE) {
            info->rare = 1;
        } else {
            info->rare = 0;
        }
        if (doesPokerusHaveDuration(pkm) == TRUE) {
            info->pokerus = 1;
        } else if (doesPokeHavePokerus(pkm) == TRUE) {
            info->pokerus = 2;
        } else {
            info->pokerus = 0;
        }
        // Nidoran's names already say their sex
        if (info->species != SPECIES_NIDORAN_F && info->species != SPECIES_NIDORAN_M && info->egg == 0) {
            info->sexPut = 1;
        } else {
            info->sexPut = 0;
        }
        for (i = 0; i < 4; i++) {
            info->waza[i] = PML_PkmGetParam(pkm, PKM_PARAM_MOVE1 + i, NULL);
        }
    } else {
        info = NULL;
    }
    return info;
}

// pokebw2's PokeInfoDataFree
static void func_ov170_021def58(Box2PokeInfo *info) {
    GFL_HeapFree(info);
}

// pokebw2's PokeInfoPutModeNormal
static void func_ov170_021def60(Box2SysWork *syswk, Box2PokeInfo *info) {
    func_ov170_021ecccc(syswk, info, 11);
    func_ov170_021ed028(syswk->app, info);
    func_ov170_021eb2a4(syswk, info);
    func_ov170_021df11c(syswk, info->mark);
    func_ov170_021df144(syswk->app, info);
    GFL_BGSysMoveBGReq(5, BG_MOVE_SET_Y, 0);
}

// Shows the Pokémon at a position on the upper screen; FALSE if there is none
// pokebw2's Box2Main_PokeInfoPutCore
BOOL func_ov170_021def98(Box2SysWork *syswk, u32 tray, u32 pos) {
    BoxPkm *pkm = func_ov170_021dd2d0(syswk, tray, pos);
    BOOL encrypted;
    Box2PokeInfo *info;

    if (pkm != NULL) {
        encrypted = PML_PkmDecrypt(pkm);
        info = func_ov170_021dedd8(pkm);
        if (info != NULL) {
            func_ov170_021def60(syswk, info);
            func_ov170_021def58(info);
            PML_PkmReEncrypt(pkm, encrypted);
        } else {
            func_ov170_021df040(syswk);
            PML_PkmReEncrypt(pkm, encrypted);
            return FALSE;
        }
    } else {
        func_ov170_021df040(syswk);
        return FALSE;
    }
    return TRUE;
}

// pokebw2's Box2Main_PokeInfoPut
BOOL func_ov170_021defec(Box2SysWork *syswk, u32 pos) {
    return func_ov170_021def98(syswk, syswk->tray, pos);
}

// Redraws the upper screen's Pokémon, as after its item changed
// pokebw2's Box2Main_PokeInfoRewrite
void func_ov170_021deff8(Box2SysWork *syswk, u32 pos) {
    BoxPkm *pkm = func_ov170_021dd2d0(syswk, syswk->tray, pos);
    BOOL encrypted = PML_PkmDecrypt(pkm);
    Box2PokeInfo *info = func_ov170_021dedd8(pkm);

    func_ov170_021ecccc(syswk, info, 11);
    func_ov170_021ed028(syswk->app, info);
    func_ov170_021eb2a4(syswk, info);
    func_ov170_021def58(info);
    PML_PkmReEncrypt(pkm, encrypted);
}

void func_ov170_021df040(Box2SysWork *syswk) {
    u32 i;

    if (syswk->app->unk9E74[0xB] != NULL) {
        func_ov170_021ec000(syswk->app, 0xB, FALSE);
    }
    if (syswk->app->unk9E74[0xC] != NULL) {
        func_ov170_021ec000(syswk->app, 0xC, FALSE);
    }
    for (i = 0x1C; i < 0x2D; i++) {
        func_ov170_021ec000(syswk->app, i, FALSE);
    }
    func_ov170_021eb324(syswk->app);
    func_ov170_021df128(syswk->app);
    func_ov170_021ec000(syswk->app, 0x19, FALSE);
    func_ov170_021ec000(syswk->app, 0x1A, FALSE);
    func_ov170_021ec000(syswk->app, 0x1B, FALSE);
    GFL_BGSysMoveBGReq(5, BG_MOVE_SET_Y, 0xC0);
}


// pokebw2's Box2Main_PokeSelectOff
void func_ov170_021df0bc(Box2SysWork *syswk) {
    func_ov170_021df040(syswk);
    func_ov170_021ed6e4(syswk, 0);
    syswk->pos = BOX2_GET_NONE;
}

// Shows the six marks of a Pokémon from actor base on, each lit if it is set
// pokebw2's MarkingPut
static void func_ov170_021df0d4(Box2SysWork *syswk, u32 base, u32 mark) {
    u16 i;
    u16 anim;

    for (i = 0; i < 6; i++) {
        if (mark & (1 << i)) {
            anim = i * 2 + 1;
        } else {
            anim = i * 2;
        }
        func_ov170_021ebfa8(syswk->app, base + i, anim);
        func_ov170_021ec000(syswk->app, base + i, TRUE);
    }
}

// pokebw2's Box2Main_MarkingPutMain
void func_ov170_021df110(Box2SysWork *syswk, u32 mark) {
    func_ov170_021df0d4(syswk, 13, mark);
}

// pokebw2's Box2Main_MarkingPutSub
void func_ov170_021df11c(Box2SysWork *syswk, u32 mark) {
    func_ov170_021df0d4(syswk, 19, mark);
}

// pokebw2's Box2Main_MarkingOff
static void func_ov170_021df128(Box2AppWork *app) {
    u32 i;

    for (i = 0; i < 6; i++) {
        func_ov170_021ec000(app, i + 19, FALSE);
    }
}

// Shows the shiny star and the Pokérus marks of the upper screen's Pokémon
// pokebw2's PokeInfoIconPut
static void func_ov170_021df144(Box2AppWork *app, Box2PokeInfo *info) {
    if (info->rare == 0 || info->egg == 1) {
        func_ov170_021ec000(app, 25, FALSE);
    } else {
        func_ov170_021ec000(app, 25, TRUE);
    }
    if (info->pokerus == 0) {
        func_ov170_021ec000(app, 26, FALSE);
        func_ov170_021ec000(app, 27, FALSE);
    } else if (info->pokerus == 1) {
        func_ov170_021ec000(app, 26, FALSE);
        func_ov170_021ec000(app, 27, TRUE);
    } else {
        func_ov170_021ec000(app, 26, TRUE);
        func_ov170_021ec000(app, 27, FALSE);
    }
}

// pokebw2's AreaCheck
BOOL func_ov170_021df1bc(int x, int y, const Box2Area *area) {
    if (x >= area->left && x <= area->right && y >= area->top && y < area->bottom) {
        return TRUE;
    }
    return FALSE;
}

// The tray position at (x, y), or BOX2_GET_NONE
// pokebw2's TrayPokePutAreaCheck
u32 func_ov170_021df1dc(s16 x, s16 y) {
    if (func_ov170_021df1bc(x, y, &data_ov170_021ef798) == TRUE) {
        if (x < 12) {
            x = 0;
        } else if (x >= 156) {
            x = 5;
        } else {
            x = (x - 12) / 24;
        }
        y = (y - 40) / 24;
        return x + y * 6;
    }
    return BOX2_GET_NONE;
}

// The party position at (x, y), or BOX2_GET_NONE
// pokebw2's PartyPokePutAreaCheck
u32 func_ov170_021df228(s16 x, s16 y, const Box2Area *areas) {
    u32 i;

    for (i = 0; i < 6; i++) {
        if (func_ov170_021df1bc(x, y, &areas[i]) == TRUE) {
            return i + BOX2_PARTY_POS;
        }
    }
    return BOX2_GET_NONE;
}

u32 func_ov170_021df254(Box2SysWork *syswk, s16 x, s16 y) {
    u32 pos = func_ov170_021ef224(syswk->app->tpx, syswk->app->tpy);
    u32 tray;

    if (pos != BOX2_GET_NONE) {
        tray = pos - BOX2_BOXLIST_POS + syswk->trayScroll;
        if (tray >= syswk->trayMax) {
            tray -= syswk->trayMax;
        }
        if (tray == syswk->tray) {
            pos = BOX2_GET_NONE;
        }
    }
    return pos;
}

int func_ov170_021df288(Box2SysWork *syswk) {
    PStatusParam *param = GFL_HeapAllocate(HEAPID_BOX2, sizeof(PStatusParam), FALSE, "box2_main.c", 0xEAC);

    if (syswk->pos < BOX2_PARTY_POS) {
        param->party = func_ov170_021dd2d0(syswk, syswk->getTray, 0);
        param->dataType = 2;
        param->partyCount = BOX2_TRAY_POKE_MAX;
        param->partyIndex = syswk->pos;
    } else {
        param->party = syswk->param->party;
        param->dataType = 1;
        param->partyCount = PokeParty_GetPkmCount(syswk->param->party);
        param->partyIndex = syswk->pos - BOX2_PARTY_POS;
    }
    param->page = 0;
    param->gameData = syswk->param->gameData;
    param->trainerData = syswk->param->trainerData;
    param->isNationalDex = syswk->param->unk1C;
    if (syswk->param->unk14 == 1) {
        param->mode = 6;
    } else {
        param->mode = 0;
    }
    QueueGameProc(syswk->procManager, OVERLAY_PSTATUS, &data_ov131_021dc800, param);
    syswk->subProcWork = param;
    return 0;
}


// pokebw2's Box2Main_PokeStatusExit
int func_ov170_021df324(Box2SysWork *syswk) {
    PStatusParam *param = syswk->subProcWork;

    if (syswk->unk18 == 0) {
        if (syswk->pos < BOX2_PARTY_POS) {
            syswk->pos = param->partyIndex;
        } else {
            syswk->pos = param->partyIndex + BOX2_PARTY_POS;
        }
    }
    GFL_HeapFree(syswk->subProcWork);
    return 0;
}

extern GameProcFunctions BAG_PROC_FUNCTIONS;

int func_ov170_021df348(Box2SysWork *syswk) {
    void *param = func_0202e474(syswk->param->gameData, 0, 2, 0x4B);

    QueueGameProc(syswk->procManager, OVERLAY_ID(70), &BAG_PROC_FUNCTIONS, param);
    syswk->subProcWork = param;
    return 0;
}


int func_ov170_021df378(Box2SysWork *syswk) {
    BagProcessData *bag = syswk->subProcWork;

    syswk->subRet = bag->item;
    GFL_HeapFree(syswk->subProcWork);
    return 0;
}

BOOL func_ov170_021df38c(Box2SysWork *syswk) {
    SaveControl *save = GameData_GetSaveControl(syswk->param->gameData);
    TrainerGameInfoSave *gameInfo = getTrainerGameInfoAddress(save);
    Box2NameInWork *work = GFL_HeapAllocate(0x4B, sizeof(Box2NameInWork), TRUE, "box2_main.c", 0xF1F);

    work->name = GFL_StrBufCreate(0x14, 0x4B);
    loadBoxNameToStrbuf(syswk->param->boxes, syswk->tray, work->name);
    work->param = setupNameEntry(0x4B, 2, 0, 0, 8, work->name, gameInfo);
    QueueGameProc(syswk->procManager, OVERLAY_ID(194), &data_ov194_021f5580, work->param);
    syswk->subProcWork = work;
    return FALSE;
}

// Black's code at 0x021df404-0x021e1e80, once the port's ov170_021df404.c: the same file (its .text follows
// without a gap and its code reads the party area tables above)

typedef struct seq seq;

void func_ov170_021ec804(Box2SysWork *syswk, s16 dir);
extern u8 data_ov170_021efa7c;
void func_ov170_021eab50(Box2AppWork *app);
void func_ov170_021ebf74(Box2AppWork *app);
void func_ov170_021e0fb0(Box2AppWork *param_1, u32 param_2, u32 param_3);
void func_ov170_021de428(Box2SysWork *syswk, u32 getPos, u32 putPos);
BOOL func_ov170_021ef248(void);
BOOL func_ov170_021ddae8(Box2SysWork *syswk);

// pokebw2's Box2Main_NameInExit
int func_ov170_021df404(Box2SysWork *syswk) {
    Box2NameInWork *work = syswk->subProcWork;

    if (work->param->unk1C == 0) {
        getBoxNameFromStrbuf(syswk->param->boxes, syswk->tray, work->param->name);
    }
    syswk->subRet = work->param->unk1C;
    func_ov010_0216c0c0(work->param);
    GFL_StrBufFree(work->name);
    GFL_HeapFree(syswk->subProcWork);
    return 0;
}

void func_ov170_021df43c(Box2SysWork *syswk, u32 x, u32 y) {
    func_ov170_021ec08c(syswk->app, (u32)syswk->app->pokeIconId[0x24], (s16)x, (s16)y + -8, 0);
    func_ov170_021ed7c4(syswk->app, 0x24);
    syswk->app->tpx = x;
    syswk->app->tpy = y;
}

BOOL func_ov170_021df484(Box2SysWork *syswk) {
    Box2IrqWork *vf = &syswk->app->vfunk;
    BOOL frameMove = func_ov170_021ef400(syswk);
    BOOL frameMove2 = func_ov170_021ef5c4(syswk->app->bgWinFrame);
    u32 x, y;
    u32 pos;
    u32 res;

    switch (vf->seq) {
    case 0:
        if (func_02035c38() == TRUE) {
            func_ov170_021ef57c(syswk->app->bgWinFrame);
        }
        vf->seq = 1;
        break;
    case 1:
        if (func_02035cb8(&x, &y) == FALSE) {
            syswk->app->unkA170[1] = 0;
            vf->cnt = 0;
            pos = BOX2_GET_NONE;
            if (func_ov170_021ef770(syswk->app->bgWinFrame) == TRUE) {
                func_ov170_021ee008(syswk, 0, 0);
                pos = func_ov170_021df254(syswk, (s16)syswk->app->tpx, (s16)syswk->app->tpy);
                if (pos == BOX2_GET_NONE || func_ov170_021de158(syswk, syswk->pos, pos) == FALSE) {
                    func_ov170_021ef75c(syswk->app->bgWinFrame);
                    GFL_SndSEPlay(SEQ_SE_SYS_42);
                    vf->seq = 14;
                    break;
                }
            }
            if (func_ov170_021ef45c(syswk->app->bgWinFrame) == TRUE) {
                pos = func_ov170_021df228((s16)syswk->app->tpx, (s16)syswk->app->tpy, data_ov170_021ef79c);
                if (pos == BOX2_GET_NONE) {
                    pos = func_ov170_021df1dc((s16)syswk->app->tpx, (s16)syswk->app->tpy);
                }
            }
            if (func_ov170_021ef480(syswk->app->bgWinFrame) == TRUE) {
                pos = func_ov170_021df228((s16)syswk->app->tpx, (s16)syswk->app->tpy, data_ov170_021ef7b4);
            }
            if (syswk->param->mode == 4 && syswk->param->unk14 == 1) {
                pos = BOX2_GET_NONE;
                syswk->app->moveErr = 5;
            }
            func_ov170_021de158(syswk, syswk->pos, pos);
            if (func_ov170_021ef60c(syswk->app->bgWinFrame) == FALSE) {
                func_ov170_021ed6e4(syswk, 0);
            }
            if (pos != BOX2_GET_NONE) {
                syswk->pos = func_ov170_021de5c8(syswk, 36);
            }
            if (func_ov170_021ef45c(syswk->app->bgWinFrame) == FALSE &&
                func_ov170_021ef770(syswk->app->bgWinFrame) == FALSE && frameMove == FALSE) {
                func_ov170_021ef534(syswk->app->bgWinFrame);
            }
            vf->seq = 2;
            break;
        }
        if (vf->cnt != 0) {
            vf->cnt--;
        }
        if (frameMove == FALSE && frameMove2 == FALSE) {
            if (func_ov170_021ef45c(syswk->app->bgWinFrame) == FALSE &&
                func_ov170_021ef770(syswk->app->bgWinFrame) == FALSE && func_ov170_021ef248() == FALSE) {
                syswk->getTray = BOX2_GET_NONE;
                syswk->unk1A = 2;
                func_ov170_021ef3d8(syswk->app->bgWinFrame);
                GFL_SndSEPlay(SEQ_SE_SYS_42);
                func_ov170_021ecb20(syswk, 1, 0);
                func_ov170_021ed888(syswk->app, 1);
                func_ov170_021ee364(syswk, 6, syswk->pos);
                if (syswk->param->mode == 4) {
                    func_02026124(syswk->app->cursorMove, 39);
                }
                func_ov170_021ee038(syswk, 2, 1, 2);
                syswk->app->curPos = syswk->pos;
                syswk->app->unk68 = 23;
            }
            if (vf->cnt == 0) {
                res = func_ov170_021ef1b4();
                if (res == 0) {
                    GFL_SndSEPlay(SEQ_SE_SELECT1);
                    func_ov170_021dd16c(syswk, FALSE);
                    vf->seq = 6;
                } else if (res == 1) {
                    GFL_SndSEPlay(SEQ_SE_SELECT1);
                    func_ov170_021dd16c(syswk, TRUE);
                    vf->seq = 7;
                }
            }
            if (syswk->param->mode != 4) {
                if (func_ov170_021ef264() != 0xffffffff) {
                    if (syswk->app->unkA170[1] == 0) {
                        if (func_ov170_021ef770(syswk->app->bgWinFrame) == TRUE) {
                            func_ov170_021ef75c(syswk->app->bgWinFrame);
                            func_ov170_021dd090(syswk, 10);
                            GFL_SndSEPlay(SEQ_SE_DECIDE1);
                            GFL_SndSEPlay(SEQ_SE_SYS_42);
                            syswk->app->unkA170[1] = 1;
                            vf->seq = 11;
                        }
                        if (func_ov170_021ef45c(syswk->app->bgWinFrame) == TRUE) {
                            func_ov170_021ef3a0(syswk->app->bgWinFrame);
                            func_ov170_021dd090(syswk, 11);
                            GFL_SndSEPlay(SEQ_SE_DECIDE1);
                            GFL_SndSEPlay(SEQ_SE_SYS_42);
                            syswk->app->unkA170[1] = 1;
                            vf->seq = 13;
                        }
                    }
                } else {
                    syswk->app->unkA170[1] = 0;
                }
            }
            if (func_ov170_021ef770(syswk->app->bgWinFrame) == TRUE) {
                res = func_ov170_021ef1c4();
                if (res == 0) {
                    GFL_SndSEPlay(SEQ_SE_SELECT1);
                    func_ov170_021dd1c4(syswk, -1);
                    vf->seq = 8;
                    vf->cnt = 0;
                } else if (res == 1) {
                    GFL_SndSEPlay(SEQ_SE_SELECT1);
                    func_ov170_021dd1c4(syswk, 1);
                    vf->seq = 9;
                    vf->cnt = 0;
                }
            }
            if (func_ov170_021ef770(syswk->app->bgWinFrame) == TRUE) {
                pos = func_ov170_021ef224(x, y);
                if (pos != BOX2_GET_NONE) {
                    func_ov170_021ee008(syswk, pos - 36, 1);
                } else {
                    func_ov170_021ee008(syswk, 0, 0);
                }
            }
        }
        func_ov170_021df43c(syswk, x, y);
        break;
    case 2:
        if (func_ov170_021dd930(syswk) == FALSE) {
            GFL_SndSEPlay(SEQ_SE_SYS_40);
            vf->seq = 3;
        }
        break;
    case 3:
        if (frameMove == FALSE && frameMove2 == FALSE) {
            vf->seq = 16;
        }
        break;
    case 6:
        if (func_ov170_021e00c4(syswk) == FALSE) {
            vf->cnt = 16;
            vf->seq = 1;
        }
        if (func_02035cb8(&x, &y) == TRUE) {
            func_ov170_021df43c(syswk, x, y);
        }
        break;
    case 7:
        if (func_ov170_021e0128(syswk) == FALSE) {
            vf->cnt = 16;
            vf->seq = 1;
        }
        if (func_02035cb8(&x, &y) == TRUE) {
            func_ov170_021df43c(syswk, x, y);
        }
        break;
    case 8:
        if (func_ov170_021e1a28(syswk) == FALSE) {
            vf->seq = 1;
        }
        if (func_02035cb8(&x, &y) == TRUE) {
            func_ov170_021df43c(syswk, x, y);
        }
        break;
    case 9:
        if (func_ov170_021e19d4(syswk) == FALSE) {
            vf->seq = 1;
        }
        if (func_02035cb8(&x, &y) == TRUE) {
            func_ov170_021df43c(syswk, x, y);
        }
        break;
    case 10:
        if (func_ov170_021e01c4(syswk) == FALSE) {
            vf->seq = 1;
        }
        if (func_02035cb8(&x, &y) == TRUE) {
            func_ov170_021df43c(syswk, x, y);
        }
        break;
    case 11:
        func_ov170_021dcfc0(syswk);
        if (func_ov170_021e01e0(syswk) == FALSE) {
            func_ov170_021ef35c(syswk->app->bgWinFrame);
            func_ov170_021ef36c(syswk->app->bgWinFrame);
            func_ov170_021ec5a0(syswk, 1);
            func_ov170_021ef680(syswk->app);
            func_ov170_021ef68c(syswk->app);
            GFL_SndSEPlay(SEQ_SE_SYS_42);
            vf->seq = 12;
        }
        if (func_02035cb8(&x, &y) == TRUE) {
            func_ov170_021df43c(syswk, x, y);
        }
        break;
    case 12:
        if (frameMove == FALSE) {
            vf->seq = 1;
        }
        if (func_02035cb8(&x, &y) == TRUE) {
            func_ov170_021df43c(syswk, x, y);
        }
        break;
    case 13:
        func_ov170_021dcfc0(syswk);
        if (frameMove == FALSE) {
            func_ov170_021dd1e8(syswk);
            func_ov170_021ef69c(syswk->app);
            func_ov170_021ef670(syswk->app);
            GFL_SndSEPlay(SEQ_SE_SYS_42);
            vf->seq = 10;
        }
        if (func_02035cb8(&x, &y) == TRUE) {
            func_ov170_021df43c(syswk, x, y);
        }
        break;
    case 16:
        func_ov170_021de66c(syswk, vf->work);
        if (func_ov170_021ef480(syswk->app->bgWinFrame) == TRUE) {
            func_ov170_021ed7c4(syswk->app, syswk->pos);
            func_ov170_021ed6e4(syswk, 1);
        } else {
            func_ov170_021ed6e4(syswk, 0);
            func_020260e4(syswk->app->cursorMove, syswk->pos);
            func_ov170_021ee3e4(syswk, syswk->pos);
            func_ov170_021ed55c(syswk, 1);
        }
        vf->seq = 0;
        return FALSE;
    case 14:
        if (func_ov170_021e01e0(syswk) == FALSE) {
            func_ov170_021ef35c(syswk->app->bgWinFrame);
            func_ov170_021ef36c(syswk->app->bgWinFrame);
            GFL_SndSEPlay(SEQ_SE_SYS_42);
            vf->seq = 15;
        }
        break;
    case 15:
        if (frameMove == FALSE) {
            syswk->unk1A = 2;
            func_ov170_021de158(syswk, syswk->pos, func_ov170_021df1dc((s16)syswk->app->tpx, (s16)syswk->app->tpy));
            func_ov170_021ed6e4(syswk, 0);
            vf->seq = 2;
        }
        break;
    }
    return TRUE;
}

BOOL func_ov170_021dfa30(Box2SysWork *syswk) {
    Box2IrqWork *vf = &syswk->app->vfunk;
    BOOL frameMove = FALSE;
    BOOL frameMove2;
    u32 x, y;
    u32 pos;
    u32 res;
    BOOL dir;

    if (vf->seq != 10 && vf->seq != 11 && vf->seq != 14 && func_0202ce40(syswk->app->bgWinFrame, 9) == TRUE) {
        frameMove = func_ov170_021e01c4(syswk);
    }
    frameMove2 = func_ov170_021ef5c4(syswk->app->bgWinFrame);

    switch (vf->seq) {
    case 0:
        if (func_02035c38() == TRUE) {
            func_ov170_021ef57c(syswk->app->bgWinFrame);
        }
        vf->seq = 1;
        break;
    case 1:
        if (func_02035cb8(&x, &y) == FALSE) {
            syswk->app->unkA170[1] = 0;
            pos = BOX2_GET_NONE;
            if (func_ov170_021ef770(syswk->app->bgWinFrame) == TRUE) {
                pos = func_ov170_021df254(syswk, (s16)syswk->app->tpx, (s16)syswk->app->tpy);
                if (syswk->pos >= BOX2_TRAY_POKE_MAX &&
                    (pos == BOX2_GET_NONE || func_ov170_021de158(syswk, syswk->pos, pos) == FALSE)) {
                    func_ov170_021ef75c(syswk->app->bgWinFrame);
                    func_ov170_021ee008(syswk, 0, 0);
                    GFL_SndSEPlay(SEQ_SE_SYS_42);
                    vf->seq = 14;
                    break;
                }
            }
            if (func_ov170_021ef45c(syswk->app->bgWinFrame) == TRUE) {
                pos = func_ov170_021df228((s16)syswk->app->tpx, (s16)syswk->app->tpy, data_ov170_021ef79c);
            }
            if (pos == BOX2_GET_NONE) {
                pos = func_ov170_021df1dc((s16)syswk->app->tpx, (s16)syswk->app->tpy);
            }
            if (func_ov170_021de158(syswk, syswk->pos, pos) == FALSE) {
                pos = BOX2_GET_NONE;
            }
            if (func_ov170_021ef60c(syswk->app->bgWinFrame) == FALSE) {
                func_ov170_021ed6e4(syswk, 0);
            }
            if (pos != BOX2_GET_NONE) {
                if (pos >= 36) {
                    syswk->pos = pos - 2;
                } else {
                    syswk->pos = func_ov170_021de5c8(syswk, 36);
                }
            } else {
                func_ov170_021ee008(syswk, 0, 0);
            }
            if (func_ov170_021ef770(syswk->app->bgWinFrame) == FALSE &&
                func_ov170_021ef45c(syswk->app->bgWinFrame) == FALSE && frameMove == FALSE) {
                func_ov170_021ef534(syswk->app->bgWinFrame);
            }
            if (pos == BOX2_GET_NONE && syswk->pos < BOX2_TRAY_POKE_MAX && syswk->tray != syswk->getTray) {
                dir = func_ov170_021dd210(syswk, syswk->tray, syswk->getTray);
                syswk->tray = syswk->getTray;
                func_ov170_021ec35c(syswk, syswk->tray);
                func_ov170_021dec98(syswk, func_ov170_021dedcc(syswk, syswk->tray), dir);
                if (dir == FALSE) {
                    vf->seq = 4;
                } else {
                    vf->seq = 5;
                }
            } else {
                vf->seq = 2;
            }
            vf->cnt = 0;
            break;
        }
        if (vf->cnt != 0) {
            vf->cnt--;
        }
        if (frameMove == FALSE && frameMove2 == FALSE && func_ov170_021ef770(syswk->app->bgWinFrame) == FALSE &&
            func_ov170_021ef45c(syswk->app->bgWinFrame) == FALSE && syswk->pos != func_ov170_021ef180(x, y)) {
            syswk->trayScroll = syswk->tray;
            func_ov170_021dd1e8(syswk);
            GFL_SndSEPlay(SEQ_SE_SYS_42);
            func_ov170_021ee364(syswk, 4, syswk->pos);
            func_ov170_021ee038(syswk, 2, 1, 2);
            syswk->app->curPos = syswk->pos;
            syswk->app->unk68 = 23;
        }
        if (vf->cnt == 0) {
            res = func_ov170_021ef1b4();
            if (res == 0) {
                GFL_SndSEPlay(SEQ_SE_SELECT1);
                func_ov170_021dd16c(syswk, FALSE);
                vf->seq = 6;
            } else if (res == 1) {
                GFL_SndSEPlay(SEQ_SE_SELECT1);
                func_ov170_021dd16c(syswk, TRUE);
                vf->seq = 7;
            }
        }
        if (frameMove == FALSE && frameMove2 == FALSE) {
            if (func_ov170_021ef264() != 0xffffffff) {
                if (syswk->app->unkA170[1] == 0) {
                    if (func_ov170_021ef770(syswk->app->bgWinFrame) == TRUE) {
                        func_ov170_021ef75c(syswk->app->bgWinFrame);
                        func_ov170_021dd090(syswk, 10);
                        GFL_SndSEPlay(SEQ_SE_DECIDE1);
                        GFL_SndSEPlay(SEQ_SE_SYS_42);
                        syswk->app->unkA170[1] = 1;
                        vf->seq = 11;
                    }
                    if (func_ov170_021ef45c(syswk->app->bgWinFrame) == TRUE) {
                        func_ov170_021ef3a0(syswk->app->bgWinFrame);
                        func_ov170_021dd090(syswk, 11);
                        GFL_SndSEPlay(SEQ_SE_DECIDE1);
                        GFL_SndSEPlay(SEQ_SE_SYS_42);
                        syswk->app->unkA170[1] = 1;
                        vf->seq = 13;
                    }
                }
            } else {
                syswk->app->unkA170[1] = 0;
            }
        }
        if (func_ov170_021ef770(syswk->app->bgWinFrame) == TRUE) {
            res = func_ov170_021ef1c4();
            if (res == 0) {
                GFL_SndSEPlay(SEQ_SE_SELECT1);
                func_ov170_021dd1c4(syswk, -1);
                vf->seq = 8;
                vf->cnt = 0;
            } else if (res == 1) {
                GFL_SndSEPlay(SEQ_SE_SELECT1);
                func_ov170_021dd1c4(syswk, 1);
                vf->seq = 9;
                vf->cnt = 0;
            }
        }
        if (func_ov170_021ef770(syswk->app->bgWinFrame) == TRUE) {
            pos = func_ov170_021ef224(x, y);
            if (pos != BOX2_GET_NONE) {
                func_ov170_021ee008(syswk, pos - 36, 1);
            } else {
                func_ov170_021ee008(syswk, 0, 0);
            }
        }
        func_ov170_021df43c(syswk, x, y);
        break;
    case 4:
        if (func_ov170_021e00c4(syswk) == FALSE) {
            vf->seq = 2;
        }
        break;
    case 5:
        if (func_ov170_021e0128(syswk) == FALSE) {
            vf->seq = 2;
        }
        break;
    case 2:
        if (func_ov170_021dd930(syswk) == FALSE) {
            GFL_SndSEPlay(SEQ_SE_SYS_40);
            vf->seq = 3;
        }
        break;
    case 3:
        if (frameMove == FALSE && frameMove2 == FALSE) {
            vf->seq = 16;
        }
        break;
    case 6:
        if (func_ov170_021e00c4(syswk) == FALSE) {
            vf->cnt = 16;
            vf->seq = 1;
        }
        if (func_02035cb8(&x, &y) == TRUE) {
            func_ov170_021df43c(syswk, x, y);
        }
        break;
    case 7:
        if (func_ov170_021e0128(syswk) == FALSE) {
            vf->cnt = 16;
            vf->seq = 1;
        }
        if (func_02035cb8(&x, &y) == TRUE) {
            func_ov170_021df43c(syswk, x, y);
        }
        break;
    case 8:
        if (func_ov170_021e1a28(syswk) == FALSE) {
            vf->seq = 1;
        }
        if (func_02035cb8(&x, &y) == TRUE) {
            func_ov170_021df43c(syswk, x, y);
        }
        break;
    case 9:
        if (func_ov170_021e19d4(syswk) == FALSE) {
            vf->seq = 1;
        }
        if (func_02035cb8(&x, &y) == TRUE) {
            func_ov170_021df43c(syswk, x, y);
        }
        break;
    case 10:
        if (func_ov170_021e01c4(syswk) == FALSE) {
            vf->seq = 1;
        }
        if (func_02035cb8(&x, &y) == TRUE) {
            func_ov170_021df43c(syswk, x, y);
        }
        break;
    case 11:
        func_ov170_021dcfc0(syswk);
        if (func_ov170_021e01e0(syswk) == FALSE) {
            func_ov170_021ef35c(syswk->app->bgWinFrame);
            func_ov170_021ef36c(syswk->app->bgWinFrame);
            func_ov170_021ec5a0(syswk, 1);
            func_ov170_021ef680(syswk->app);
            func_ov170_021ef68c(syswk->app);
            GFL_SndSEPlay(SEQ_SE_SYS_42);
            vf->seq = 12;
        }
        if (func_02035cb8(&x, &y) == TRUE) {
            func_ov170_021df43c(syswk, x, y);
        }
        break;
    case 12:
        if (func_ov170_021ef400(syswk) == FALSE) {
            syswk->unk1A = 2;
            vf->seq = 1;
        }
        if (func_02035cb8(&x, &y) == TRUE) {
            func_ov170_021df43c(syswk, x, y);
        }
        break;
    case 13:
        func_ov170_021dcfc0(syswk);
        if (func_ov170_021ef400(syswk) == FALSE) {
            func_ov170_021dd1e8(syswk);
            func_ov170_021ef69c(syswk->app);
            func_ov170_021ef670(syswk->app);
            GFL_SndSEPlay(SEQ_SE_SYS_42);
            vf->seq = 10;
        }
        if (func_02035cb8(&x, &y) == TRUE) {
            func_ov170_021df43c(syswk, x, y);
        }
        break;
    case 14:
        if (func_ov170_021e01e0(syswk) == FALSE) {
            func_ov170_021ef35c(syswk->app->bgWinFrame);
            func_ov170_021ef36c(syswk->app->bgWinFrame);
            GFL_SndSEPlay(SEQ_SE_SYS_42);
            vf->seq = 15;
        }
        break;
    case 15:
        if (func_ov170_021ef400(syswk) == FALSE) {
            func_ov170_021de158(syswk, syswk->pos, func_ov170_021df1dc((s16)syswk->app->tpx, (s16)syswk->app->tpy));
            func_ov170_021ed6e4(syswk, 0);
            vf->seq = 2;
        }
        break;
    case 16:
        func_ov170_021de66c(syswk, vf->work);
        if (func_ov170_021ef770(syswk->app->bgWinFrame) == FALSE &&
            func_ov170_021ef45c(syswk->app->bgWinFrame) == FALSE) {
            func_ov170_021ed7c4(syswk->app, syswk->pos);
            func_ov170_021ed6e4(syswk, 1);
        } else {
            func_ov170_021ed6e4(syswk, 0);
            func_020260e4(syswk->app->cursorMove, syswk->pos);
            func_ov170_021ee3e4(syswk, syswk->pos);
            func_ov170_021ed55c(syswk, 1);
        }
        vf->seq = 0;
        return FALSE;
    }
    return TRUE;
}

u32 func_ov170_021e002c(Box2SysWork *syswk) {
    struct Box2IrqWork *vf = &syswk->app->vfunk;

    switch (vf->seq) {
    case 0:
        func_ov170_021de5f8(syswk, syswk->pos);
        vf->seq++;
    case 1:
        if (func_ov170_021dd930(syswk) == FALSE) {
            func_ov170_021de66c(syswk, vf->work);
            vf->seq = 0;
            return 0;
        }
    default:
        return 1;
    }
}


BOOL func_ov170_021e0068(Box2SysWork *syswk) {
    Box2IrqWork *vf = &syswk->app->vfunk;
    u32 putPos;

    switch (vf->seq) {
    case 0:
        putPos = 0x1e + PokeParty_GetPkmCount(syswk->param->party);
        func_ov170_021de158(syswk, syswk->pos, putPos);
        func_ov170_021ed6e4(syswk, 0);
        vf->seq++;
        break;
    case 1:
        if (func_ov170_021dd930(syswk) == 0) {
            func_ov170_021de66c(syswk, vf->work);
            GFL_SndSEPlay(0x667);
            vf->seq = 0;
            return 0;
        }
        break;
    }
    return 1;
}


BOOL func_ov170_021e00c4(Box2SysWork *param_1) {
    BOOL BVar1;

    if ((param_1->app->vfunk).cnt == 0x17) {
        func_ov170_021ed55c(param_1, 1);
        BVar1 = func_ov170_021ef770(param_1->app->bgWinFrame);
        if (BVar1 == 1) {
            func_ov170_021edf5c(param_1);
        }
        (param_1->app->vfunk).cnt = 0;
        return 0;
    }
    GFL_BGSysMoveBGReq(3, 2, 8);
    func_ov170_021ec804(param_1, 8);
    func_ov170_021ee26c(param_1->app, 8);
    func_ov170_021ef5c4(param_1->app->bgWinFrame);
    (param_1->app->vfunk).cnt = (param_1->app->vfunk).cnt + 1;
    return 1;
}

BOOL func_ov170_021e0128(Box2SysWork *param_1) {
    BOOL BVar1;

    if ((param_1->app->vfunk).cnt == 0x17) {
        func_ov170_021ed55c(param_1, 1);
        BVar1 = func_ov170_021ef770(param_1->app->bgWinFrame);
        if (BVar1 == 1) {
            func_ov170_021edf5c(param_1);
        }
        (param_1->app->vfunk).cnt = 0;
        return 0;
    }
    GFL_BGSysMoveBGReq(3, 1, 8);
    func_ov170_021ec804(param_1, -8);
    func_ov170_021ee26c(param_1->app, -8);
    func_ov170_021ef5c4(param_1->app->bgWinFrame);
    (param_1->app->vfunk).cnt = (param_1->app->vfunk).cnt + 1;
    return 1;
}

BOOL func_ov170_021e0190(Box2SysWork *syswk) {
    BOOL BVar1;

    BVar1 = func_ov170_021ef5c4(syswk->app->bgWinFrame);
    if (BVar1 != 0) {
        return 1;
    }
    return 0;
}

BOOL func_ov170_021e01a8(Box2SysWork *arg0) {
    return func_ov170_021ef6e0(arg0);
}

u32 func_ov170_021e01b0(Box2SysWork *param_1) {
    BOOL BVar1;

    BVar1 = func_ov170_021ef400(param_1);
    if (BVar1 != 0) {
        return 1;
    }
    return 0;
}

BOOL func_ov170_021e01c4(Box2SysWork *syswk) {
    BOOL BVar1;

    BVar1 = func_0202cdd4(syswk->app->bgWinFrame, 9);
    func_ov170_021ed918(syswk, 8);
    return BVar1;
}

BOOL func_ov170_021e01e0(Box2SysWork *syswk) {
    BOOL BVar1;

    BVar1 = func_0202cdd4(syswk->app->bgWinFrame, 9);
    func_ov170_021ed918(syswk, -8);
    return BVar1;
}

BOOL func_ov170_021e0200(Box2SysWork *syswk) {
    Box2IrqWork *vf = &syswk->app->vfunk;
    BOOL frameMove2 = func_ov170_021ef5c4(syswk->app->bgWinFrame);
    BOOL frameMove = func_ov170_021ef400(syswk);
    u32 x;
    u32 y;
    u32 pos;
    u32 res;

    switch (vf->seq) {
    case 0:
        if (func_02035c38() == TRUE) {
            func_ov170_021ef57c(syswk->app->bgWinFrame);
        }
        vf->seq = 1;
        break;
    case 1:
        if (func_ov170_021ef480(syswk->app->bgWinFrame) == FALSE) {
            func_ov170_021ed888(syswk->app, TRUE);
        }
        if (func_02035cb8(&x, &y) == FALSE) {
            pos = BOX2_GET_NONE;
            if (func_ov170_021ef480(syswk->app->bgWinFrame) == FALSE && frameMove == FALSE) {
                pos = func_ov170_021df1dc((s16)syswk->app->tpx, (s16)syswk->app->tpy);
            }
            func_ov170_021de428(syswk, syswk->pos, pos);
            if (func_ov170_021ef60c(syswk->app->bgWinFrame) == FALSE) {
                func_ov170_021ed6e4(syswk, FALSE);
            }
            syswk->pos = func_ov170_021de5c8(syswk, 0x24);
            if (syswk->pos < BOX2_PARTY_POS) {
                vf->seq = 6;
            } else {
                if (func_ov170_021ef480(syswk->app->bgWinFrame) == FALSE) {
                    GFL_SndSEPlay(0x669);
                }
                func_ov170_021ef36c(syswk->app->bgWinFrame);
                func_ov170_021ef534(syswk->app->bgWinFrame);
                vf->seq = 4;
            }
            vf->cnt = 0;
            break;
        }
        if (vf->cnt != 0) {
            vf->cnt--;
        }
        if (frameMove2 == FALSE && frameMove == FALSE) {
            if (func_ov170_021ef480(syswk->app->bgWinFrame) == TRUE) {
                if (func_ov170_021ef248() == FALSE) {
                    syswk->getTray = BOX2_GET_NONE;
                    syswk->unk1A = 0;
                    func_ov170_021ef3a0(syswk->app->bgWinFrame);
                    GFL_SndSEPlay(0x669);
                }
            } else if (vf->cnt == 0) {
                res = func_ov170_021ef1b4();
                if (res == 0) {
                    GFL_SndSEPlay(0x548);
                    func_ov170_021dd16c(syswk, FALSE);
                    vf->seq = 2;
                } else if (res == 1) {
                    GFL_SndSEPlay(0x548);
                    func_ov170_021dd16c(syswk, TRUE);
                    vf->seq = 3;
                }
            }
        }
        func_ov170_021df43c(syswk, x, y);
        break;
    case 2:
        if (func_ov170_021e00c4(syswk) == FALSE) {
            vf->cnt = 0x10;
            vf->seq = 1;
        }
        if (func_02035cb8(&x, &y) == TRUE) {
            func_ov170_021df43c(syswk, x, y);
        }
        break;
    case 3:
        if (func_ov170_021e0128(syswk) == FALSE) {
            vf->cnt = 0x10;
            vf->seq = 1;
        }
        if (func_02035cb8(&x, &y) == TRUE) {
            func_ov170_021df43c(syswk, x, y);
        }
        break;
    case 4:
        if (func_ov170_021dd930(syswk) == FALSE) {
            GFL_SndSEPlay(0x667);
            vf->seq = 5;
        }
        break;
    case 5:
        if (frameMove == FALSE && frameMove2 == FALSE) {
            func_ov170_021de66c(syswk, vf->work);
            func_ov170_021ed7c4(syswk->app, syswk->pos);
            func_ov170_021ed6e4(syswk, TRUE);
            vf->seq = 8;
        }
        break;
    case 6:
        if (func_ov170_021ddae8(syswk) == FALSE) {
            func_ov170_021de66c(syswk, vf->work);
            func_ov170_021df040(syswk);
            GFL_SndSEPlay(0x667);
            GFL_SndSEPlay(0x669);
            func_ov170_021ef36c(syswk->app->bgWinFrame);
            syswk->pos = BOX2_GET_NONE;
            vf->seq = 7;
        }
        break;
    case 7:
        if (frameMove == FALSE) {
            vf->seq = 8;
        }
        break;
    case 8:
        func_ov170_021ed888(syswk->app, FALSE);
        func_ov170_021ecb20(syswk, 1, TRUE);
        func_ov170_021ecb20(syswk, 2, FALSE);
        vf->seq = 0;
        return FALSE;
    }
    return TRUE;
}


BOOL func_ov170_021e0470(Box2SysWork *syswk) {
    Box2IrqWork *vf = &syswk->app->vfunk;
    BOOL frameMove2 = func_ov170_021ef5c4(syswk->app->bgWinFrame);
    BOOL frameMove = func_ov170_021ef400(syswk);
    u32 x, y;
    u32 pos;

    switch (vf->seq) {
    case 0:
        if (func_02035c38() == TRUE) {
            func_ov170_021ef57c(syswk->app->bgWinFrame);
        }
        vf->seq = 1;
        break;
    case 1:
        if (func_02035cb8(&x, &y) == FALSE) {
            pos = BOX2_GET_NONE;
            if (func_ov170_021ef45c(syswk->app->bgWinFrame) == TRUE) {
                pos = func_ov170_021df228(syswk->app->tpx, syswk->app->tpy, data_ov170_021ef79c);
            }
            func_ov170_021de530(syswk, syswk->pos, pos);
            if (func_ov170_021ef60c(syswk->app->bgWinFrame) == FALSE) {
                func_ov170_021ed6e4(syswk, FALSE);
            }
            syswk->pos = func_ov170_021de5c8(syswk, BOX2_BOXLIST_POS);
            if (syswk->pos >= BOX2_PARTY_POS) {
                vf->seq = 5;
                break;
            }
            if (frameMove == TRUE || func_ov170_021ef45c(syswk->app->bgWinFrame) == TRUE) {
                GFL_SndSEPlay(SEQ_SE_SYS_42);
                func_ov170_021ef3a0(syswk->app->bgWinFrame);
            }
            vf->seq = 2;
            break;
        }
        if (frameMove2 == FALSE && frameMove == FALSE && func_ov170_021ef45c(syswk->app->bgWinFrame) == FALSE
            && syswk->pos != func_ov170_021ef180(x, y)) {
            GFL_SndSEPlay(SEQ_SE_SYS_42);
            func_ov170_021ef35c(syswk->app->bgWinFrame);
            func_ov170_021ef36c(syswk->app->bgWinFrame);
            func_ov170_021ee038(syswk, 2, 2, 1);
            syswk->app->curPos = syswk->pos;
        }
        func_ov170_021df43c(syswk, x, y);
        break;
    case 2:
        if (func_ov170_021dd930(syswk) == FALSE) {
            GFL_SndSEPlay(SEQ_SE_SYS_40);
            vf->seq = 3;
        }
        if (frameMove == FALSE) {
            func_ov170_021ef534(syswk->app->bgWinFrame);
        }
        break;
    case 3:
        if (frameMove == FALSE) {
            func_ov170_021ef534(syswk->app->bgWinFrame);
            vf->seq = 4;
        }
        break;
    case 4:
        if (frameMove2 == FALSE) {
            func_ov170_021de66c(syswk, vf->work);
            func_ov170_021ed7c4(syswk->app, syswk->pos);
            func_ov170_021ed6e4(syswk, TRUE);
            vf->seq = 7;
        }
        break;
    case 5:
        if (func_ov170_021dd930(syswk) == FALSE) {
            func_ov170_021de66c(syswk, vf->work);
            GFL_SndSEPlay(SEQ_SE_SYS_40);
            GFL_SndSEPlay(SEQ_SE_SYS_42);
            syswk->pos = BOX2_GET_NONE;
            func_ov170_021ef3a0(syswk->app->bgWinFrame);
            vf->seq = 6;
        }
        break;
    case 6:
        if (frameMove == FALSE) {
            func_ov170_021df040(syswk);
            vf->seq = 7;
        }
        break;
    case 7:
        func_ov170_021ecb20(syswk, 1, FALSE);
        func_ov170_021ecb20(syswk, 2, FALSE);
        vf->seq = 0;
        return FALSE;
    }
    return TRUE;
}


BOOL func_ov170_021e066c(Box2SysWork *syswk) {
    Box2CursorMoveWork *mv = syswk->app->vfunk.work;
    s16 pos[2];

    if (mv->cnt == 0) {
        func_ov170_021ec08c(syswk->app, 4, mv->px, mv->py, 0);
        func_ov170_021ecc1c(syswk);
        return FALSE;
    }
    mv->cnt--;
    func_ov170_021ec0b0(syswk->app, 4, &pos[1], &pos[0], 0);
    if (mv->mx == 0) {
        pos[1] += mv->vx;
    } else {
        pos[1] -= mv->vx;
    }
    if (mv->my == 0) {
        pos[0] += mv->vy;
    } else {
        pos[0] -= mv->vy;
    }
    func_ov170_021ec08c(syswk->app, 4, pos[1], pos[0], 0);
    func_ov170_021ecc1c(syswk);
    return TRUE;
}


BOOL func_ov170_021e070c(Box2SysWork *syswk) {
    if (func_ov170_021e066c(syswk) == FALSE && func_ov170_021ef5c4(syswk->app->bgWinFrame) == FALSE) {
        return FALSE;
    }
    return TRUE;
}

void func_ov170_021e072c(Box2SysWork *param_1) {
    s16 asStack_12 [1];
    s16 sStack_14;


    func_ov170_021ed544(param_1, 8);
    func_ov170_021ec0b0(param_1->app, 4, asStack_12, &sStack_14, 0);
    func_ov170_021ec08c(param_1->app, (u32)param_1->app->pokeIconId[0x24], asStack_12[0], sStack_14 + 4, 0);
    func_ov170_021ec768(param_1->app, 0x24, 0);
}

BOOL func_ov170_021e0778(Box2SysWork *syswk) {
    Box2IrqWork *vf = &syswk->app->vfunk;
    s16 x;
    s16 y;
    u32 id;

    switch (vf->seq) {
    case 0:
        func_ov170_021ed544(syswk, 7);
        vf->seq++;
    case 1:
        if (vf->cnt == 4) {
            vf->cnt = 0;
            vf->seq++;
        } else {
            func_ov170_021ec0b0(syswk->app, BOX2_ACTOR_CURSOR, &x, &y, 0);
            func_ov170_021ec08c(syswk->app, BOX2_ACTOR_CURSOR, x, y + 2, 0);
            vf->cnt++;
        }
        break;
    case 2:
        GFL_SndSEPlay(SEQ_SE_SYS_39);
        func_ov170_021ed544(syswk, 8);
        vf->seq++;
    case 3:
        if (vf->cnt == 4) {
            vf->cnt = 0;
            vf->seq = 0;
            return FALSE;
        }
        func_ov170_021ec0b0(syswk->app, BOX2_ACTOR_CURSOR, &x, &y, 0);
        func_ov170_021ec08c(syswk->app, BOX2_ACTOR_CURSOR, x, y - 2, 0);
        id = syswk->app->pokeIconId[BOX2_BOXLIST_POS];
        func_ov170_021ec0b0(syswk->app, id, &x, &y, 0);
        func_ov170_021ec08c(syswk->app, id, x, y - 2, 0);
        func_ov170_021ed7c4(syswk->app, BOX2_BOXLIST_POS);
        vf->cnt++;
        break;
    }
    return TRUE;
}


BOOL func_ov170_021e087c(Box2SysWork *syswk) {
    Box2IrqWork *vf = &syswk->app->vfunk;
    BOOL dir;

    switch (vf->seq) {
    case 0:
        if (syswk->pos >= BOX2_TRAY_POKE_MAX && func_ov170_021ef770(syswk->app->bgWinFrame) == TRUE
            && (syswk->app->pokePutKey < BOX2_TRAY_POKE_MAX || syswk->app->pokePutKey == BOX2_GET_NONE
                || func_ov170_021de158(syswk, syswk->pos, syswk->app->pokePutKey) == FALSE)) {
            func_ov170_021ee008(syswk, 0, FALSE);
            func_ov170_021ef75c(syswk->app->bgWinFrame);
            GFL_SndSEPlay(SEQ_SE_SYS_42);
            vf->seq = 5;
            break;
        }
        if (syswk->pos < BOX2_TRAY_POKE_MAX && syswk->tray != syswk->getTray
            && (syswk->app->pokePutKey == BOX2_GET_NONE
                || func_ov170_021de158(syswk, syswk->pos, syswk->app->pokePutKey) == FALSE)) {
            dir = func_ov170_021dd210(syswk, syswk->tray, syswk->getTray);
            syswk->tray = syswk->getTray;
            func_ov170_021ec35c(syswk, syswk->tray);
            func_ov170_021dec98(syswk, func_ov170_021dedcc(syswk, syswk->tray), dir);
            if (dir == FALSE) {
                vf->seq = 2;
            } else {
                vf->seq = 3;
            }
            break;
        }
    case 1:
        func_ov170_021ed544(syswk, 7);
        func_ov170_021de158(syswk, syswk->pos, syswk->app->pokePutKey);
        vf->seq = 4;
        break;
    case 2:
        if (func_ov170_021e00c4(syswk) == FALSE) {
            vf->seq = 1;
        }
        break;
    case 3:
        if (func_ov170_021e0128(syswk) == FALSE) {
            vf->seq = 1;
        }
        break;
    case 4:
        if (func_ov170_021dd930(syswk) == FALSE) {
            func_ov170_021de66c(syswk, vf->work);
            func_ov170_021ed544(syswk, 6);
            func_ov170_021ed6e4(syswk, FALSE);
            GFL_SndSEPlay(SEQ_SE_SYS_40);
            vf->seq = 0;
            return FALSE;
        }
        break;
    case 5:
        if (func_ov170_021e01e0(syswk) == FALSE) {
            func_ov170_021ef35c(syswk->app->bgWinFrame);
            func_ov170_021ef36c(syswk->app->bgWinFrame);
            GFL_SndSEPlay(SEQ_SE_SYS_42);
            vf->seq = 6;
        }
        break;
    case 6:
        if (func_ov170_021e01b0(syswk) == FALSE) {
            vf->seq = 1;
        }
        break;
    }
    return TRUE;
}

u32 func_ov170_021e09e0(Box2SysWork *syswk) {
    Box2IrqWork *vf = &syswk->app->vfunk;

    switch (vf->seq) {
    case 0:
        func_ov170_021ed544(syswk, 7);
        func_ov170_021de428(syswk, syswk->pos, syswk->app->pokePutKey);
        vf->seq = 1;
        break;
    case 1:
        if (func_ov170_021ddae8(syswk) == FALSE) {
            func_ov170_021de66c(syswk, vf->work);
            func_ov170_021ed544(syswk, 6);
            GFL_SndSEPlay(SEQ_SE_SYS_40);
            vf->seq = 0;
            return FALSE;
        }
        break;
    }
    return TRUE;
}

u32 func_ov170_021e0a40(Box2SysWork *syswk) {
    Box2IrqWork *vf = &syswk->app->vfunk;
    BOOL moving;
    BOOL frameMove;

    switch (vf->seq) {
    case 0:
        if (syswk->app->itemId != 0) {
            func_ov170_021ed108(syswk->app, TRUE);
            func_ov170_021ebfa8(syswk->app, 10, 1);
            func_ov170_021ec000(syswk->app, 10, TRUE);
        }
        vf->seq++;
    case 1:
        moving = func_ov170_021e066c(syswk);
        frameMove = func_ov170_021ef5c4(syswk->app->bgWinFrame);
        if (moving == FALSE && frameMove == FALSE) {
            if (syswk->app->itemId != 0) {
                func_ov170_021ed108(syswk->app, FALSE);
                func_ov170_021ed2f4(syswk->app);
            }
            vf->seq = 0;
            return FALSE;
        }
    }
    return TRUE;
}


BOOL func_ov170_021e0abc(Box2SysWork *syswk) {
    Box2IrqWork *vf = &syswk->app->vfunk;
    BOOL frameMove2 = func_ov170_021ef5c4(syswk->app->bgWinFrame);
    BOOL frameMove = func_ov170_021ef400(syswk);
    u32 x;
    u32 y;
    u16 setPos;
    BOOL party;
    u16 pos;
    BOOL cancel;
    BOOL dir;
    u32 wallpaper;
    u32 res;

    switch (vf->seq) {
    case 0:
        func_ov170_021ea878(syswk, syswk->app->itemId);
        if (0 != syswk->app->itemId) {
            func_ov170_021ed108(syswk->app, TRUE);
            func_ov170_021ebfa8(syswk->app, BOX2_ACTOR_ITEM_ICON, 1);
            func_ov170_021ec000(syswk->app, BOX2_ACTOR_ITEM_ICON, TRUE);
            vf->seq = 1;
        } else {
            func_ov170_021ef534(syswk->app->bgWinFrame);
            vf->seq = 11;
        }
        break;
    case 1:
        if (func_ov170_021ebfec(syswk->app, BOX2_ACTOR_ITEM_ICON) == TRUE) {
            break;
        }
        func_ov170_021ed108(syswk->app, FALSE);
        func_ov170_021ed2f4(syswk->app);
        vf->seq = 2;
    case 2:
        if (func_02035cb8(&x, &y) == FALSE) {
            if (frameMove == TRUE || func_ov170_021ef45c(syswk->app->bgWinFrame) == TRUE) {
                party = TRUE;
            } else {
                party = FALSE;
            }
            pos = func_ov170_021df1dc((s16)syswk->app->tpx, (s16)syswk->app->tpy);
            if (frameMove == FALSE && func_ov170_021ef45c(syswk->app->bgWinFrame) == TRUE && pos == BOX2_GET_NONE) {
                pos = func_ov170_021df228((s16)syswk->app->tpx, (s16)syswk->app->tpy, data_ov170_021ef79c);
            }
            cancel = FALSE;
            setPos = pos;
            if (pos == BOX2_GET_NONE) {
                pos = syswk->pos;
                cancel = TRUE;
            } else if (func_ov170_021dde80(syswk, syswk->pos, pos) == FALSE) {
                pos = syswk->pos;
                cancel = TRUE;
            }
            if (party == TRUE) {
                func_ov170_021dea3c(syswk, setPos, pos, 2);
                if (cancel == TRUE && syswk->getTray != BOX2_GET_NONE && syswk->getTray != syswk->tray) {
                    dir = func_ov170_021dd210(syswk, syswk->tray, syswk->getTray);
                    syswk->tray = syswk->getTray;
                    func_ov170_021ec35c(syswk, syswk->tray);
                    wallpaper = func_ov170_021dedcc(syswk, syswk->tray);
                    func_ov170_021dec98(syswk, wallpaper, dir);
                    if (dir == FALSE) {
                        vf->seq = 5;
                    } else {
                        vf->seq = 6;
                    }
                } else {
                    vf->seq = 7;
                }
            } else {
                func_ov170_021dea3c(syswk, setPos, pos, 0);
                func_ov170_021ef534(syswk->app->bgWinFrame);
                vf->seq = 8;
            }
            vf->cnt = 0;
            break;
        }
        if (vf->cnt != 0) {
            vf->cnt--;
        }
        if (frameMove2 == FALSE && frameMove == FALSE && func_ov170_021ef45c(syswk->app->bgWinFrame) == FALSE && syswk->pos != func_ov170_021ef180(x, y)) {
            func_ov170_021ef35c(syswk->app->bgWinFrame);
            func_ov170_021ef36c(syswk->app->bgWinFrame);
            func_ov170_021ec5a0(syswk, TRUE);
            func_ov170_021ecb20(syswk, 0x82, TRUE);
            GFL_SndSEPlay(0x669);
            func_ov170_021ee364(syswk, 6, syswk->pos);
            func_02026124(syswk->app->cursorMove, 0x27);
            func_02026124(syswk->app->cursorMove, 0x28);
            func_ov170_021ee038(syswk, 2, TRUE, TRUE);
            syswk->unk1E = 0;
            syswk->app->unkA172 = 0;
            syswk->app->curPos = syswk->pos;
            syswk->app->unk68 = 0x42;
        }
        if (vf->cnt == 0) {
            res = func_ov170_021ef1b4();
            if (res == 0) {
                GFL_SndSEPlay(0x548);
                func_ov170_021dd16c(syswk, FALSE);
                vf->seq = 3;
            } else if (res == 1) {
                GFL_SndSEPlay(0x548);
                func_ov170_021dd16c(syswk, TRUE);
                vf->seq = 4;
            }
        }
        func_ov170_021e0fb0(syswk->app, x, y);
        break;
    case 3:
        if (func_ov170_021e00c4(syswk) == FALSE) {
            vf->cnt = 0x10;
            vf->seq = 2;
        }
        if (func_02035cb8(&x, &y) == TRUE) {
            func_ov170_021e0fb0(syswk->app, x, y);
        }
        break;
    case 4:
        if (func_ov170_021e0128(syswk) == FALSE) {
            vf->cnt = 0x10;
            vf->seq = 2;
        }
        if (func_02035cb8(&x, &y) == TRUE) {
            func_ov170_021e0fb0(syswk->app, x, y);
        }
        break;
    case 5:
        if (func_ov170_021e00c4(syswk) == FALSE) {
            vf->seq = 7;
        }
        break;
    case 6:
        if (func_ov170_021e0128(syswk) == FALSE) {
            vf->seq = 7;
        }
        break;
    case 7:
        if (func_ov170_021deafc(syswk) == FALSE) {
            GFL_SndSEPlay(0x667);
            vf->seq = 9;
        }
        break;
    case 8:
        if (func_ov170_021deafc(syswk) == FALSE) {
            GFL_SndSEPlay(0x667);
            if (syswk->app->itemId != 0) {
                func_ov170_021ed108(syswk->app, FALSE);
                func_ov170_021ed2f4(syswk->app);
            }
            func_ov170_021ee038(syswk, 0, 0, 1);
            vf->seq = 11;
        }
        break;
    case 9:
        func_ov170_021ed108(syswk->app, TRUE);
        func_ov170_021ebfa8(syswk->app, BOX2_ACTOR_ITEM_ICON, 2);
        func_ov170_021ed6e4(syswk, FALSE);
        vf->seq = 10;
        break;
    case 10:
        if (func_ov170_021ebfec(syswk->app, BOX2_ACTOR_ITEM_ICON) == FALSE) {
            func_ov170_021ec000(syswk->app, BOX2_ACTOR_ITEM_ICON, FALSE);
            func_ov170_021ed108(syswk->app, FALSE);
            vf->seq = 11;
        }
        break;
    case 11:
        if (frameMove2 == FALSE && frameMove == FALSE) {
            vf->seq = 0;
            return FALSE;
        }
        break;
    }
    return TRUE;
}

u32 func_ov170_021e0e50(Box2SysWork *syswk) {
    Box2IrqWork *vf = &syswk->app->vfunk;
    BOOL moving;
    BOOL frameMove;

    switch (vf->seq) {
    case 0:
        if (func_ov170_021ec01c(syswk->app, 10) == TRUE) {
            func_ov170_021ed108(syswk->app, TRUE);
            func_ov170_021ebfa8(syswk->app, 10, 2);
        }
        vf->seq++;
    case 1:
        moving = func_ov170_021e066c(syswk);
        frameMove = func_ov170_021e0190(syswk);
        if (moving == FALSE && frameMove == FALSE) {
            vf->seq++;
        }
        break;
    case 2:
        if (func_ov170_021ec01c(syswk->app, 10) == TRUE) {
            func_ov170_021ed108(syswk->app, FALSE);
            func_ov170_021ec000(syswk->app, 10, FALSE);
        }
        vf->seq = 0;
        return FALSE;
    }
    return TRUE;
}


u32 func_ov170_021e0ed4(Box2SysWork *syswk) {
    Box2IrqWork *vf = &syswk->app->vfunk;

    switch (vf->seq) {
    case 0:
        if (func_ov170_021ec01c(syswk->app, 10) == TRUE) {
            func_ov170_021ed108(syswk->app, TRUE);
            func_ov170_021ebfa8(syswk->app, 10, 2);
        }
        vf->seq++;
    case 1:
        if (func_ov170_021e0190(syswk) == FALSE) {
            vf->seq++;
        }
        break;
    case 2:
        if (func_ov170_021ec01c(syswk->app, 10) == TRUE) {
            func_ov170_021ed108(syswk->app, FALSE);
            func_ov170_021ec000(syswk->app, 10, FALSE);
        }
        vf->seq = 0;
        return FALSE;
    }
    return TRUE;
}


u32 func_ov170_021e0f4c(Box2SysWork *syswk) {
    Box2IrqWork *vf = &syswk->app->vfunk;

    switch (vf->seq) {
    case 0:
        if (func_ov170_021ec01c(syswk->app, 10) == TRUE) {
            func_ov170_021ed108(syswk->app, TRUE);
            func_ov170_021ebfa8(syswk->app, 10, 2);
        }
        vf->seq++;
        break;
    case 1:
        if (func_ov170_021ebfec(syswk->app, 10) == FALSE) {
            func_ov170_021ed108(syswk->app, FALSE);
            func_ov170_021ec000(syswk->app, 10, FALSE);
        }
        vf->seq = 0;
        return FALSE;
    }
    return TRUE;
}


void func_ov170_021e0fb0(Box2AppWork *param_1, u32 param_2, u32 param_3) {
    func_ov170_021ed14c(param_1, (s16)param_2, (s16)param_3);
    func_ov170_021ed310(param_1);
    param_1->tpx = param_2;
    param_1->tpy = param_3;
}


BOOL func_ov170_021e0fd8(Box2SysWork *syswk) {
    Box2IrqWork *vf = &syswk->app->vfunk;
    u16 pos;
    BOOL cancel;
    BOOL dir;
    u32 res;
    u32 wallpaper;
    u32 x;
    u32 y;
    u32 setPos;

    switch (vf->seq) {
    case 0:
        func_ov170_021ed108(syswk->app, TRUE);
        func_ov170_021ebfa8(syswk->app, BOX2_ACTOR_ITEM_ICON, 1);
        func_ov170_021ec000(syswk->app, BOX2_ACTOR_ITEM_ICON, TRUE);
        vf->seq = 1;
        break;
    case 1:
        if (func_ov170_021ebfec(syswk->app, BOX2_ACTOR_ITEM_ICON) == FALSE) {
            func_ov170_021ed108(syswk->app, FALSE);
            func_ov170_021ed2f4(syswk->app);
            vf->seq = 2;
        }
        break;
    case 2:
        if (func_02035cb8(&x, &y) == FALSE) {
            pos = func_ov170_021df1dc((s16)syswk->app->tpx, (s16)syswk->app->tpy);
            if (pos == BOX2_GET_NONE) {
                pos = func_ov170_021df228((s16)syswk->app->tpx, (s16)syswk->app->tpy, data_ov170_021ef79c);
            }
            setPos = pos;
            cancel = FALSE;
            if (pos == BOX2_GET_NONE) {
                pos = syswk->pos;
                cancel = TRUE;
            } else if (func_ov170_021dde80(syswk, syswk->pos, pos) == FALSE) {
                pos = syswk->pos;
                cancel = TRUE;
            }
            func_ov170_021dea3c(syswk, setPos, pos, 2);
            if (cancel == TRUE && syswk->getTray != BOX2_GET_NONE && syswk->getTray != syswk->tray) {
                dir = func_ov170_021dd210(syswk, syswk->tray, syswk->getTray);
                syswk->tray = syswk->getTray;
                func_ov170_021ec35c(syswk, syswk->tray);
                wallpaper = func_ov170_021dedcc(syswk, syswk->tray);
                func_ov170_021dec98(syswk, wallpaper, dir);
                if (dir == FALSE) {
                    vf->seq = 5;
                } else {
                    vf->seq = 6;
                }
            } else {
                vf->seq = 7;
            }
            vf->cnt = 0;
            break;
        }
        if (vf->cnt != 0) {
            vf->cnt--;
        }
        if (vf->cnt == 0) {
            res = func_ov170_021ef1b4();
            if (res == 0) {
                GFL_SndSEPlay(0x548);
                func_ov170_021dd16c(syswk, FALSE);
                vf->seq = 3;
            } else if (res == 1) {
                GFL_SndSEPlay(0x548);
                func_ov170_021dd16c(syswk, TRUE);
                vf->seq = 4;
            }
        }
        func_ov170_021e0fb0(syswk->app, x, y);
        break;
    case 3:
        if (func_ov170_021e00c4(syswk) == FALSE) {
            vf->cnt = 0x10;
            vf->seq = 2;
        }
        if (func_02035cb8(&x, &y) == TRUE) {
            func_ov170_021e0fb0(syswk->app, x, y);
        }
        break;
    case 4:
        if (func_ov170_021e0128(syswk) == FALSE) {
            vf->cnt = 0x10;
            vf->seq = 2;
        }
        if (func_02035cb8(&x, &y) == TRUE) {
            func_ov170_021e0fb0(syswk->app, x, y);
        }
        break;
    case 5:
    case 6:
        if (vf->seq == 5) {
            if (func_ov170_021e00c4(syswk) == FALSE) {
                vf->seq = 7;
            }
        }
        if (vf->seq == 6) {
            if (func_ov170_021e0128(syswk) == FALSE) {
                vf->seq = 7;
            }
        }
    case 7:
        if (func_ov170_021deafc(syswk) == FALSE) {
            if (vf->seq == 7) {
                GFL_SndSEPlay(0x667);
                vf->seq = 8;
            }
        }
        break;
    case 8:
        func_ov170_021ed108(syswk->app, TRUE);
        func_ov170_021ebfa8(syswk->app, BOX2_ACTOR_ITEM_ICON, 2);
        func_ov170_021ed6e4(syswk, FALSE);
        vf->seq = 9;
        break;
    case 9:
        if (func_ov170_021ebfec(syswk->app, BOX2_ACTOR_ITEM_ICON) == FALSE) {
            func_ov170_021ec000(syswk->app, BOX2_ACTOR_ITEM_ICON, FALSE);
            func_ov170_021ed108(syswk->app, FALSE);
            vf->seq = 10;
        }
        break;
    case 10:
        vf->seq = 0;
        return FALSE;
    }
    return TRUE;
}

BOOL func_ov170_021e122c(Box2SysWork *syswk) {
    Box2IrqWork *vf = &syswk->app->vfunk;
    u8 pos;

    switch (vf->seq) {
    case 0:
        func_ov170_021ed108(syswk->app, TRUE);
        func_ov170_021ebfa8(syswk->app, BOX2_ACTOR_ITEM_ICON, 1);
        func_ov170_021ec000(syswk->app, BOX2_ACTOR_ITEM_ICON, TRUE);
        vf->seq = 1;
        break;
    case 1:
        if (func_ov170_021ebfec(syswk->app, BOX2_ACTOR_ITEM_ICON) == FALSE) {
            func_ov170_021ed108(syswk->app, FALSE);
            vf->seq = 2;
        }
        break;
    case 2:
        pos = syswk->pos;
        syswk->pos = ((Box2ItemMoveWork *)vf->work)->putPos;
        if (syswk->getTray != BOX2_GET_NONE && syswk->getTray != syswk->tray) {
            func_ov170_021de980(syswk, 2);
        } else {
            func_ov170_021dea3c(syswk, pos, pos, 2);
        }
        vf->seq = 3;
        break;
    case 3:
        if (func_ov170_021deafc(syswk) != FALSE) {
            break;
        }
        if (syswk->getTray != BOX2_GET_NONE && syswk->getTray != syswk->tray) {
            vf->seq = 5;
        } else {
            GFL_SndSEPlay(SEQ_SE_SYS_40);
            vf->seq = 4;
        }
        break;
    case 4:
        func_ov170_021ed108(syswk->app, TRUE);
        func_ov170_021ebfa8(syswk->app, BOX2_ACTOR_ITEM_ICON, 2);
        vf->seq = 5;
        break;
    case 5:
        if (func_ov170_021ebfec(syswk->app, BOX2_ACTOR_ITEM_ICON) == FALSE) {
            func_ov170_021ec000(syswk->app, BOX2_ACTOR_ITEM_ICON, FALSE);
            func_ov170_021ed108(syswk->app, FALSE);
            vf->seq = 6;
        }
        break;
    case 6:
        vf->seq = 0;
        return FALSE;
    }
    return TRUE;
}


BOOL func_ov170_021e1324(Box2SysWork *syswk) {
    Box2IrqWork *vf = &syswk->app->vfunk;
    s16 x, y;

    switch (vf->seq) {
    case 0:
        func_ov170_021ed108(syswk->app, TRUE);
        func_ov170_021ebfa8(syswk->app, BOX2_ACTOR_ITEM_ICON, 1);
        func_ov170_021ec000(syswk->app, BOX2_ACTOR_ITEM_ICON, TRUE);
        vf->seq++;
        break;
    case 1:
        if (func_ov170_021ebfec(syswk->app, BOX2_ACTOR_ITEM_ICON) == FALSE) {
            func_ov170_021ed108(syswk->app, FALSE);
            vf->seq++;
        }
        break;
    case 2:
        func_ov170_021ed544(syswk, 7);
        vf->seq++;
    case 3:
        if (vf->cnt == 4) {
            vf->cnt = 0;
            vf->seq++;
        } else {
            func_ov170_021ec0b0(syswk->app, BOX2_ACTOR_CURSOR, &x, &y, 0);
            func_ov170_021ec08c(syswk->app, BOX2_ACTOR_CURSOR, x, y + 2, 0);
            vf->cnt++;
        }
        break;
    case 4:
        GFL_SndSEPlay(SEQ_SE_SYS_39);
        func_ov170_021ed544(syswk, 8);
        vf->seq++;
    case 5:
        if (vf->cnt == 4) {
            func_ov170_021ed1c8(syswk->app);
            func_ov170_021ed2f4(syswk->app);
            vf->cnt = 0;
            vf->seq = 0;
            return FALSE;
        }
        func_ov170_021ec0b0(syswk->app, BOX2_ACTOR_CURSOR, &x, &y, 0);
        func_ov170_021ec08c(syswk->app, BOX2_ACTOR_CURSOR, x, y - 2, 0);
        func_ov170_021ec0b0(syswk->app, BOX2_ACTOR_ITEM_ICON, &x, &y, 0);
        func_ov170_021ec08c(syswk->app, BOX2_ACTOR_ITEM_ICON, x, y - 2, 0);
        vf->cnt++;
        break;
    }
    return TRUE;
}

BOOL func_ov170_021e1458(Box2SysWork *syswk) {
    Box2IrqWork *vf = &syswk->app->vfunk;
    s16 x, y;

    switch (vf->seq) {
    case 0:
        func_ov170_021ed544(syswk, 7);
        vf->seq = 1;
    case 1:
        if (vf->cnt == 4) {
            vf->cnt = 0;
            vf->seq = 2;
        } else {
            func_ov170_021ec0b0(syswk->app, BOX2_ACTOR_ITEM_ICON, &x, &y, 0);
            func_ov170_021ec08c(syswk->app, BOX2_ACTOR_ITEM_ICON, x, y + 2, 0);
            vf->cnt++;
        }
        break;
    case 2:
        GFL_SndSEPlay(SEQ_SE_SYS_40);
        func_ov170_021ed544(syswk, 6);
        func_ov170_021ed108(syswk->app, TRUE);
        func_ov170_021ebfa8(syswk->app, BOX2_ACTOR_ITEM_ICON, 2);
        vf->seq = 3;
        break;
    case 3:
        if (func_ov170_021ebfec(syswk->app, BOX2_ACTOR_ITEM_ICON) == FALSE) {
            func_ov170_021ed108(syswk->app, FALSE);
            func_ov170_021ec000(syswk->app, BOX2_ACTOR_ITEM_ICON, FALSE);
            vf->seq = 4;
        }
        break;
    case 4:
        func_ov170_021ed544(syswk, 6);
        vf->seq = 0;
        return FALSE;
    }
    return TRUE;
}


BOOL func_ov170_021e152c(Box2SysWork *syswk) {
    Box2IrqWork *vf = &syswk->app->vfunk;
    BOOL dir;

    switch (vf->seq) {
    case 0:
        if (syswk->getTray != BOX2_GET_NONE && syswk->getTray != syswk->tray) {
            dir = func_ov170_021dd210(syswk, syswk->tray, syswk->getTray);
            syswk->tray = syswk->getTray;
            func_ov170_021ec35c(syswk, syswk->tray);
            func_ov170_021dec98(syswk, func_ov170_021dedcc(syswk, syswk->tray), dir);
            if (dir == FALSE) {
                vf->seq = 2;
            } else {
                vf->seq = 3;
            }
            break;
        }
    case 1:
        func_ov170_021ed544(syswk, 7);
        func_ov170_021dea4c(syswk, syswk->app->unkA16E, 2);
        vf->seq = 4;
        break;
    case 2:
        if (func_ov170_021e00c4(syswk) == FALSE) {
            vf->seq = 1;
        }
        break;
    case 3:
        if (func_ov170_021e0128(syswk) == FALSE) {
            vf->seq = 1;
        }
        break;
    case 4:
        if (func_ov170_021deb08(syswk) == FALSE) {
            GFL_SndSEPlay(SEQ_SE_SYS_40);
            vf->seq = 5;
        }
        break;
    case 5:
        func_ov170_021ed108(syswk->app, TRUE);
        func_ov170_021ebfa8(syswk->app, BOX2_ACTOR_ITEM_ICON, 2);
        vf->seq = 6;
        break;
    case 6:
        if (func_ov170_021ebfec(syswk->app, BOX2_ACTOR_ITEM_ICON) == FALSE) {
            func_ov170_021ed108(syswk->app, FALSE);
            func_ov170_021ec000(syswk->app, BOX2_ACTOR_ITEM_ICON, FALSE);
            vf->seq = 7;
        }
        break;
    case 7:
        func_ov170_021ed544(syswk, 6);
        vf->seq = 0;
        return FALSE;
    }
    return TRUE;
}

BOOL func_ov170_021e1628(Box2SysWork *syswk) {
    Box2IrqWork *vf = &syswk->app->vfunk;
    BOOL frameMove2 = func_ov170_021ef5c4(syswk->app->bgWinFrame);
    BOOL frameMove = func_ov170_021ef400(syswk);
    u32 x;
    u32 y;
    BOOL party;
    u16 pos;
    u16 setPos;
    u32 res;

    switch (vf->seq) {
    case 0:
        func_ov170_021ea878(syswk, syswk->app->itemId);
        if (syswk->app->itemId != 0) {
            func_ov170_021ed108(syswk->app, TRUE);
            func_ov170_021ebfa8(syswk->app, BOX2_ACTOR_ITEM_ICON, 1);
            func_ov170_021ec000(syswk->app, BOX2_ACTOR_ITEM_ICON, TRUE);
            vf->seq = 1;
        } else {
            func_ov170_021ef534(syswk->app->bgWinFrame);
            vf->seq = 11;
        }
        break;
    case 1:
        if (func_ov170_021ebfec(syswk->app, BOX2_ACTOR_ITEM_ICON) == TRUE) {
            break;
        }
        func_ov170_021ed108(syswk->app, FALSE);
        func_ov170_021ed2f4(syswk->app);
        vf->seq = 2;
    case 2:
        if (func_ov170_021ef45c(syswk->app->bgWinFrame) == TRUE) {
            func_ov170_021ed888(syswk->app, TRUE);
        }
        if (func_02035cb8(&x, &y) == FALSE) {
            if (frameMove == TRUE || func_ov170_021ef45c(syswk->app->bgWinFrame) == TRUE) {
                party = TRUE;
            } else {
                party = FALSE;
            }
            if (frameMove == FALSE && func_ov170_021ef45c(syswk->app->bgWinFrame) == TRUE) {
                pos = func_ov170_021df1dc((s16)syswk->app->tpx, (s16)syswk->app->tpy);
                if (pos == BOX2_GET_NONE) {
                    pos = func_ov170_021df228((s16)syswk->app->tpx, (s16)syswk->app->tpy, data_ov170_021ef79c);
                }
            } else {
                pos = func_ov170_021df228((s16)syswk->app->tpx, (s16)syswk->app->tpy, data_ov170_021ef7b4);
            }
            setPos = pos;
            if (pos == BOX2_GET_NONE) {
                pos = syswk->pos;
            } else if (func_ov170_021dde80(syswk, syswk->pos, pos) == FALSE) {
                pos = syswk->pos;
            }
            if (party == TRUE) {
                func_ov170_021dea3c(syswk, setPos, pos, 2);
                vf->seq = 7;
            } else {
                func_ov170_021dea3c(syswk, setPos, pos, 1);
                if (pos == syswk->pos && (PML_ItemIsMail(syswk->app->itemId) == FALSE || setPos == syswk->pos)) {
                    func_ov170_021ef534(syswk->app->bgWinFrame);
                }
                vf->seq = 8;
            }
            vf->cnt = 0;
            break;
        }
        if (vf->cnt != 0) {
            vf->cnt--;
        }
        if (frameMove2 == FALSE && frameMove == FALSE) {
            if (func_ov170_021ef45c(syswk->app->bgWinFrame) == FALSE && func_ov170_021ef248() == FALSE) {
                func_ov170_021ef3d8(syswk->app->bgWinFrame);
                func_ov170_021ecb20(syswk, 0x81, TRUE);
                GFL_SndSEPlay(0x669);
                func_ov170_021ee364(syswk, 6, syswk->pos);
                func_02026124(syswk->app->cursorMove, 0x27);
                func_02026124(syswk->app->cursorMove, 0x28);
                func_ov170_021ec000(syswk->app, 4, FALSE);
                func_ov170_021ee038(syswk, 2, TRUE, TRUE);
                syswk->unk1E = 1;
                syswk->app->curPos = syswk->pos;
                syswk->app->unk68 = 0x42;
            }
        }
        if (vf->cnt == 0) {
            res = func_ov170_021ef1b4();
            if (res == 0) {
                GFL_SndSEPlay(0x548);
                func_ov170_021dd16c(syswk, FALSE);
                vf->seq = 3;
            } else if (res == 1) {
                GFL_SndSEPlay(0x548);
                func_ov170_021dd16c(syswk, TRUE);
                vf->seq = 4;
            }
        }
        func_ov170_021e0fb0(syswk->app, x, y);
        break;
    case 3:
        if (func_ov170_021e00c4(syswk) == FALSE) {
            vf->cnt = 0x10;
            vf->seq = 2;
        }
        if (func_02035cb8(&x, &y) == TRUE) {
            func_ov170_021e0fb0(syswk->app, x, y);
        }
        break;
    case 4:
        if (func_ov170_021e0128(syswk) == FALSE) {
            vf->cnt = 0x10;
            vf->seq = 2;
        }
        if (func_02035cb8(&x, &y) == TRUE) {
            func_ov170_021e0fb0(syswk->app, x, y);
        }
        break;
    case 7:
    case 8:
        if (func_ov170_021deafc(syswk) == FALSE) {
            GFL_SndSEPlay(0x667);
            if (vf->seq == 7) {
                vf->seq = 9;
            } else if (vf->seq == 8) {
                if (syswk->app->itemId != 0) {
                    func_ov170_021ed108(syswk->app, FALSE);
                    func_ov170_021ed2f4(syswk->app);
                }
                func_ov170_021ee038(syswk, 0, 0, 1);
                vf->seq = 11;
            }
        }
        break;
    case 9:
        func_ov170_021ed108(syswk->app, TRUE);
        func_ov170_021ebfa8(syswk->app, BOX2_ACTOR_ITEM_ICON, 2);
        func_ov170_021ed6e4(syswk, FALSE);
        vf->seq = 10;
        break;
    case 10:
        if (func_ov170_021ebfec(syswk->app, BOX2_ACTOR_ITEM_ICON) == FALSE) {
            func_ov170_021ec000(syswk->app, BOX2_ACTOR_ITEM_ICON, FALSE);
            func_ov170_021ed108(syswk->app, FALSE);
            vf->seq = 11;
        }
        break;
    case 11:
        if (frameMove2 == FALSE && frameMove == FALSE) {
            vf->seq = 0;
            return FALSE;
        }
        break;
    }
    return TRUE;
}

u32 func_ov170_021e1988(Box2SysWork *syswk) {
    struct Box2IrqWork *vf = &syswk->app->vfunk;
    u8 pos;

    switch (vf->seq) {
    case 0:
        if (func_ov170_021ebfec(syswk->app, 0xA) == FALSE) {
            pos = syswk->pos;
            syswk->pos = *(u16 *)vf->work;
            func_ov170_021dea3c(syswk, pos, pos, 1);
            vf->seq = 1;
        }
        break;
    case 1:
        if (func_ov170_021deafc(syswk) == FALSE) {
            vf->seq = 0;
            return 0;
        }
        break;
    }
    return 1;
}


BOOL func_ov170_021e19d4(Box2SysWork *syswk) {
    Box2IrqWork *vf = &syswk->app->vfunk;
    u8 pos;

    if (vf->cnt == 5) {
        pos = func_020260e0(syswk->app->cursorMove);
        if (pos >= 0x22 && pos <= 0x25) {
            func_ov170_021ee008(syswk, pos - 0x22, TRUE);
        } else {
            func_ov170_021ee008(syswk, 0, FALSE);
        }
        func_ov170_021edf5c(syswk);
        vf->cnt = 0;
        return FALSE;
    }
    vf->cnt++;
    func_ov170_021edd38(syswk, -vf->cnt);
    return TRUE;
}

BOOL func_ov170_021e1a28(Box2SysWork *syswk) {
    Box2IrqWork *vf = &syswk->app->vfunk;
    u8 pos;

    if (vf->cnt == 5) {
        pos = func_020260e0(syswk->app->cursorMove);
        if (pos >= 0x22 && pos <= 0x25) {
            func_ov170_021ee008(syswk, pos - 0x22, TRUE);
        } else {
            func_ov170_021ee008(syswk, 0, FALSE);
        }
        func_ov170_021edf5c(syswk);
        vf->cnt = 0;
        return FALSE;
    }
    vf->cnt++;
    func_ov170_021edd38(syswk, vf->cnt);
    return TRUE;
}

BOOL func_ov170_021e1a78(Box2SysWork *syswk) {
    Box2IrqWork *vf = &syswk->app->vfunk;
    u8 pos;

    if (vf->cnt == 5) {
        pos = func_020260e0(syswk->app->cursorMove);
        if (pos >= 1 && pos <= 4) {
            func_ov170_021ee008(syswk, pos - 1, TRUE);
        } else {
            func_ov170_021ee008(syswk, 0, FALSE);
        }
        func_ov170_021edf5c(syswk);
        vf->cnt = 0;
        return FALSE;
    }
    vf->cnt++;
    func_ov170_021edd38(syswk, -vf->cnt);
    return TRUE;
}

BOOL func_ov170_021e1acc(Box2SysWork *syswk) {
    Box2IrqWork *vf = &syswk->app->vfunk;
    u8 pos;

    if (vf->cnt == 5) {
        pos = func_020260e0(syswk->app->cursorMove);
        if (pos >= 1 && pos <= 4) {
            func_ov170_021ee008(syswk, pos - 1, TRUE);
        } else {
            func_ov170_021ee008(syswk, 0, FALSE);
        }
        func_ov170_021edf5c(syswk);
        vf->cnt = 0;
        return FALSE;
    }
    vf->cnt++;
    func_ov170_021edd38(syswk, vf->cnt);
    return TRUE;
}


BOOL func_ov170_021e1b1c(Box2SysWork *syswk) {
    Box2IrqWork *vf = &syswk->app->vfunk;
    BOOL frameMove2;
    u32 unk7;
    u32 x;
    u32 y;
    u32 pos;
    BOOL dir;
    u32 wallpaper;
    u32 res;

    if (func_0202ce40(syswk->app->bgWinFrame, 8) == TRUE) {
        unk7 = func_ov170_021e01b0(syswk);
    } else {
        unk7 = 0;
    }
    frameMove2 = func_ov170_021ef5c4(syswk->app->bgWinFrame);
    switch (vf->seq) {
    case 0:
        if (func_02035c38() == TRUE) {
            func_ov170_021ef57c(syswk->app->bgWinFrame);
        }
        vf->seq = 1;
        break;
    case 1:
        if (func_02035cb8(&x, &y) == FALSE) {
            pos = BOX2_GET_NONE;
            if (func_ov170_021ef45c(syswk->app->bgWinFrame) == TRUE) {
                pos = func_ov170_021df228((s16)syswk->app->tpx, (s16)syswk->app->tpy, data_ov170_021ef79c);
            }
            if (pos == BOX2_GET_NONE) {
                pos = func_ov170_021df1dc((s16)syswk->app->tpx, (s16)syswk->app->tpy);
            }
            if (syswk->param->mode == 4 && syswk->param->unk14 == 1) {
                pos = BOX2_GET_NONE;
                syswk->app->moveErr = 5;
            }
            if (func_ov170_021de158(syswk, syswk->pos, pos) == FALSE) {
                pos = BOX2_GET_NONE;
            }
            if (func_ov170_021ef60c(syswk->app->bgWinFrame) == FALSE) {
                func_ov170_021ed6e4(syswk, FALSE);
            }
            if (pos != BOX2_GET_NONE) {
                syswk->pos = func_ov170_021de5c8(syswk, 0x24);
            }
            if (func_ov170_021ef45c(syswk->app->bgWinFrame) == FALSE && unk7 == 0) {
                func_ov170_021ef534(syswk->app->bgWinFrame);
            }
            if (pos == BOX2_GET_NONE && syswk->tray != syswk->getTray) {
                dir = func_ov170_021dd210(syswk, syswk->tray, syswk->getTray);
                syswk->tray = syswk->getTray;
                func_ov170_021ec35c(syswk, syswk->tray);
                wallpaper = func_ov170_021dedcc(syswk, syswk->tray);
                func_ov170_021dec98(syswk, wallpaper, dir);
                if (dir == FALSE) {
                    vf->seq = 4;
                } else {
                    vf->seq = 5;
                }
            } else {
                vf->seq = 2;
            }
            vf->cnt = 0;
            break;
        }
        if (vf->cnt != 0) {
            vf->cnt--;
        }
        if (unk7 == 0 && frameMove2 == FALSE && func_ov170_021ef45c(syswk->app->bgWinFrame) == FALSE && syswk->pos != func_ov170_021ef180(x, y)) {
            syswk->unk1A = 2;
            func_ov170_021ef35c(syswk->app->bgWinFrame);
            func_ov170_021ef36c(syswk->app->bgWinFrame);
            func_ov170_021ec5a0(syswk, TRUE);
            GFL_SndSEPlay(0x669);
            func_ov170_021ee364(syswk, 6, syswk->pos);
            func_ov170_021ee038(syswk, 2, TRUE, 2);
            syswk->app->curPos = syswk->pos;
            syswk->app->unk68 = 0x17;
        }
        if (vf->cnt == 0) {
            res = func_ov170_021ef1b4();
            if (res == 0) {
                GFL_SndSEPlay(0x548);
                func_ov170_021dd16c(syswk, FALSE);
                vf->seq = 6;
            } else if (res == 1) {
                GFL_SndSEPlay(0x548);
                func_ov170_021dd16c(syswk, TRUE);
                vf->seq = 7;
            }
        }
        func_ov170_021df43c(syswk, x, y);
        break;
    case 4:
        if (func_ov170_021e00c4(syswk) != FALSE) {
            break;
        }
        vf->seq = 2;
        break;
    case 5:
        if (func_ov170_021e0128(syswk) != FALSE) {
            break;
        }
        vf->seq = 2;
        break;
    case 2:
        if (func_ov170_021dd930(syswk) == FALSE) {
            GFL_SndSEPlay(0x667);
            vf->seq = 3;
        }
        break;
    case 3:
        if (unk7 == 0 && frameMove2 == FALSE) {
            vf->seq = 16;
        }
        break;
    case 6:
        if (func_ov170_021e00c4(syswk) == FALSE) {
            vf->cnt = 0x10;
            vf->seq = 1;
        }
        if (func_02035cb8(&x, &y) == TRUE) {
            func_ov170_021df43c(syswk, x, y);
        }
        break;
    case 7:
        if (func_ov170_021e0128(syswk) == FALSE) {
            vf->cnt = 0x10;
            vf->seq = 1;
        }
        if (func_02035cb8(&x, &y) == TRUE) {
            func_ov170_021df43c(syswk, x, y);
        }
        break;
    case 16:
        func_ov170_021de66c(syswk, vf->work);
        if (func_ov170_021ef770(syswk->app->bgWinFrame) == FALSE && func_ov170_021ef45c(syswk->app->bgWinFrame) == FALSE) {
            func_ov170_021ed7c4(syswk->app, syswk->pos);
            func_ov170_021ed6e4(syswk, TRUE);
        } else {
            func_ov170_021ed6e4(syswk, FALSE);
            func_020260e4(syswk->app->cursorMove, syswk->pos);
            func_ov170_021ee3e4(syswk, syswk->pos);
        }
        vf->seq = 0;
        return FALSE;
    }
    return TRUE;
}


BOOL func_ov170_021e1e34(Box2SysWork *syswk, u32 *seq) {
    PrintQueue *queue;
    BOOL BVar1;

    if (((syswk->app == (Box2AppWork *)0x0) ||
            (queue = syswk->app->printQueue, queue == (PrintQueue *)0x0)) ||
          (BVar1 = func_0201c6b4(queue), BVar1 != 0)) {
        *seq = (**(u32 (**)())((void *)&data_ov170_021efa7c + *seq * 4))(syswk);
    }
    if (*seq == 0x69) {
        return 0;
    }
    if (syswk->app != (Box2AppWork *)0x0) {
        func_ov170_021eab50(syswk->app);
        func_ov170_021ebf74(syswk->app);
    }
    return 1;
}
