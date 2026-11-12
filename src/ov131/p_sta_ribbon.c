// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov207/p_sta_ribbon.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/touchpanel.h"
#include "nitro/gx.h"
#include "p_status_local.h"
#include "pml/poke_party.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/msg.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/wordset.h"


// Corrected PStatusWork with the fields this file uses

PStaOam *func_ov131_021dba90(HeapID heapId, ClActUnit *actorUnit);
void func_ov131_021dbac0(PStaOam *oam);
void func_ov131_021d5a9c(PStatusWork *wk, u32 mode);
PStaOamActor *func_ov131_021dbad0(PStaOam *oam, const struct PStaOamSetup *setup);
void func_ov131_021dbca4(PStaOamActor *oam, BOOL param);
void func_ov131_021dbc3c(PStaOamActor *oam);

// The summary screen's ribbon page: a scrolling list of the Pokémon's ribbons, each a row drawn into a bitmap shown
// by sprites, and the name, description and icon of the ribbon picked. The names of the fields and functions are
// guesses

#define ROW_COUNT 10
#define ROWS_SHOWN 6
#define ROW_HEIGHT 24
#define ROW_X 8
#define ROW_NONE 0xff
// No entry has the cursor
#define CURSOR_NONE 0xff
// The list's area: the first row's y, the right edge and the bottom
#define LIST_TOP 8
#define LIST_RIGHT 144
#define LIST_BOTTOM 168
// The lowest y the cursor's row is scrolled to
#define CURSOR_Y_MAX 128
// The message of the first category's heading, followed by the others in RIBBON_CATEGORY_ order
#define MSG_CATEGORY_FIRST 0xa0
#define ENTRY_NONE 0xffff
// The size of a row's characters, 17x3 tiles
#define ROW_CHAR_SIZE (17 * 3 * 32)

// A row of the list, which shows an entry
// Black 2's; twinned code confirms the offsets of entry, isPrinting, bitmap and oam in Black
typedef struct {
    u16 entry;
    u32 unk4;
    // Never set, and the work isn't cleared when it is allocated, so the check of it reads what the heap held
    BOOL isPrinting;
    GFLBitmap *bitmap;
    PStaOamActor *oam;
} RibbonRow;

// A ribbon the Pokémon has, its category and its number among them
typedef struct {
    BOOL isValid;
    u8 ribbon;
    u8 category;
    u8 number;
} RibbonEntry;

struct PStaRibbonWork {
    BOOL isShown;
    BOOL isDetailShown;
    BOOL isDetailPending;
    BOOL needsRedraw;
    u8 ribbonCount;
    u32 scrollY;
    u32 iconChars;
    ClActor *icon;
    BmpWin *nameWindow;
    BmpWin *descWindow;
    PStaScreen screens[4];
    NNSG2dCharacterData *rowChars;
    void *rowCharsFile;
    MsgData *msgData;
    ClActor *cursor;
    u8 cursorRow;
    u8 cursorEntry;
    u8 prevCursorRow;
    int scrollSpeed;
    u8 seWait;
    BOOL isDragging;
    BOOL cursorMoved;
    PStaOam *oam;
    RibbonRow rows[ROW_COUNT];
    RibbonEntry entries[RIBBON_COUNT];
};

void func_ov131_021d8a2c(PStatusWork *wk, PStaRibbonWork *ribbon);
BOOL func_ov131_021d8ad0(PStatusWork *wk, PStaRibbonWork *ribbon);
void func_ov131_021d8cd0(PStatusWork *wk, PStaRibbonWork *ribbon);
s32 func_ov131_021d8e24(PStatusWork *wk, PStaRibbonWork *ribbon);
void func_ov131_021d8ea8(PStatusWork *wk, PStaRibbonWork *ribbon);
static void func_ov131_021d8ed0(PStatusWork *wk, PStaRibbonWork *ribbon, u8 row);
void func_ov131_021d8f0c(PStatusWork *wk, PStaRibbonWork *ribbon, s16 dy);
void func_ov131_021d91f0(PStatusWork *wk, PStaRibbonWork *ribbon, RibbonRow *row);
void func_ov131_021d92b0(PStatusWork *wk, PStaRibbonWork *ribbon);
static u8 func_ov131_021d93e4(PStaRibbonWork *ribbon, u8 row);
static u32 func_ov131_021d9428(PStaRibbonWork *ribbon, u8 row);
static s16 func_ov131_021d9440(PStaRibbonWork *ribbon, u8 row);
void func_ov131_021d949c(PStatusWork *wk, PStaRibbonWork *ribbon);
void func_ov131_021d9568(PStatusWork *wk, PStaRibbonWork *ribbon);
void func_ov131_021d964c(PStatusWork *wk, PStaRibbonWork *ribbon);
void func_ov131_021d9670(PStatusWork *wk, PStaRibbonWork *ribbon);

