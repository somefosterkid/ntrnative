// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov262/wifibattlematch_view.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/livebattlematch_flow.h"
#include "app/wifibattlematch_view.h"
#include "constants/arc.h"
#include "constants/pokemon.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nnsys/gfd.h"
#include "field/unity_tower.h"
#include "pml/poke_party.h"
#include "save/box.h"
#include "save/records.h"
#include "save/tournament_block.h"
#include "system/app_menu_common.h"
#include "system/bmp_winframe.h"
#include "system/gf_font.h"
#include "system/pms_draw.h"
#include "system/printsys.h"
#include "system/wordset.h"

// The backdrop of the Wi-Fi and live competition screens and what they show on it: the digital cards of the players,
// the other player's card, the icon of the link and the list of the team (the file name is the ROM's own string)

// The archive of the screens' graphics. It has no name yet
#define ARCID_WIFIBATTLEMATCH 133

#define CARD_WINDOW_MAX 30
#define CARD_ACTOR_COUNT 22
#define CARD_PARTY_COUNT 6
#define CARD_STAR_MAX 5
// The frames that a card takes to slide in or out
#define SLIDE_FRAMES 20

// The cell actors of a card, by index: the Pokémon, the player's badge and mark, the stars of the rank, and the held
// items
#define CARD_ACTOR_BADGE 7
#define CARD_ACTOR_MARK 8
#define CARD_ACTOR_STAR 9
#define CARD_ACTOR_ITEM 15

// The resources of the backdrop, by index: its palettes, characters and cells, of the main screen and then of the sub
// screen
#define RES_PALETTE 0
#define RES_CHARS 2
#define RES_CELLS 4

#define TEAM_ACTOR_COUNT 14
#define TEAM_ACTOR_ITEM 7

#define PARTNER_WINDOW_COUNT 8
#define PARTNER_WINDOW_PMS 7

struct WifiBattleMatchRes {
    u32 res[6];
    // The palette as it is now, and the two palettes that it goes between
    u16 colors[16];
    u16 colorsA[16];
    u16 colorsB[16];
    u16 angle;
};

struct WifiBattleMatchCard {
    u32 windowCount;
    BmpWin *windows[CARD_WINDOW_MAX];
    PrintWindow printWindows[CARD_WINDOW_MAX];
    ClActor *actors[CARD_ACTOR_COUNT];
    u32 iconChars[CARD_PARTY_COUNT];
    u32 unk1DC;
    u32 iconCells;
    u32 iconPalette;
    u32 badgePalette;
    u32 badgeChars;
    u32 badgeCells;
    u32 itemPalette;
    u32 itemChars;
    u32 itemCells;
    u32 slideCount;
    s16 slideX[CARD_ACTOR_COUNT];
    // The BG of the card, which the one after it is the frame of, and its palettes
    u8 bg;
    u8 palette;
    u8 iconPaletteSlot;
    u8 itemPaletteSlot;
    // CLACT_VRAM_MAIN or CLACT_VRAM_SUB
    u32 surface;
    void *records;
    u16 heapId;
    ClActUnit *unit;
};

struct WifiBattleMatchPartnerCard {
    BOOL shown;
    BmpWin *windows[PARTNER_WINDOW_COUNT];
    PrintWindow printWindows[PARTNER_WINDOW_COUNT];
    ClActor *badge;
    ClActor *stars[CARD_STAR_MAX];
    PMSDraw *pmsDraw;
    u16 badgePalette;
    u16 badgeChars;
    u16 badgeCells;
    u32 slideCount;
    u16 badgeX;
    u16 starX[CARD_STAR_MAX];
    u32 kind;
};

struct WifiBattleMatchLinkIcon {
    ClActor *actors[2];
};

struct WifiBattleMatchTeamList {
    u32 palette;
    u32 chars[CARD_PARTY_COUNT];
    u32 unk1C;
    u32 cells;
    u32 itemPalette;
    u32 itemChars;
    u32 itemCells;
    ClActor *actors[TEAM_ACTOR_COUNT];
    BmpWin *windows[CARD_PARTY_COUNT];
    PrintWindow printWindows[CARD_PARTY_COUNT];
    u32 slideCount;
    s16 slideX[TEAM_ACTOR_COUNT];
};

static u32 func_ov177_021e5978(WifiBattleMatchRes *res, u32 index);
static void func_ov177_021e5a30(NNS_GFD_DST_TYPE type, u16 *dst, const u16 *angle, u32 a3, u8 row,
                                           u8 index, u16 colorA, u16 colorB);
void func_ov177_021e5adc(PrintWindow *printWindow, PrintQueue *queue, Font *font, u32 count1, u32 count2, HeapID heapId);
void func_ov177_021e6718(WifiBattleMatchCard *card, u32 titleMsg, const WifiBattleMatchRankInfo *info, PlayerInfo *playerInfo, Font *font, PrintQueue *printQueue, MsgData *msgData, WordSet *wordSet, u32 colorKind, HeapID heapId);
void func_ov177_021e6940(WifiBattleMatchCard *card, BOOL flag, const WifiBattleMatchWifiCardInfo *info, PlayerInfo *playerInfo, Font *font, PrintQueue *printQueue, MsgData *msgData, WordSet *wordSet, HeapID heapId);
void func_ov177_021e6be8(WifiBattleMatchCard *card, const WifiBattleMatchLiveCardInfo *info, PlayerInfo *playerInfo, Font *font, PrintQueue *printQueue, MsgData *msgData, WordSet *wordSet, HeapID heapId);
static BOOL func_ov177_021e6e8c(WifiBattleMatchCard *card, PrintQueue *printQueue);
static void func_ov177_021e6efc(WifiBattleMatchCard *card);
static void func_ov177_021e6f20(WifiBattleMatchCard *card, ClActUnit *unit, void *records,
                                              HeapID heapId);
static void func_ov177_021e7270(WifiBattleMatchCard *card);
static void func_ov177_021e72fc(WifiBattleMatchCard *card, u32 look, ClActUnit *unit, HeapID heapId);
static void func_ov177_021e73b8(WifiBattleMatchCard *card);
static void func_ov177_021e73e4(WifiBattleMatchCard *card, ClActUnit *unit, WifiBattleMatchRes *res,
                                             HeapID heapId);
static void func_ov177_021e7454(WifiBattleMatchCard *card);
static void func_ov177_021e7464(WifiBattleMatchCard *card, const WifiBattleMatchRankInfo *info,
                                              ClActUnit *unit, WifiBattleMatchRes *res, HeapID heapId);
static void func_ov177_021e74fc(WifiBattleMatchCard *card);
void func_ov177_021e78b0(WifiBattleMatchPartnerCard *card, const LiveBattleMatchPlayerData *data, Font *font, PrintQueue *printQueue, MsgData *msgData, WordSet *wordSet, u32 colorKind, HeapID heapId);
static BOOL func_ov177_021e7ae0(WifiBattleMatchPartnerCard *card, PrintQueue *printQueue);
static void func_ov177_021e7b44(WifiBattleMatchPartnerCard *card);
static void func_ov177_021e7b68(WifiBattleMatchPartnerCard *card, u32 look, ClActUnit *unit,
                                                 HeapID heapId);
static void func_ov177_021e7c14(WifiBattleMatchPartnerCard *card);
static void func_ov177_021e7c3c(WifiBattleMatchPartnerCard *card,
                                                 const LiveBattleMatchPlayerData *data, ClActUnit *unit,
                                                 WifiBattleMatchRes *res, HeapID heapId);
static void func_ov177_021e7cd0(WifiBattleMatchPartnerCard *card);
static u32 func_ov177_021e7cf0(u32 unk0, u32 count, u32 unk2);

// The first palette of the badge's colors that each look uses
// pokebw2's sBadgePaletteStarts
static const u8 data_ov177_021e8cbe[16] = {
    2, 3, 6, 5, 4, 5, 2, 0, 1, 3, 6, 5, 4, 7, 7, 0,
};

// The windows of the Wi-Fi competition's card, of the other player's card, of the live one's card and of the rank
// card, each with its x, y, width and height
// pokebw2's sPartnerCardRects
static const u8 data_ov177_021e8cee[8][4] = {
#include "asset/wifibattlematch_view_data_ov177_021e8cee.inc"
};

// pokebw2's sWifiCardRects
static const u8 data_ov177_021e8cce[8][4] = {
#include "asset/wifibattlematch_view_data_ov177_021e8cce.inc"
};

// pokebw2's sLiveCardRects
static const u8 data_ov177_021e8d0e[10][4] = {
#include "asset/wifibattlematch_view_data_ov177_021e8d0e.inc"
};

// pokebw2's sRankCardRects
static const u8 data_ov177_021e8d36[12][4] = {
#include "asset/wifibattlematch_view_data_ov177_021e8d36.inc"
};

// pokebw2's WifiBattleMatchRes_Create
WifiBattleMatchRes *func_ov177_021e567c(ClActUnit *unit, u32 kind, HeapID heapId) {
    WifiBattleMatchRes *res = GFL_HeapAllocate(heapId, sizeof(WifiBattleMatchRes), FALSE, "wifibattlematch_view.c", 125);
    ArcTool *arc;
    NNSG2dPaletteData *palette;
    void *file;
    u8 *rawData;

    MI_CpuFill8(res, 0, sizeof(WifiBattleMatchRes));
    arc = GFL_ArcSysCreateFileHandle(ARCID_WIFIBATTLEMATCH, HEAPID_TAIL(heapId));
    GFL_G2DIOLoadArcNCLRDefault(arc, 0, PALTYPE_MAIN_BG, 0, 0, HEAPID_TAIL(heapId));
    GFL_G2DIOLoadArcNCLRDefault(arc, 0, PALTYPE_SUB_BG, 0, 0, HEAPID_TAIL(heapId));
    GFL_BGSysLoadArcNCGRStatic(arc, 2, 3, 0, 0, FALSE, HEAPID_TAIL(heapId));
    GFL_BGSysLoadArcNCGRStatic(arc, 2, 7, 0, 0, FALSE, HEAPID_TAIL(heapId));
    func_ov177_021e58a4(res, kind, heapId);
    res->res[RES_PALETTE + CLACT_VRAM_MAIN] = func_0204abf0(arc, 1, CLACT_VRAM_MAIN, 0, HEAPID_TAIL(heapId));
    res->res[RES_CHARS + CLACT_VRAM_MAIN] = func_0204a6c8(arc, 3, FALSE, CLACT_VRAM_MAIN, HEAPID_TAIL(heapId));
    res->res[RES_CELLS + CLACT_VRAM_MAIN] = func_0204af28(arc, 4, 11, HEAPID_TAIL(heapId));
    res->res[RES_PALETTE + CLACT_VRAM_SUB] = func_0204abf0(arc, 1, CLACT_VRAM_SUB, 0, HEAPID_TAIL(heapId));
    res->res[RES_CHARS + CLACT_VRAM_SUB] = func_0204a6c8(arc, 3, FALSE, CLACT_VRAM_SUB, HEAPID_TAIL(heapId));
    res->res[RES_CELLS + CLACT_VRAM_SUB] = func_0204af28(arc, 4, 11, HEAPID_TAIL(heapId));
    file = GFL_G2DIOReadNCLRArc(arc, 0, &palette, heapId);
    rawData = palette->pRawData;
    MI_CpuCopy8(rawData, res->colorsA, sizeof(res->colorsA));
    MI_CpuCopy8(rawData + 0x120, res->colorsB, sizeof(res->colorsB));
    GFL_HeapFree(file);
    GFL_ArcToolFree(arc);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, PALTYPE_MAIN_BG, 0x1e0, 0x20, HEAPID_TAIL(heapId));
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, PALTYPE_SUB_BG, 0x1e0, 0x20, HEAPID_TAIL(heapId));
    GFL_BGSysFillChar(0, 0, 1, 0);
    LoadSysMsgBox(0, 1, 13, 0, HEAPID_TAIL(heapId));
    LoadSysMsgBox(0, 10, 14, 0, HEAPID_TAIL(heapId));
    GFL_BGSysMoveBG(2, BG_MOVE_SET_X, -256);
    GFL_BGSysMoveBG(1, BG_MOVE_SET_X, -256);
    GFL_BGSysMoveBG(6, BG_MOVE_SET_X, -256);
    GFL_BGSysMoveBG(5, BG_MOVE_SET_X, -256);
    return res;
}

