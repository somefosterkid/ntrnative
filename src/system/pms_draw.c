// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/pms_draw.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "system/pms_draw.h"
#include "types.h"
#include "constants/arc.h"
#include "gfl/arc.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/ui.h"
#include "nitro/gx.h"
#include "system/pms_data.h"
#include "system/pms_word.h"
#include "system/printsys.h"

void BmpWin_ClearScreenNow(BmpWin *window);

// Draws sentences into windows: the text through a print queue, and the words that are icons as cell actors over the
// window. Our names

// The files of ARCID_PMSI the icons use
#define PMSI_PALETTE_FILE 1
#define PMSI_ICON_PALETTE_COUNT 5
#define PMSI_ICON_CELL_FILE_1D_128K 0x20
#define PMSI_ICON_CELL_FILE_1D_32K 0x21
#define PMSI_ICON_CELL_FILE_1D_64K 0x22
#define PMSI_ICON_ANIM_FILE_1D_128K 0x26
#define PMSI_ICON_ANIM_FILE_1D_32K 0x27
#define PMSI_ICON_ANIM_FILE_1D_64K 0x28
#define PMSI_ICON_CHAR_FILE_1D_128K 0x2c
#define PMSI_ICON_CHAR_FILE_1D_32K 0x2d
#define PMSI_ICON_CHAR_FILE_1D_64K 0x2e

// The space a word that is left empty takes, in pixels
#define PMS_DRAW_EMPTY_WORD_WIDTH 84
#define PMS_DRAW_LINE_HEIGHT 16

// The icons' resources and the unit their actors are on
typedef struct {
    ClActUnit *unit;
    u32 vramType;
    u32 palette;
    u32 chars;
    u32 cellAnims;
} PMSDrawRes;

// One sentence, with an actor for each word that may be an icon
typedef struct {
    BmpWin *window;
    // Whether the text is still in the print queue
    u8 printing;
    ClActor *icons[PMS_SENTENCE_WORD_MAX];
    // The BG's offset when the icons were last moved with it
    int scrollX;
    int scrollY;
    u32 drawn : 1;
    u32 visible : 1;
    u32 showIcons : 1;
    u32 unk1C;
    BOOL isIcon[PMS_SENTENCE_WORD_MAX];
} PMSDrawSlot;

struct PMSDraw {
    u16 heapId;
    Font *font;
    PrintQueue *queue;
    PMSDrawSlot *slots;
    PMSDrawRes res;
    u16 color;
    BOOL printEnd;
    BOOL followScroll;
    u8 count;
    u8 backColor;
};

void func_0202de6c(BmpWin *window, u32 width, u32 lines, const PMSDrawPos *offset, PMSDrawPos *pos);
static u32 func_0202de9c(u32 vramType);
static u32 func_0202dee0(u32 vramType);
static u32 func_0202df24(u32 vramType);
static void func_0202df68(PMSDrawRes *res, u8 palette, HeapID heapId);
static void func_0202dfd4(PMSDrawRes *res);
static ClActor *func_0202dfec(PMSDrawRes *res, HeapID heapId);
void func_0202e040(ClActor *icon, BmpWin *window, u32 width, u32 lines, u32 word, const PMSDrawPos *offset);
static void func_0202e09c(PMSDrawSlot *slot, PMSDrawRes *res, HeapID heapId);
static void func_0202e0c8(PMSDrawSlot *slot);
static BOOL func_0202e0e0(PMSDrawSlot *slot, PrintQueue *queue, BOOL followScroll);
static void func_0202e1f0(PMSDrawSlot *slot, PrintQueue *queue, Font *font, BmpWin *window, const PMSData *sentence, const PMSDrawPos *offset, u16 color, u8 backColor, HeapID heapId);
static void func_0202e38c(PMSDrawSlot *slot, BOOL clearScreen);
static void func_0202e3e4(PMSDrawSlot *slot, BOOL visible);
static u32 func_0202e430(u32 bg);