// pokebw2's PStaRibbon_Create
// Corrected PStaScreen: in Black it stores a pointer to the screen data, not the data inline
PStaRibbonWork *func_ov131_021d87e0(PStatusWork *wk) {
    PStaRibbonWork *ribbon = GFL_HeapAllocate(wk->heapId, sizeof(PStaRibbonWork), FALSE, "p_sta_ribbon.c", 175);

    ribbon->isShown = FALSE;
    ribbon->seWait = 0;
    ribbon->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_RIBBON_NAMES, wk->heapId);
    return ribbon;
}

// pokebw2's PStaRibbon_Free
void func_ov131_021d881c(PStatusWork *wk, PStaRibbonWork *ribbon) {
    GFL_MsgDataFree(ribbon->msgData);
    GFL_HeapFree(ribbon);
}

// pokebw2's PStaRibbon_Main
void func_ov131_021d8830(PStatusWork *wk, PStaRibbonWork *ribbon) {
    func_ov131_021d8a2c(wk, ribbon);
    if (ribbon->isDetailPending == TRUE) {
        if (func_0201c6b4(wk->printQueue) == TRUE) {
            func_ov131_021d9670(wk, ribbon);
            func_ov131_021d9568(wk, ribbon);
            ribbon->isDetailPending = FALSE;
        }
    } else {
        func_ov131_021d92b0(wk, ribbon);
    }
    if (ribbon->seWait != 0) {
        ribbon->seWait--;
    }
}

// Corrected PStaScreen: screen data pointer first, file handle second (32-bit, not HeapID)
void func_ov131_021d8884(PStatusWork *wk, PStaRibbonWork *ribbon, ArcTool *arc) {
    ribbon->screens[0].file = GFL_G2DIOReadNSCRArc(arc, 0x42, FALSE, &ribbon->screens[0].screen, wk->heapId);
    ribbon->screens[1].file = GFL_G2DIOReadNSCRArc(arc, 0x43, FALSE, &ribbon->screens[1].screen, wk->heapId);
    ribbon->screens[2].file = GFL_G2DIOReadNSCRArc(arc, 0x44, FALSE, &ribbon->screens[2].screen, wk->heapId);
    ribbon->screens[3].file = GFL_G2DIOReadNSCRArc(arc, 0x45, FALSE, &ribbon->screens[3].screen, wk->heapId);
    ribbon->rowCharsFile = GFL_G2DIOReadOBJNCGRArc(arc, 0x0c, FALSE, &ribbon->rowChars, wk->heapId);
    ribbon->oam = func_ov131_021dba90(wk->heapId, wk->actorUnit);
}


// pokebw2's PStaRibbon_FreeResources
void func_ov131_021d8904(PStatusWork *wk, PStaRibbonWork *ribbon) {
    if (ribbon->isShown == TRUE) {
        func_ov131_021d909c(wk, ribbon);
    }
    func_ov131_021dbac0(ribbon->oam);
    GFL_HeapFree(ribbon->rowCharsFile);
    GFL_HeapFree(ribbon->screens[0].file);
    GFL_HeapFree(ribbon->screens[1].file);
    GFL_HeapFree(ribbon->screens[2].file);
    GFL_HeapFree(ribbon->screens[3].file);
}