// pokebw2's WifiBattleMatchRes_Free
void func_ov177_021e5868(WifiBattleMatchRes *res) {
    func_0204ada4(res->res[RES_PALETTE + CLACT_VRAM_MAIN]);
    func_0204a8d4(res->res[RES_CHARS + CLACT_VRAM_MAIN]);
    func_0204afd8(res->res[RES_CELLS + CLACT_VRAM_MAIN]);
    func_0204ada4(res->res[RES_PALETTE + CLACT_VRAM_SUB]);
    func_0204a8d4(res->res[RES_CHARS + CLACT_VRAM_SUB]);
    func_0204afd8(res->res[RES_CELLS + CLACT_VRAM_SUB]);
    GFL_BGSysFreeFilledChar(0, 1, 0);
    GFL_HeapFree(res);
}

// pokebw2's WifiBattleMatchRes_LoadBG
void func_ov177_021e58a4(WifiBattleMatchRes *res, u32 kind, HeapID heapId) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_WIFIBATTLEMATCH, HEAPID_TAIL(heapId));

    loadBGScrToVramByFileNoReserveNegAlign(arc, 5, 3, 0, 0, FALSE, HEAPID_TAIL(heapId));
    loadBGScrToVramByFileNoReserveNegAlign(arc, 6, 7, 0, 0, FALSE, HEAPID_TAIL(heapId));
    switch (kind) {
    case 2:
        loadBGScrToVramByFileNoReserveNegAlign(arc, 8, 6, 0, 0, FALSE, HEAPID_TAIL(heapId));
        break;
    case 3:
        loadBGScrToVramByFileNoReserveNegAlign(arc, 10, 2, 0, 0, FALSE, HEAPID_TAIL(heapId));
        break;
    case 0:
    case 1:
        loadBGScrToVramByFileNoReserveNegAlign(arc, 10, 6, 0, 0, FALSE, HEAPID_TAIL(heapId));
        break;
    }
    GFL_BGSysMoveBG(2, BG_MOVE_SET_X, -256);
    GFL_BGSysMoveBG(1, BG_MOVE_SET_X, -256);
    GFL_BGSysMoveBG(6, BG_MOVE_SET_X, -256);
    GFL_BGSysMoveBG(5, BG_MOVE_SET_X, -256);
    GFL_ArcToolFree(arc);
}

// pokebw2's WifiBattleMatchView_GetRes
static u32 func_ov177_021e5978(WifiBattleMatchRes *res, u32 index) {
    return res->res[index];
}

// pokebw2's WifiBattleMatchRes_Update
void func_ov177_021e5980(WifiBattleMatchRes *res) {
    // The palette rows that are blended: the first of the BGs and the third of the main screen's OBJs
    static const u8 sRows[2] = { 0, 2 };
    int i;

    if (res->angle + 0x200 >= 0x10000) {
        res->angle = res->angle - 0xfe00;
    } else {
        res->angle += 0x200;
    }
    for (i = 0; i < 16; i++) {
        func_ov177_021e5a30(NNS_GFD_DST_2D_BG_PLTT_MAIN, &res->colors[i], &res->angle, 0, sRows[0], i,
                                       res->colorsA[i], res->colorsB[i]);
        func_ov177_021e5a30(NNS_GFD_DST_2D_BG_PLTT_SUB, &res->colors[i], &res->angle, 0, sRows[0], i,
                                       res->colorsA[i], res->colorsB[i]);
        func_ov177_021e5a30(NNS_GFD_DST_2D_OBJ_PLTT_MAIN, &res->colors[i], &res->angle, 0, sRows[1], i,
                                       res->colorsA[i], res->colorsB[i]);
    }
}

// Blends two colors by the angle, and queues the result for a palette
static void func_ov177_021e5a30(NNS_GFD_DST_TYPE type, u16 *dst, const u16 *angle, u32 a3, u8 row,
                                           u8 index, u16 colorA, u16 colorB) {
    s16 ratio = (FX_CosIdx(*angle) + FX32_ONE) / 2;
    u8 redA = colorA & 0x1f;
    u8 greenA = (colorA & 0x3e0) >> 5;
    u8 blueA = (colorA & 0x7c00) >> 10;
    u8 redB = colorB & 0x1f;
    u8 greenB = (colorB & 0x3e0) >> 5;
    u8 blueB = (colorB & 0x7c00) >> 10;
    u8 red = redA + ((redB - redA) * ratio >> 12);
    u8 green = greenA + ((greenB - greenA) * ratio >> 12);
    u8 blue = blueA + ((blueB - blueA) * ratio >> 12);

    *dst = red | (green << 5) | (blue << 10);
    NNS_GfdRegisterNewVramTransferTask(type, row * 0x20 + index * 2, dst, 2);
}

void func_ov177_021e5adc(PrintWindow *printWindow, PrintQueue *queue, Font *font, u32 count1, u32 count2,
                         HeapID heapId) {
    MsgData *msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_BTL_MAIN_12, heapId);
    WordSet *wordSet = GFL_WordSetSystemCreateDefault(heapId);
    StrBuf *text = GFL_StrBufCreate(0x20, heapId);
    u16 right = func_02045738(printWindow->window) * 8;
    int count2X;
    int middleX;
    u16 width;
    StrBuf *maxStr;

    maxStr = GFL_MsgDataLoadStrbufNew(msgData, 2);
    WordSetNumber(wordSet, 0, count2, 8, 0, TRUE);
    GFL_WordSetFormatStrbuf(wordSet, text, maxStr);
    width = GFL_FontGetBlockWidth(text, font, 0);
    count2X = right - width;
    PrintWindow_PrintNoColor(printWindow, queue, count2X, 0, text, font);
    GFL_StrBufFree(maxStr);
    maxStr = GFL_MsgDataLoadStrbufNew(msgData, 0);
    width = GFL_FontGetBlockWidth(maxStr, font, 0);
    middleX = count2X - width;
    PrintWindow_PrintNoColor(printWindow, queue, middleX, 0, maxStr, font);
    GFL_StrBufFree(maxStr);
    maxStr = GFL_MsgDataLoadStrbufNew(msgData, 1);
    WordSetNumber(wordSet, 0, count1, 8, 0, TRUE);
    GFL_WordSetFormatStrbuf(wordSet, text, maxStr);
    width = GFL_FontGetBlockWidth(text, font, 0);
    PrintWindow_PrintNoColor(printWindow, queue, middleX - width, 0, text, font);
    GFL_StrBufFree(maxStr);
    GFL_StrBufFree(text);
    GFL_WordSetSystemFree(wordSet);
    GFL_MsgDataFree(msgData);
}


WifiBattleMatchCard *func_ov177_021e5c20(const WifiBattleMatchRankInfo *info, u32 titleMsg, PlayerInfo *playerInfo,
                                         ClActUnit *unit, WifiBattleMatchRes *res, Font *font,
                                         PrintQueue *printQueue, MsgData *msgData, WordSet *wordSet, HeapID heapId) {
    WifiBattleMatchCard *card = GFL_HeapAllocate(heapId, sizeof(WifiBattleMatchCard), FALSE, "wifibattlematch_view.c",
                                                 553);
    ClActorPos pos;
    u32 stars;
    u32 colorKind;

    MI_CpuFill8(card, 0, sizeof(WifiBattleMatchCard));
    card->bg = 5;
    card->palette = 15;
    card->iconPaletteSlot = 3;
    card->itemPaletteSlot = 7;
    card->surface = CLACT_VRAM_SUB;
    G2S_SetBlendAlpha(GX_PLANEMASK_BG2, GX_PLANEMASK_BG3, 15, 3);
    colorKind = 1;
    stars = func_ov177_021e7cf0(info->unkC, info->unk10, info->unk12);
    if (stars != 5) {
        colorKind = 0;
    }
    func_ov177_021e6718(card, titleMsg, info, playerInfo, font, printQueue, msgData, wordSet,
                                        colorKind, heapId);
    func_ov177_021e72fc(card, info->look, unit, heapId);
    pos.x = 0x120;
    pos.y = 0x44;
    func_0204b404(card->actors[CARD_ACTOR_BADGE], &pos, CLACT_VRAM_SUB);
    func_ov177_021e7464(card, info, unit, res, heapId);
    GFL_BGSysSetScrPaletteNo(6, 0, 0, 32, 6, 1);
    GFL_BGSysSetScrPaletteNo(6, 0, 6, 32, 18, stars + 3);
    GFL_BGSysLoadScr(6);
    G2S_SetBlendAlpha(GX_PLANEMASK_BG2, GX_PLANEMASK_BG3, 15, 3);
    return card;
}

// pokebw2's WifiBattleMatchRankCard_Free
void func_ov177_021e5d40(WifiBattleMatchCard *card) {
    G2S_BlendNone();
    func_ov177_021e74fc(card);
    func_ov177_021e73b8(card);
    func_ov177_021e6efc(card);
    GFL_HeapFree(card);
}

