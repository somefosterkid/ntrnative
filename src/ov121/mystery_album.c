// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov197/mystery_album.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/mystery/mystery_album.h"
#include "app/mystery/mystery_gift_data.h"
#include "app/mystery/mystery_util.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/touchpanel.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/memorymap.h"
#include "nitro/math.h"
#include "pml/item.h"
#include "pml/poke_graphic.h"
#include "pml/poke_party.h"
#include "save/mystery_gift.h"
#include "system/bmp_winframe.h"
#include "system/game_data.h"
#include "system/printsys.h"
#include "system/wordset.h"

// Mystery Gift's cards: the album of saved cards, where they are looked at, moved and thrown away, and the card shown
// on the top screen, with the Pokémon of a Pokémon gift coming out of it. Our names; swan has none for this overlay

#define CARD_SLOTS 12
#define CARDS_PER_PAGE 4
// The cursor's position below the cards, on the button that leaves
#define CURSOR_BACK 4

// A card of the album: a copy of the saved gift, and its date drawn into a bitmap
typedef struct {
    BOOL used;
    MysteryGift gift;
    GFLBitmap *dateBitmap;
    u8 unkD4[8];
} MysteryCard;

// Where a card of a page is drawn: its frame's tile, its date window's tile and its icon
typedef struct {
    u8 x;
    u8 y;
    u8 winX;
    u8 winY;
    s16 iconX;
    s16 iconY;
} MysteryCardPos;

// The colors a palette cycles between
typedef struct {
    u16 colors[16];
    u16 from[16];
    u16 to[16];
} MysteryPalFade;

typedef struct {
    s32 x;
    s32 y;
} MysteryAlbumPos;

// The card's shake when its Pokémon comes out: steps that each turn and move the Pokémon in some frames
typedef struct {
    u16 rotation;
    u16 fromRotation;
    u16 toRotation;
    s8 dir;
    u8 state;
    u16 step;
    u16 stepCount;
    u32 frame;
    u32 frames;
    MysteryAlbumPos from;
    MysteryAlbumPos to;
    MysteryAlbumPos pos;
} MysteryAlbumShake;

struct MysteryCardView {
    MysteryCardViewSetup setup;
    HeapID heapId;
    MysteryCard cards[CARD_SLOTS];
    // Set when the cursor or the page changed, to show the card under the cursor
    BOOL moved;
    MysteryAlbum *albums[CARD_SLOTS];
    // The card whose Pokémon was brought out by a touch
    MysteryAlbum *opened;
    MysteryCardRes *res;
    MysteryTextLine *pageLine;
    MysteryTextWin *textWin;
    MysteryMsgWin *msgWin;
    MysteryYesNo *yesNo;
    MysterySeq *seq;
    int page;
    int cursor;
    // Which half of the 512-pixel BG shows the page
    BOOL bgHalf;
    // Set when a card was moved or thrown away, to save the album when leaving
    BOOL changed;
    u32 scrollFrame;
    s8 scrollDir;
    // The card being moved
    u16 moveSlot;
    BOOL moving;
    // Whether the cursor went to the back button from the left column
    BOOL fromLeft;
    u32 fadeFrame;
    ClActor *cursorActor;
    ClActor *moveCursor;
    ClActor *arrows[2];
    u32 chars;
    u32 cellAnims;
    u32 palette;
    // The date windows of the cards, for both halves of the BG
    BmpWin *windows[CARDS_PER_PAGE * 2];
    ClActor *icons[CARDS_PER_PAGE * 2];
    u32 iconChars[CARDS_PER_PAGE * 2];
    u32 iconCellAnims[CARDS_PER_PAGE * 2];
    u32 iconPalettes[CARDS_PER_PAGE * 2];
    u32 pokeIconPalette;
    u16 palFrame;
    MysteryPalFade fades[2];
};

struct MysteryCardRes {
    MysteryTextWin *textWin;
    u32 palette;
    u32 chars;
    u32 cellAnims;
    u32 pokePalette;
    u32 pokeChars;
    u32 pokeCellAnims;
    ClActor *icon;
    ClActor *poke;
    MysteryPalFade fade;
    BOOL isMain;
    u32 pokePaletteNo;
    u32 bgPriority;
    u32 textBgPriority;
    u16 bg;
    u16 textBg;
    u32 vramType;
    u32 palType;
    MysteryCardResSetup setup;
};

struct MysteryAlbum {
    MysteryCard card;
    u16 state;
    u16 frame;
    BOOL isEgg;
    u16 species;
    u16 form;
    u32 voice;
    MysteryAlbumShake shake;
    HeapID heapId;
    MysteryCardRes *res;
    MysteryGift *gift;
    GameData *gameData;
    TCB *tcb;
    BOOL loaded;
    BOOL paletteOnly;
    void *charBuf;
    NNSG2dCharacterData *chars;
    void *palBuf;
    NNSG2dPaletteData *pal;
    MysteryTextWinCopy *textCopy;
};

void func_ov121_021db018(MysteryCardView *view, HeapID heapId);
static void func_ov121_021db3e8(MysteryCardView *view);
void func_ov121_021db478(MysteryCardView *view, BOOL shown, BOOL left, u32 page, HeapID heapId);
static void func_ov121_021db528(MysteryCardView *view, BOOL shown);
static void func_ov121_021db54c(MysteryCardView *view, BOOL shown, BOOL left, u32 page, u16 index, HeapID heapId);
static void func_ov121_021db938(MysteryCardView *view, BOOL shown, u16 index);
void func_ov121_021dba78(MysteryCardView *view, BOOL left);
BOOL func_ov121_021dba94(MysteryCardView *view);
void func_ov121_021dbc08(MysteryCardView *view, int slot);
void func_ov121_021dbcc8(MysteryCardView *view, u32 slot1, u32 slot2);
int func_ov121_021dbdb8(MysteryCardView *view);
u32 func_ov121_021dbdbc(MysteryCardView *view);
void func_ov121_021dbdf4(MysteryCard *card, const MysteryGift *gift, const MysteryCardViewSetup *setup,
                             HeapID heapId);
static void func_ov121_021dbee4(MysteryCard *card);
static BOOL func_ov121_021dbf04(MysteryCard *card);
GFLBitmap *func_ov121_021dbf08(MysteryCard *card);
ArcTool *func_ov121_021dbf10(MysteryCard *card, HeapID heapId);
static u32 func_ov121_021dbf40(MysteryCard *card);
static u32 func_ov121_021dbf70(MysteryCard *card);
static u32 func_ov121_021dbf9c(MysteryCard *card);
u32 func_ov121_021dbfe4(MysteryCard *card);
u32 func_ov121_021dc004(MysteryCard *card);
static u8 func_ov121_021dc024(MysteryCard *card);
static MysteryGift *func_ov121_021dc02c(MysteryCard *card);
static BOOL func_ov121_021dc030(MysteryCard *card);
u32 func_ov121_021dc03c(MysteryCard *card);
static u32 func_ov121_021dc064(MysteryCard *card);
void func_ov121_021dc08c(MysterySeq *seq, u32 *state, void *work);
void func_ov121_021dc4f0(MysterySeq *seq, u32 *state, void *work);
void func_ov121_021dc734(MysterySeq *seq, u32 *state, void *work);
void func_ov121_021dc9cc(MysterySeq *seq, u32 *state, void *work);
void func_ov121_021dcce4(MysterySeq *seq, u32 *state, void *work);
void func_ov121_021dcec4(MysterySeq *seq, u32 *state, void *work);
void func_ov121_021dcf54(MysteryPalFade *fade, ArcTool *arc, u32 fileId, u8 to, u8 from, HeapID heapId);
static void func_ov121_021dcfa4(MysteryPalFade *fade, u32 type, u32 palette, u16 angle);
static void func_ov121_021dd240(MysteryCardRes *res);
static void func_ov121_021ddc30(int eva, int evb);
static void func_ov121_021ddc44(int eva, int evb);
static void func_ov121_021ddc58(const MysteryCardResSetup *setup, MysteryCard *card, HeapID heapId);
static void func_ov121_021ddcf8(const MysteryCardResSetup *setup, MysteryCard *card, HeapID heapId);
static void func_ov121_021ddd64(MysteryAlbum *album, MysteryCardRes *res, HeapID heapId);
static void func_ov121_021ddf84(MysteryAlbum *album);
static void func_ov121_021de024(TCB *tcb, void *work);
static void func_ov121_021de0dc(MysteryCardRes *res);
static void func_ov121_021de128(MysteryAlbumShake *shake, u16 rotation, s32 x, s32 y);
static BOOL func_ov121_021de154(MysteryAlbumShake *shake, u16 *rotation, MysteryAlbumPos *pos);

// The BG palettes of the main and of the sub screen. No code loads from it: its reads have constant indices
// pokebw2's sBgPalTypes
static const u8 data_ov121_021df820[2] = { PALTYPE_MAIN_BG, PALTYPE_SUB_BG };

// The shake's steps
// pokebw2's sShakeFrames
static const s8 data_ov121_021df82d[11] = { 2, 3, 3, 2, 2, 1, 1, 1, 0, 0, 10 };
// pokebw2's sShakeDirs
static const s8 data_ov121_021df822[11] = { 1, -1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };

// The BG palettes of a card's frame by the gift's kind (Black has 4 kinds, Black 2 5), 5 palettes later for a gift
// not picked up yet
// pokebw2's sCardPalettes
static const u32 data_ov121_021df838[4] = { 0, 0, 1, 2 };
// The palettes of the card on the top screen by the gift's kind, 3 palettes later for a gift picked up
// pokebw2's sCardBgPalettes
static const u32 data_ov121_021df848[4] = { 0, 0, 1, 2 };

// pokebw2's sShakeRotations
static const u16 data_ov121_021df858[11] = { 0x71c, 0xf8e2, 0xffff, 0, 0, 0, 0, 0, 0, 0, 0 };

// pokebw2's sCardPositions
static const MysteryCardPos data_ov121_021df86e[CARDS_PER_PAGE] = {
#include "asset/mystery_album_data_ov121_021df86e.inc"
};

// The texts of the album: its title, and the back button
// pokebw2's sAlbumEmptyTexts
static const MysteryTextWinEntry data_ov121_021df890[2] = {
    { 1, 1, 30, 2, 0x93, NULL, 1, 0, 0, PRINT_COLOR(14, 15, 0) },
    { 23, 20, 7, 2, 0x95, NULL, 1, 0, 0, PRINT_COLOR(14, 15, 2) },
};

// The same with the hint of the buttons, when the album has cards
// pokebw2's sAlbumTexts
static const MysteryTextWinEntry data_ov121_021df8fc[3] = {
    { 1, 1, 30, 2, 0x93, NULL, 1, 0, 0, PRINT_COLOR(14, 15, 0) },
    { 1, 22, 11, 2, 0x94, NULL, 0, 0, 0, PRINT_COLOR(14, 15, 10) },
    { 23, 20, 7, 2, 0x95, NULL, 1, 0, 0, PRINT_COLOR(14, 15, 2) },
};