PMSDraw *func_0202db0c(ClActUnit *unit, u32 vramType, PrintQueue *queue, Font *font, u8 palette, u8 count,
                        HeapID heapId) {
    int i;
    PMSDraw *draw = GFL_HeapAllocate(heapId, sizeof(PMSDraw), TRUE, "pms_draw.c", 129);

    MI_CpuFill8(draw, 0, sizeof(PMSDraw));
    draw->heapId = heapId;
    draw->queue = queue;
    draw->font = font;
    draw->count = count;
    draw->backColor = 15;
    draw->followScroll = FALSE;
    draw->slots = GFL_HeapAllocate(heapId, count * sizeof(PMSDrawSlot), TRUE, "pms_draw.c", 140);
    MI_CpuFill8(draw->slots, 0, count * sizeof(PMSDrawSlot));
    draw->res.unit = unit;
    draw->res.vramType = vramType;
    draw->color = PRINT_COLOR(1, 2, 0);
    func_0202df68(&draw->res, palette, draw->heapId);
    for (i = 0; i < draw->count; i++) {
        func_0202e09c(&draw->slots[i], &draw->res, draw->heapId);
    }
    return draw;
}

// pokebw2's PMSDraw_Main
void func_0202dbc4(PMSDraw *draw) {
    int i;

    draw->printEnd = TRUE;
    for (i = 0; i < draw->count; i++) {
        if (draw->slots[i].drawn && func_0202e0e0(&draw->slots[i], draw->queue, draw->followScroll)) {
            draw->printEnd = FALSE;
        }
    }
}

// pokebw2's PMSDraw_Delete
void func_0202dc08(PMSDraw *draw) {
    int i;

    for (i = 0; i < draw->count; i++) {
        func_0202e0c8(&draw->slots[i]);
    }
    GFL_HeapFree(draw->slots);
    func_0202dfd4(&draw->res);
    GFL_HeapFree(draw);
}

// pokebw2's PMSDraw_Print
void func_0202dc48(PMSDraw *draw, BmpWin *window, const PMSData *sentence, u8 slot) {
    PMSDrawPos offset = { 0, 0 };

    func_0202dc60(draw, window, sentence, slot, &offset);
}

// pokebw2's PMSDraw_PrintEx
void func_0202dc60(PMSDraw *draw, BmpWin *window, const PMSData *sentence, u8 slot, const PMSDrawPos *pos) {
    func_0202e1f0(&draw->slots[slot], draw->queue, draw->font, window, sentence, pos, draw->color, draw->backColor,
                      draw->heapId);
    draw->printEnd = FALSE;
}

// pokebw2's PMSDraw_IsPrintEnd
BOOL func_0202dc98(PMSDraw *draw) {
    return draw->printEnd;
}

// pokebw2's PMSDraw_Clear
void func_0202dc9c(PMSDraw *draw, u8 slot, BOOL clearScreen) {
    func_0202e38c(&draw->slots[slot], clearScreen);
}

// pokebw2's PMSDraw_SetVisible
void func_0202dcb0(PMSDraw *draw, u8 slot, BOOL visible) {
    func_0202e3e4(&draw->slots[slot], visible);
}

// pokebw2's PMSDraw_IsDrawn
BOOL func_0202dcc4(PMSDraw *draw, u8 slot) {
    return draw->slots[slot].drawn;
}

// pokebw2's PMSDraw_SetIconVisible
void func_0202dcd4(PMSDraw *draw, u8 slot, BOOL visible) {
    if (func_0202dcc4(draw, slot)) {
        draw->slots[slot].showIcons = visible;
    }
}

// pokebw2's PMSDraw_SetObjMode
void func_0202dcfc(PMSDraw *draw, u8 slot, u32 mode) {
    int i;

    for (i = 0; i < PMS_SENTENCE_WORD_MAX; i++) {
        func_0204b688(draw->slots[slot].icons[i], mode);
    }
}

// pokebw2's PMSDraw_Copy
void func_0202dd24(PMSDraw *draw, u8 src, u8 dest) {
    int i;
    ClActorPos pos;
    PMSDrawSlot *srcSlot = &draw->slots[src];
    PMSDrawSlot *destSlot = &draw->slots[dest];
    int moveX = BmpWin_GetPosX(destSlot->window) - BmpWin_GetPosX(srcSlot->window);
    int moveY = BmpWin_GetPosY(destSlot->window) - BmpWin_GetPosY(srcSlot->window);

    GFL_BitmapCopy(BmpWin_GetBitmap(srcSlot->window), BmpWin_GetBitmap(destSlot->window));
    BmpWin_Transfer(destSlot->window);
    for (i = 0; i < PMS_SENTENCE_WORD_MAX; i++) {
        u32 surface = func_0202e430(BmpWin_GetBGIndex(srcSlot->window));

        func_0204b454(srcSlot->icons[i], &pos, surface);
        pos.x += moveX * 8;
        pos.y += moveY * 8;
        func_0204b404(destSlot->icons[i], &pos, surface);
        func_0204b878(destSlot->icons[i], func_0204b898(srcSlot->icons[i]));
        func_0204b3dc(destSlot->icons[i], srcSlot->isIcon[i]);
    }
    destSlot->printing = srcSlot->printing;
    destSlot->drawn = srcSlot->drawn;
    destSlot->visible = srcSlot->visible;
    for (i = 0; i < PMS_SENTENCE_WORD_MAX; i++) {
        destSlot->isIcon[i] = srcSlot->isIcon[i];
    }
}