WifiBattleMatchCard *func_ov177_021e5d68(const WifiBattleMatchWifiCardInfo *info, u32 ratio, PlayerInfo *playerInfo,
                                         ClActUnit *unit, WifiBattleMatchRes *res, Font *font,
                                         PrintQueue *printQueue, MsgData *msgData, WordSet *wordSet, void *records,
                                         BOOL main, HeapID heapId) {
    WifiBattleMatchCard *card = GFL_HeapAllocate(heapId, sizeof(WifiBattleMatchCard), FALSE, "wifibattlematch_view.c",
                                                 639);
    ClActorPos pos;
    u32 frameBG;

    MI_CpuFill8(card, 0, sizeof(WifiBattleMatchCard));
    card->records = records;
    card->heapId = heapId;
    card->unit = unit;
    if (main) {
        frameBG = 2;
        card->bg = 1;
        card->palette = 15;
        card->iconPaletteSlot = 5;
        card->itemPaletteSlot = 9;
        card->surface = CLACT_VRAM_MAIN;
        G2_SetBlendAlpha(GX_PLANEMASK_BG2, GX_PLANEMASK_BG3, 15, 3);
    } else {
        frameBG = 6;
        card->bg = 5;
        card->palette = 15;
        card->iconPaletteSlot = 3;
        card->itemPaletteSlot = 7;
        card->surface = CLACT_VRAM_SUB;
        G2S_SetBlendAlpha(GX_PLANEMASK_BG2, GX_PLANEMASK_BG3, 15, 3);
    }
    switch (info->variant) {
    case 0:
        GFL_BGSysSetScrPaletteNo(frameBG, 0, 0, 32, 5, 1);
        GFL_BGSysSetScrPaletteNo(frameBG, 0, 5, 32, 18, 3);
        GFL_BGSysLoadScr(frameBG);
        break;
    case 1:
        GFL_BGSysSetScrPaletteNo(frameBG, 0, 0, 32, 5, 1);
        GFL_BGSysSetScrPaletteNo(frameBG, 0, 5, 32, 18, 7);
        GFL_BGSysLoadScr(frameBG);
        break;
    }
    func_ov177_021e6940(card, ratio, info, playerInfo, font, printQueue, msgData, wordSet, heapId);
    func_ov177_021e6f20(card, unit, records, heapId);
    func_ov177_021e72fc(card, info->look, unit, heapId);
    pos.x = 0x120;
    pos.y = 0x6e;
    func_0204b404(card->actors[CARD_ACTOR_BADGE], &pos, card->surface);
    func_ov177_021e73e4(card, unit, res, heapId);
    return card;
}

// pokebw2's WifiBattleMatchWifiCard_Free
void func_ov177_021e5ef8(WifiBattleMatchCard *card) {
    G2S_BlendNone();
    func_ov177_021e7454(card);
    func_ov177_021e73b8(card);
    func_ov177_021e7270(card);
    func_ov177_021e6efc(card);
    GFL_HeapFree(card);
}

void func_ov177_021e5f24(WifiBattleMatchCard *card, u32 page, MsgData *msgData, PrintQueue *printQueue, Font *font,
                         HeapID heapId) {
    ClActorPos pos;
    StrBuf *text;
    s32 baseX;
    int i;

    GFL_BitmapFill(BmpWin_GetBitmap(card->windows[7]), 0);
    func_ov177_021e7270(card);
    func_ov177_021e6f20(card, card->unit, card->records, card->heapId);
    baseX = 0x20 - GFL_BGSysGetBGOffsetX(card->bg);

    for (i = 0; i < CARD_PARTY_COUNT; i++) {
        pos.y = 0xa8;
        pos.x = baseX + i * 0x20;
        if (card->actors[i] != NULL) {
            func_0204b404(card->actors[i], &pos, card->surface);
        }
        if (card->actors[CARD_ACTOR_ITEM + i] != NULL) {
            pos.x += 0xc;
            pos.y += 8;
            func_0204b404(card->actors[CARD_ACTOR_ITEM + i], &pos, card->surface);
        }
    }

    switch (page) {
    case 0:
        for (i = 0; i < CARD_PARTY_COUNT; i++) {
            if (card->actors[CARD_ACTOR_ITEM + i] != NULL) {
                func_0204b3dc(card->actors[CARD_ACTOR_ITEM + i], 1);
            }
            if (card->actors[i] != NULL) {
                func_0204b3dc(card->actors[i], 1);
            }
        }
        func_0204b3dc(card->actors[CARD_ACTOR_MARK], 1);
        text = GFL_MsgDataLoadStrbufNew(msgData, 0x11);
        func_0201c724(printQueue, BmpWin_GetBitmap(card->printWindows[7].window), 0, 0, text, font, 0x3c40);
        card->printWindows[7].flushPending = TRUE;
        break;
    case 1:
        for (i = 0; i < CARD_PARTY_COUNT; i++) {
            if (card->actors[CARD_ACTOR_ITEM + i] != NULL) {
                func_0204b3dc(card->actors[CARD_ACTOR_ITEM + i], 1);
            }
            if (card->actors[i] != NULL) {
                func_0204b3dc(card->actors[i], 1);
            }
        }
        func_0204b3dc(card->actors[CARD_ACTOR_MARK], 0);
        text = GFL_MsgDataLoadStrbufNew(msgData, 0x12);
        func_0201c724(printQueue, BmpWin_GetBitmap(card->printWindows[7].window), 0, 0, text, font, 0x3c40);
        card->printWindows[7].flushPending = TRUE;
        break;
    case 2:
        for (i = 0; i < CARD_PARTY_COUNT; i++) {
            if (card->actors[CARD_ACTOR_ITEM + i] != NULL) {
                func_0204b3dc(card->actors[CARD_ACTOR_ITEM + i], 0);
            }
            if (card->actors[i] != NULL) {
                func_0204b3dc(card->actors[i], 0);
            }
        }
        func_0204b3dc(card->actors[CARD_ACTOR_MARK], 0);
        text = GFL_MsgDataLoadStrbufNew(msgData, 0x13);
        func_0201c724(printQueue, BmpWin_GetBitmap(card->printWindows[7].window), 0, 0, text, font, 0x3c40);
        card->printWindows[7].flushPending = TRUE;
        break;
    }
    GFL_StrBufFree(text);
}



WifiBattleMatchCard *func_ov177_021e6118(const WifiBattleMatchLiveCardInfo *info, PlayerInfo *playerInfo,
                                         ClActUnit *unit, WifiBattleMatchRes *res, Font *font,
                                         PrintQueue *printQueue, MsgData *msgData, WordSet *wordSet, void *records,
                                         BOOL main, HeapID heapId) {
    WifiBattleMatchCard *card = GFL_HeapAllocate(heapId, sizeof(WifiBattleMatchCard), FALSE, "wifibattlematch_view.c",
                                                 852);
    ClActorPos pos;
    u32 frameBG;

    MI_CpuFill8(card, 0, sizeof(WifiBattleMatchCard));
    card->records = records;
    card->heapId = heapId;
    card->unit = unit;
    if (main) {
        frameBG = 2;
        card->bg = 1;
        card->palette = 15;
        card->iconPaletteSlot = 5;
        card->itemPaletteSlot = 9;
        card->surface = CLACT_VRAM_MAIN;
        G2_SetBlendAlpha(GX_PLANEMASK_BG2, GX_PLANEMASK_BG3, 15, 3);
    } else {
        frameBG = 6;
        card->bg = 5;
        card->palette = 15;
        card->iconPaletteSlot = 3;
        card->itemPaletteSlot = 7;
        card->surface = CLACT_VRAM_SUB;
        G2S_SetBlendAlpha(GX_PLANEMASK_BG2, GX_PLANEMASK_BG3, 15, 3);
    }
    switch (info->variant) {
    case 0:
        GFL_BGSysSetScrPaletteNo(frameBG, 0, 0, 32, 5, 1);
        GFL_BGSysSetScrPaletteNo(frameBG, 0, 5, 32, 18, 3);
        GFL_BGSysLoadScr(frameBG);
        break;
    case 1:
        GFL_BGSysSetScrPaletteNo(frameBG, 0, 0, 32, 5, 1);
        GFL_BGSysSetScrPaletteNo(frameBG, 0, 5, 32, 18, 7);
        GFL_BGSysLoadScr(frameBG);
        break;
    }
    func_ov177_021e6be8(card, info, playerInfo, font, printQueue, msgData, wordSet, heapId);
    func_ov177_021e6f20(card, unit, records, heapId);
    func_ov177_021e72fc(card, info->look, unit, heapId);
    pos.x = 0x120;
    pos.y = 0x6e;
    func_0204b404(card->actors[CARD_ACTOR_BADGE], &pos, card->surface);
    func_ov177_021e73e4(card, unit, res, heapId);
    return card;
}

// pokebw2's WifiBattleMatchLiveCard_Free
void func_ov177_021e62a0(WifiBattleMatchCard *card) {
    G2S_BlendNone();
    func_ov177_021e7454(card);
    func_ov177_021e73b8(card);
    func_ov177_021e7270(card);
    func_ov177_021e6efc(card);
    GFL_HeapFree(card);
}