void func_ov131_021d8938(PStatusWork *wk, PStaRibbonWork *ribbon) {
    ClActorSetup setup;
    struct PStaOamSetup oamSetup;
    u8 i;
    u8 *vram;

    setup.x = 0;
    setup.y = 0;
    setup.priority = 10;
    setup.bgPriority = 0;
    setup.sequence = 0;
    ribbon->cursor = func_0204b294(wk->actorUnit, wk->unk98[PSTA_RES_CHAR(12)], wk->unk98[PSTA_RES_PLTT(11)],
                                   wk->unk98[PSTA_RES_CELL(12)], &setup, 0, wk->heapId);
    func_0204b3dc(ribbon->cursor, FALSE);
    oamSetup.x = ROW_X;
    oamSetup.palette = wk->unk98[PSTA_RES_PLTT(8)];
    oamSetup.paletteOffset = 0;
    oamSetup.priority = 0;
    oamSetup.bgPriority = 2;
    oamSetup.surface = 0;
    oamSetup.vramType = 0;
    vram = (u8 *)G2_GetOBJCharPtr() + 0x20000;
    for (i = 0; i < ROW_COUNT; i++) {
        oamSetup.y = func_ov131_021d9440(ribbon, i);
        ribbon->rows[i].bitmap = GFL_BitmapWrapVRAM(vram - (i + 1) * ROW_CHAR_SIZE, 17, 3, 32, wk->heapId);
        oamSetup.bitmap = ribbon->rows[i].bitmap;
        ribbon->rows[i].oam = func_ov131_021dbad0(ribbon->oam, &oamSetup);
        func_ov131_021dbca4(ribbon->rows[i].oam, FALSE);
    }
}


// pokebw2's PStaRibbon_FreeActors
void func_ov131_021d89fc(PStatusWork *wk, PStaRibbonWork *ribbon) {
    u8 i;

    func_0204b3b4(ribbon->cursor);
    for (i = 0; i < ROW_COUNT; i++) {
        func_ov131_021dbc3c(ribbon->rows[i].oam);
        GFL_BitmapFree(ribbon->rows[i].bitmap);
    }
}

void func_ov131_021d8a2c(PStatusWork *wk, PStaRibbonWork *ribbon) {
    s32 hit;

    if (wk->unk44 == 1) {
        if (ribbon->ribbonCount == 0) {
            return;
        }
        hit = func_ov131_021d8e24(wk, ribbon);
        if (GCTX_HIDGetPressedKeys() == PAD_BUTTON_A || hit != TOUCH_RECT_NONE) {
            ribbon->scrollSpeed = 0;
            func_ov131_021d5a9c(wk, FALSE);
            if (GCTX_HIDGetPressedKeys() == PAD_BUTTON_A) {
                func_ov131_021d8ea8(wk, ribbon);
                wk->unk1C = 0;
            } else {
                ribbon->isDragging = TRUE;
                ribbon->cursorRow = hit;
                ribbon->isDragging = TRUE;
                ribbon->cursorMoved = TRUE;
                ribbon->cursorEntry = func_ov131_021d9428(ribbon, hit);
                func_ov131_021d5a9c(wk, FALSE);
                wk->unk1C = TRUE;
            }
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_ov131_021d964c(wk, ribbon);
            func_ov131_021d949c(wk, ribbon);
        }
    } else if (ribbon->isDetailPending == FALSE) {
        if (func_ov131_021d8ad0(wk, ribbon) == FALSE) {
            func_ov131_021d8cd0(wk, ribbon);
        }
    }
}