// pokebw2's sShakePositions
static const MysteryAlbumPos data_ov121_021df944[11] = {
#include "asset/mystery_album_data_ov121_021df944.inc"
};

// pokebw2's MysteryCardView_Delete

MysteryCardView *func_ov121_021dac18(const MysteryCardViewSetup *setup, HeapID heapId) {
    MysteryCardView *view;
    u32 count = 0;
    int i;
    MysteryGift gift;
    MysteryCardResSetup resSetup;
    const MysteryTextWinEntry *texts;
    u32 textCount;

    view = GFL_HeapAllocate(heapId, sizeof(MysteryCardView), FALSE, "mystery_album.c", 415);
    MI_CpuFill8(view, 0, sizeof(MysteryCardView));
    view->bgHalf = TRUE;
    view->setup = *setup;
    view->heapId = heapId;
    MIi_CpuClear32(-1, view->iconChars, sizeof(view->iconChars));
    MIi_CpuClear32(-1, view->iconPalettes, sizeof(view->iconPalettes));
    MIi_CpuClear32(-1, view->iconCellAnims, sizeof(view->iconCellAnims));
    for (i = 0; i < func_0200a348(view->setup.giftSave); i++) {
        if (func_0200a0e4(view->setup.giftSave, i)) {
            count++;
            func_0200a000(view->setup.giftSave, i, &gift);
            func_ov121_021dbdf4(&view->cards[i], &gift, &view->setup, heapId);
        }
    }
    view->seq = func_ov121_021da854(view, view->setup.mode == 0 ? func_ov121_021dc08c : func_ov121_021dcec4, heapId);
    func_ov121_021db018(view, heapId);
    func_ov121_021db478(view, TRUE, FALSE, view->page, heapId);
    if (count != 0) {
        texts = data_ov121_021df8fc;
        textCount = 3;
    } else {
        texts = data_ov121_021df890;
        textCount = 2;
    }
    view->textWin = func_ov121_021da1bc(FALSE, texts, textCount, 1, 3, view->setup.queue, view->setup.msgData, view->setup.font, heapId);
    MI_CpuFill8(&resSetup, 0, sizeof(MysteryCardResSetup));
    resSetup.bg = 6;
    resSetup.textBg = 4;
    resSetup.bgPalette = 14;
    resSetup.textPalette = 15;
    resSetup.iconPalette = 11;
    resSetup.pokePalette = 14;
    resSetup.unit = view->setup.unit;
    resSetup.giftSave = view->setup.giftSave;
    resSetup.msgData = view->setup.msgData;
    resSetup.font = view->setup.font;
    resSetup.queue = view->setup.queue;
    resSetup.wordSet = view->setup.wordSet;
    view->res = func_ov121_021dcfc8(&resSetup, heapId);
    if (func_ov121_021dbdbc(view) != 0) {
        for (i = 0; i < CARD_SLOTS; i++) {
            if (func_ov121_021dbf04(&view->cards[i])) {
                view->albums[i] = func_ov121_021dd24c(func_ov121_021dc02c(&view->cards[i]), view->res, setup->gameData, heapId);
            }
        }
        func_ov121_021de0ac(view->albums[0], FALSE);
        GFL_HeapStatusValidate(0x1d);
    }
    GFL_BGSysMoveBG(3, BG_MOVE_SET_Y, 0);
    GFL_BGSysMoveBG(7, BG_MOVE_SET_Y, 0);
    return view;
}

void func_ov121_021dae1c(MysteryCardView *view) {
    int i;

    for (i = 0; i < CARD_SLOTS; i++) {
        if (view->albums[i] != NULL) {
            func_ov121_021dd980(view->albums[i]);
            view->albums[i] = NULL;
        }
    }
    func_ov121_021dd1fc(view->res);
    if (view->msgWin != NULL) {
        func_ov121_021d9e04(view->msgWin);
        view->msgWin = NULL;
    }
    GFL_BGSysMoveBG(3, BG_MOVE_SET_X, 0);
    GFL_BGSysMoveBG(2, BG_MOVE_SET_X, 0);
    func_ov121_021da2a8(view->textWin);
    func_ov121_021da280(view->textWin);
    func_ov121_021db528(view, TRUE);
    func_ov121_021db528(view, FALSE);
    func_ov121_021db3e8(view);
    for (i = 0; i < func_0200a348(view->setup.giftSave); i++) {
        func_ov121_021dbee4(&view->cards[i]);
    }
    GFL_BGSysClearScr(4);
    GFL_BGSysClearScr(6);
    GFL_BGSysQueueScrLoad(4);
    GFL_BGSysQueueScrLoad(6);
    func_ov121_021da890(view->seq);
    GFL_HeapFree(view);
}

// The slot of the card under the cursor
static inline int MysteryCardView_GetSlot(const MysteryCardView *view) {
    return view->cursor + view->page * 4;
}

// pokebw2's MysteryCardView_Main
void func_ov121_021daef0(MysteryCardView *view) {
    BOOL shown;

    func_ov121_021da898(view->seq);
    func_ov121_021dd240(view->res);
    if (view->moved) {
        shown = FALSE;
        if (view->opened != NULL) {
            func_ov121_021ddf84(view->opened);
            view->opened = NULL;
        }
        if (view->cursor < CURSOR_BACK && func_ov121_021dbf04(&view->cards[MysteryCardView_GetSlot(view)])) {
            func_ov121_021de0ac(view->albums[MysteryCardView_GetSlot(view)], TRUE);
            shown = TRUE;
        }
        if (!shown) {
            func_ov121_021de0dc(view->res);
        }
        view->moved = FALSE;
    }
    if (view->palFrame + 0x400 >= 0x10000) {
        view->palFrame = view->palFrame + 0x400 - 0x10000;
    } else {
        view->palFrame += 0x400;
    }
    func_ov121_021dcfa4(&view->fades[0], 14, 0, view->palFrame);
    func_ov121_021dcfa4(&view->fades[1], 14, 2, view->palFrame);
}

// pokebw2's MysteryCardView_Draw
void func_ov121_021dafcc(MysteryCardView *view) {
    int i;

    func_ov121_021da2c8(view->textWin);
    if (view->pageLine != NULL) {
        func_ov121_021d9b34(view->pageLine);
    }
    for (i = 0; i < CARD_SLOTS; i++) {
        if (view->albums[i] != NULL) {
            if (view->albums[i] != NULL) {
                func_ov121_021dd9cc(view->albums[i]);
            }
        }
    }
}

// pokebw2's MysteryCardView_IsEnd
BOOL func_ov121_021db008(MysteryCardView *view) {
    return func_ov121_021da8ac(view->seq);
}

// Loads the album's BGs, its windows and its cursor and arrows
// pokebw2's MysteryCardView_LoadGraphics
void func_ov121_021db018(MysteryCardView *view, HeapID heapId) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_MYSTERY, heapId);
    int i;
    MysteryCardPos pos;
    ClActorSetup actorSetup;

    GFL_G2DIOLoadArcNCLRDefault(arc, 1, data_ov121_021df820[0], 0, 0x140, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 10, 3, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 17, 3, 0, 0, FALSE, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 10, 1, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 19, 1, 0, 0, FALSE, heapId);
    GFL_BGSysLoadScr(3);
    GFL_G2DIOLoadArcNCLRDefault(arc, 1, data_ov121_021df820[1], 0, 0x140, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 10, 7, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 18, 7, 0, 0, FALSE, heapId);
    GFL_ArcToolFree(arc);
    arc = GFL_ArcSysCreateFileHandle(ARCID_FONT, heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 5, data_ov121_021df820[0], 0x1e0, 0x20, heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 5, data_ov121_021df820[1], 0x1e0, 0x20, heapId);
    GFL_ArcToolFree(arc);
    GFL_BGSysFillChar(1, 0, 1, 0);
    GFL_BGSysFillChar(2, 0, 1, 0);
    LoadSysMsgBox(0, 1, 13, 0, heapId);
    arc = GFL_ArcSysCreateFileHandle(ARCID_POKEICON, heapId);
    view->pokeIconPalette = func_0204acec(arc, func_0201bc3c(), 0, 0x180, heapId);
    GFL_ArcToolFree(arc);
    for (i = 0; i < CARDS_PER_PAGE * 2; i++) {
        if (i >= CARDS_PER_PAGE) {
            pos = data_ov121_021df86e[i - CARDS_PER_PAGE];
            pos.x += 32;
            pos.winX += 32;
        } else {
            pos = data_ov121_021df86e[i];
        }
        view->windows[i] = BmpWin_CreateDynamic(2, pos.winX, pos.winY, 10, 2, 15, FALSE);
        BmpWin_FlushMap(view->windows[i]);
        GFL_BGSysLoadScr(2);
    }
    view->pageLine = func_ov121_021d99a4(FALSE, 1, 13, 22, 6, 2, 3, view->setup.queue, heapId);
    arc = GFL_ArcSysCreateFileHandle(ARCID_MYSTERY, heapId);
    view->palette = func_0204ac18(arc, 0, 0, 0, 0, 6, heapId);
    view->cellAnims = func_0204af28(arc, 32, 35, heapId);
    view->chars = func_0204a6c8(arc, 9, FALSE, 0, heapId);
    func_ov121_021dcf54(&view->fades[0], arc, 0, 0, 1, heapId);
    func_ov121_021dcf54(&view->fades[1], arc, 0, 2, 3, heapId);
    GFL_ArcToolFree(arc);
    MI_CpuFill8(&actorSetup, 0, sizeof(ClActorSetup));
    actorSetup.x = 0;
    actorSetup.y = 24;
    actorSetup.bgPriority = 1;
    actorSetup.priority = 1;
    view->cursorActor =
        func_0204b294(view->setup.unit, view->chars, view->palette, view->cellAnims, &actorSetup, 0, heapId);
    actorSetup.sequence = 2;
    view->moveCursor =
        func_0204b294(view->setup.unit, view->chars, view->palette, view->cellAnims, &actorSetup, 0, heapId);
    func_0204b3dc(view->moveCursor, FALSE);
    actorSetup.priority = 0;
    for (i = 0; i < 2; i++) {
        if (i == 0) {
            actorSetup.x = 16;
            actorSetup.y = 16;
            actorSetup.sequence = 9;
        } else {
            actorSetup.x = 240;
            actorSetup.y = 16;
            actorSetup.sequence = 9;
        }
        view->arrows[i] =
            func_0204b294(view->setup.unit, view->chars, view->palette, view->cellAnims, &actorSetup, 0, heapId);
        func_0204b92c(view->arrows[i], TRUE);
        func_0204b3dc(view->arrows[i], FALSE);
        if (i == 0) {
            func_0204b604(view->arrows[i], 1, TRUE);
        } else {
            func_0204b604(view->arrows[i], 1, FALSE);
        }
    }
    GFL_BGSysSetBGEnabled(2, TRUE);
}