// pokebw2's PMSDraw_SetBackColor
void func_0202de5c(PMSDraw *draw, u8 color) {
    draw->backColor = color;
}

// pokebw2's PMSDraw_SetColor
void func_0202de64(PMSDraw *draw, u16 color) {
    draw->color = color;
}

// pokebw2's PMSDraw_SetFollowScroll
void func_0202de68(PMSDraw *draw, BOOL follow) {
    draw->followScroll = follow;
}

// Where an icon goes on the screen: after width pixels of text on line lines of the sentence
// pokebw2's PMSDraw_GetIconPos
void func_0202de6c(BmpWin *window, u32 width, u32 lines, const PMSDrawPos *offset, PMSDrawPos *pos) {
    pos->x = offset->x + (width + BmpWin_GetPosX(window) * 8);
    pos->y = offset->y + (lines * PMS_DRAW_LINE_HEIGHT + BmpWin_GetPosY(window) * 8);
}

// pokebw2's PMSDraw_GetIconCellFile
static u32 func_0202de9c(u32 vramType) {
    GXOBJVRamModeChar mode;

    if (vramType == CLACT_VRAM_MAIN) {
        mode = GX_GetOBJVRamModeChar();
    } else {
        mode = GXS_GetOBJVRamModeChar();
    }
    switch (mode) {
    case GX_OBJVRAMMODE_CHAR_1D_32K:
        return PMSI_ICON_CELL_FILE_1D_32K;
    case GX_OBJVRAMMODE_CHAR_1D_64K:
        return PMSI_ICON_CELL_FILE_1D_64K;
    case GX_OBJVRAMMODE_CHAR_1D_128K:
        return PMSI_ICON_CELL_FILE_1D_128K;
    default:
        return PMSI_ICON_CELL_FILE_1D_128K;
    }
}

// pokebw2's PMSDraw_GetIconAnimFile
static u32 func_0202dee0(u32 vramType) {
    GXOBJVRamModeChar mode;

    if (vramType == CLACT_VRAM_MAIN) {
        mode = GX_GetOBJVRamModeChar();
    } else {
        mode = GXS_GetOBJVRamModeChar();
    }
    switch (mode) {
    case GX_OBJVRAMMODE_CHAR_1D_32K:
        return PMSI_ICON_ANIM_FILE_1D_32K;
    case GX_OBJVRAMMODE_CHAR_1D_64K:
        return PMSI_ICON_ANIM_FILE_1D_64K;
    case GX_OBJVRAMMODE_CHAR_1D_128K:
        return PMSI_ICON_ANIM_FILE_1D_128K;
    default:
        return PMSI_ICON_ANIM_FILE_1D_128K;
    }
}

// pokebw2's PMSDraw_GetIconCharFile
static u32 func_0202df24(u32 vramType) {
    GXOBJVRamModeChar mode;

    if (vramType == CLACT_VRAM_MAIN) {
        mode = GX_GetOBJVRamModeChar();
    } else {
        mode = GXS_GetOBJVRamModeChar();
    }
    switch (mode) {
    case GX_OBJVRAMMODE_CHAR_1D_32K:
        return PMSI_ICON_CHAR_FILE_1D_32K;
    case GX_OBJVRAMMODE_CHAR_1D_64K:
        return PMSI_ICON_CHAR_FILE_1D_64K;
    case GX_OBJVRAMMODE_CHAR_1D_128K:
        return PMSI_ICON_CHAR_FILE_1D_128K;
    default:
        // BUG: The default is the 128K animation file, not the characters. No screen uses another OBJ mapping, so it
        // is never reached
#ifdef BUGFIX
        return PMSI_ICON_CHAR_FILE_1D_128K;
#else
        return PMSI_ICON_ANIM_FILE_1D_128K;
#endif
    }
}