void func_ov177_021e62cc(WifiBattleMatchCard *card, u32 page, MsgData *msgData, PrintQueue *printQueue, Font *font,
                         HeapID heapId) {
    ClActorPos pos;
    StrBuf *text;
    s32 baseX;
    int i;

    GFL_BitmapFill(BmpWin_GetBitmap(card->windows[9]), 0);
    func_ov177_021e7270(card);
    func_ov177_021e6f20(card, card->unit, card->records, card->heapId);
    baseX = 0x20 - GFL_BGSysGetBGOffsetX(card->bg);

    for (i = 0; i < CARD_PARTY_COUNT; i++) {
        pos.y = 0xa8;
        pos.x = baseX + i * 0x20;
        if (card->actors[i] != NULL) {
            func_0204b404(card->actors[i], &pos, card->surface);
        }
        if (card->actors[CARD_ACTOR_ITEM + i] != NULL) {
            pos.x += 0xc;
            pos.y += 8;
            func_0204b404(card->actors[CARD_ACTOR_ITEM + i], &pos, card->surface);
        }
    }

    switch (page) {
    case 0:
        for (i = 0; i < CARD_PARTY_COUNT; i++) {
            if (card->actors[CARD_ACTOR_ITEM + i] != NULL) {
                func_0204b3dc(card->actors[CARD_ACTOR_ITEM + i], 1);
            }
            if (card->actors[i] != NULL) {
                func_0204b3dc(card->actors[i], 1);
            }
        }
        func_0204b3dc(card->actors[CARD_ACTOR_MARK], 1);
        text = GFL_MsgDataLoadStrbufNew(msgData, 0x11);
        func_0201c724(printQueue, BmpWin_GetBitmap(card->printWindows[9].window), 0, 0, text, font, 0x3c40);
        card->printWindows[9].flushPending = TRUE;
        break;
    case 1:
        for (i = 0; i < CARD_PARTY_COUNT; i++) {
            if (card->actors[CARD_ACTOR_ITEM + i] != NULL) {
                func_0204b3dc(card->actors[CARD_ACTOR_ITEM + i], 1);
            }
            if (card->actors[i] != NULL) {
                func_0204b3dc(card->actors[i], 1);
            }
        }
        func_0204b3dc(card->actors[CARD_ACTOR_MARK], 0);
        text = GFL_MsgDataLoadStrbufNew(msgData, 0x12);
        func_0201c724(printQueue, BmpWin_GetBitmap(card->printWindows[9].window), 0, 0, text, font, 0x3c40);
        card->printWindows[9].flushPending = TRUE;
        break;
    case 2:
        for (i = 0; i < CARD_PARTY_COUNT; i++) {
            if (card->actors[CARD_ACTOR_ITEM + i] != NULL) {
                func_0204b3dc(card->actors[CARD_ACTOR_ITEM + i], 0);
            }
            if (card->actors[i] != NULL) {
                func_0204b3dc(card->actors[i], 0);
            }
        }
        func_0204b3dc(card->actors[CARD_ACTOR_MARK], 0);
        text = GFL_MsgDataLoadStrbufNew(msgData, 0x13);
        func_0201c724(printQueue, BmpWin_GetBitmap(card->printWindows[9].window), 0, 0, text, font, 0x3c40);
        card->printWindows[9].flushPending = TRUE;
        break;
    }
    GFL_StrBufFree(text);
}


// pokebw2's WifiBattleMatchCard_Flush
BOOL func_ov177_021e64c0(WifiBattleMatchCard *card, PrintQueue *printQueue) {
    return func_ov177_021e6e8c(card, printQueue);
}

// pokebw2's WifiBattleMatchCard_SlideIn
BOOL func_ov177_021e64c8(WifiBattleMatchCard *card) {
    BOOL done;
    s32 offset;
    ClActorPos start;
    ClActorPos pos;
    int i;

    if (GFL_BGSysGetBGOffsetX(card->bg) == 0) {
        return TRUE;
    }
    done = FALSE;
    if (card->slideCount == 0) {
        for (i = 0; i < CARD_ACTOR_COUNT; i++) {
            if (card->actors[i] != NULL) {
                func_0204b454(card->actors[i], &start, card->surface);
                card->slideX[i] = start.x;
            }
        }
        GFL_SndSEPlay(SEQ_SE_SYS_75);
    }
    offset = (card->slideCount << 8) / SLIDE_FRAMES - 256;
    for (i = 0; i < CARD_ACTOR_COUNT; i++) {
        if (card->actors[i] != NULL) {
            func_0204b454(card->actors[i], &pos, card->surface);
            pos.x = card->slideX[i] - (card->slideCount << 8) / SLIDE_FRAMES;
            func_0204b404(card->actors[i], &pos, card->surface);
        }
    }
    if (card->slideCount++ >= SLIDE_FRAMES) {
        offset = 0;
        card->slideCount = 0;
        done = TRUE;
    }
    GFL_BGSysMoveBGReq(card->bg + 1, BG_MOVE_SET_X, offset);
    GFL_BGSysMoveBGReq(card->bg, BG_MOVE_SET_X, offset);
    return done;
}

// pokebw2's WifiBattleMatchCard_SlideOut
BOOL func_ov177_021e65fc(WifiBattleMatchCard *card) {
    BOOL done = FALSE;
    s32 offset;
    ClActorPos start;
    ClActorPos pos;
    int i;

    if (card->slideCount == 0) {
        for (i = 0; i < CARD_ACTOR_COUNT; i++) {
            if (card->actors[i] != NULL) {
                func_0204b454(card->actors[i], &start, card->surface);
                card->slideX[i] = start.x;
            }
        }
        GFL_SndSEPlay(SEQ_SE_SYS_75);
    }
    offset = (card->slideCount << 8) / SLIDE_FRAMES;
    for (i = 0; i < CARD_ACTOR_COUNT; i++) {
        if (card->actors[i] != NULL) {
            func_0204b454(card->actors[i], &pos, card->surface);
            pos.x = card->slideX[i] - (card->slideCount << 8) / SLIDE_FRAMES;
            func_0204b404(card->actors[i], &pos, card->surface);
        }
    }
    if (card->slideCount++ >= SLIDE_FRAMES) {
        offset = 256;
        card->slideCount = 0;
        done = TRUE;
    }
    GFL_BGSysMoveBGReq(card->bg + 1, BG_MOVE_SET_X, offset);
    GFL_BGSysMoveBGReq(card->bg, BG_MOVE_SET_X, offset);
    return done;
}

void func_ov177_021e6718(WifiBattleMatchCard *card, u32 titleMsg, const WifiBattleMatchRankInfo *info,
                         PlayerInfo *playerInfo, Font *font, PrintQueue *printQueue, MsgData *msgData,
                         WordSet *wordSet, u32 colorKind, HeapID heapId) {
    StrBuf *format;
    StrBuf *text;
    u32 i;
    int j;
    BOOL draw;
    u16 color;
    BmpWin *window;

    card->windowCount = 11;
    for (i = 0; i < card->windowCount; i++) {
        card->windows[i] = BmpWin_CreateDynamic(5, data_ov177_021e8d36[i][0], data_ov177_021e8d36[i][1], data_ov177_021e8d36[i][2],
                                                data_ov177_021e8d36[i][3], 15, TRUE);
        GFL_BitmapFill(BmpWin_GetBitmap(card->windows[i]), 0);
        window = card->windows[i];
        BmpWin_FlushChar(window);
        BmpWin_FlushMap(window);
        GFL_BGSysLoadScr(BmpWin_GetBGIndex(window));
        if (card->windows[i] != NULL) {
            card->printWindows[i].window = card->windows[i];
            card->printWindows[i].flushPending = FALSE;
        }
    }
    format = GFL_StrBufCreate(0x80, heapId);
    text = GFL_StrBufCreate(0x80, heapId);
    for (j = 0; j < 11; j++) {
        color = colorKind == 1 ? PRINT_COLOR(15, 2, 0) : PRINT_COLOR(1, 2, 0);
        draw = TRUE;
        switch (j) {
        case 0:
            GFL_MsgDataLoadStrbuf(msgData, titleMsg, text);
            color = PRINT_COLOR(15, 2, 0);
            break;
        case 1:
            GFL_MsgDataLoadStrbuf(msgData, info->titleMsg + 2, text);
            color = PRINT_COLOR(15, 2, 0);
            break;
        case 2:
            GFL_MsgDataLoadStrbuf(msgData, 7, format);
            copyVarForText(wordSet, 0, playerInfo);
            GFL_WordSetFormatStrbuf(wordSet, text, format);
            color = PRINT_COLOR(15, 2, 0);
            break;
        case 3:
            if (titleMsg != 0) {
                GFL_MsgDataLoadStrbuf(msgData, 8, text);
            } else {
                draw = FALSE;
            }
            break;
        case 4:
            if (titleMsg != 0) {
                GFL_MsgDataLoadStrbuf(msgData, 9, format);
                WordSetNumber(wordSet, 0, info->unk4, 4, 1, 1);
                GFL_WordSetFormatStrbuf(wordSet, text, format);
            } else {
                draw = FALSE;
            }
            break;
        case 5:
            GFL_MsgDataLoadStrbuf(msgData, 10, text);
            break;
        case 6:
            GFL_MsgDataLoadStrbuf(msgData, 11, format);
            WordSetNumber(wordSet, 0, info->unkC, 6, 1, 1);
            GFL_WordSetFormatStrbuf(wordSet, text, format);
            break;
        case 7:
            GFL_MsgDataLoadStrbuf(msgData, 12, text);
            break;
        case 8:
            GFL_MsgDataLoadStrbuf(msgData, 13, format);
            WordSetNumber(wordSet, 0, info->unk10, 5, 1, 1);
            GFL_WordSetFormatStrbuf(wordSet, text, format);
            break;
        case 9:
            GFL_MsgDataLoadStrbuf(msgData, 14, text);
            break;
        case 10:
            GFL_MsgDataLoadStrbuf(msgData, 15, format);
            WordSetNumber(wordSet, 0, info->unk12, 5, 1, 1);
            GFL_WordSetFormatStrbuf(wordSet, text, format);
            break;
        }
        if (draw) {
            func_0201c724(printQueue, BmpWin_GetBitmap(card->printWindows[j].window), 0, 0, text, font, color);
            card->printWindows[j].flushPending = TRUE;
        }
    }
    GFL_StrBufFree(format);
    GFL_StrBufFree(text);
}