// pokebw2's MysteryCardView_FreeGraphics
static void func_ov121_021db3e8(MysteryCardView *view) {
    int i;

    GFL_BGSysFreeFilledChar(2, 1, 0);
    GFL_BGSysFreeFilledChar(1, 1, 0);
    for (i = 0; i < 2; i++) {
        func_0204b3b4(view->arrows[i]);
    }
    func_0204b3b4(view->moveCursor);
    func_0204b3b4(view->cursorActor);
    func_0204a8d4(view->chars);
    func_0204afd8(view->cellAnims);
    func_0204ada4(view->palette);
    func_ov121_021d9a40(view->pageLine);
    view->pageLine = NULL;
    func_0204ada4(view->pokeIconPalette);
    for (i = 0; i < CARDS_PER_PAGE * 2; i++) {
        BmpWin_Free(view->windows[i]);
    }
}

// Draws the cards of a page and its number, on the half of the BG that shows or on the other when the page scrolls
// in from the left or the right
// pokebw2's MysteryCardView_DrawPage
void func_ov121_021db478(MysteryCardView *view, BOOL shown, BOOL left, u32 page, HeapID heapId) {
    int i;
    StrBuf *str;
    StrBuf *fmt;

    for (i = 0; i < CARDS_PER_PAGE; i++) {
        func_ov121_021db54c(view, shown, left, page, i, heapId);
    }
    str = GFL_StrBufCreate(128, heapId);
    fmt = GFL_MsgDataLoadStrbufNew(view->setup.msgData, 0x96);
    WordSetNumber(view->setup.wordSet, 0, page + 1, 1, 1, TRUE);
    WordSetNumber(view->setup.wordSet, 1, func_ov121_021dbdb8(view), 1, 1, TRUE);
    GFL_WordSetFormatStrbuf(view->setup.wordSet, str, fmt);
    func_ov121_021d9b28(view->pageLine, PRINT_COLOR(14, 15, 10));
    func_ov121_021d9b2c(view->pageLine, 0, 0, 1);
    func_ov121_021d9ac8(view->pageLine, str, view->setup.font);
    GFL_StrBufFree(str);
    GFL_StrBufFree(fmt);
    GFL_BGSysLoadScr(3);
}

// pokebw2's MysteryCardView_ClearPage
static void func_ov121_021db528(MysteryCardView *view, BOOL shown) {
    int i;

    for (i = 0; i < CARDS_PER_PAGE; i++) {
        func_ov121_021db938(view, shown, i);
    }
    GFL_BGSysLoadScr(3);
}

// Loads a card's icon (its palette, unless it's a Pokémon's, its cells and its characters) for a window's slot. The
// window is pokebw2's u32, which is the SDK's unsigned long: this tree's u32 (unsigned int) gives other code
static inline void MysteryCardView_LoadIcon(MysteryCardView *view, MysteryCard *card, unsigned long win,
                                            HeapID heapId) {
    ArcTool *arc = func_ov121_021dbf10(card, heapId);

    if (func_ov121_021dc024(card) != 1) {
        view->iconPalettes[win] =
            func_0204ac18(arc, func_ov121_021dbf40(card), 0, (win + 4) * 32, 0, 1, heapId);
    }
    view->iconCellAnims[win] =
        func_0204af28(arc, func_ov121_021dbfe4(card), func_ov121_021dc004(card), heapId);
    view->iconChars[win] = func_0204a6c8(arc, func_ov121_021dbf9c(card), FALSE, 0, heapId);
    GFL_ArcToolFree(arc);
}

// pokebw2's MysteryCardView_DrawCard
static void func_ov121_021db54c(MysteryCardView *view, BOOL shown, BOOL left, u32 page, u16 index, HeapID heapId) {
    MysteryCard *card = &view->cards[index + page * 4];
    unsigned long win = index;
    MysteryCardPos pos = data_ov121_021df86e[index];
    GFLBitmap *date;
    ClActorSetup actorSetup;
    s16 dx;
    s16 dy;
    u32 palette;
    ClActorPos cursorPos;

    if ((shown && !view->bgHalf) || (!shown && view->bgHalf)) {
        win += CARDS_PER_PAGE;
        pos.x += 32;
        pos.winX += 32;
    }
    {
        int i;
        int j;

        for (j = 0; j < 2; j++) {
            for (i = 0; i < 2; i++) {
                u8 num = index + page * 4;

                GFL_BGSysFillScrArea(3, num % 3 * 64 + 0x58 + num / 3 * 2 + j * 32 + i, pos.x + i - 2, pos.y + j, 1, 1,
                                     3);
            }
        }
    }
    if (!func_ov121_021dbf04(card)) {
        int i;
        int j;

        for (j = 0; j < 7; j++) {
            for (i = 0; i < 12; i++) {
                GFL_BGSysFillScrArea(3, 0x180 + j * 32 + i, pos.x + i, pos.y + j, 1, 1, 3);
            }
        }
        GFL_BitmapFill(BmpWin_GetBitmap(view->windows[win]), 0);
        BmpWin_FlushChar(view->windows[win]);
        return;
    }
    {
        int i;
        int j;

        for (j = 0; j < 7; j++) {
            for (i = 0; i < 12; i++) {
                GFL_BGSysFillScrArea(3, 0x40 + j * 32 + i, pos.x + i, pos.y + j, 1, 1, 5);
            }
        }
    }
    GFL_BGSysSetScrPaletteNo(3, pos.x, pos.y, 12, 7, func_ov121_021dc03c(card));
    date = func_ov121_021dbf08(card);
    GFL_BitmapCopy(date, BmpWin_GetBitmap(view->windows[win]));
    BmpWin_FlushChar(view->windows[win]);
    MysteryCardView_LoadIcon(view, card, win, heapId);
    dx = 0;
    dy = 0;
    MI_CpuFill8(&actorSetup, 0, sizeof(ClActorSetup));
    actorSetup.y = pos.iconY;
    actorSetup.bgPriority = 1;
    if (func_ov121_021dc024(card) == 2) {
        dx = 4;
        dy = 8;
    }
    if (shown) {
        actorSetup.x = pos.iconX;
    } else if (left) {
        actorSetup.x = pos.iconX - 256;
    } else if (!left) {
        actorSetup.x = pos.iconX + 256;
    } else {
        actorSetup.x = pos.iconX;
    }
    actorSetup.x += dx;
    actorSetup.y += dy;
    if (func_ov121_021dc024(card) == 1) {
        palette = view->pokeIconPalette;
    } else {
        palette = view->iconPalettes[win];
    }
    view->icons[win] = func_0204b294(view->setup.unit, view->iconChars[win], palette, view->iconCellAnims[win],
                                     &actorSetup, 0, view->heapId);
    func_0204b6f4(view->icons[win], (u8)func_ov121_021dbf70(card), 0);
    func_0204b3dc(view->icons[win], TRUE);
    if (view->moving && view->moveSlot == index + page * 4) {
        cursorPos.x = data_ov121_021df86e[index].x * 8 - 24;
        cursorPos.y = data_ov121_021df86e[index].y * 8 - 8;
        if (!shown) {
            if (left) {
                cursorPos.x -= 256;
            } else if (!left) {
                cursorPos.x += 256;
            }
        }
        func_0204b404(view->moveCursor, &cursorPos, 0);
    }
}

// pokebw2's MysteryCardView_ClearCard
static void func_ov121_021db938(MysteryCardView *view, BOOL shown, u16 index) {
    MysteryCardPos pos = data_ov121_021df86e[index];
    int i;
    int j;

    if ((shown && !view->bgHalf) || (!shown && view->bgHalf)) {
        index += CARDS_PER_PAGE;
        pos.x += 32;
        pos.winX += 32;
    }
    for (j = 0; j < 7; j++) {
        for (i = 0; i < 12; i++) {
            GFL_BGSysFillScrArea(3, 0x180 + j * 32 + i, pos.x + i, pos.y + j, 1, 1, 3);
        }
    }
    GFL_BitmapFill(BmpWin_GetBitmap(view->windows[index]), 0);
    BmpWin_FlushChar(view->windows[index]);
    if (view->icons[index] != NULL) {
        func_0204b3b4(view->icons[index]);
        view->icons[index] = NULL;
        if (view->iconPalettes[index] != -1) {
            func_0204ada4(view->iconPalettes[index]);
            view->iconPalettes[index] = -1;
        }
        if (view->iconChars[index] != -1) {
            func_0204a8d4(view->iconChars[index]);
            view->iconChars[index] = -1;
        }
        if (view->iconCellAnims[index] != -1) {
            func_0204afd8(view->iconCellAnims[index]);
            view->iconCellAnims[index] = -1;
        }
    }
}

// pokebw2's MysteryCardView_StartScroll
void func_ov121_021dba78(MysteryCardView *view, BOOL left) {
    view->scrollDir = left ? -1 : 1;
    view->scrollFrame = 0;
}

// Scrolls the page out and the next in, and returns TRUE when they are done
// pokebw2's MysteryCardView_Scroll
BOOL func_ov121_021dba94(MysteryCardView *view) {
    int speed;
    int i;
    ClActorPos pos;
    ClActorPos screenPos;

    if (view->scrollDir != 0) {
        speed = view->scrollDir * 256 / 16;
        for (i = 0; i < CARDS_PER_PAGE * 2; i++) {
            if (view->icons[i] != NULL) {
                func_0204b454(view->icons[i], &pos, 0);
                pos.x -= speed;
                func_0204b404(view->icons[i], &pos, 0);
                func_0204b454(view->icons[i], &screenPos, 0);
                if (screenPos.x >= -16 && screenPos.x <= 272 && screenPos.y >= -16 && screenPos.y <= 208) {
                    func_0204b3dc(view->icons[i], TRUE);
                } else {
                    func_0204b3dc(view->icons[i], FALSE);
                }
            }
        }
        if (view->moving) {
            func_0204b454(view->moveCursor, &pos, 0);
            pos.x -= speed;
            func_0204b404(view->moveCursor, &pos, 0);
            if (pos.x >= -96 && pos.x <= 352 && pos.y >= -96 && pos.y <= 288) {
                func_0204b3dc(view->moveCursor, TRUE);
            } else {
                func_0204b3dc(view->moveCursor, FALSE);
            }
        }
        GFL_BGSysMoveBG(3, BG_MOVE_RIGHT, speed);
        GFL_BGSysMoveBG(2, BG_MOVE_RIGHT, speed);
        if (view->scrollFrame++ >= 15) {
            view->bgHalf ^= TRUE;
            view->scrollDir = 0;
        }
        return FALSE;
    }
    return TRUE;
}

// Throws away the card in the slot, and moves the cards after it up
// pokebw2's MysteryCardView_ThrowAway
void func_ov121_021dbc08(MysteryCardView *view, int slot) {
    MysteryGift gift;
    u32 i;
    MysteryAlbum *album;

    func_0200a094(view->setup.giftSave, slot);
    for (i = 0; i < func_0200a348(view->setup.giftSave); i++) {
        func_ov121_021dbee4(&view->cards[i]);
    }
    for (i = 0; i < func_0200a348(view->setup.giftSave); i++) {
        if (func_0200a0e4(view->setup.giftSave, i)) {
            func_0200a000(view->setup.giftSave, i, &gift);
            func_ov121_021dbdf4(&view->cards[i], &gift, &view->setup, view->heapId);
        }
    }
    if (view->albums[slot] != NULL) {
        func_ov121_021dd980(view->albums[slot]);
        view->albums[slot] = NULL;
    }
    for (; slot < CARD_SLOTS - 1; slot++) {
        album = view->albums[slot];
        if (album == NULL) {
            view->albums[slot] = view->albums[slot + 1];
            view->albums[slot + 1] = album;
        }
    }
    view->changed = TRUE;
}