BOOL func_ov131_021d8ad0(PStatusWork *wk, PStaRibbonWork *ribbon) {
    s16 y;
    u8 i;
    u8 row;
    u8 dy;

    if (GCTX_HIDGetPressedKeys() == PAD_BUTTON_B) {
        func_ov131_021d5a9c(wk, TRUE);
        func_0204b878(wk->unk178[7], 9);
        func_0204b3dc(ribbon->cursor, FALSE);
        ribbon->cursorRow = ROW_NONE;
        ribbon->cursorEntry = CURSOR_NONE;
        ribbon->cursorMoved = TRUE;
        func_ov131_021d964c(wk, ribbon);
        func_ov131_021d9670(wk, ribbon);
        wk->unk1C = FALSE;
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        return TRUE;
    }
    if (wk->unk1C == TRUE) {
        if (GCTX_HIDGetPressedKeys() == PAD_BUTTON_A || GCTX_HIDGetPressedKeys() == PAD_KEY_DOWN ||
            GCTX_HIDGetPressedKeys() == PAD_KEY_UP) {
            y = -16;
            ribbon->scrollSpeed = 0;
            ribbon->isDragging = FALSE;
            for (i = 0; i < ROW_COUNT; i++) {
                if (ribbon->cursorEntry == ribbon->rows[i].entry) {
                    y = func_ov131_021d9440(ribbon, i);
                }
            }
            if (y < -4 || y > LIST_BOTTOM - ROW_HEIGHT) {
                func_ov131_021d8ea8(wk, ribbon);
            }
            func_ov131_021d964c(wk, ribbon);
            func_ov131_021d949c(wk, ribbon);
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            wk->unk1C = FALSE;
            return TRUE;
        }
    } else if (GCTX_HIDGetTypedKeys() == PAD_KEY_DOWN) {
        row = ribbon->cursorRow + 1;
        if (row >= ROW_COUNT) {
            row -= ROW_COUNT;
        }
        if (ribbon->rows[row].entry != ENTRY_NONE) {
            ribbon->cursorRow = row;
            ribbon->cursorMoved = TRUE;
            if (func_ov131_021d9440(ribbon, ribbon->cursorRow) > LIST_BOTTOM - ROW_HEIGHT - 16) {
                dy = func_ov131_021d9440(ribbon, ribbon->cursorRow) - CURSOR_Y_MAX;
                func_ov131_021d8f0c(wk, ribbon, dy);
            }
            func_ov131_021d8ed0(wk, ribbon, ribbon->cursorRow);
            func_ov131_021d964c(wk, ribbon);
            func_ov131_021d949c(wk, ribbon);
            GFL_SndSEPlay(SEQ_SE_SELECT1);
        }
        return TRUE;
    } else if (GCTX_HIDGetTypedKeys() == PAD_KEY_UP) {
        if (ribbon->cursorRow == 0) {
            row = ROW_COUNT - 1;
        } else {
            row = ribbon->cursorRow - 1;
        }
        if (ribbon->rows[row].entry != ENTRY_NONE) {
            ribbon->cursorRow = row;
            ribbon->cursorMoved = TRUE;
            if (func_ov131_021d9440(ribbon, ribbon->cursorRow) < LIST_TOP) {
                s16 dy2 = func_ov131_021d9440(ribbon, ribbon->cursorRow) - LIST_TOP;
                func_ov131_021d8f0c(wk, ribbon, dy2);
            }
            func_ov131_021d8ed0(wk, ribbon, ribbon->cursorRow);
            func_ov131_021d964c(wk, ribbon);
            func_ov131_021d949c(wk, ribbon);
            GFL_SndSEPlay(SEQ_SE_SELECT1);
        }
        return TRUE;
    }
    return FALSE;
}


void func_ov131_021d8cd0(PStatusWork *wk, PStaRibbonWork *ribbon) {
    s32 hit;
    u8 row;
    s16 dy;
    s32 speed;

    if (wk->touchHit == 7) {
        func_ov131_021d5a9c(wk, TRUE);
        func_0204b878(wk->unk178[7], 9);
        func_0204b3dc(ribbon->cursor, FALSE);
        ribbon->cursorRow = ROW_NONE;
        ribbon->cursorEntry = CURSOR_NONE;
        ribbon->cursorMoved = TRUE;
        wk->unk1C = TRUE;
        func_ov131_021d964c(wk, ribbon);
        func_ov131_021d9670(wk, ribbon);
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        return;
    }
    if (func_02035c60() == TRUE && wk->unk28 > 8 && wk->unk28 < 0x90) {
        hit = func_ov131_021d8e24(wk, ribbon);
        if (hit != TOUCH_RECT_NONE) {
            row = hit;
            ribbon->cursorRow = row;
            ribbon->cursorMoved = TRUE;
            ribbon->cursorEntry = func_ov131_021d9428(ribbon, row);
            ribbon->isDragging = TRUE;
            ribbon->scrollSpeed = 0;
            func_0204b3dc(ribbon->cursor, FALSE);
            func_ov131_021d964c(wk, ribbon);
            func_ov131_021d949c(wk, ribbon);
        }
        wk->unk1C = TRUE;
        return;
    }
    if (func_02035c38() == TRUE && ribbon->isDragging == TRUE && wk->unk28 > 8 && wk->unk28 < 0x90) {
        dy = wk->unk34 - wk->unk2C;
        speed = dy * 2;
        func_ov131_021d8f0c(wk, ribbon, dy);
        if (speed >= 0 && ribbon->scrollSpeed > 0 && speed < ribbon->scrollSpeed) {
            ribbon->scrollSpeed = (ribbon->scrollSpeed + speed) / 2;
        } else if (speed <= 0 && ribbon->scrollSpeed < 0 && speed > ribbon->scrollSpeed) {
            ribbon->scrollSpeed = (ribbon->scrollSpeed + speed) / 2;
        } else {
            ribbon->scrollSpeed = speed;
        }
    } else {
        ribbon->isDragging = FALSE;
        if (ribbon->scrollSpeed < 0) {
            ribbon->scrollSpeed++;
            func_ov131_021d8f0c(wk, ribbon, ribbon->scrollSpeed / 2);
        }
        if (ribbon->scrollSpeed > 0) {
            ribbon->scrollSpeed--;
            func_ov131_021d8f0c(wk, ribbon, ribbon->scrollSpeed / 2);
        }
    }
}