void func_ov177_021e6940(WifiBattleMatchCard *card, BOOL flag, const WifiBattleMatchWifiCardInfo *info,
                         PlayerInfo *playerInfo, Font *font, PrintQueue *printQueue, MsgData *msgData, WordSet *wordSet,
                         HeapID heapId) {
    StrBuf *format;
    StrBuf *text;
    int i;
    int j;
    BOOL draw;
    u16 color;
    BmpWin *window;
    u32 start;
    u32 end;

    card->windowCount = 8;
    for (i = 0; i < card->windowCount; i++) {
        card->windows[i] = BmpWin_CreateDynamic(card->bg, data_ov177_021e8cce[i][0], data_ov177_021e8cce[i][1],
                                                data_ov177_021e8cce[i][2], data_ov177_021e8cce[i][3], card->palette, TRUE);
        GFL_BitmapFill(BmpWin_GetBitmap(card->windows[i]), 0);
        window = card->windows[i];
        BmpWin_FlushChar(window);
        BmpWin_FlushMap(window);
        GFL_BGSysLoadScr(BmpWin_GetBGIndex(window));
        if (card->windows[i] != NULL) {
            card->printWindows[i].window = card->windows[i];
            card->printWindows[i].flushPending = FALSE;
        }
    }
    format = GFL_StrBufCreate(0x80, heapId);
    text = GFL_StrBufCreate(0x80, heapId);
    for (j = 0; j < 8; j++) {
        draw = TRUE;
        color = PRINT_COLOR(1, 2, 0);
        switch (j) {
        case 0:
            GFL_StrBufLoadString(text, (const u16 *)info);
            color = PRINT_COLOR(15, 2, 0);
            break;
        case 1:
            start = info->startDate;
            end = info->endDate;
            GFL_MsgDataLoadStrbuf(msgData, 16, format);
            WordSetNumber(wordSet, 0, (u16)((u8)(start >> 24) + 2000), 4, 0, 1);
            loadMonthToStrbuf(wordSet, 1, (u8)(start >> 16));
            WordSetNumber(wordSet, 2, (u8)(start >> 8), 2, 0, 1);
            WordSetNumber(wordSet, 3, (u16)((u8)(end >> 24) + 2000), 4, 0, 1);
            loadMonthToStrbuf(wordSet, 4, (u8)(end >> 16));
            WordSetNumber(wordSet, 5, (u8)(end >> 8), 2, 0, 1);
            GFL_WordSetFormatStrbuf(wordSet, text, format);
            color = PRINT_COLOR(15, 2, 0);
            break;
        case 2:
            textCopy(playerInfo->name, text);
            color = PRINT_COLOR(15, 2, 0);
            break;
        case 3:
            GFL_MsgDataLoadStrbuf(msgData, 8, text);
            break;
        case 4:
            GFL_MsgDataLoadStrbuf(msgData, 9, format);
            WordSetNumber(wordSet, 0, info->unkA0, 4, 1, 1);
            GFL_WordSetFormatStrbuf(wordSet, text, format);
            break;
        case 5:
            GFL_MsgDataLoadStrbuf(msgData, 10, text);
            break;
        case 6:
            if (flag) {
                func_ov177_021e5adc(&card->printWindows[j], printQueue, font, info->unkA4, info->unkA8, heapId);
                draw = FALSE;
            } else {
                GFL_MsgDataLoadStrbuf(msgData, 11, format);
                WordSetNumber(wordSet, 0, info->unkA4, 6, 1, 1);
                GFL_WordSetFormatStrbuf(wordSet, text, format);
            }
            break;
        case 7:
            GFL_MsgDataLoadStrbuf(msgData, 17, text);
            color = PRINT_COLOR(15, 2, 0);
            break;
        }
        if (draw) {
            func_0201c724(printQueue, BmpWin_GetBitmap(card->printWindows[j].window), 0, 0, text, font, color);
            card->printWindows[j].flushPending = TRUE;
        }
    }
    GFL_StrBufFree(format);
    GFL_StrBufFree(text);
}

void func_ov177_021e6be8(WifiBattleMatchCard *card, const WifiBattleMatchLiveCardInfo *info, PlayerInfo *playerInfo,
                         Font *font, PrintQueue *printQueue, MsgData *msgData, WordSet *wordSet, HeapID heapId) {
    StrBuf *format;
    StrBuf *text;
    int i;
    int j;
    BOOL draw;
    u16 color;
    BmpWin *window;
    u32 start;
    u32 end;

    card->windowCount = 10;
    for (i = 0; i < card->windowCount; i++) {
        card->windows[i] = BmpWin_CreateDynamic(card->bg, data_ov177_021e8d0e[i][0], data_ov177_021e8d0e[i][1],
                                                data_ov177_021e8d0e[i][2], data_ov177_021e8d0e[i][3], card->palette, TRUE);
        GFL_BitmapFill(BmpWin_GetBitmap(card->windows[i]), 0);
        window = card->windows[i];
        BmpWin_FlushChar(window);
        BmpWin_FlushMap(window);
        GFL_BGSysLoadScr(BmpWin_GetBGIndex(window));
        if (card->windows[i] != NULL) {
            card->printWindows[i].window = card->windows[i];
            card->printWindows[i].flushPending = FALSE;
        }
    }
    format = GFL_StrBufCreate(0x80, heapId);
    text = GFL_StrBufCreate(0x80, heapId);
    for (j = 0; j < 10; j++) {
        draw = TRUE;
        color = PRINT_COLOR(1, 2, 0);
        switch (j) {
        case 0:
            GFL_StrBufLoadString(text, (const u16 *)info);
            color = PRINT_COLOR(15, 2, 0);
            break;
        case 1:
            start = info->startDate;
            end = info->endDate;
            GFL_MsgDataLoadStrbuf(msgData, 16, format);
            WordSetNumber(wordSet, 0, (u16)((u8)(start >> 24) + 2000), 4, 0, 1);
            loadMonthToStrbuf(wordSet, 1, (u8)(start >> 16));
            WordSetNumber(wordSet, 2, (u8)(start >> 8), 2, 0, 1);
            WordSetNumber(wordSet, 3, (u16)((u8)(end >> 24) + 2000), 4, 0, 1);
            loadMonthToStrbuf(wordSet, 4, (u8)(end >> 16));
            WordSetNumber(wordSet, 5, (u8)(end >> 8), 2, 0, 1);
            GFL_WordSetFormatStrbuf(wordSet, text, format);
            color = PRINT_COLOR(15, 2, 0);
            break;
        case 2:
            textCopy(playerInfo->name, text);
            color = PRINT_COLOR(15, 2, 0);
            break;
        case 3:
            GFL_MsgDataLoadStrbuf(msgData, 12, text);
            break;
        case 4:
            GFL_MsgDataLoadStrbuf(msgData, 13, format);
            WordSetNumber(wordSet, 0, info->unkA0, 5, 1, 1);
            GFL_WordSetFormatStrbuf(wordSet, text, format);
            break;
        case 5:
            GFL_MsgDataLoadStrbuf(msgData, 14, text);
            break;
        case 6:
            GFL_MsgDataLoadStrbuf(msgData, 15, format);
            WordSetNumber(wordSet, 0, info->unkA2, 5, 1, 1);
            GFL_WordSetFormatStrbuf(wordSet, text, format);
            break;
        case 7:
            GFL_MsgDataLoadStrbuf(msgData, 10, text);
            break;
        case 8:
            func_ov177_021e5adc(&card->printWindows[j], printQueue, font, info->unkA4, info->unkA6, heapId);
            draw = FALSE;
            break;
        case 9:
            GFL_MsgDataLoadStrbuf(msgData, 17, text);
            color = PRINT_COLOR(15, 2, 0);
            break;
        }
        if (draw) {
            func_0201c724(printQueue, BmpWin_GetBitmap(card->printWindows[j].window), 0, 0, text, font, color);
            card->printWindows[j].flushPending = TRUE;
        }
    }
    GFL_StrBufFree(format);
    GFL_StrBufFree(text);
}




// pokebw2's WifiBattleMatchView_CardFlush
static BOOL func_ov177_021e6e8c(WifiBattleMatchCard *card, PrintQueue *printQueue) {
    BOOL done = TRUE;
    u32 i;

    for (i = 0; i < card->windowCount; i++) {
        if (card->windows[i] != NULL) {
            PrintWindow_Flush(&card->printWindows[i], printQueue);
            done &= !card->printWindows[i].flushPending;
        }
    }
    return done;
}

// pokebw2's WifiBattleMatchView_CardFreeWindows
static void func_ov177_021e6efc(WifiBattleMatchCard *card) {
    u32 i;

    for (i = 0; i < card->windowCount; i++) {
        if (card->windows[i] != NULL) {
            BmpWin_Free(card->windows[i]);
        }
    }
}

static void func_ov177_021e6f20(WifiBattleMatchCard *card, ClActUnit *unit, void *records,
                                              HeapID heapId) {
    ClActorSetup setup;
    ClActorSetup itemSetup;
    int i;
    u16 species;
    u8 form;
    u8 gender;
    u8 hasItem;
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_POKEICON, heapId);

    card->iconPalette = func_0204acec(arc, func_0201bc3c(), card->surface, card->iconPaletteSlot << 5, HEAPID_TAIL(heapId));
    card->iconCells = func_0204af28(arc, func_0201bc40(), getOBJTileMapping_MainEng(), heapId);
    for (i = 0; i < CARD_PARTY_COUNT; i++) {
        species = func_0200b214(records, 0, i);
        form = func_0200b214(records, 1, i);
        gender = func_0200b214(records, 3, i);

        card->iconChars[i] =
            func_0204a6c8(arc, PokeParty_GetIconIndex((u16)(species ? species : 1), form, gender, FALSE), FALSE, card->surface, HEAPID_TAIL(heapId));
    }
    GFL_ArcToolFree(arc);
    arc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), heapId);
    card->itemPalette = func_0204ac18(arc, func_02027694(), card->surface, (card->iconPaletteSlot + 3) << 5, 0, 1,
                                      HEAPID_TAIL(heapId));
    card->itemChars = func_0204a6c8(arc, func_02027698(), FALSE, card->surface, HEAPID_TAIL(heapId));
    card->itemCells = func_0204af28(arc, func_0202769c(2), func_020276a0(2), heapId);
    GFL_ArcToolFree(arc);
    MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
    setup.y = 168;
    setup.sequence = 1;
    setup.bgPriority = 1;
    setup.priority = 1;
    for (i = 0; i < CARD_PARTY_COUNT; i++) {
        species = func_0200b214(records, 0, i);
        form = func_0200b214(records, 1, i);
        gender = func_0200b214(records, 3, i);
        hasItem = func_0200b214(records, 2, i);

        if (species != 0) {
            setup.x = i * 32 + 288;
            card->actors[i] = func_0204b294(unit, card->iconChars[i], card->iconPalette, card->iconCells, &setup,
                                            card->surface, heapId);
            func_0204b6f4(card->actors[i], func_0201bb5c(species, form, gender, FALSE), 0);
            if (hasItem) {
                itemSetup = setup;
                itemSetup.x += 12;
                itemSetup.y += 8;
                itemSetup.priority = 0;
                itemSetup.sequence = 0;
                card->actors[CARD_ACTOR_ITEM + i] = func_0204b294(unit, card->itemChars, card->itemPalette,
                                                                  card->itemCells, &itemSetup, card->surface, heapId);
            } else {
                card->actors[CARD_ACTOR_ITEM + i] = NULL;
            }
        } else {
            card->actors[i] = NULL;
            card->actors[CARD_ACTOR_ITEM + i] = NULL;
        }
    }
}

// pokebw2's WifiBattleMatchView_CardFreeIcons
static void func_ov177_021e7270(WifiBattleMatchCard *card) {
    int i;

    for (i = 0; i < CARD_PARTY_COUNT; i++) {
        if (card->actors[i] != NULL) {
            func_0204b3b4(card->actors[i]);
            card->actors[i] = NULL;
        }
        if (card->actors[CARD_ACTOR_ITEM + i] != NULL) {
            func_0204b3b4(card->actors[CARD_ACTOR_ITEM + i]);
            card->actors[CARD_ACTOR_ITEM + i] = NULL;
        }
    }
    for (i = 0; i < CARD_PARTY_COUNT; i++) {
        func_0204a8d4(card->iconChars[i]);
    }
    func_0204afd8(card->iconCells);
    func_0204ada4(card->iconPalette);
    func_0204a8d4(card->itemChars);
    func_0204ada4(card->itemPalette);
    func_0204afd8(card->itemCells);
}