// Swaps the cards of two slots
// pokebw2's MysteryCardView_Swap
void func_ov121_021dbcc8(MysteryCardView *view, u32 slot1, u32 slot2) {
    MysteryGift gift;
    u32 i;

    func_0200a254(view->setup.giftSave, slot1, slot2);
    for (i = 0; i < func_0200a348(view->setup.giftSave); i++) {
        func_ov121_021dbee4(&view->cards[i]);
    }
    for (i = 0; i < func_0200a348(view->setup.giftSave); i++) {
        if (func_0200a0e4(view->setup.giftSave, i)) {
            func_0200a000(view->setup.giftSave, i, &gift);
            func_ov121_021dbdf4(&view->cards[i], &gift, &view->setup, view->heapId);
        }
    }
    func_ov121_021dd980(view->albums[slot1]);
    func_ov121_021dd980(view->albums[slot2]);
    view->albums[slot1] = func_ov121_021dd24c(func_ov121_021dc02c(&view->cards[slot1]), view->res,
                                                      view->setup.gameData, view->heapId);
    view->albums[slot2] = func_ov121_021dd24c(func_ov121_021dc02c(&view->cards[slot2]), view->res,
                                                      view->setup.gameData, view->heapId);
    view->changed = TRUE;
}

// The number of pages
// pokebw2's MysteryCardView_GetPageCount
int func_ov121_021dbdb8(MysteryCardView *view) {
    return CARD_SLOTS / CARDS_PER_PAGE;
}

// The number of cards
// pokebw2's MysteryCardView_GetCardCount
u32 func_ov121_021dbdbc(MysteryCardView *view) {
    u32 i = 0;
    u32 count = 0;

    for (i = 0; i < func_0200a348(view->setup.giftSave); i++) {
        if (func_ov121_021dbf04(&view->cards[i])) {
            count++;
        }
    }
    return count;
}

void func_ov121_021dbdf4(MysteryCard *card, const MysteryGift *gift, const MysteryCardViewSetup *setup,
                             HeapID heapId) {
    s32 date;
    StrBuf *str;
    StrBuf *fmt;
    s16 x;
    s16 y;

    MI_CpuFill8(card, 0, sizeof(MysteryCard));
    if (gift != NULL) {
        card->used = TRUE;
        card->gift = *gift;
        card->dateBitmap = GFL_BitmapCreate(10, 2, 32, heapId);
        date = gift->date;
        str = GFL_StrBufCreate(128, heapId);
        fmt = GFL_MsgDataLoadStrbufNew(setup->msgData, 0x97);
        WordSetNumber(setup->wordSet, 0, (u16)(date >> 16), 4, 1, TRUE);
        WordSetNumber(setup->wordSet, 1, (u8)(date >> 8), 2, 0, TRUE);
        WordSetNumber(setup->wordSet, 2, (u8)date, 2, 0, TRUE);
        GFL_WordSetFormatStrbuf(setup->wordSet, str, fmt);
        x = 40;
        x -= GFL_FontGetBlockWidth(str, setup->font, 0) / 2;
        y = 8;
        y -= GFL_FontGetBlockHeight(str, setup->font) / 2;
        GFL_TextRendererDrawToBitmap(card->dateBitmap, x, y, str, setup->font);
        GFL_StrBufFree(str);
        GFL_StrBufFree(fmt);
    }
}

// pokebw2's MysteryCard_Exit
static void func_ov121_021dbee4(MysteryCard *card) {
    if (card->used) {
        GFL_BitmapFree(card->dateBitmap);
        MI_CpuFill8(card, 0, sizeof(MysteryCard));
    }
}

// pokebw2's MysteryCard_IsUsed
static BOOL func_ov121_021dbf04(MysteryCard *card) {
    return card->used;
}

// pokebw2's MysteryCard_GetDateBitmap
GFLBitmap *func_ov121_021dbf08(MysteryCard *card) {
    return card->dateBitmap;
}

// The archive of the gift's icon
// pokebw2's MysteryCard_OpenIconArc
ArcTool *func_ov121_021dbf10(MysteryCard *card, HeapID heapId) {
    switch (func_ov121_021dc024(card)) {
    case 1:
        return GFL_ArcSysCreateFileHandle(ARCID_POKEICON, heapId);
    case 2:
        return GFL_ArcSysCreateFileHandle(ARCID_ITEMGRA, heapId);
    default:
        return GFL_ArcSysCreateFileHandle(ARCID_MYSTERY, heapId);
    }
}

// The palette file of the gift's icon
// pokebw2's MysteryCard_GetIconPaletteFile
static u32 func_ov121_021dbf40(MysteryCard *card) {
    switch (func_ov121_021dc024(card)) {
    case 1:
        return func_0201bc3c();
    case 2:
        return GetItemGraphicsDatID(func_ov121_021dc02c(card)->value, 2);
    default:
        return 3;
    }
}

// The palette of a Pokémon gift's icon
// pokebw2's MysteryCard_GetPokeIconPalette
static u32 func_ov121_021dbf70(MysteryCard *card) {
    if (func_ov121_021dc024(card) == 1) {
        MysteryGiftPokemon *poke = (MysteryGiftPokemon *)func_ov121_021dc02c(card);

        return func_0201bb5c(poke->species, poke->form, poke->gender, poke->isEgg);
    }
    return 0;
}

// The character file of the gift's icon
// pokebw2's MysteryCard_GetIconCharFile
static u32 func_ov121_021dbf9c(MysteryCard *card) {
    MysteryGiftPokemon *poke;

    switch (func_ov121_021dc024(card)) {
    case 1:
        poke = (MysteryGiftPokemon *)func_ov121_021dc02c(card);
        return PokeParty_GetIconIndex(poke->species, poke->form, poke->gender, poke->isEgg);
    case 2:
        return GetItemGraphicsDatID(func_ov121_021dc02c(card)->value, 1);
    default:
        return 12;
    }
}

// The cell file of the gift's icon
// pokebw2's MysteryCard_GetIconCellFile
u32 func_ov121_021dbfe4(MysteryCard *card) {
    switch (func_ov121_021dc024(card)) {
    case 1:
        return func_0201bc40();
    case 2:
        return 1;
    default:
        return 0x21;
    }
}

// The animation file of the gift's icon
// pokebw2's MysteryCard_GetIconAnimFile
u32 func_ov121_021dc004(MysteryCard *card) {
    switch (func_ov121_021dc024(card)) {
    case 1:
        return getOBJTileMapping_MainEng();
    case 2:
        return 0;
    default:
        return 0x24;
    }
}

// pokebw2's MysteryCard_GetKind
static u8 func_ov121_021dc024(MysteryCard *card) {
    return card->gift.kind;
}

// pokebw2's MysteryCard_GetGift
static MysteryGift *func_ov121_021dc02c(MysteryCard *card) {
    return &card->gift;
}

// pokebw2's MysteryCard_IsDelivered
static BOOL func_ov121_021dc030(MysteryCard *card) {
    return card->gift.delivered;
}

// pokebw2's MysteryCard_GetFramePalette
u32 func_ov121_021dc03c(MysteryCard *card) {
    u8 kind = func_ov121_021dc024(card);
    u32 offset = func_ov121_021dc030(card) ? 0 : 5;

    return offset + data_ov121_021df838[kind];
}

// pokebw2's MysteryCard_GetBgPalette
static u32 func_ov121_021dc064(MysteryCard *card) {
    u8 kind = func_ov121_021dc024(card);
    u32 offset = func_ov121_021dc030(card) ? 3 : 0;

    return offset + data_ov121_021df848[kind];
}