// pokebw2's PMSDrawRes_Load
static void func_0202df68(PMSDrawRes *res, u8 palette, HeapID heapId) {
    u32 cellFile = func_0202de9c(res->vramType);
    u32 animFile = func_0202dee0(res->vramType);
    u32 charFile = func_0202df24(res->vramType);
    ArcTool *arc = GFL_ArcSysCreateFileHandle(0x2b, heapId);

    res->palette =
        func_0204ac18(arc, PMSI_PALETTE_FILE, res->vramType, palette * 0x20, 0, PMSI_ICON_PALETTE_COUNT, heapId);
    res->chars = func_0204a6c8(arc, charFile, FALSE, res->vramType, heapId);
    res->cellAnims = func_0204af28(arc, cellFile, animFile, heapId);
    GFL_ArcToolFree(arc);
}

// pokebw2's PMSDrawRes_Free
static void func_0202dfd4(PMSDrawRes *res) {
    func_0204ada4(res->palette);
    func_0204a8d4(res->chars);
    func_0204afd8(res->cellAnims);
}

// pokebw2's PMSDrawRes_CreateIcon
static ClActor *func_0202dfec(PMSDrawRes *res, HeapID heapId) {
    ClActorSetup setup = { 0 };
    ClActor *icon = func_0204b294(res->unit, res->chars, res->palette, res->cellAnims, &setup, res->vramType, heapId);

    func_0204b3dc(icon, FALSE);
    func_0204b92c(icon, TRUE);
    if (GCTX_HIDGetUpdateRate() == 30) {
        func_0204b954(icon, FX32_CONST(2));
    }
    return icon;
}

// pokebw2's PMSDraw_SetIcon
void func_0202e040(ClActor *icon, BmpWin *window, u32 width, u32 lines, u32 word, const PMSDrawPos *offset) {
    PMSDrawPos pos;
    ClActorPos actorPos;
    u8 bg;

    func_0202de6c(window, width, lines, offset, &pos);
    actorPos.x = pos.x;
    actorPos.y = pos.y;
    bg = BmpWin_GetBGIndex(window);
    func_0204b404(icon, &actorPos, func_0202e430(bg));
    func_0204b84c(icon, GFL_BGSysGetBGPriority(bg));
    func_0204b878(icon, word - 1);
}

// pokebw2's PMSDrawSlot_Init
static void func_0202e09c(PMSDrawSlot *slot, PMSDrawRes *res, HeapID heapId) {
    int i;

    for (i = 0; i < PMS_SENTENCE_WORD_MAX; i++) {
        slot->icons[i] = func_0202dfec(res, heapId);
    }
    slot->drawn = FALSE;
    slot->showIcons = TRUE;
}

// pokebw2's PMSDrawSlot_Delete
static void func_0202e0c8(PMSDrawSlot *slot) {
    int i;

    for (i = 0; i < PMS_SENTENCE_WORD_MAX; i++) {
        func_0204b3b4(slot->icons[i]);
    }
}

// Returns whether the text is still printing
// pokebw2's PMSDrawSlot_Main
static BOOL func_0202e0e0(PMSDrawSlot *slot, PrintQueue *queue, BOOL followScroll) {
    int i;
    BOOL printing;

    if (slot->printing && !func_0201c6c4(queue, BmpWin_GetBitmap(slot->window))) {
        BmpWin_FlushChar(slot->window);
        slot->printing = FALSE;
    }
    printing = slot->printing ? TRUE : FALSE;
    if (!printing && slot->visible) {
        for (i = 0; i < PMS_SENTENCE_WORD_MAX; i++) {
            if (slot->isIcon[i] && slot->showIcons) {
                func_0204b3dc(slot->icons[i], TRUE);
            } else {
                func_0204b3dc(slot->icons[i], FALSE);
            }
        }
    }
    if (followScroll) {
        u32 bg = BmpWin_GetBGIndex(slot->window);
        ClActorPos move = { 0, 0 };
        ClActorPos pos;
        int scrollX = GFL_BGSysGetBGOffsetX2(bg);
        int scrollY = GFL_BGSysGetBGOffsetY2(bg);

        if (scrollX != slot->scrollX) {
            move.x = slot->scrollX - scrollX;
            slot->scrollX = scrollX;
        }
        if (scrollY != slot->scrollY) {
            move.y = slot->scrollY - scrollY;
            slot->scrollY = scrollY;
        }
        if (move.x != 0 || move.y != 0) {
            for (i = 0; i < PMS_SENTENCE_WORD_MAX; i++) {
                func_0204b53c(slot->icons[i], &pos);
                pos.x += move.x;
                pos.y += move.y;
                func_0204b528(slot->icons[i], &pos);
            }
        }
    }
    return printing;
}