// pokebw2's PStaRibbon_GetTouchedRow
s32 func_ov131_021d8e24(PStatusWork *wk, PStaRibbonWork *ribbon) {
    TouchRect rects[ROW_COUNT + 1];
    TouchRect *rect;
    u8 i;
    int y;

    for (i = 0; i < ROW_COUNT; i++) {
        y = func_ov131_021d9440(ribbon, i);
        rect = &rects[i];
        rect->left = ROW_X;
        rect->right = LIST_RIGHT;
        if (y < 0) {
            rect->top = 0;
        } else if (y > LIST_BOTTOM) {
            rect->top = LIST_BOTTOM;
        } else {
            rect->top = y;
        }
        y += ROW_HEIGHT;
        if (y < 0) {
            rect->bottom = 0;
        } else if (y > LIST_BOTTOM) {
            rect->bottom = LIST_BOTTOM;
        } else {
            rect->bottom = y;
        }
        if (rect->top == 0 && rect->bottom == 0) {
            rect->top = TOUCH_RECT_SKIP;
        }
        if (func_ov131_021d9428(ribbon, i) == ENTRY_NONE) {
            rect->top = TOUCH_RECT_SKIP;
        }
    }
    rects[ROW_COUNT].top = TOUCH_RECT_END;
    return func_02035c08(rects);
}

// pokebw2's PStaRibbon_SelectVisibleRow
void func_ov131_021d8ea8(PStatusWork *wk, PStaRibbonWork *ribbon) {
    func_ov131_021d8ed0(wk, ribbon, (ribbon->scrollY + ROW_HEIGHT - 1) / ROW_HEIGHT % ROW_COUNT);
}

// pokebw2's PStaRibbon_MoveCursor
static void func_ov131_021d8ed0(PStatusWork *wk, PStaRibbonWork *ribbon, u8 row) {
    ClActorPos pos;

    pos.x = ROW_X;
    pos.y = func_ov131_021d9440(ribbon, row);
    func_0204b404(ribbon->cursor, &pos, 0);
    ribbon->cursorRow = row;
    ribbon->cursorMoved = TRUE;
    ribbon->cursorEntry = func_ov131_021d9428(ribbon, row);
}

// pokebw2's PStaRibbon_Scroll
void func_ov131_021d8f0c(PStatusWork *wk, PStaRibbonWork *ribbon, s16 dy) {
    int max = (ribbon->ribbonCount - ROWS_SHOWN) * ROW_HEIGHT;

    if (max < 0) {
        max = 0;
    }
    if (dy > ROW_HEIGHT) {
        dy = ROW_HEIGHT;
    }
    if (dy < -ROW_HEIGHT) {
        dy = -ROW_HEIGHT;
    }
    if ((s16)ribbon->scrollY + dy < 0) {
        ribbon->scrollY = 0;
    } else if (ribbon->scrollY + dy > max) {
        ribbon->scrollY = max;
    } else {
        ribbon->scrollY = ribbon->scrollY + dy;
    }
    if (dy != 0) {
        ribbon->needsRedraw = TRUE;
    }
}

// pokebw2's PStaRibbon_Load
void func_ov131_021d8f50(PStatusWork *wk, PStaRibbonWork *ribbon) {
    u8 i;

    ribbon->cursorRow = ROW_NONE;
    ribbon->cursorEntry = CURSOR_NONE;
    ribbon->prevCursorRow = ROW_NONE;
    ribbon->scrollY = 0;
    ribbon->isShown = TRUE;
    ribbon->isDetailShown = FALSE;
    ribbon->isDetailPending = FALSE;
    ribbon->needsRedraw = TRUE;
    ribbon->nameWindow = NULL;
    ribbon->descWindow = NULL;
    for (i = 0; i < ROW_COUNT; i++) {
        ribbon->rows[i].entry = func_ov131_021d9428(ribbon, i);
        func_ov131_021d91f0(wk, ribbon, &ribbon->rows[i]);
    }
}