// Moves the cursor and turns the pages
// pokebw2's MysteryCardView_SeqMain
void func_ov121_021dc08c(MysterySeq *seq, u32 *state, void *work) {
    MysteryCardView *view = work;
    u32 typed;
    int dir;
    BOOL moved;
    int pages;
    int page;
    BOOL left;
    ClActorPos pos;

    switch (*state) {
    case 0:
        func_0204b3dc(view->arrows[0], TRUE);
        func_0204b3dc(view->arrows[1], TRUE);
        func_0204b3dc(view->cursorActor, TRUE);
        typed = GCTX_HIDGetTypedKeys();
        moved = FALSE;
        dir = 0;
        if (func_02035c60()) {
            u16 slot = view->cursor + view->page * 4;

            view->opened = view->albums[slot];
            if (view->opened != NULL) {
                func_ov121_021ddf5c(view->opened);
            }
        }
        if (typed == PAD_BUTTON_L) {
            dir = -1;
        } else if (typed == PAD_BUTTON_R) {
            dir = 1;
        } else if (typed & PAD_KEY_UP) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            switch (view->cursor) {
            case 0:
                if (view->moving) {
                    view->cursor = 2;
                } else {
                    view->fromLeft = TRUE;
                    view->cursor = CURSOR_BACK;
                }
                break;
            case 1:
                if (view->moving) {
                    view->cursor = 3;
                } else {
                    view->fromLeft = FALSE;
                    view->cursor = CURSOR_BACK;
                }
                break;
            case 2:
                view->cursor = 0;
                break;
            case 3:
                view->cursor = 1;
                break;
            case CURSOR_BACK:
                if (view->fromLeft) {
                    view->cursor = 2;
                } else {
                    view->cursor = 3;
                }
                break;
            }
            moved = TRUE;
        } else if (typed & PAD_KEY_DOWN) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            switch (view->cursor) {
            case 0:
                view->cursor = 2;
                break;
            case 1:
                view->cursor = 3;
                break;
            case 2:
                if (view->moving) {
                    view->cursor = 0;
                } else {
                    view->fromLeft = TRUE;
                    view->cursor = CURSOR_BACK;
                }
                break;
            case 3:
                if (view->moving) {
                    view->cursor = 1;
                } else {
                    view->fromLeft = FALSE;
                    view->cursor = CURSOR_BACK;
                }
                break;
            case CURSOR_BACK:
                if (view->fromLeft) {
                    view->cursor = 0;
                } else {
                    view->cursor = 1;
                }
                break;
            }
            moved = TRUE;
        } else if (typed & PAD_KEY_LEFT) {
            func_ov121_021dbdb8(view);
            if (view->cursor == 0 || view->cursor == 2) {
                dir = -1;
            } else if (view->cursor != CURSOR_BACK) {
                GFL_SndSEPlay(SEQ_SE_SELECT1);
            }
            if (view->cursor != CURSOR_BACK) {
                if (view->cursor == 0 || view->cursor == 1) {
                    view->cursor = view->cursor - 1;
                    view->cursor = view->cursor > 1 ? 0 : (view->cursor < 0 ? 1 : view->cursor);
                } else {
                    view->cursor = view->cursor - 1;
                    view->cursor = view->cursor > 3 ? 2 : (view->cursor < 2 ? 3 : view->cursor);
                }
                moved = TRUE;
            }
        } else if (typed & PAD_KEY_RIGHT) {
            func_ov121_021dbdb8(view);
            if (view->cursor == 1 || view->cursor == 3) {
                dir = 1;
            } else if (view->cursor != CURSOR_BACK) {
                GFL_SndSEPlay(SEQ_SE_SELECT1);
            }
            if (view->cursor != CURSOR_BACK) {
                if (view->cursor == 0 || view->cursor == 1) {
                    view->cursor = view->cursor + 1;
                    view->cursor = view->cursor > 1 ? 0 : (view->cursor < 0 ? 1 : view->cursor);
                } else {
                    view->cursor = view->cursor + 1;
                    view->cursor = view->cursor > 3 ? 2 : (view->cursor < 2 ? 3 : view->cursor);
                }
                moved = TRUE;
            }
        } else if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
            if (view->cursor == CURSOR_BACK) {
                if (!view->moving) {
                    func_0204b3dc(view->arrows[0], FALSE);
                    func_0204b3dc(view->arrows[1], FALSE);
                    GFL_SndSEPlay(SEQ_SE_DECIDE1);
                    func_ov121_021da8b0(seq, func_ov121_021dcce4);
                }
            } else if (func_ov121_021dbf04(&view->cards[MysteryCardView_GetSlot(view)])) {
                func_0204b3dc(view->arrows[0], FALSE);
                func_0204b3dc(view->arrows[1], FALSE);
                GFL_SndSEPlay(SEQ_SE_DECIDE1);
                if (view->moving) {
                    func_ov121_021da8b0(seq, func_ov121_021dc9cc);
                } else {
                    func_ov121_021da8b0(seq, func_ov121_021dc4f0);
                }
            }
        } else if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            if (view->moving) {
                if (view->msgWin != NULL) {
                    func_ov121_021d9e04(view->msgWin);
                    view->msgWin = NULL;
                }
                func_0204b3dc(view->moveCursor, FALSE);
                view->moving = FALSE;
            } else {
                func_0204b3dc(view->arrows[0], FALSE);
                func_0204b3dc(view->arrows[1], FALSE);
                func_ov121_021da8b0(seq, func_ov121_021dcce4);
            }
        }
        if (dir != 0) {
            pages = func_ov121_021dbdb8(view);
            if (dir < 0) {
                if (view->page != 0) {
                    page = view->page - 1;
                } else {
                    page = pages - 1;
                }
                left = TRUE;
            } else if (dir > 0) {
                if (view->page < pages - 1) {
                    page = view->page + 1;
                } else {
                    page = 0;
                }
                left = FALSE;
            }
            GFL_SndSEPlay(SEQ_SE_SYS_03);
            func_ov121_021dba78(view, left);
            func_ov121_021db528(view, FALSE);
            func_ov121_021db478(view, FALSE, left, page, view->heapId);
            view->page = page;
            func_0204b3dc(view->cursorActor, FALSE);
            func_0204b3dc(view->arrows[0], FALSE);
            func_0204b3dc(view->arrows[1], FALSE);
            view->palFrame = 0;
            view->moved = TRUE;
            *state = 1;
        }
        if (moved) {
            if (view->cursor < CURSOR_BACK) {
                pos.x = data_ov121_021df86e[view->cursor].x * 8 - 24;
                pos.y = data_ov121_021df86e[view->cursor].y * 8 - 8;
                func_0204b878(view->cursorActor, 0);
            } else {
                pos.x = 176;
                pos.y = 152;
                func_0204b878(view->cursorActor, 11);
            }
            func_0204b404(view->cursorActor, &pos, 0);
            view->palFrame = 0;
            view->moved = TRUE;
        }
        break;
    case 1:
        if (func_ov121_021dba94(view)) {
            *state = 0;
        }
        break;
    }
}

void func_ov121_021dc4f0(MysterySeq *seq, u32 *state, void *work) {
    MysteryCardView *view = work;
    MysteryYesNoSetup setup;
    ClActorPos pos;
    u32 result;

    switch (*state) {
    case 0:
        if (view->msgWin == NULL) {
            view->msgWin = func_ov121_021d9ca4(0, 15, view->setup.queue, view->setup.font, 0x1d);
            func_ov121_021d9ffc(view->msgWin, 1, 13);
        }
        func_ov121_021d9f08(view->msgWin, view->setup.msgData, 0x74, MYSTERY_PRINT_QUEUE);
        *state = 1;
        break;
    case 1:
        MI_CpuFill8(&setup, 0, sizeof(MysteryYesNoSetup));
        setup.msgData = view->setup.msgData;
        setup.font = view->setup.font;
        setup.queue = view->setup.queue;
        setup.msgIds[0] = 0x75;
        setup.msgIds[1] = 0x76;
        setup.msgIds[2] = 0x77;
        setup.count = 3;
        setup.bg = 0;
        setup.palette = 15;
        setup.framePalette = 13;
        setup.frameChar = 1;
        view->yesNo = func_ov121_021da038(&setup, view->heapId);
        *state = 2;
        break;
    case 2:
        result = func_ov121_021da178(view->yesNo);
        view->palFrame = 0x7fff;
        if (result == MYSTERY_MENU_NONE) {
            break;
        }
        if (result == 0) {
            view->moving = TRUE;
            pos.x = data_ov121_021df86e[view->cursor].x * 8 - 24;
            pos.y = data_ov121_021df86e[view->cursor].y * 8 - 8;
            func_0204b404(view->moveCursor, &pos, 0);
            func_0204b3dc(view->moveCursor, TRUE);
            view->moveSlot = view->cursor + view->page * 4;
            if (view->yesNo != NULL) {
                func_ov121_021da148(view->yesNo);
                view->yesNo = NULL;
            }
            if (view->msgWin != NULL) {
                func_ov121_021d9e04(view->msgWin);
                view->msgWin = NULL;
            }
            if (view->msgWin == NULL) {
                view->msgWin = func_ov121_021d9d4c(0, 15, view->setup.queue, view->setup.font, 0x1d);
                func_ov121_021d9ffc(view->msgWin, 1, 13);
            }
            func_ov121_021d9f08(view->msgWin, view->setup.msgData, 0x7a, 0);
            *state = 6;
        } else if (result == 1) {
            if (func_ov121_021dc030(&view->cards[view->cursor + view->page * 4])) {
                *state = 7;
            } else {
                *state = 3;
            }
        } else if (result == 2) {
            *state = 5;
        }
        break;
    case 3:
        if (view->yesNo != NULL) {
            func_ov121_021da148(view->yesNo);
            view->yesNo = NULL;
        }
        func_ov121_021d9f08(view->msgWin, view->setup.msgData, 0x78, MYSTERY_PRINT_STREAM);
        *state = 4;
        break;
    case 4:
        if (func_ov121_021d9ff8(view->msgWin)) {
            *state = 5;
        }
        break;
    case 5:
        if (view->msgWin != NULL) {
            func_ov121_021d9e04(view->msgWin);
            view->msgWin = NULL;
        }
        if (view->yesNo != NULL) {
            func_ov121_021da148(view->yesNo);
            view->yesNo = NULL;
        }
        func_ov121_021da8b0(seq, func_ov121_021dc08c);
        break;
    case 6:
        func_ov121_021da8b0(seq, func_ov121_021dc08c);
        break;
    case 7:
        func_ov121_021da148(view->yesNo);
        view->yesNo = NULL;
        func_ov121_021da8b0(seq, func_ov121_021dc734);
        break;
    }
    if (view->msgWin != NULL) {
        func_ov121_021d9e50(view->msgWin);
    }
}


// Throws the card under the cursor away
// pokebw2's MysteryCardView_SeqThrowAway
void func_ov121_021dc734(MysterySeq *seq, u32 *state, void *work) {
    MysteryCardView *view = work;
    u32 slot = view->cursor + view->page * 4;
    MysteryYesNoSetup setup;
    u32 result;

    switch (*state) {
    case 0:
        func_ov121_021d9f08(view->msgWin, view->setup.msgData, 0x7b, MYSTERY_PRINT_STREAM);
        *state = 1;
        break;
    case 1:
        if (func_ov121_021d9ff8(view->msgWin)) {
            MI_CpuFill8(&setup, 0, sizeof(MysteryYesNoSetup));
            setup.msgData = view->setup.msgData;
            setup.font = view->setup.font;
            setup.queue = view->setup.queue;
            setup.msgIds[0] = 0x7c;
            setup.msgIds[1] = 0x7d;
            setup.count = 2;
            setup.bg = 0;
            setup.palette = 15;
            setup.framePalette = 13;
            setup.frameChar = 1;
            view->yesNo = func_ov121_021da038(&setup, view->heapId);
            *state = 2;
        }
        break;
    case 2:
        result = func_ov121_021da178(view->yesNo);
        view->palFrame = 0x7fff;
        if (result == MYSTERY_MENU_NONE) {
            break;
        }
        func_ov121_021da148(view->yesNo);
        view->yesNo = NULL;
        if (result == 0) {
            *state = 3;
        } else {
            if (view->msgWin != NULL) {
                func_ov121_021d9e04(view->msgWin);
                view->msgWin = NULL;
            }
            func_ov121_021da8b0(seq, func_ov121_021dc08c);
        }
        break;
    case 3: {
        u8 pos;
        int x;
        int y;

        GX_SetVisibleWnd(GX_WNDMASK_W0);
        pos = slot % 4;
        x = data_ov121_021df86e[pos].x;
        y = data_ov121_021df86e[pos].y;
        G2_SetWnd0InsidePlane(0x1e, TRUE);
        G2_SetWnd0Position(x * 8, y * 8, (x + 12) * 8, (y + 7) * 8);
        G2_SetWndOutsidePlane(0x1e, FALSE);
        *state = 4;
        break;
    }
    case 4:
        G2_SetBlendBrightness(0x1e, view->fadeFrame * 16 / 30);
        if (view->fadeFrame++ > 30) {
            view->fadeFrame = 0;
            *state = 5;
        }
        break;
    case 5:
        func_ov121_021dbc08(view, slot);
        func_ov121_021db528(view, TRUE);
        func_ov121_021db478(view, TRUE, FALSE, view->page, view->heapId);
        view->moved = TRUE;
        *state = 6;
        break;
    case 6:
        G2_SetBlendBrightness(0x1e, 16 - view->fadeFrame * 16 / 30);
        if (view->fadeFrame++ > 30) {
            view->fadeFrame = 0;
            *state = 7;
        }
        break;
    case 7:
        if (!func_0200a170(view->setup.giftSave)) {
            func_ov121_021da2f0(view->textWin, 1);
        }
        GX_SetVisibleWnd(GX_WNDMASK_NONE);
        *state = 8;
        break;
    case 8:
        func_ov121_021d9f08(view->msgWin, view->setup.msgData, 0x79, MYSTERY_PRINT_STREAM);
        *state = 9;
        break;
    case 9:
        if (func_ov121_021d9ff8(view->msgWin)) {
            *state = 10;
        }
        break;
    case 10:
        if (view->msgWin != NULL) {
            func_ov121_021d9e04(view->msgWin);
            view->msgWin = NULL;
        }
        func_ov121_021da8b0(seq, func_ov121_021dc08c);
        break;
    }
    if (view->msgWin != NULL) {
        func_ov121_021d9e50(view->msgWin);
    }
}