// pokebw2's WifiBattleMatchView_CardLoadBadge
static void func_ov177_021e72fc(WifiBattleMatchCard *card, u32 look, ClActUnit *unit, HeapID heapId) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(32, heapId);
    ClActorSetup setup;

    card->badgePalette =
        func_0204ac18(arc, 0, card->surface, card->itemPaletteSlot << 5, data_ov177_021e8cbe[look], 1, heapId);
    card->badgeChars = func_0204a6c8(arc, look + 49, FALSE, card->surface, heapId);
    card->badgeCells = func_0204af28(arc, 65, 66, heapId);
    GFL_ArcToolFree(arc);
    MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
    setup.y = 0;
    setup.x = 0;
    setup.bgPriority = 1;
    card->actors[CARD_ACTOR_BADGE] = func_0204b294(unit, card->badgeChars, card->badgePalette, card->badgeCells,
                                                   &setup, card->surface, heapId);
    func_0204b6f4(card->actors[CARD_ACTOR_BADGE], 0, 1);
}

// pokebw2's WifiBattleMatchView_CardFreeBadge
static void func_ov177_021e73b8(WifiBattleMatchCard *card) {
    func_0204b3b4(card->actors[CARD_ACTOR_BADGE]);
    func_0204a8d4(card->badgeChars);
    func_0204afd8(card->badgeCells);
    func_0204ada4(card->badgePalette);
}

static void func_ov177_021e73e4(WifiBattleMatchCard *card, ClActUnit *unit, WifiBattleMatchRes *res,
                                             HeapID heapId) {
    ClActorSetup setup;

    MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
    setup.x = 280;
    setup.y = 152;
    setup.sequence = 7;
    setup.bgPriority = 1;
    card->actors[CARD_ACTOR_MARK] = func_0204b294(
        unit, func_ov177_021e5978(res, RES_CHARS + card->surface),
        func_ov177_021e5978(res, RES_PALETTE + card->surface),
        func_ov177_021e5978(res, RES_CELLS + card->surface), &setup, card->surface, heapId);
}

// pokebw2's WifiBattleMatchView_CardFreeMark
static void func_ov177_021e7454(WifiBattleMatchCard *card) {
    func_0204b3b4(card->actors[CARD_ACTOR_MARK]);
}

static void func_ov177_021e7464(WifiBattleMatchCard *card, const WifiBattleMatchRankInfo *info,
                                              ClActUnit *unit, WifiBattleMatchRes *res, HeapID heapId) {
    u32 count = func_ov177_021e7cf0(info->unkC, info->unk10, info->unk12);
    ClActorSetup setup;
    u32 i;

    MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
    setup.x = 256;
    setup.y = 16;
    for (i = 0; i < count; i++) {
        setup.x += 16;
        setup.sequence = i + 2;
        card->actors[CARD_ACTOR_STAR + i] = func_0204b294(
            unit, func_ov177_021e5978(res, RES_CHARS + CLACT_VRAM_SUB),
            func_ov177_021e5978(res, RES_PALETTE + CLACT_VRAM_SUB),
            func_ov177_021e5978(res, RES_CELLS + CLACT_VRAM_SUB), &setup, CLACT_VRAM_SUB, heapId);
    }
}

// pokebw2's WifiBattleMatchView_CardFreeStars
static void func_ov177_021e74fc(WifiBattleMatchCard *card) {
    int i;

    for (i = CARD_ACTOR_STAR; i < CARD_ACTOR_STAR + CARD_STAR_MAX; i++) {
        if (card->actors[i] != NULL) {
            func_0204b3b4(card->actors[i]);
            card->actors[i] = NULL;
        }
    }
}

WifiBattleMatchPartnerCard *func_ov177_021e7520(const LiveBattleMatchPlayerData *data, ClActUnit *unit,
                                                WifiBattleMatchRes *res, Font *font, PrintQueue *printQueue,
                                                MsgData *msgData, WordSet *wordSet, u32 kind, u32 shown, u32 variant,
                                                HeapID heapId) {
    WifiBattleMatchPartnerCard *card = GFL_HeapAllocate(heapId, sizeof(WifiBattleMatchPartnerCard), FALSE,
                                                        "wifibattlematch_view.c", 2386);
    u32 stars;
    u32 colorKind = 0;

    MI_CpuFill8(card, 0, sizeof(WifiBattleMatchPartnerCard));
    card->shown = shown;
    card->kind = kind;
    card->pmsDraw = func_0202db0c(unit, CLACT_VRAM_MAIN, printQueue, font, 5, 1, heapId);
    func_0202de68(card->pmsDraw, TRUE);
    func_0202de5c(card->pmsDraw, 0);
    stars = func_ov177_021e7cf0(data->record0, data->record1, data->record2);
    if (kind == 2 || kind == 3) {
        colorKind = stars == 5;
    }
    func_ov177_021e78b0(card, data, font, printQueue, msgData, wordSet, colorKind, heapId);
    func_ov177_021e7b68(card, func_02008554((PlayerInfo *)&data->info), unit, heapId);
    loadBGScrToVramByNarcNoReserveNegAlign(ARCID_WIFIBATTLEMATCH, 9, 2, 0, 0, FALSE, HEAPID_TAIL(heapId));
    switch (kind) {
    case 2:
    case 3:
        func_ov177_021e7c3c(card, data, unit, res, heapId);
        GFL_BGSysSetScrPaletteNo(2, 0, 0, 32, 2, 1);
        GFL_BGSysSetScrPaletteNo(2, 0, 3, 32, 32, stars + 3);
        GFL_BGSysSetScrPaletteNo(2, 0, 3, 1, 2, 1);
        GFL_BGSysSetScrPaletteNo(2, 31, 3, 1, 2, 1);
        break;
    case 0:
    case 1:
        GFL_BGSysLoadScrArea(2, 0, 0, 32, 2, GFL_BGSysIsScrHeapExists(2), 0, 24, 32, 32);
        switch (variant) {
        case 0:
            GFL_BGSysSetScrPaletteNo(2, 0, 0, 32, 2, 1);
            GFL_BGSysSetScrPaletteNo(2, 0, 3, 32, 32, 3);
            GFL_BGSysSetScrPaletteNo(2, 0, 3, 1, 2, 1);
            GFL_BGSysSetScrPaletteNo(2, 31, 3, 1, 2, 1);
            break;
        case 1:
            GFL_BGSysSetScrPaletteNo(2, 0, 0, 32, 2, 1);
            GFL_BGSysSetScrPaletteNo(2, 0, 3, 32, 32, 7);
            GFL_BGSysSetScrPaletteNo(2, 0, 3, 1, 2, 1);
            GFL_BGSysSetScrPaletteNo(2, 31, 3, 1, 2, 1);
            break;
        }
        break;
    }
    GFL_BGSysLoadScr(2);
    GFL_BGSysMoveBG(2, BG_MOVE_SET_X, -256);
    GFL_BGSysMoveBG(1, BG_MOVE_SET_X, -256);
    G2_SetBlendAlpha(GX_PLANEMASK_BG2, GX_PLANEMASK_BG3, 15, 3);
    return card;
}

// pokebw2's WifiBattleMatchPartnerCard_Free
void func_ov177_021e7710(WifiBattleMatchPartnerCard *card) {
    G2_BlendNone();
    switch (card->kind) {
    case 0:
    case 1:
        break;
    case 2:
    case 3:
        func_ov177_021e7cd0(card);
        break;
    }
    func_ov177_021e7c14(card);
    func_ov177_021e7b44(card);
    func_0202dc08(card->pmsDraw);
    GFL_BGSysMoveBG(2, BG_MOVE_SET_X, -256);
    GFL_BGSysMoveBG(1, BG_MOVE_SET_X, -256);
    GFL_HeapFree(card);
}

// pokebw2's WifiBattleMatchPartnerCard_Flush
BOOL func_ov177_021e7774(WifiBattleMatchPartnerCard *card, PrintQueue *printQueue) {
    return func_ov177_021e7ae0(card, printQueue);
}

// pokebw2's WifiBattleMatchPartnerCard_SlideIn
BOOL func_ov177_021e777c(WifiBattleMatchPartnerCard *card) {
    BOOL done = FALSE;
    s32 offset;
    ClActorPos start;
    ClActorPos pos;
    int i;

    if (card->slideCount == 0) {
        if (card->badge != NULL) {
            func_0204b454(card->badge, &start, CLACT_VRAM_MAIN);
            card->badgeX = start.x;
        }
        for (i = 0; i < CARD_STAR_MAX; i++) {
            if (card->stars[i] != NULL) {
                func_0204b454(card->stars[i], &start, CLACT_VRAM_MAIN);
                card->starX[i] = start.x;
            }
        }
        GFL_SndSEPlay(SEQ_SE_SYS_75);
    }
    offset = (card->slideCount << 8) / SLIDE_FRAMES - 256;
    if (card->badge != NULL) {
        func_0204b454(card->badge, &pos, CLACT_VRAM_MAIN);
        pos.x = card->badgeX - (card->slideCount << 8) / SLIDE_FRAMES;
        func_0204b404(card->badge, &pos, CLACT_VRAM_MAIN);
    }
    for (i = 0; i < CARD_STAR_MAX; i++) {
        if (card->stars[i] != NULL) {
            func_0204b454(card->stars[i], &pos, CLACT_VRAM_MAIN);
            pos.x = card->starX[i] - (card->slideCount << 8) / SLIDE_FRAMES;
            func_0204b404(card->stars[i], &pos, CLACT_VRAM_MAIN);
        }
    }
    if (card->slideCount++ >= SLIDE_FRAMES) {
        offset = 0;
        card->slideCount = 0;
        done = TRUE;
    }
    GFL_BGSysMoveBGReq(2, BG_MOVE_SET_X, offset);
    GFL_BGSysMoveBGReq(1, BG_MOVE_SET_X, offset);
    return done;
}