void func_ov131_021dbe2c(PStaOamActor *oam, s16 x, s16 y);
void func_ov131_021dbcf4(PStaOamActor *oam);
void func_ov131_021d8fac(PStatusWork *wk, PStaRibbonWork *ribbon) {
    u8 i;
    u16 entry;

    GFL_BGSysLoadScrArea(2, 0, 0, 32, 24, ribbon->screens[0].screen->rawData, 0, 0, 32, 32);
    GFL_BGSysQueueScrLoad(2);
    GFL_BGSysBufferScrDefault(6, ribbon->screens[1].screen->rawData, ribbon->screens[1].screen->szByte);
    GFL_BGSysQueueScrLoad(6);
    GFL_BGSysLoadScrArea(5, 0, 0, 32, 3, ribbon->screens[3].screen->rawData, 0, 0, 32, 32);
    GFL_BGSysQueueScrLoad(5);
    for (i = 0; i < ROW_COUNT; i++) {
        entry = ribbon->rows[i].entry;
        func_ov131_021dbe2c(ribbon->rows[i].oam, ROW_X, func_ov131_021d9440(ribbon, i));
        if (entry != ENTRY_NONE) {
            func_ov131_021dbca4(ribbon->rows[i].oam, TRUE);
            func_ov131_021dbcf4(ribbon->rows[i].oam);
        } else {
            func_ov131_021dbca4(ribbon->rows[i].oam, FALSE);
        }
    }
    ribbon->scrollY = 0;
    ribbon->cursorRow = ROW_NONE;
    ribbon->cursorEntry = CURSOR_NONE;
    ribbon->prevCursorRow = ROW_NONE;
    ribbon->isShown = TRUE;
    ribbon->needsRedraw = TRUE;
    func_0204b878(wk->unk178[2], 5);
}


// pokebw2's PStaRibbon_Unload
void func_ov131_021d909c(PStatusWork *wk, PStaRibbonWork *ribbon) {
    ribbon->isShown = FALSE;
    if (ribbon->nameWindow != NULL) {
        BmpWin_Free(ribbon->nameWindow);
        ribbon->nameWindow = NULL;
    }
    if (ribbon->descWindow != NULL) {
        BmpWin_Free(ribbon->descWindow);
        ribbon->descWindow = NULL;
    }
}

void func_ov131_021d90c0(PStatusWork *wk, PStaRibbonWork *ribbon) {
    u8 i;

    for (i = 0; i < ROW_COUNT; i++) {
        func_ov131_021dbca4(ribbon->rows[i].oam, FALSE);
    }
    GFL_BGSysFillScrArea(1, 0, 0, 0, 19, 21, 16);
    GFL_BGSysLoadScr(1);
    ribbon->isShown = FALSE;
    func_0204b878(wk->unk178[2], 2);
}


// pokebw2's PStaRibbon_LoadPokemon
void func_ov131_021d911c(PStatusWork *wk, PStaRibbonWork *ribbon) {
    u8 counts[RIBBON_CATEGORY_COUNT] = { 1, 1, 1, 1, 1 };
    BoxPkm *pkm = func_ov131_021d6064(wk);
    u8 i;
    u8 count;
    u8 category;

    for (i = 0; i < RIBBON_COUNT; i++) {
        ribbon->entries[i].isValid = FALSE;
    }
    count = 0;
    for (i = 0; i < RIBBON_COUNT; i++) {
        if (PML_PkmGetParam(pkm, func_ov131_021dbf2c(i, RIBBON_DATA_PARAM), NULL) == TRUE) {
            category = func_ov131_021dbf2c(i, RIBBON_DATA_CATEGORY);
            ribbon->entries[count].isValid = TRUE;
            ribbon->entries[count].ribbon = i;
            ribbon->entries[count].category = category;
            ribbon->entries[count].number = counts[category];
            counts[category]++;
            count++;
        }
    }
    ribbon->ribbonCount = count;
    for (i = 0; i < ROW_COUNT; i++) {
        ribbon->rows[i].entry = func_ov131_021d9428(ribbon, i);
    }
}

// pokebw2's PStaRibbon_UnloadPokemon
void func_ov131_021d91ec(PStatusWork *wk, PStaRibbonWork *ribbon) {
}