// Puts the card being moved where the cursor is, swapping it with the card there
// pokebw2's MysteryCardView_SeqMove
void func_ov121_021dc9cc(MysterySeq *seq, u32 *state, void *work) {
    MysteryCardView *view = work;
    u32 slot = MysteryCardView_GetSlot(view);

    switch (*state) {
    case 0:
        if (view->msgWin != NULL) {
            func_ov121_021d9e04(view->msgWin);
            view->msgWin = NULL;
        }
        view->moving = FALSE;
        if (slot == view->moveSlot) {
            func_0204b3dc(view->moveCursor, FALSE);
            func_ov121_021da8b0(seq, func_ov121_021dc08c);
        } else {
            *state = 1;
        }
        break;
    case 1: {
        u8 pos1 = slot % 4;
        u8 pos2 = view->moveSlot % 4;
        BOOL shown1 = view->page == slot / 4;
        BOOL shown2 = view->page == view->moveSlot / 4;
        int windows = GX_WNDMASK_NONE;

        if (shown1) {
            windows |= GX_WNDMASK_W0;
        }
        if (shown2) {
            windows |= GX_WNDMASK_W1;
        }
        GX_SetVisibleWnd(windows);
        if (shown1) {
            int x = data_ov121_021df86e[pos1].x;
            int y = data_ov121_021df86e[pos1].y;

            G2_SetWnd0InsidePlane(0x1e, TRUE);
            G2_SetWnd0Position(x * 8, y * 8, (x + 12) * 8, (y + 7) * 8);
        }
        if (shown2) {
            int x = data_ov121_021df86e[pos2].x;
            int y = data_ov121_021df86e[pos2].y;

            G2_SetWnd1InsidePlane(0x1e, TRUE);
            G2_SetWnd1Position(x * 8, y * 8, (x + 12) * 8, (y + 7) * 8);
        }
        G2_SetWndOutsidePlane(0x1e, FALSE);
        *state = 2;
        break;
    }
    case 2:
        G2_SetBlendBrightness(0x1e, view->fadeFrame * 16 / 30);
        if (view->fadeFrame++ > 30) {
            view->fadeFrame = 0;
            *state = 3;
        }
        break;
    case 3: {
        int pos2;
        u32 page2;
        BOOL shown2;

        func_ov121_021dbcc8(view, slot, view->moveSlot);
        GFL_SndSEPlay(SEQ_SE_SYS_49);
        page2 = view->moveSlot / 4;
        pos2 = view->moveSlot % 4;
        shown2 = view->page == page2;
        if (view->page == slot / 4) {
            func_ov121_021db938(view, TRUE, view->cursor);
            func_ov121_021db54c(view, TRUE, FALSE, view->page, view->cursor, view->heapId);
        }
        if (shown2) {
            func_ov121_021db938(view, TRUE, pos2);
            func_ov121_021db54c(view, TRUE, FALSE, page2, pos2, view->heapId);
        }
        GFL_BGSysLoadScr(3);
        view->moved = TRUE;
        *state = 4;
        break;
    }
    case 4:
        G2_SetBlendBrightness(0x1e, 16 - view->fadeFrame * 16 / 30);
        if (view->fadeFrame++ > 30) {
            view->fadeFrame = 0;
            *state = 5;
        }
        break;
    case 5:
        GX_SetVisibleWnd(GX_WNDMASK_NONE);
        *state = 6;
        break;
    case 6:
        view->moving = FALSE;
        func_0204b3dc(view->moveCursor, FALSE);
        if (view->msgWin != NULL) {
            func_ov121_021d9e04(view->msgWin);
            view->msgWin = NULL;
        }
        func_ov121_021da8b0(seq, func_ov121_021dc08c);
        break;
    }
}

void func_ov121_021dcce4(MysterySeq *seq, u32 *state, void *work) {
    MysteryCardView *view = work;
    MysteryYesNoSetup setup;
    u32 result;
    BOOL done;

    switch (*state) {
    case 0:
        *state = 1;
        break;
    case 1:
        if (view->setup.mode == 0) {
            *state = 5;
        } else if (func_0200a0c8(view->setup.giftSave)) {
            *state = 5;
        } else {
            *state = 2;
        }
        break;
    case 2:
        if (view->msgWin == NULL) {
            view->msgWin = func_ov121_021d9ca4(0, 15, view->setup.queue, view->setup.font, 0x1d);
            func_ov121_021d9ffc(view->msgWin, 1, 13);
        }
        func_ov121_021d9f08(view->msgWin, view->setup.msgData, 0x5d, 1);
        *state = 3;
        break;
    case 3:
        if (func_ov121_021d9ff8(view->msgWin)) {
            MI_CpuFill8(&setup, 0, sizeof(MysteryYesNoSetup));
            setup.msgData = view->setup.msgData;
            setup.font = view->setup.font;
            setup.queue = view->setup.queue;
            setup.msgIds[0] = 0x7c;
            setup.msgIds[1] = 0x7d;
            setup.count = 2;
            setup.bg = 0;
            setup.palette = 15;
            setup.framePalette = 13;
            setup.frameChar = 1;
            view->yesNo = func_ov121_021da038(&setup, view->heapId);
            *state = 4;
        }
        break;
    case 4:
        result = func_ov121_021da178(view->yesNo);
        view->palFrame = 0x7fff;
        if (result != MYSTERY_MENU_NONE) {
            func_ov121_021da148(view->yesNo);
            view->yesNo = NULL;
            if (result == 0) {
                *state = 5;
            } else {
                if (view->msgWin != NULL) {
                    func_ov121_021d9e04(view->msgWin);
                    view->msgWin = NULL;
                }
                func_ov121_021da8b0(seq, func_ov121_021dc08c);
            }
        }
        break;
    case 5:
        if (view->changed) {
            *state = 6;
        } else {
            *state = 10;
        }
        break;
    case 6:
        if (view->msgWin == NULL) {
            view->msgWin = func_ov121_021d9ca4(0, 15, view->setup.queue, view->setup.font, 0x1d);
            func_ov121_021d9ffc(view->msgWin, 1, 13);
        }
        func_ov121_021d9f08(view->msgWin, view->setup.msgData, 0x73, 2);
        *state = 7;
        break;
    case 7:
        if (func_ov121_021d9ff8(view->msgWin)) {
            *state = 8;
        }
        break;
    case 8:
        done = TRUE;
        if (view->opened != NULL && !func_ov121_021de014(view->opened)) {
            done = FALSE;
        }
        if (done) {
            func_0200a2b8(view->setup.giftSave, view->setup.gameData);
            *state = 9;
        }
        break;
    case 9:
        if (func_0200a2d8(view->setup.giftSave, view->setup.gameData) == 2) {
            *state = 10;
        }
        break;
    case 10:
        if (view->msgWin != NULL) {
            func_ov121_021d9e04(view->msgWin);
            view->msgWin = NULL;
        }
        func_ov121_021da8b8(seq);
        break;
    }
    if (view->msgWin != NULL) {
        func_ov121_021d9e50(view->msgWin);
    }
}


void func_ov121_021dcec4(MysterySeq *seq, u32 *state, void *work) {
    MysteryCardView *view = work;

    switch (*state) {
    case 0:
        if (view->msgWin == NULL) {
            view->msgWin = func_ov121_021d9ca4(0, 15, view->setup.queue, view->setup.font, 0x1d);
        }
        func_ov121_021d9ffc(view->msgWin, 1, 13);
        func_ov121_021d9f08(view->msgWin, view->setup.msgData, 0x5c, 1);
        *state = 1;
        break;
    case 1:
        if (func_ov121_021d9ff8(view->msgWin)) {
            *state = 2;
        }
        break;
    case 2:
        if (view->msgWin != NULL) {
            func_ov121_021d9e04(view->msgWin);
            view->msgWin = NULL;
        }
        func_ov121_021da8b0(seq, func_ov121_021dc08c);
        break;
    }
    if (view->msgWin != NULL) {
        func_ov121_021d9e50(view->msgWin);
    }
}


// pokebw2's MysteryPalFade_Init
void func_ov121_021dcf54(MysteryPalFade *fade, ArcTool *arc, u32 fileId, u8 to, u8 from, HeapID heapId) {
    NNSG2dPaletteData *pal;
    void *buf;
    u8 *data;

    MI_CpuFill8(fade, 0, sizeof(MysteryPalFade));
    buf = GFL_G2DIOReadNCLRArc(arc, fileId, &pal, heapId);
    data = pal->pRawData;
    MI_CpuCopy8(data + to * 32, fade->to, sizeof(fade->to));
    MI_CpuCopy8(data + from * 32, fade->from, sizeof(fade->from));
    GFL_HeapFree(buf);
}

// pokebw2's MysteryPalFade_Update
static void func_ov121_021dcfa4(MysteryPalFade *fade, u32 type, u32 palette, u16 angle) {
    func_ov121_021dab50(type, fade->colors, angle, palette, fade->from, fade->to);
}