static void func_0202e1f0(PMSDrawSlot *slot, PrintQueue *queue, Font *font, BmpWin *window, const PMSData *sentence,
                          const PMSDrawPos *offset, u16 color, u8 backColor, HeapID heapId) {
    StrBuf *format;
    int wordCount;
    int i;
    StrBuf *str;
    BOOL isEmpty[PMS_SENTENCE_WORD_MAX] = { FALSE, FALSE };
    PMSData data;

    // Draws a valid copy of the sentence, leaving the caller's as it is
    func_02024570(&data, sentence);
    sentence = &data;
    func_0202465c(&data, TRUE, heapId);
    slot->window = window;
    slot->printing = FALSE;
    format = func_0202441c(sentence, heapId);
    wordCount = (u8)GFL_StrCmdGetWordSetCommandCount(format);
    for (i = 0; i < wordCount; i++) {
        if (func_020244f8(sentence, i)) {
            slot->isIcon[i] = TRUE;
        } else if (func_020244d4(sentence, i) == PMS_WORD_NULL) {
            isEmpty[i] = TRUE;
        }
    }
    str = func_02024368(sentence, heapId, wordCount);
    GFL_BitmapFill(BmpWin_GetBitmap(slot->window), backColor);
    func_0201c724(queue, BmpWin_GetBitmap(slot->window), offset->x, offset->y, str, font, color);
    slot->printing = TRUE;
    for (i = 0; i < wordCount; i++) {
        if (slot->isIcon[i] == TRUE) {
            u8 width = GFL_StrCmdGetStrWidthUntilWordSetIndex(str, i, font, 0);
            u8 lines = GFL_StrCmdCountLinesUntilWordSetIndex(str, i);

            func_0202e040(slot->icons[i], window, width, lines, func_020244d4(sentence, i), offset);
        } else if (isEmpty[i] == TRUE) {
            u8 width = GFL_StrCmdGetStrWidthUntilWordSetIndex(str, i, font, 0);
            u8 lines = GFL_StrCmdCountLinesUntilWordSetIndex(str, i);

            GFL_BitmapFillArea(BmpWin_GetBitmap(window), width, lines * PMS_DRAW_LINE_HEIGHT, PMS_DRAW_EMPTY_WORD_WIDTH,
                               PMS_DRAW_LINE_HEIGHT, backColor);
        }
    }
    GFL_StrBufFree(str);
    GFL_StrBufFree(format);
    BmpWin_FlushMap(slot->window);
    GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(slot->window));
    slot->drawn = TRUE;
    slot->visible = TRUE;
}


// pokebw2's PMSDrawSlot_Clear
static void func_0202e38c(PMSDrawSlot *slot, BOOL clearScreen) {
    int i;
    BmpWin *window;

    GFL_BitmapFill(BmpWin_GetBitmap(slot->window), 0);
    if (clearScreen) {
        BmpWin_FlushChar(slot->window);
        window = slot->window;
        func_020456f8(window);
        GFL_BGSysLoadScr(BmpWin_GetBGIndex(window));
    }
    for (i = 0; i < PMS_SENTENCE_WORD_MAX; i++) {
        func_0204b3dc(slot->icons[i], FALSE);
        slot->isIcon[i] = FALSE;
    }
    slot->drawn = FALSE;
    slot->scrollX = 0;
    slot->scrollY = 0;
    slot->visible = FALSE;
}

// pokebw2's PMSDrawSlot_SetVisible
static void func_0202e3e4(PMSDrawSlot *slot, BOOL visible) {
    int i;

    slot->visible = visible;
    if (visible) {
        BmpWin_FlushMap(slot->window);
    } else {
        func_020456f8(slot->window);
    }
    GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(slot->window));
    for (i = 0; i < PMS_SENTENCE_WORD_MAX; i++) {
        if (slot->isIcon[i]) {
            func_0204b3dc(slot->icons[i], visible);
        }
    }
}

// The surface of the default renderer that a BG's screen is
// pokebw2's PMSDraw_GetSurface
static u32 func_0202e430(u32 bg) {
    return bg >= BGSYS_BG_SUB ? CLACT_SURFACE_SUB : CLACT_SURFACE_MAIN;
}