void func_ov131_021d91f0(PStatusWork *wk, PStaRibbonWork *ribbon, RibbonRow *row) {
    RibbonEntry *entry = &ribbon->entries[row->entry];
    u8 *src;
    u8 *dst;
    StrBuf *format;
    StrBuf *str;
    WordSet *wordSet;

    if (entry->isValid == TRUE) {
        src = ribbon->rowChars->pRawData;
        dst = GFL_BitmapGetPixelData(row->bitmap);
        if (row->entry == ribbon->cursorEntry) {
            src += ROW_CHAR_SIZE;
        }
        MI_CpuCopy8(src, dst, ROW_CHAR_SIZE);
        format = GFL_MsgDataLoadStrbufNew(ribbon->msgData, MSG_CATEGORY_FIRST + entry->category);
        str = GFL_StrBufCreate(32, wk->heapId);
        wordSet = GFL_WordSetSystemCreateDefault(wk->heapId);
        WordSetNumber(wordSet, 0, entry->number, 2, 2, 1);
        GFL_WordSetFormatStrbuf(wordSet, str, format);
        GFL_TextRndUpdateColorIndexLUT(1, 2, 0);
        GFL_TextRendererDrawToBitmap(row->bitmap, 10, 6, str, wk->font);
        func_0201dd68();
        GFL_StrBufFree(format);
        GFL_StrBufFree(str);
        GFL_WordSetSystemFree(wordSet);
        func_ov131_021dbcf4(row->oam);
    }
}

void func_ov131_021d92b0(PStatusWork *wk, PStaRibbonWork *ribbon) {
    BOOL redrawn = FALSE;
    u8 i;
    u32 entry;
    s16 y;

    for (i = 0; i < ROW_COUNT; i++) {
        if (ribbon->needsRedraw == TRUE) {
            y = func_ov131_021d9440(ribbon, i);
            func_ov131_021dbe2c(ribbon->rows[i].oam, ROW_X, y);
            entry = func_ov131_021d9428(ribbon, i);
            if (entry == ENTRY_NONE) {
                ribbon->rows[i].entry = entry;
                func_ov131_021dbca4(ribbon->rows[i].oam, FALSE);
            } else if (entry != ribbon->rows[i].entry) {
                redrawn = TRUE;
                ribbon->rows[i].entry = entry;
                func_ov131_021d91f0(wk, ribbon, &ribbon->rows[i]);
                func_ov131_021dbca4(ribbon->rows[i].oam, TRUE);
            }
        }
        if (ribbon->rows[i].isPrinting == TRUE && func_0201c6c4(wk->printQueue, ribbon->rows[i].bitmap) == FALSE) {
            ribbon->rows[i].isPrinting = FALSE;
            func_ov131_021dbcf4(ribbon->rows[i].oam);
        }
    }
    if (redrawn == TRUE && ribbon->seWait == 0) {
        GFL_SEPlayKeepVol(SEQ_SE_SYS_06, 1);
        ribbon->seWait = 3;
    }
    if (ribbon->cursorMoved == TRUE) {
        if (ribbon->cursorRow < ROW_COUNT) {
            func_ov131_021d91f0(wk, ribbon, &ribbon->rows[ribbon->cursorRow]);
        }
        if (ribbon->prevCursorRow < ROW_COUNT) {
            func_ov131_021d91f0(wk, ribbon, &ribbon->rows[ribbon->prevCursorRow]);
        }
        ribbon->prevCursorRow = ribbon->cursorRow;
        ribbon->cursorMoved = FALSE;
    }
    ribbon->needsRedraw = FALSE;
}



// pokebw2's PStaRibbon_GetRowIndex
static u8 func_ov131_021d93e4(PStaRibbonWork *ribbon, u8 row) {
    u8 top = ribbon->scrollY / ROW_HEIGHT;
    s8 offset = row - (u8)(top % ROW_COUNT);

    if (offset < -2) {
        offset += ROW_COUNT;
    }
    if (offset > 8) {
        offset -= ROW_COUNT;
    }
    return top + offset;
}

// pokebw2's PStaRibbon_GetRowEntry
static u32 func_ov131_021d9428(PStaRibbonWork *ribbon, u8 row) {
    u32 entry = func_ov131_021d93e4(ribbon, row);

    if (entry >= ribbon->ribbonCount) {
        entry = ENTRY_NONE;
    }
    return entry;
}