// pokebw2's MysteryCardRes_Create
MysteryCardRes *func_ov121_021dcfc8(const MysteryCardResSetup *setup, HeapID heapId) {
    MysteryCardRes *res = GFL_HeapAllocate(heapId, sizeof(MysteryCardRes), FALSE, "mystery_album.c", 3327);
    ArcTool *arc;
    ArcTool *pokeArc;
    ClActorSetup iconSetup;
    ClActorSetup pokeSetup;

    res->pokePaletteNo = setup->pokePalette;
    res->bg = setup->bg;
    res->textBg = setup->textBg;
    res->setup = *setup;
    if (setup->bg < 4) {
        res->palType = PALTYPE_MAIN_BG;
        res->vramType = 0;
        res->isMain = TRUE;
    } else {
        res->palType = PALTYPE_SUB_BG;
        res->vramType = 1;
        res->isMain = FALSE;
    }
    {
        MysteryTextWinEntry entries[5] = {
            { 6, 2, 13, 2, 0, NULL, 0, 0, 0, PRINT_COLOR(15, 2, 0) },
            { 2, 6, 28, 2, 0, NULL, 0, 0, 0, PRINT_COLOR(1, 2, 0) },
            { 2, 9, 28, 10, 0, NULL, 0, 0, 0, PRINT_COLOR(1, 2, 0) },
            { 4, 20, 14, 2, 0, NULL, 0, 0, 0, PRINT_COLOR(15, 2, 0) },
            { 18, 20, 11, 2, 0, NULL, 3, 0, 0, PRINT_COLOR(15, 2, 0) },
        };

        res->textWin = func_ov121_021da1bc(TRUE, entries, 5, res->setup.textBg, res->setup.textPalette,
                                             res->setup.queue, res->setup.msgData, res->setup.font, heapId);
    }
    arc = GFL_ArcSysCreateFileHandle(ARCID_MYSTERY, heapId);
    res->palette = func_0204abf0(arc, 3, res->vramType, setup->iconPalette * 32, heapId);
    res->cellAnims = func_0204af28(arc, 33, 36, heapId);
    res->chars = func_0204a6c8(arc, 12, FALSE, res->vramType, heapId);
    GFL_ArcToolFree(arc);
    MI_CpuFill8(&iconSetup, 0, sizeof(ClActorSetup));
    iconSetup.x = 216;
    iconSetup.y = 22;
    res->icon = func_0204b294(setup->unit, res->chars, res->palette, res->cellAnims, &iconSetup, res->vramType, heapId);
    func_0204b688(res->icon, 0);
    func_0204b3dc(res->icon, FALSE);
    pokeArc = MakePokeGraArcHandle(heapId);
    res->pokePalette =
        func_0202d7d8(pokeArc, 1, 0, 0, FALSE, 0, FALSE, res->vramType, setup->pokePalette * 32, heapId);
    res->pokeCellAnims = func_0202d898(1, 0, 0, FALSE, 0, FALSE, 2, res->vramType, heapId);
    res->pokeChars = func_0202d81c(pokeArc, 1, 0, 0, FALSE, 0, FALSE, 0, res->vramType, heapId);
    GFL_ArcToolFree(pokeArc);
    MI_CpuFill8(&pokeSetup, 0, sizeof(ClActorSetup));
    pokeSetup.x = 184;
    pokeSetup.y = 112;
    res->bgPriority = pokeSetup.bgPriority = setup->bg & 3;
    res->textBgPriority = setup->textBg & 3;
    res->poke = func_0204b294(setup->unit, res->pokeChars, res->pokePalette, res->pokeCellAnims, &pokeSetup,
                              res->vramType, heapId);
    func_0204b570(res->poke, 2);
    func_0204b688(res->poke, 1);
    func_0204b3dc(res->poke, FALSE);
    return res;
}

// pokebw2's MysteryCardRes_Delete
void func_ov121_021dd1fc(MysteryCardRes *res) {
    func_0204b3b4(res->poke);
    func_0204a8d4(res->pokeChars);
    func_0204afd8(res->pokeCellAnims);
    func_0204ada4(res->pokePalette);
    func_0204b3b4(res->icon);
    func_0204a8d4(res->chars);
    func_0204afd8(res->cellAnims);
    func_0204ada4(res->palette);
    func_ov121_021da280(res->textWin);
    GFL_HeapFree(res);
}

// pokebw2's MysteryCardRes_Update
static void func_ov121_021dd240(MysteryCardRes *res) {
    func_ov121_021da2c8(res->textWin);
}

// pokebw2's MysteryAlbum_CreateReceived
MysteryAlbum *func_ov121_021dd24c(MysteryGift *gift, MysteryCardRes *res, GameData *gameData, HeapID heapId) {
    MysteryAlbum *album = GFL_HeapAllocate(heapId, sizeof(MysteryAlbum), FALSE, "mystery_album.c", 3540);
    MysteryCardViewSetup setup;

    MI_CpuFill8(album, 0, sizeof(MysteryAlbum));
    album->heapId = heapId;
    album->gift = gift;
    album->res = res;
    album->gameData = gameData;
    MI_CpuFill8(&setup, 0, sizeof(MysteryCardViewSetup));
    setup.mode = 0;
    setup.unit = res->setup.unit;
    setup.giftSave = res->setup.giftSave;
    setup.font = res->setup.font;
    setup.queue = res->setup.queue;
    setup.wordSet = res->setup.wordSet;
    setup.msgData = res->setup.msgData;
    setup.gameData = gameData;
    func_ov121_021dbdf4(&album->card, gift, &setup, heapId);
    {
        MysteryTextWinUpdate updates[5] = {
            { TRUE, 0x98, NULL, 0, 0, 0, PRINT_COLOR(15, 2, 0) }, { TRUE, 0, NULL, 0, 1, 0, PRINT_COLOR(1, 2, 0) },
            { TRUE, 0, NULL, 0, 1, 0, PRINT_COLOR(1, 2, 0) },     { TRUE, 0x99, NULL, 0, 1, 0, PRINT_COLOR(15, 2, 0) },
            { TRUE, 0, NULL, 0, 0, 0, PRINT_COLOR(15, 2, 0) },
        };
        u16 msgId;
        StrBuf *fmt;
        s32 date;

        updates[1].str = GFL_StrBufCreate(37, HEAPID_TAIL(heapId));
        GFL_StrBufLoadFixedString(updates[1].str, album->gift->title, 36);
        if (album->gift->delivered) {
            msgId = album->gift->msgIndex + 0x19d;
        } else {
            msgId = album->gift->msgIndex + 0x9d;
        }
        updates[2].str = GFL_MsgDataLoadStrbufNew(res->setup.msgData, msgId);
        updates[4].str = GFL_StrBufCreate(128, heapId);
        date = album->gift->date;
        fmt = GFL_MsgDataLoadStrbufNew(res->setup.msgData, 0x9a);
        WordSetNumber(res->setup.wordSet, 0, (u16)(date >> 16), 4, 1, TRUE);
        WordSetNumber(res->setup.wordSet, 1, (u8)(date >> 8), 2, 0, TRUE);
        WordSetNumber(res->setup.wordSet, 2, (u8)date, 2, 0, TRUE);
        GFL_WordSetFormatStrbuf(res->setup.wordSet, updates[4].str, fmt);
        album->textCopy = func_ov121_021da338(res->textWin, heapId);
        func_ov121_021da3ec(album->textCopy, updates, res->setup.queue);
        GFL_StrBufFree(updates[1].str);
        GFL_StrBufFree(updates[2].str);
        GFL_StrBufFree(updates[4].str);
        GFL_StrBufFree(fmt);
    }
    if (func_ov121_021dc024(&album->card) == 1) {
        MysteryGiftPokemon *poke = (MysteryGiftPokemon *)album->gift;
        u16 palFile;
        PartyPkm *pkm = Mystery_CreateGiftPokemon(album->gift, HEAPID_TAIL(heapId), album->gameData);
        ArcTool *arc = MakePokeGraArcHandle(HEAPID_TAIL(heapId));

        album->species = poke->species;
        album->form = poke->form;
        album->isEgg = poke->isEgg;
        palFile = GetPokemonPaletteDataNo(GetPokemonGraphicsARCID(), PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL),
                                          PokeParty_GetParam(pkm, PKM_PARAM_FORM, NULL),
                                          PokeParty_GetParam(pkm, PKM_PARAM_SEX, NULL), PokeParty_IsRare(pkm), 0,
                                          PokeParty_GetParam(pkm, PKM_PARAM_IS_EGG, NULL));
        album->palBuf = GFL_G2DIOReadNCLRArc(arc, palFile, &album->pal, album->heapId);
        album->charBuf = func_0202d6f4(&album->chars, func_0201d624(pkm), 0, album->heapId);
        GFL_HeapFree(pkm);
        GFL_ArcToolFree(arc);
    }
    return album;
}

// pokebw2's MysteryAlbum_Delete
void func_ov121_021dd980(MysteryAlbum *album) {
    if (album->tcb != NULL) {
        GFL_TCBRemove(album->tcb);
        album->tcb = NULL;
    }
    if (album->palBuf != NULL) {
        GFL_HeapFree(album->palBuf);
    }
    if (album->charBuf != NULL) {
        GFL_HeapFree(album->charBuf);
    }
    func_ov121_021da3c0(album->textCopy);
    func_ov121_021dbee4(&album->card);
    GFL_HeapFree(album);
}

// pokebw2's MysteryAlbum_Main
void func_ov121_021dd9cc(MysteryAlbum *album) {
    MysteryCardRes *res = album->res;
    void (*setBlend)(int eva, int evb);
    u32 type;
    BOOL done;
    BOOL voiceDone;
    ClActorPos actorPos;
    MysteryAlbumPos shakePos;
    ClActorPos pos;
    u16 rotation;

    if (res->isMain) {
        type = 14;
        setBlend = func_ov121_021ddc30;
    } else {
        setBlend = func_ov121_021ddc44;
        type = 30;
    }
    switch (album->state) {
    case 0:
        break;
    case 1:
        func_0204b84c(res->poke, (u8)res->textBgPriority);
        album->state = 2;
        break;
    case 2:
        func_ov121_021dab50(type, res->fade.colors, album->frame * 0x7fff / 60, (u8)res->pokePaletteNo, res->fade.to,
                         res->fade.from);
        setBlend(2 + album->frame * (16 - 2) / 60, 12 + album->frame * (0 - 12) / 60);
        if (album->frame++ > 60) {
            album->frame = 0;
            album->state = 3;
        }
        break;
    case 3:
        album->state = 4;
        break;
    case 4:
        if (!album->isEgg) {
            album->voice = PokeVoice_Play(album->species, album->form, 64, 0, 0, 0, 0, NULL);
        }
        func_0204b454(res->poke, &actorPos, res->vramType);
        func_ov121_021de128(&album->shake, func_0204b5fc(res->poke), actorPos.x, actorPos.y);
        album->state = 5;
        break;
    case 5:
        voiceDone = TRUE;
        done = TRUE;
        done &= func_ov121_021de154(&album->shake, &rotation, &shakePos);
        pos.x = shakePos.x;
        pos.y = shakePos.y;
        func_0204b404(res->poke, &pos, res->vramType);
        if (!album->isEgg) {
            if (PokeVoice_IsPlaying(album->voice)) {
                voiceDone = FALSE;
            }
            done &= voiceDone;
        }
        if (done) {
            album->state = 6;
        }
        break;
    case 6:
        func_ov121_021dab50(type, res->fade.colors, 0x7fff - album->frame * 0x7fff / 60, (u8)res->pokePaletteNo,
                         res->fade.to, res->fade.from);
        setBlend(16 + (2 - 16) * album->frame / 60, 0 + (12 - 0) * album->frame / 60);
        if (album->frame++ > 60) {
            album->frame = 0;
            album->state = 7;
        }
        break;
    case 7:
        album->state = 8;
        break;
    case 8:
        func_0204b84c(res->poke, (u8)res->bgPriority);
        album->state = 0;
        break;
    }
}

#pragma thumb off

// pokebw2's Mystery_SetBlendAlphaMain
static void func_ov121_021ddc30(int eva, int evb) {
    reg_G2_BLDALPHA = eva | (evb << 8);
}

// pokebw2's Mystery_SetBlendAlphaSub
static void func_ov121_021ddc44(int eva, int evb) {
    *(vu16 *)REG_DB_BLDALPHA_ADDR = eva | (evb << 8);
}

#pragma thumb reset