void func_ov177_021e78b0(WifiBattleMatchPartnerCard *card, const LiveBattleMatchPlayerData *data, Font *font,
                         PrintQueue *printQueue, MsgData *msgData, WordSet *wordSet, u32 colorKind,
                         HeapID heapId) {
    StrBuf *format;
    StrBuf *text;
    int i;
    int j;
    u16 color;
    BOOL draw;
    BmpWin *window;

    for (i = 0; i < PARTNER_WINDOW_COUNT; i++) {
        card->windows[i] = BmpWin_CreateDynamic(1, data_ov177_021e8cee[i][0], data_ov177_021e8cee[i][1],
                                                data_ov177_021e8cee[i][2], data_ov177_021e8cee[i][3], 15, TRUE);
        GFL_BitmapFill(BmpWin_GetBitmap(card->windows[i]), 0);
        window = card->windows[i];
        BmpWin_FlushChar(window);
        BmpWin_FlushMap(window);
        GFL_BGSysLoadScr(BmpWin_GetBGIndex(window));
        if (i != PARTNER_WINDOW_PMS) {
            PrintWindow_Init(&card->printWindows[i], card->windows[i]);
        }
    }
    format = GFL_StrBufCreate(0x80, heapId);
    text = GFL_StrBufCreate(0x80, heapId);
    for (j = 0; j < PARTNER_WINDOW_COUNT; j++) {
        color = colorKind == 1 ? PRINT_COLOR(15, 2, 0) : PRINT_COLOR(1, 2, 0);
        draw = TRUE;
        switch (j) {
        case 0:
            GFL_StrBufLoadString(text, GetPlayerName((PlayerInfo *)&data->info));
            color = PRINT_COLOR(15, 2, 0);
            break;
        case 1:
            if (card->kind == 3) {
                GFL_MsgDataLoadStrbuf(msgData, 12, text);
                color = PRINT_COLOR(15, 2, 0);
            } else if (card->shown != 0) {
                GFL_MsgDataLoadStrbuf(msgData, 8, text);
                color = PRINT_COLOR(15, 2, 0);
            } else {
                draw = FALSE;
            }
            break;
        case 2:
            if (card->kind == 3) {
                GFL_MsgDataLoadStrbuf(msgData, 13, format);
                WordSetNumber(wordSet, 0, data->record1, 5, 1, TRUE);
                GFL_WordSetFormatStrbuf(wordSet, text, format);
                color = PRINT_COLOR(15, 2, 0);
            } else if (card->shown != 0) {
                GFL_MsgDataLoadStrbuf(msgData, 9, format);
                WordSetNumber(wordSet, 0, data->unk34, 4, 1, TRUE);
                GFL_WordSetFormatStrbuf(wordSet, text, format);
                color = PRINT_COLOR(15, 2, 0);
            } else {
                draw = FALSE;
            }
            break;
        case 3:
            GFL_MsgDataLoadStrbuf(msgData, 20, text);
            color = PRINT_COLOR(15, 2, 0);
            break;
        case 4:
            GFL_MsgDataLoadStrbuf(msgData, 22, format);
            loadCountryToStrbuf(wordSet, 0, UnityTowerVisitor_GetCountry((PlayerInfo *)&data->info));
            GFL_WordSetFormatStrbuf(wordSet, text, format);
            break;
        case 5: {
            u32 country;

            GFL_MsgDataLoadStrbuf(msgData, 23, format);
            country = UnityTowerVisitor_GetCountry((PlayerInfo *)&data->info);
            loadCountryAreaToStrbuf(wordSet, 0, country, UnityTowerVisitor_GetProvince((PlayerInfo *)&data->info));
            GFL_WordSetFormatStrbuf(wordSet, text, format);
            break;
        }
        case 6:
            GFL_MsgDataLoadStrbuf(msgData, 21, text);
            color = PRINT_COLOR(15, 2, 0);
            break;
        case 7:
            func_0202de64(card->pmsDraw, color);
            func_0202dc48(card->pmsDraw, card->windows[j], &data->sentence, 0);
            draw = FALSE;
            break;
        }
        if (draw) {
            PrintWindow_Print(&card->printWindows[j], printQueue, 0, 0, text, font, color);
        }
    }
    GFL_StrBufFree(format);
    GFL_StrBufFree(text);
}


// pokebw2's WifiBattleMatchView_PartnerFlush
static BOOL func_ov177_021e7ae0(WifiBattleMatchPartnerCard *card, PrintQueue *printQueue) {
    BOOL done = TRUE;
    int i;

    for (i = 0; i < PARTNER_WINDOW_COUNT; i++) {
        if (i != PARTNER_WINDOW_PMS) {
            PrintWindow_Flush(&card->printWindows[i], printQueue);
            done &= !card->printWindows[i].flushPending;
        } else {
            func_0202dbc4(card->pmsDraw);
            done &= func_0202dc98(card->pmsDraw);
        }
    }
    return done;
}

// pokebw2's WifiBattleMatchView_PartnerFreeWindows
static void func_ov177_021e7b44(WifiBattleMatchPartnerCard *card) {
    int i;

    for (i = 0; i < PARTNER_WINDOW_COUNT; i++) {
        if (card->windows[i] != NULL) {
            func_020456f8(card->windows[i]);
            BmpWin_Free(card->windows[i]);
        }
    }
}

static void func_ov177_021e7b68(WifiBattleMatchPartnerCard *card, u32 look, ClActUnit *unit,
                                                 HeapID heapId) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(32, heapId);
    ClActorSetup setup;

    card->badgePalette = func_0204ac18(arc, 0, CLACT_VRAM_MAIN, 320, data_ov177_021e8cbe[look], 1, heapId);
    card->badgeChars = func_0204a6c8(arc, look + 49, FALSE, CLACT_VRAM_MAIN, heapId);
    card->badgeCells = func_0204af28(arc, 65, 66, heapId);
    GFL_ArcToolFree(arc);
    MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
    setup.x = 288;
    setup.y = 44;
    card->badge = func_0204b294(unit, card->badgeChars, card->badgePalette, card->badgeCells, &setup, CLACT_VRAM_MAIN,
                                heapId);
    func_0204b6f4(card->badge, 0, 1);
}

// pokebw2's WifiBattleMatchView_PartnerFreeBadge
static void func_ov177_021e7c14(WifiBattleMatchPartnerCard *card) {
    func_0204b3b4(card->badge);
    func_0204a8d4(card->badgeChars);
    func_0204afd8(card->badgeCells);
    func_0204ada4(card->badgePalette);
}

static void func_ov177_021e7c3c(WifiBattleMatchPartnerCard *card,
                                                 const LiveBattleMatchPlayerData *data, ClActUnit *unit,
                                                 WifiBattleMatchRes *res, HeapID heapId) {
    u32 count = func_ov177_021e7cf0(data->record0, data->record1, data->record2);
    ClActorSetup setup;
    u32 i;

    MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
    setup.x = 256;
    setup.y = 16;
    for (i = 0; i < count; i++) {
        setup.x += 16;
        setup.sequence = i + 2;
        card->stars[i] = func_0204b294(unit, func_ov177_021e5978(res, RES_CHARS + CLACT_VRAM_MAIN),
                                       func_ov177_021e5978(res, RES_PALETTE + CLACT_VRAM_MAIN),
                                       func_ov177_021e5978(res, RES_CELLS + CLACT_VRAM_MAIN), &setup,
                                       CLACT_VRAM_MAIN, heapId);
    }
}

// pokebw2's WifiBattleMatchView_PartnerFreeStars
static void func_ov177_021e7cd0(WifiBattleMatchPartnerCard *card) {
    int i;

    for (i = 0; i < CARD_STAR_MAX; i++) {
        if (card->stars[i] != NULL) {
            func_0204b3b4(card->stars[i]);
            card->stars[i] = NULL;
        }
    }
}

// The number of stars of a rank, by the number of matches
// pokebw2's WifiBattleMatchView_GetStarCount
static u32 func_ov177_021e7cf0(u32 unk0, u32 count, u32 unk2) {
    if (count >= 1000) {
        return 5;
    }
    if (count >= 500) {
        return 4;
    }
    if (count >= 200) {
        return 3;
    }
    if (count >= 100) {
        return 2;
    }
    if (count >= 50) {
        return 1;
    }
    return 0;
}

// pokebw2's WifiBattleMatchLinkIcon_Create
WifiBattleMatchLinkIcon *func_ov177_021e7d20(ClActUnit *unit, WifiBattleMatchRes *res, HeapID heapId) {
    WifiBattleMatchLinkIcon *icon = GFL_HeapAllocate(heapId, sizeof(WifiBattleMatchLinkIcon), FALSE,
                                                     "wifibattlematch_view.c", 3055);
    ClActorSetup setup;

    MI_CpuFill8(icon, 0, sizeof(WifiBattleMatchLinkIcon));
    MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
    setup.x = 128;
    setup.y = 96;
    setup.sequence = 1;
    setup.priority = 1;
    setup.bgPriority = 1;
    icon->actors[0] = func_0204b294(unit, func_ov177_021e5978(res, RES_CHARS + CLACT_VRAM_MAIN),
                                    func_ov177_021e5978(res, RES_PALETTE + CLACT_VRAM_MAIN),
                                    func_ov177_021e5978(res, RES_CELLS + CLACT_VRAM_MAIN), &setup,
                                    CLACT_VRAM_MAIN, heapId);
    func_0204b92c(icon->actors[0], TRUE);
    setup.sequence = 0;
    setup.priority = 0;
    setup.bgPriority = 1;
    icon->actors[1] = func_0204b294(unit, func_ov177_021e5978(res, RES_CHARS + CLACT_VRAM_MAIN),
                                    func_ov177_021e5978(res, RES_PALETTE + CLACT_VRAM_MAIN),
                                    func_ov177_021e5978(res, RES_CELLS + CLACT_VRAM_MAIN), &setup,
                                    CLACT_VRAM_MAIN, heapId);
    func_0204b92c(icon->actors[1], TRUE);
    return icon;
}

// pokebw2's WifiBattleMatchLinkIcon_Free
void func_ov177_021e7df0(WifiBattleMatchLinkIcon *icon) {
    func_0204b3b4(icon->actors[1]);
    func_0204b3b4(icon->actors[0]);
    GFL_HeapFree(icon);
}

// pokebw2's WifiBattleMatchLinkIcon_Update
void func_ov177_021e7e08(WifiBattleMatchLinkIcon *icon) {
}

// pokebw2's WifiBattleMatchLinkIcon_SetShown
void func_ov177_021e7e0c(WifiBattleMatchLinkIcon *icon, BOOL shown) {
    if (shown == TRUE) {
        GFL_SndSEPlay(SEQ_SE_SYS_102);
    } else {
        GFL_SndPlayerStop(2);
    }
    func_0204b3dc(icon->actors[0], shown);
    func_0204b3dc(icon->actors[1], shown);
}