// pokebw2's PStaRibbon_GetRowY
static s16 func_ov131_021d9440(PStaRibbonWork *ribbon, u8 row) {
    u8 top = ribbon->scrollY / ROW_HEIGHT;
    u8 scroll = ribbon->scrollY % ROW_HEIGHT;
    s8 offset = row - (u8)(top % ROW_COUNT);

    if (offset < -2) {
        offset += ROW_COUNT;
    }
    if (offset > 8 && top > ROWS_SHOWN) {
        offset -= ROW_COUNT;
    }
    return offset * ROW_HEIGHT + LIST_TOP - scroll;
}

// pokebw2's PStaRibbon_PrintDetail
void func_ov131_021d949c(PStatusWork *wk, PStaRibbonWork *ribbon) {
    u8 id;
    StrBuf *str;

    ribbon->nameWindow = BmpWin_CreateDynamic(4, 5, 7, 22, 2, 14, 1);
    ribbon->descWindow = BmpWin_CreateDynamic(4, 1, 16, 30, 4, 14, 1);
    id = ribbon->entries[ribbon->cursorEntry].ribbon;
    str = GFL_MsgDataLoadStrbufNew(ribbon->msgData, func_ov131_021dbf2c(id, RIBBON_DATA_NAME));
    func_0201c724(wk->printQueue, BmpWin_GetBitmap(ribbon->nameWindow), 1, 1, str, wk->font, 0x440);
    GFL_StrBufFree(str);
    str = GFL_MsgDataLoadStrbufNew(ribbon->msgData, func_ov131_021dbf94(id));
    func_0201c724(wk->printQueue, BmpWin_GetBitmap(ribbon->descWindow), 1, 1, str, wk->font, 0x440);
    GFL_StrBufFree(str);
    ribbon->isDetailPending = TRUE;
}

void func_ov131_021d9568(PStatusWork *wk, PStaRibbonWork *ribbon) {
    u8 id = ribbon->entries[ribbon->cursorEntry].ribbon;
    BmpWin *window;
    ArcTool *arc;
    ClActorSetup setup;

    window = ribbon->nameWindow;
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(window));
    window = ribbon->descWindow;
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(window));
    arc = GFL_ArcSysCreateFileHandle(ARCID_P_STATUS, wk->heapId);
    ribbon->iconChars = func_0204a6c8(arc, func_ov131_021dbf2c(id, 1), FALSE, 1, wk->heapId);
    GFL_ArcToolFree(arc);
    setup.x = 128;
    setup.y = 100;
    setup.priority = 10;
    setup.bgPriority = 0;
    setup.sequence = 0;
    ribbon->icon = func_0204b294(wk->actorUnit, ribbon->iconChars, wk->unk98[PSTA_RES_PLTT(7)],
                                 wk->unk98[PSTA_RES_CELL(11)], &setup, 1, wk->heapId);
    func_0204b6f4(ribbon->icon, (u8)func_ov131_021dbf2c(id, 2), 1);
    func_0204b3dc(ribbon->icon, TRUE);
    GFL_BGSysBufferScrDefault(6, ribbon->screens[2].screen->rawData, ribbon->screens[2].screen->szByte);
    GFL_BGSysQueueScrLoad(6);
    ribbon->isDetailShown = TRUE;
}


// pokebw2's PStaRibbon_FreeDetailWindows
void func_ov131_021d964c(PStatusWork *wk, PStaRibbonWork *ribbon) {
    if (ribbon->nameWindow != NULL) {
        BmpWin_Free(ribbon->nameWindow);
        ribbon->nameWindow = NULL;
    }
    if (ribbon->descWindow != NULL) {
        BmpWin_Free(ribbon->descWindow);
        ribbon->descWindow = NULL;
    }
}

void func_ov131_021d9670(PStatusWork *wk, PStaRibbonWork *ribbon) {
    if (ribbon->isDetailShown == TRUE) {
        func_0204a8d4(ribbon->iconChars);
        func_0204b3b4(ribbon->icon);
        GFL_BGSysFillScrAsync(4, 0);
        GFL_BGSysQueueScrLoad(4);
        ribbon->isDetailShown = FALSE;
        GFL_BGSysBufferScrDefault(6, &ribbon->screens[1].screen->rawData, ribbon->screens[1].screen->szByte);
        GFL_BGSysQueueScrLoad(6);
    }
}