// Loads the card's BG with its palette
// pokebw2's MysteryCardRes_LoadBg
static void func_ov121_021ddc58(const MysteryCardResSetup *setup, MysteryCard *card, HeapID heapId) {
    u32 palType = PALTYPE_MAIN_BG;
    u32 scrFile;
    u16 palette;
    ArcTool *arc;

    if (setup->bg >= 4) {
        palType = PALTYPE_SUB_BG;
    }
    scrFile = 26;
    if (setup->bg <= 3) {
        scrFile = 27;
    }
    palette = func_ov121_021dc064(card);
    arc = GFL_ArcSysCreateFileHandle(ARCID_MYSTERY, heapId);
    GFL_G2DIOLoadArcNCLR(arc, 4, palType, palette * 32, setup->bgPalette * 32, 32, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 13, setup->bg, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, (u16)scrFile, setup->bg, 0, 0, FALSE, heapId);
    GFL_BGSysSetScrPaletteNo(setup->bg, 0, 0, 34, 24, setup->bgPalette);
    GFL_BGSysLoadScr(setup->bg);
    GFL_ArcToolFree(arc);
}

// Loads only the palette of the card's BG
// pokebw2's MysteryCardRes_LoadBgPalette
static void func_ov121_021ddcf8(const MysteryCardResSetup *setup, MysteryCard *card, HeapID heapId) {
    u32 palType = PALTYPE_MAIN_BG;
    u16 palette;
    ArcTool *arc;

    if (setup->bg >= 4) {
        palType = PALTYPE_SUB_BG;
    }
    palette = func_ov121_021dc064(card);
    arc = GFL_ArcSysCreateFileHandle(ARCID_MYSTERY, heapId);
    GFL_G2DIOLoadArcNCLR(arc, 4, palType, palette * 32, setup->bgPalette * 32, 32, heapId);
    GFL_BGSysSetScrPaletteNo(setup->bg, 0, 0, 34, 24, setup->bgPalette);
    GFL_BGSysLoadScr(setup->bg);
    GFL_ArcToolFree(arc);
}

// Loads the gift's icon, and a Pokémon gift's Pokémon, hidden in the card
// pokebw2's MysteryAlbum_LoadIcon
static void func_ov121_021ddd64(MysteryAlbum *album, MysteryCardRes *res, HeapID heapId) {
    ArcTool *arc = func_ov121_021dbf10(&album->card, heapId);
    void *buf;
    NNSG2dPaletteData *pal;
    NNSG2dCharacterData *chars;
    ClActorPos pos;
    u16 *pltt;
    uintptr_t dst;
    int i;

    buf = GFL_G2DIOReadNCLRArc(arc, func_ov121_021dbf40(&album->card), &pal, album->heapId);
    dst = res->setup.iconPalette * 32;
    if (res->vramType == 0) {
        dst += HW_OBJ_PLTT;
    } else {
        dst += HW_DB_OBJ_PLTT;
    }
    MI_CpuCopy8(pal->pRawData, (void *)dst, 0x60);
    GFL_HeapFree(buf);
    buf = GFL_G2DIOReadOBJNCGRArc(arc, func_ov121_021dbf9c(&album->card), FALSE, &chars, album->heapId);
    func_0204aa94(res->chars, chars->pRawData, 0x200, 0, res->vramType);
    GFL_HeapFree(buf);
    GFL_ArcToolFree(arc);
    pos.x = 216;
    pos.y = 22;
    if (func_ov121_021dc024(&album->card) == 2) {
        pos.x += 4;
        pos.y += 8;
    }
    func_0204b6f4(res->icon, (u8)func_ov121_021dbf70(&album->card), 0);
    func_0204b404(res->icon, &pos, res->vramType);
    func_0204b3dc(res->icon, TRUE);
    if (func_ov121_021dc024(&album->card) == 1) {
        // BUG: this passes the palette's colors where the palette data goes, so it uploads 32 bytes from wherever its
        // seventh and eighth colors point, read as the data's pointer. The colors written to the palette below hide it
#ifdef BUGFIX
        func_0204adec(res->pokePalette, album->pal, 1);
#else
        func_0204adec(res->pokePalette, album->pal->pRawData, 1);
#endif
        MI_CpuCopy8(album->pal->pRawData, res->fade.to, sizeof(res->fade.to));
        MIi_CpuClear16(0, res->fade.from, sizeof(res->fade.from));
        func_0204aa94(res->pokeChars, album->chars->pRawData, 0x1200, 0, res->vramType);
        func_0204b5f4(res->poke, 0);
        func_0204b3dc(res->poke, TRUE);
        if (res->setup.bg < 4) {
            pltt = (u16 *)(func_0204aef4(res->pokePalette, res->vramType) + HW_OBJ_PLTT);
        } else {
            pltt = (u16 *)(func_0204aef4(res->pokePalette, res->vramType) + HW_DB_OBJ_PLTT);
        }
        for (i = 0; i < 16; i++) {
            pltt[i] = res->fade.from[i];
        }
        if (res->setup.bg < 4) {
            G2_SetBlendAlpha(0, (1 << (u8)res->setup.bg) | (1 << (u8)res->setup.textBg), 2, 12);
        } else {
            G2S_SetBlendAlpha(0, (1 << (u8)(res->setup.bg - 4)) | (1 << (u8)(res->setup.textBg - 4)), 2, 12);
        }
    } else {
        func_0204b3dc(res->poke, FALSE);
    }
}

// pokebw2's MysteryAlbum_StartOpen
void func_ov121_021ddf5c(MysteryAlbum *album) {
    if (func_ov121_021dc024(&album->card) == 1 && album->state == 0) {
        album->state = 1;
        album->frame = 0;
    }
}

// Puts a Pokémon gift's Pokémon back into the card at once
// pokebw2's MysteryAlbum_Close
static void func_ov121_021ddf84(MysteryAlbum *album) {
    u16 *pltt;
    void (*setBlend)(int eva, int evb);

    if (func_ov121_021dc024(&album->card) == 1) {
        album->state = 0;
        album->frame = 0;
        func_0204b84c(album->res->poke, (u8)album->res->bgPriority);
        if (album->res->isMain) {
            pltt = (u16 *)(func_0204aef4(album->res->pokePalette, album->res->vramType) + HW_OBJ_PLTT);
            setBlend = func_ov121_021ddc30;
        } else {
            pltt = (u16 *)(func_0204aef4(album->res->pokePalette, album->res->vramType) + HW_DB_OBJ_PLTT);
            setBlend = func_ov121_021ddc44;
        }
        setBlend(2, 12);
        MIi_CpuClear16(0, album->res->fade.from, sizeof(album->res->fade.from));
        MIi_CpuCopy16(album->res->fade.from, pltt, sizeof(album->res->fade.from));
    }
}

// pokebw2's MysteryAlbum_IsOpened
BOOL func_ov121_021de014(MysteryAlbum *album) {
    if (album->state == 0) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's MysteryAlbum_VBlank
static void func_ov121_021de024(TCB *tcb, void *work) {
    MysteryAlbum *album = work;

    if (!album->loaded) {
        album->loaded = TRUE;
        switch (album->paletteOnly) {
        case FALSE:
            func_ov121_021ddc58(&album->res->setup, &album->card, album->heapId);
            break;
        case TRUE:
            func_ov121_021ddcf8(&album->res->setup, &album->card, album->heapId);
            break;
        }
        func_ov121_021ddd64(album, album->res, album->heapId);
        func_ov121_021da4a8(album->textCopy);
        func_ov121_021da318(album->res->textWin);
        GFL_BGSysSetBGEnabled(album->res->textBg, TRUE);
        GFL_BGSysSetBGEnabled(album->res->bg, TRUE);
    }
}

// pokebw2's MysteryAlbum_SetVisible
void func_ov121_021de0ac(MysteryAlbum *album, BOOL paletteOnly) {
    album->paletteOnly = paletteOnly;
    if (album->tcb == NULL) {
        album->tcb = GFL_VBlankTCBAdd(func_ov121_021de024, album, 0);
    } else {
        album->loaded = FALSE;
    }
}

// Hides the card on the top screen
// pokebw2's MysteryCardRes_Clear
static void func_ov121_021de0dc(MysteryCardRes *res) {
    ClActorPos pos;

    GFL_BGSysClearScr(4);
    GFL_BGSysLoadScr(4);
    func_0204b3dc(res->icon, FALSE);
    pos.x = 184;
    pos.y = 112;
    func_0204b404(res->poke, &pos, res->vramType);
    func_0204b3dc(res->poke, FALSE);
    GFL_BGSysSetBGEnabled(6, FALSE);
}

// pokebw2's MysteryAlbumShake_Init
static void func_ov121_021de128(MysteryAlbumShake *shake, u16 rotation, s32 x, s32 y) {
    MI_CpuFill8(shake, 0, sizeof(MysteryAlbumShake));
    shake->fromRotation = rotation;
    shake->toRotation = rotation;
    shake->stepCount = 11;
    shake->from.x = x;
    shake->from.y = y;
    shake->to.x = x;
    shake->to.y = y;
    shake->rotation = shake->fromRotation;
    shake->pos.x = shake->to.x;
    shake->pos.y = shake->to.y;
}

// Advances the shake, and returns TRUE once it is done
// pokebw2's MysteryAlbumShake_Update
static BOOL func_ov121_021de154(MysteryAlbumShake *shake, u16 *rotation, MysteryAlbumPos *pos) {
    BOOL done = FALSE;
    s32 fromX;
    s32 dx;
    s32 sign;

    switch (shake->state) {
    case 0:
        shake->frame = 0;
        shake->fromRotation = shake->toRotation;
        shake->toRotation = data_ov121_021df858[shake->step];
        shake->dir = data_ov121_021df822[shake->step];
        shake->frames = data_ov121_021df82d[shake->step];
        shake->from = shake->to;
        shake->to = data_ov121_021df944[shake->step];
        if (shake->dir > 0) {
            shake->fromRotation = shake->fromRotation == 0xffff ? 0 : shake->fromRotation;
            shake->toRotation = shake->toRotation == 0 ? 0xffff : shake->toRotation;
        } else {
            shake->fromRotation = shake->fromRotation == 0 ? 0xffff : shake->fromRotation;
            shake->toRotation = shake->toRotation == 0xffff ? 0 : shake->toRotation;
        }
        shake->state++;
        break;
    case 1:
        // The direction of x is also used for y, which no step moves
        fromX = shake->from.x;
        dx = shake->to.x - fromX;
        sign = MATH_ABS(dx) / dx;
        if (dx < 0) {
            dx = -dx;
        }
        shake->pos.x = fromX + sign * (shake->frame * dx / shake->frames);
        shake->pos.y = shake->from.y + MATH_ABS(shake->to.y - shake->from.y) * shake->frame / shake->frames * sign;
        if (shake->frame++ >= shake->frames) {
            shake->rotation = shake->toRotation;
            shake->pos = shake->to;
            shake->state++;
        }
        break;
    case 2:
        shake->step++;
        if (shake->step < shake->stepCount) {
            shake->state = 0;
        } else {
            shake->state++;
        }
        break;
    case 3:
        done = TRUE;
        break;
    }
    if (rotation != NULL) {
        *rotation = shake->rotation;
    }
    if (pos != NULL) {
        *pos = shake->pos;
    }
    return done;
}