// pokebw2's WifiBattleMatchTeamList_Create
WifiBattleMatchTeamList *func_ov177_021e7e3c(BattleBoxSave *battleBox, ClActUnit *unit, WifiBattleMatchRes *res,
                                             Font *font, PrintQueue *printQueue, MsgData *msgData, HeapID heapId) {
    WifiBattleMatchTeamList *list = GFL_HeapAllocate(heapId, sizeof(WifiBattleMatchTeamList), FALSE,
                                                     "wifibattlematch_view.c", 3210);
    PokeParty *party;
    ArcTool *arc;
    ClActorSetup setup;
    ClActorSetup itemSetup;
    ClActorPos pos;
    u8 x;
    u8 level;
    WordSet *wordSet;
    StrBuf *text;
    StrBuf *levelFormat;
    StrBuf *maleText;
    StrBuf *femaleText;
    PartyPkm *pkm;
    int i;

    MI_CpuFill8(list, 0, sizeof(WifiBattleMatchTeamList));
    party = convertBoxedPokeSetToParty(battleBox, HEAPID_TAIL(heapId));
    loadBGScrToVramByNarcNoReserveNegAlign(ARCID_WIFIBATTLEMATCH, 7, 2, 0, 0, FALSE, HEAPID_TAIL(heapId));
    arc = GFL_ArcSysCreateFileHandle(ARCID_POKEICON, heapId);
    list->palette = func_0204acec(arc, func_0201bc3c(), CLACT_VRAM_MAIN, 160, HEAPID_TAIL(heapId));
    list->cells = func_0204af28(arc, func_0201bc40(), getOBJTileMapping_MainEng(), HEAPID_TAIL(heapId));
    for (i = 0; i < PokeParty_GetPkmCount(party); i++) {
        pkm = PokeParty_GetPkm(party, i);
        if (PokeParty_GetParam(pkm, PKM_PARAM_SPECIES_VALID, NULL)) {
            list->chars[i] = func_0204a6c8(arc, func_0201ba68(func_0201d624(pkm)), FALSE, CLACT_VRAM_MAIN,
                                           HEAPID_TAIL(heapId));
        }
    }
    GFL_ArcToolFree(arc);
    arc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), heapId);
    i = 0;
    list->itemPalette = func_0204ac18(arc, func_02027694(), CLACT_VRAM_MAIN, 256, i, 1, HEAPID_TAIL(heapId));
    list->itemCells = func_0204af28(arc, func_0202769c(2), func_020276a0(2), HEAPID_TAIL(heapId));
    list->itemChars = func_0204a6c8(arc, func_02027698(), FALSE, CLACT_VRAM_MAIN, HEAPID_TAIL(heapId));
    GFL_ArcToolFree(arc);
    MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
    setup.x = 24;
    setup.y = 44;
    setup.sequence = 1;
    setup.priority = 1;
    for (i = 0; i < PokeParty_GetPkmCount(party); i++) {
        pkm = PokeParty_GetPkm(party, i);
        if (PokeParty_GetParam(pkm, PKM_PARAM_SPECIES_VALID, NULL)) {
            setup.x = i * 40 + 24;
            list->actors[i] = func_0204b294(unit, list->chars[i], list->palette, list->cells, &setup,
                                            CLACT_VRAM_MAIN, heapId);
            func_0204b6f4(list->actors[i], func_0201bbe8(func_0201d624(pkm)), 0);
            if (PokeParty_GetParam(pkm, PKM_PARAM_ITEM, NULL)) {
                itemSetup = setup;
                itemSetup.x += 12;
                itemSetup.y += 8;
                itemSetup.priority = 0;
                itemSetup.sequence = 0;
                list->actors[TEAM_ACTOR_ITEM + i] = func_0204b294(unit, list->itemChars, list->itemPalette,
                                                                  list->itemCells, &itemSetup, CLACT_VRAM_MAIN, heapId);
            }
        }
    }
    wordSet = GFL_WordSetSystemCreateDefault(HEAPID_TAIL(heapId));
    text = GFL_StrBufCreate(128, HEAPID_TAIL(heapId));
    levelFormat = GFL_MsgDataLoadStrbufNew(msgData, 99);
    maleText = GFL_MsgDataLoadStrbufNew(msgData, 100);
    femaleText = GFL_MsgDataLoadStrbufNew(msgData, 101);
    for (i = 0; i < PokeParty_GetPkmCount(party); i++) {
        pkm = PokeParty_GetPkm(party, i);
        if (PokeParty_GetParam(pkm, PKM_PARAM_SPECIES_VALID, NULL)) {
            BmpWin *window;

            list->windows[i] = BmpWin_CreateDynamic(1, i * 5 + 1, 4, 5, 6, 15, TRUE);
            window = list->windows[i];
            BmpWin_FlushChar(window);
            BmpWin_FlushMap(window);
            GFL_BGSysLoadScr(BmpWin_GetBGIndex(window));
            PrintWindow_Init(&list->printWindows[i], list->windows[i]);
            if (PokeParty_GetParam(pkm, PKM_PARAM_SEX, NULL) == 0) {
                PrintWindow_Print(&list->printWindows[i], printQueue, 32, 0, maleText, font, PRINT_COLOR(5, 6, 0));
            } else if (PokeParty_GetParam(pkm, PKM_PARAM_SEX, NULL) == 1) {
                PrintWindow_Print(&list->printWindows[i], printQueue, 32, 0, femaleText, font,
                                  PRINT_COLOR(3, 4, 0));
            }
            level = PokeParty_GetParam(pkm, PKM_PARAM_LEVEL, NULL);
            WordSetNumber(wordSet, 0, level, 3, 1, TRUE);
            GFL_WordSetFormatStrbuf(wordSet, text, levelFormat);
            x = 20 - GFL_FontGetBlockWidth(text, font, 0) / 2;
            PrintWindow_Print(&list->printWindows[i], printQueue, x, 36, text, font, PRINT_COLOR(1, 2, 0));
        }
    }
    GFL_StrBufFree(femaleText);
    GFL_StrBufFree(maleText);
    GFL_StrBufFree(levelFormat);
    GFL_StrBufFree(text);
    GFL_WordSetSystemFree(wordSet);
    for (i = 0; i < TEAM_ACTOR_COUNT; i++) {
        if (list->actors[i] != NULL) {
            func_0204b454(list->actors[i], &pos, CLACT_VRAM_MAIN);
            pos.x += 256;
            func_0204b404(list->actors[i], &pos, CLACT_VRAM_MAIN);
        }
    }
    GFL_BGSysMoveBG(2, BG_MOVE_SET_X, -256);
    GFL_BGSysMoveBG(1, BG_MOVE_SET_X, -256);
    GFL_HeapFree(party);
    G2_SetBlendAlpha(GX_PLANEMASK_BG2, GX_PLANEMASK_BG3, 15, 3);
    return list;
}

// pokebw2's WifiBattleMatchTeamList_Free
void func_ov177_021e82bc(WifiBattleMatchTeamList *list) {
    int i;

    G2_BlendNone();
    for (i = 0; i < CARD_PARTY_COUNT; i++) {
        if (list->windows[i] != NULL) {
            func_020456f8(list->windows[i]);
            BmpWin_Free(list->windows[i]);
        }
    }
    for (i = 0; i < TEAM_ACTOR_COUNT; i++) {
        if (list->actors[i] != NULL) {
            func_0204b3b4(list->actors[i]);
        }
    }
    func_0204a8d4(list->itemChars);
    func_0204afd8(list->itemCells);
    func_0204ada4(list->itemPalette);
    for (i = 0; i < CARD_PARTY_COUNT; i++) {
        if (list->actors[i] != NULL) {
            func_0204a8d4(list->chars[i]);
        }
    }
    func_0204afd8(list->cells);
    func_0204ada4(list->palette);
    GFL_HeapFree(list);
}

// pokebw2's WifiBattleMatchTeamList_SlideIn
BOOL func_ov177_021e8338(WifiBattleMatchTeamList *list) {
    BOOL done;
    s32 offset;
    ClActorPos start;
    ClActorPos pos;
    int i;

    if (GFL_BGSysGetBGOffsetX(2) == 0) {
        return TRUE;
    }
    done = FALSE;
    if (list->slideCount == 0) {
        for (i = 0; i < TEAM_ACTOR_COUNT; i++) {
            if (list->actors[i] != NULL) {
                func_0204b454(list->actors[i], &start, CLACT_VRAM_MAIN);
                list->slideX[i] = start.x;
            }
        }
    }
    offset = (list->slideCount << 8) / SLIDE_FRAMES - 256;
    for (i = 0; i < TEAM_ACTOR_COUNT; i++) {
        if (list->actors[i] != NULL) {
            func_0204b454(list->actors[i], &pos, CLACT_VRAM_MAIN);
            pos.x = list->slideX[i] - (list->slideCount << 8) / SLIDE_FRAMES;
            func_0204b404(list->actors[i], &pos, CLACT_VRAM_MAIN);
        }
    }
    if (list->slideCount++ >= SLIDE_FRAMES) {
        offset = 0;
        list->slideCount = 0;
        done = TRUE;
    }
    GFL_BGSysMoveBGReq(2, BG_MOVE_SET_X, offset);
    GFL_BGSysMoveBGReq(1, BG_MOVE_SET_X, offset);
    return done;
}

// pokebw2's WifiBattleMatchTeamList_SlideOut
BOOL func_ov177_021e8420(WifiBattleMatchTeamList *list) {
    BOOL done = FALSE;
    s32 offset;
    ClActorPos start;
    ClActorPos pos;
    int i;

    if (list->slideCount == 0) {
        for (i = 0; i < TEAM_ACTOR_COUNT; i++) {
            if (list->actors[i] != NULL) {
                func_0204b454(list->actors[i], &start, CLACT_VRAM_MAIN);
                list->slideX[i] = start.x;
            }
        }
    }
    offset = (list->slideCount << 8) / SLIDE_FRAMES;
    for (i = 0; i < TEAM_ACTOR_COUNT; i++) {
        if (list->actors[i] != NULL) {
            func_0204b454(list->actors[i], &pos, CLACT_VRAM_MAIN);
            pos.x = list->slideX[i] - (list->slideCount << 8) / SLIDE_FRAMES;
            func_0204b404(list->actors[i], &pos, CLACT_VRAM_MAIN);
        }
    }
    if (list->slideCount++ >= SLIDE_FRAMES) {
        offset = 256;
        list->slideCount = 0;
        done = TRUE;
    }
    GFL_BGSysMoveBGReq(2, BG_MOVE_SET_X, offset);
    GFL_BGSysMoveBGReq(1, BG_MOVE_SET_X, offset);
    return done;
}

// pokebw2's WifiBattleMatchTeamList_Flush
BOOL func_ov177_021e84f8(WifiBattleMatchTeamList *list, PrintQueue *printQueue) {
    BOOL done = TRUE;
    int i;

    for (i = 0; i < CARD_PARTY_COUNT; i++) {
        if (list->windows[i] != NULL) {
            PrintWindow_Flush(&list->printWindows[i], printQueue);
            done &= !list->printWindows[i].flushPending;
        }
    }
    return done;
}
