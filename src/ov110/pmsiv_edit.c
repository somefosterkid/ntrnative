// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov185/pmsiv_edit.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "app/pmsiv_edit.h"
#include "types.h"
#include "app/pms_input.h"
#include "app/pms_input_view.h"
#include "constants/arc.h"
#include "constants/text_banks.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "nitro/gx.h"
#include "nitro/os.h"
#include "system/bgwinfrm.h"
#include "system/bmp_cursor.h"
#include "system/gf_font.h"
#include "system/pms_data.h"
#include "system/pms_word.h"
#include "system/pmsi_param.h"
#include "system/printsys.h"

// The phrase input's edit area: the sentence or the words being written, drawn on BG0 of the upper screen and BG4 of
// the lower one, with a cursor on each screen and number words drawn as actors. The names are ours, guessed

// What the parser of the sentence's text finds next: text, a word's command, a new line, or the end
enum {
    PARSE_TEXT,
    PARSE_WORD,
    PARSE_NEWLINE,
    PARSE_END,
};

// The characters that end a run of text
#define CHAR_COMMAND 0xf000
#define CHAR_NEWLINE 0xfffe
#define CHAR_EOS 0xffff

// The message file of the phrase input's texts
#define PMSIV_EDIT_MSG_FILE TEXT_BANK_PMS_INPUT

typedef struct {
    StrBuf *str;
    const u16 *cur;
    u32 state;
    u16 param;
} PMSIVEditParser;

// Black 2's; twinned code confirms the offsets of vwk, mwk, win, cursor, numberAct, msgData, str, bmpCursor, wordPos,
// wordIndex, wordCount, parser, palette, frameY, scrollY, scrollCount, scrollUp, hblankTask, unk1A8, unk1AC,
// vblankTask, frames and framesDirty in Black
struct PMSIVEdit {
    PMSInputView *vwk;
    const PMSInputWork *mwk;
    const PMSInputData *dwk;
    // The area's window on each screen
    BmpWin *win[2];
    ClActor *cursor[2];
    // The actors that show number words, for each word place on each screen
    ClActor *numberAct[2][2];
    MsgData *msgData;
    StrBuf *str;
    BmpCursor *bmpCursor;
    ClActorPos wordPos[2];
    u16 wordIndex[2];
    u32 wordCount;
    PMSIVEditParser parser;
    u16 palette[160];
    s16 frameY;
    s16 scrollY;
    u8 scrollCount;
    u8 scrollUp;
    int *keyMode;
    TCB *hblankTask;
    u32 unk1A8;
    u32 unk1AC;
    TCB *vblankTask;
    void *frames;
    BOOL framesDirty;
};

static void func_ov110_021bbe80(TCB *tcb, void *data);
static void func_ov110_021bbe8c(TCB *tcb, void *data);
static void func_ov110_021bc068(PMSIVEdit *wk, ArcTool *arc);
static void func_ov110_021bc0cc(PMSIVEdit *wk);
static void func_ov110_021bc120(PMSIVEdit *wk);
static void func_ov110_021bc160(PMSIVEdit *wk);
static void func_ov110_021bc1b8(PMSIVEdit *wk);
static u32 func_ov110_021bc340(PMSIVEdit *wk, BmpWin *win, u8 lcd);
static void func_ov110_021bc458(PMSIVEditParser *parser, PMSIVEdit *wk);
static void func_ov110_021bc484(PMSIVEditParser *parser);
static u32 func_ov110_021bc490(PMSIVEditParser *parser, StrBuf *buf);
static void func_ov110_021bc564(const ClActorPos *pos, ClActorPos *out);
static void func_ov110_021bc578(const ClActorPos *pos, ClActorPos *out);
static void func_ov110_021bc58c(BmpWin *win, const ClActorPos *pos);
static void func_ov110_021bc5bc(PMSIVEdit *wk, BmpWin *win, const ClActorPos *pos, u16 word, ClActor *numberAct);
static void func_ov110_021bc69c(PMSIVEdit *wk, BOOL active);
static u32 func_ov110_021bc6d0(u32 bg);

// pokebw2's PMSIVEdit_Create
PMSIVEdit *func_ov110_021bbc5c(PMSInputView *vwk, const PMSInputWork *mwk, const PMSInputData *dwk) {
    PMSIVEdit *wk = GFL_HeapAllocate(HEAPID_PMS_INPUT, sizeof(PMSIVEdit), TRUE, "pmsiv_edit.c", 211);

    wk->vwk = vwk;
    wk->mwk = mwk;
    wk->dwk = dwk;
    wk->keyMode = func_ov110_021bf6d4(mwk);
    wk->cursor[0] = NULL;
    wk->cursor[1] = NULL;
    wk->hblankTask = NULL;
    wk->str = GFL_StrBufCreate(128, HEAPID_PMS_INPUT);
    wk->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, PMSIV_EDIT_MSG_FILE, HEAPID_PMS_INPUT);
    wk->bmpCursor = BmpCursor_Create(HEAPID_PMS_INPUT);
    wk->frameY = 0;
    wk->scrollY = 0;
    wk->unk1A8 = 0;
    wk->unk1AC = 0;
    wk->vblankTask = NULL;
    wk->frames = func_0202c9b4(1, 1, HEAPID_PMS_INPUT);
    return wk;
}

// pokebw2's PMSIVEdit_Delete
void func_ov110_021bbcf0(PMSIVEdit *wk) {
    func_0202ca0c(wk->frames);
    if (wk->vblankTask) {
        GFL_TCBRemove(wk->vblankTask);
    }
    if (wk->hblankTask) {
        GFL_TCBRemove(wk->hblankTask);
    }
    if (wk->bmpCursor) {
        func_02020d3c(wk->bmpCursor);
    }
    if (wk->cursor[0]) {
        func_0204b3b4(wk->cursor[0]);
    }
    if (wk->cursor[1]) {
        func_0204b3b4(wk->cursor[1]);
    }
    if (wk->msgData) {
        GFL_MsgDataFree(wk->msgData);
    }
    if (wk->str) {
        GFL_StrBufFree(wk->str);
    }
    BmpWin_Free(wk->win[0]);
    BmpWin_Free(wk->win[1]);
    GFL_HeapFree(wk);
}

// pokebw2's PMSIVEdit_SetupGraphicDatas
void func_ov110_021bbd60(PMSIVEdit *wk, ArcTool *arc) {
    func_ov110_021bc068(wk, arc);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 25, 0, 0, 0, FALSE, HEAPID_PMS_INPUT);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 25, 4, 0, 0, FALSE, HEAPID_PMS_INPUT);
    wk->scrollY = -192;
    GFL_BGSysMoveBG(4, BG_MOVE_SET_Y, wk->scrollY);
    GFL_BGSysLoadArcNCGRStatic(arc, 14, 0, 0, 0, FALSE, HEAPID_PMS_INPUT);
    GFL_BGSysLoadArcNCGRStatic(arc, 14, 4, 0, 0, FALSE, HEAPID_PMS_INPUT);
    wk->win[0] = BmpWin_CreateDynamic(0, 3, 1, 28, 4, 0, TRUE);
    wk->win[1] = BmpWin_CreateDynamic(4, 3, 1, 28, 4, 0, TRUE);
    BmpWin_FlushMap(wk->win[0]);
    BmpWin_FlushMap(wk->win[1]);
    func_ov110_021bc120(wk);
    func_ov110_021bc160(wk);
    func_ov110_021bc23c(wk);
    func_ov110_021bc1b8(wk);
    func_0202ca3c(wk->frames, 0, 0, 32, 6);
    func_0202cb18(wk->frames, 0, arc, 25, FALSE);
    func_0202cb4c(wk->frames, 0, 0, 0);
    func_0202cebc(wk->frames, 0, wk->win[0]);
    func_0202cc58(wk->frames, 0);
    wk->framesDirty = FALSE;
}

// pokebw2's PMSIVEdit_HBlankTask
static void func_ov110_021bbe80(TCB *tcb, void *data) {
    GX_GetVCount();
}

// pokebw2's PMSIVEdit_VBlankTask
static void func_ov110_021bbe8c(TCB *tcb, void *data) {
    PMSIVEdit *wk = data;

    wk->unk1A8 = 0;
    wk->unk1AC = 0;
    if (wk->framesDirty) {
        GFL_BGSysFillScrArea(0, 0, 0, 0, 32, 6, BGSYS_FILL_TILE_PALETTE);
        func_0202cb4c(wk->frames, 0, 0, -wk->frameY / 8);
        func_0202cc58(wk->frames, 0);
        wk->framesDirty = FALSE;
    }
}

// pokebw2's PMSIVEdit_ScrollSet
void func_ov110_021bbef4(PMSIVEdit *wk, BOOL up) {
    wk->scrollCount = 0;
    wk->scrollUp = up;
    if (wk->hblankTask) {
        GFL_TCBRemove(wk->hblankTask);
    }
    wk->hblankTask = GFL_HBlankTCBAdd(func_ov110_021bbe80, wk, 1);
    wk->unk1A8 = 0;
    wk->unk1AC = 0;
    if (wk->vblankTask) {
        GFL_TCBRemove(wk->vblankTask);
    }
    wk->vblankTask = GFL_VBlankTCBAdd(func_ov110_021bbe8c, wk, 1);
}

// pokebw2's PMSIVEdit_ScrollWait
BOOL func_ov110_021bbf54(PMSIVEdit *wk) {
    int step;
    int i, j;
    s16 y;

    if (wk->scrollCount > 5) {
        return TRUE;
    }
    if (wk->scrollUp) {
        step = -8;
    } else {
        step = 8;
    }
    wk->scrollY = wk->scrollY + step;
    wk->frameY = wk->frameY + step;
    GFL_BGSysMoveBG(4, BG_MOVE_SET_Y, wk->scrollY);
    for (i = 0; i < 2; i++) {
        y = func_0204b4ec(wk->cursor[i], i, 1) - step;
        func_0204b49c(wk->cursor[i], y, i, 1);
        for (j = 0; j < 2; j++) {
            y = func_0204b4ec(wk->numberAct[j][i], i, 1) - step;
            func_0204b49c(wk->numberAct[j][i], y, i, 1);
        }
    }
    wk->scrollCount++;
    // The count stops at 6, so the tasks are only removed by the next scroll or by func_ov110_021bbcf0
    if (wk->scrollUp && wk->scrollCount == 7) {
        GFL_TCBRemove(wk->hblankTask);
        wk->hblankTask = NULL;
        GFL_TCBRemove(wk->vblankTask);
        wk->vblankTask = NULL;
    }
    wk->framesDirty = TRUE;
    return FALSE;
}

// pokebw2's PMSIVEdit_SetupPalette
static void func_ov110_021bc068(PMSIVEdit *wk, ArcTool *arc) {
    NNSG2dPaletteData *palette;
    void *buf;

    GFL_G2DIOLoadArcNCLRDefault(arc, 3, 0, 0, 0x1c0, HEAPID_PMS_INPUT);
    GFL_G2DIOLoadArcNCLRDefault(arc, 4, 4, 0, 0xa0, HEAPID_PMS_INPUT);
    buf = GFL_G2DIOReadNCLRArc(arc, 6, &palette, HEAPID_PMS_INPUT);
    MIi_CpuCopy16(palette->pRawData, wk->palette, sizeof(wk->palette));
    DC_FlushRange(wk->palette, sizeof(wk->palette));
    GFL_HeapFree(buf);
}

// pokebw2's PMSIVEdit_UpdatePalette
static void func_ov110_021bc0cc(PMSIVEdit *wk) {
    if (func_ov110_021bf6d8(wk->mwk) == PMSI_MODE_SENTENCE && func_ov110_021bf6e4(wk->mwk) < 5) {
        u32 offset = (func_ov110_021bf6e4(wk->mwk) + 1) * 16;

        GX_LoadBGPltt(&wk->palette[offset], 0, 0x20);
        GXS_LoadBGPltt(&wk->palette[offset], 0, 0x20);
    } else {
        GX_LoadBGPltt(wk->palette, 0, 0x20);
        GXS_LoadBGPltt(wk->palette, 0, 0x20);
    }
}

// pokebw2's PMSIVEdit_SetupWordPos
static void func_ov110_021bc120(PMSIVEdit *wk) {
    switch (func_ov110_021bf6d8(wk->mwk)) {
    case PMSI_MODE_WORD:
        wk->wordPos[0].x = 104;
        wk->wordPos[0].y = 16;
        wk->wordCount = 1;
        break;
    case PMSI_MODE_DOUBLE_WORD:
        wk->wordPos[0].x = 48;
        wk->wordPos[0].y = 16;
        wk->wordPos[1].x = 160;
        wk->wordPos[1].y = 16;
        wk->wordCount = 2;
        break;
    case PMSI_MODE_SENTENCE:
        wk->wordCount = 0;
        break;
    }
}

// pokebw2's PMSIVEdit_SetupWordActors
static void func_ov110_021bc160(PMSIVEdit *wk) {
    PMSIVObjRes res;
    int i, j;

    for (i = 0; i < 2; i++) {
        func_ov110_021bac50(wk->vwk, &res, i);
        for (j = 0; j < 2; j++) {
            wk->numberAct[j][i] = func_ov110_021bac78(wk->vwk, &res, 0, 0, 2, i + 1);
            func_0204b3dc(wk->numberAct[j][i], FALSE);
        }
    }
}

// pokebw2's PMSIVEdit_SetupCursorActors
static void func_ov110_021bc1b8(PMSIVEdit *wk) {
    PMSIVObjRes res;
    ClActorPos pos;

    if (wk->wordCount) {
        func_ov110_021bc578(&wk->wordPos[0], &pos);
    } else {
        pos.x = 128;
        pos.y = 24;
    }
    func_ov110_021bac64(wk->vwk, &res, 1, 0);
    wk->cursor[1] = func_ov110_021bac78(wk->vwk, &res, pos.x, pos.y + 192, 0, NNS_G2D_VRAM_TYPE_2DSUB);
    func_0204b878(wk->cursor[1], 1);
    func_0204b84c(wk->cursor[1], 1);
    func_ov110_021bac64(wk->vwk, &res, 0, 0);
    wk->cursor[0] = func_ov110_021bac78(wk->vwk, &res, pos.x, pos.y, 0, NNS_G2D_VRAM_TYPE_2DMAIN);
    func_ov110_021bc69c(wk, TRUE);
}

// pokebw2's PMSIVEdit_UpdateEditArea
void func_ov110_021bc23c(PMSIVEdit *wk) {
    int lcd, i;

    func_ov110_021bc0cc(wk);
    for (lcd = 0; lcd < 2; lcd++) {
        for (i = 0; i < 2; i++) {
            if (wk->numberAct[i][lcd]) {
                func_0204b3dc(wk->numberAct[i][lcd], FALSE);
            }
        }
        GFL_BitmapFill(BmpWin_GetBitmap(wk->win[lcd]), 15);
        switch (func_ov110_021bf6d8(wk->mwk)) {
        case PMSI_MODE_WORD:
            func_ov110_021bc58c(wk->win[lcd], &wk->wordPos[0]);
            func_ov110_021bc5bc(wk, wk->win[lcd], &wk->wordPos[0], func_ov110_021bf6f0(wk->mwk, 0),
                                wk->numberAct[0][lcd]);
            break;
        case PMSI_MODE_DOUBLE_WORD:
            func_ov110_021bc58c(wk->win[lcd], &wk->wordPos[0]);
            func_ov110_021bc58c(wk->win[lcd], &wk->wordPos[1]);
            func_ov110_021bc5bc(wk, wk->win[lcd], &wk->wordPos[0], func_ov110_021bf6f0(wk->mwk, 0),
                                wk->numberAct[0][lcd]);
            func_ov110_021bc5bc(wk, wk->win[lcd], &wk->wordPos[1], func_ov110_021bf6f0(wk->mwk, 1),
                                wk->numberAct[1][lcd]);
            break;
        case PMSI_MODE_SENTENCE:
            wk->wordCount = func_ov110_021bc340(wk, wk->win[lcd], lcd);
            break;
        }
        BmpWin_FlushChar(wk->win[lcd]);
    }
    GFL_BGSysQueueScrLoad(0);
    GFL_BGSysQueueScrLoad(4);
}

// pokebw2's PMSIVEdit_GetWordArea
void func_ov110_021bc320(PMSIVEdit *wk, TouchRect *rect, u32 index) {
    rect->left = wk->wordPos[index].x - 24;
    rect->right = rect->left + 96;
    rect->top = wk->wordPos[index].y;
    rect->bottom = rect->top + 16;
}

// pokebw2's PMSIVEdit_PrintSentence
static u32 func_ov110_021bc340(PMSIVEdit *wk, BmpWin *win, u8 lcd) {
    StrBuf *str;
    u16 index;
    BOOL loop;
    Font *font;
    u32 count;
    int y;
    int x;

    str = func_ov110_021bf708(wk->mwk, HEAPID_PMS_INPUT);
    font = func_ov110_021bac44(wk->vwk);
    loop = TRUE;
    count = 0;
    y = 0;
    x = 0;

    func_ov110_021bc458(&wk->parser, wk);
    while (loop) {
        switch (func_ov110_021bc490(&wk->parser, str)) {
        case PARSE_TEXT:
            GFL_TextRndUpdateColorIndexLUT(1, 2, 15);
            GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(win), x, y, str, font);
            x += GFL_FontGetBlockWidth(str, font, 0);
            break;
        case PARSE_WORD:
            index = wk->parser.param;
            wk->wordIndex[count] = index;
            wk->wordPos[count].x = x + 50;
            wk->wordPos[count].y = y + 8;
            func_ov110_021bc58c(win, &wk->wordPos[count]);
            func_ov110_021bc5bc(wk, win, &wk->wordPos[count], func_ov110_021bf6f0(wk->mwk, index),
                                wk->numberAct[count][lcd]);
            count++;
            x += 100;
            break;
        case PARSE_NEWLINE:
            y += 16;
            x = 0;
            break;
        case PARSE_END:
            loop = FALSE;
            break;
        }
    }
    func_ov110_021bc484(&wk->parser);
    GFL_StrBufFree(str);
    return count;
}

// pokebw2's PMSIVEdit_ParserInit
static void func_ov110_021bc458(PMSIVEditParser *parser, PMSIVEdit *wk) {
    parser->str = func_ov110_021bf708(wk->mwk, HEAPID_PMS_INPUT);
    parser->cur = GFL_StrBufGetStringPtr(parser->str);
    if (*parser->cur == CHAR_COMMAND) {
        parser->state = PARSE_WORD;
    } else {
        parser->state = PARSE_TEXT;
    }
}

// pokebw2's PMSIVEdit_ParserEnd
static void func_ov110_021bc484(PMSIVEditParser *parser) {
    GFL_StrBufFree(parser->str);
}

// pokebw2's PMSIVEdit_ParserNext
static u32 func_ov110_021bc490(PMSIVEditParser *parser, StrBuf *buf) {
    const u16 *start = parser->cur;
    u32 type;

    switch (parser->state) {
    case PARSE_TEXT:
        while (parser->state == PARSE_TEXT) {
            switch (*parser->cur) {
            case CHAR_NEWLINE:
                parser->state = PARSE_NEWLINE;
                break;
            case CHAR_EOS:
                parser->state = PARSE_END;
                break;
            case CHAR_COMMAND:
                parser->state = PARSE_WORD;
                break;
            default:
                parser->cur++;
                break;
            }
        }
        GFL_StrBufLoadFixedString(buf, start, parser->cur - start + 1);
        return PARSE_TEXT;
    case PARSE_WORD:
        parser->param = GFL_WordSetGetCommandParameter(start, 0);
        parser->cur = GFL_StrCmdSkipCommand(parser->cur);
        type = PARSE_WORD;
        break;
    case PARSE_NEWLINE:
        parser->cur = start + 1;
        type = PARSE_NEWLINE;
        break;
    case PARSE_END:
    default:
        return PARSE_END;
    }
    switch (*parser->cur) {
    case CHAR_NEWLINE:
        parser->state = PARSE_NEWLINE;
        break;
    case CHAR_EOS:
        parser->state = PARSE_END;
        break;
    case CHAR_COMMAND:
        parser->state = PARSE_WORD;
        break;
    default:
        parser->state = PARSE_TEXT;
        break;
    }
    return type;
}

// pokebw2's PMSIVEdit_GetWordRectPos
static void func_ov110_021bc564(const ClActorPos *pos, ClActorPos *out) {
    out->x = pos->x - 48;
    out->y = pos->y - 8;
}

// pokebw2's PMSIVEdit_GetWordCursorPos
static void func_ov110_021bc578(const ClActorPos *pos, ClActorPos *out) {
    out->x = pos->x + 24;
    out->y = pos->y + 8;
}

// pokebw2's PMSIVEdit_ClearWord
static void func_ov110_021bc58c(BmpWin *win, const ClActorPos *pos) {
    ClActorPos rectPos;

    func_ov110_021bc564(pos, &rectPos);
    GFL_BitmapFillArea(BmpWin_GetBitmap(win), rectPos.x, rectPos.y, 96, 16, 14);
}

// pokebw2's PMSIVEdit_PrintWord
static void func_ov110_021bc5bc(PMSIVEdit *wk, BmpWin *win, const ClActorPos *pos, u16 word, ClActor *numberAct) {
    Font *font = func_ov110_021bac44(wk->vwk);
    ClActorPos rectPos;
    ClActorPos actPos;

    if (word == PMS_WORD_NULL) {
        return;
    }
    func_ov110_021bc564(pos, &rectPos);
    if (func_02024508(word)) {
        u32 number = func_02024520(word) - 1;
        u32 engine = func_ov110_021bc6d0(BmpWin_GetBGIndex(win));

        actPos.x = rectPos.x + BmpWin_GetPosX(win) * 8;
        actPos.y = rectPos.y + BmpWin_GetPosY(win) * 8;
        if (engine == 1) {
            actPos.y += 192;
        }
        func_0204b878(numberAct, number);
        func_0204b404(numberAct, &actPos, engine);
        func_0204b3dc(numberAct, TRUE);
        func_0204b84c(numberAct, GFL_BGSysGetBGPriority(BmpWin_GetBGIndex(win)));
    } else {
        loadSayingToString(word, wk->str, HEAPID_PMS_INPUT);
        GFL_TextRndUpdateColorIndexLUT(3, 4, 14);
        GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(win), rectPos.x, rectPos.y, wk->str, font);
    }
}

// pokebw2's PMSIVEdit_SetCursorActive
static void func_ov110_021bc69c(PMSIVEdit *wk, BOOL active) {
    if (wk->wordCount) {
        if (active) {
            func_0204b878(wk->cursor[0], 0);
        } else {
            func_0204b878(wk->cursor[0], 1);
        }
    } else {
        if (active) {
            func_0204b878(wk->cursor[0], 14);
        } else {
            func_0204b878(wk->cursor[0], 15);
        }
    }
}

// pokebw2's PMSIVEdit_GetBGEngine
static u32 func_ov110_021bc6d0(u32 bg) {
    if (bg >= BGSYS_BG_SUB) {
        return BGSYS_ENGINE_SUB;
    }
    return BGSYS_ENGINE_MAIN;
}

// pokebw2's PMSIVEdit_GetWordCount
u32 func_ov110_021bc6dc(PMSIVEdit *wk) {
    return wk->wordCount;
}

// pokebw2's PMSIVEdit_GetWordIndex
u16 func_ov110_021bc6e0(PMSIVEdit *wk, u32 index) {
    return wk->wordIndex[index];
}

// pokebw2's PMSIVEdit_StopCursor
void func_ov110_021bc6ec(PMSIVEdit *wk) {
    func_ov110_021bc69c(wk, FALSE);
}

// pokebw2's PMSIVEdit_ActiveCursor
void func_ov110_021bc6f8(PMSIVEdit *wk) {
    func_ov110_021bc69c(wk, TRUE);
}

// pokebw2's PMSIVEdit_VisibleCursor
void func_ov110_021bc704(PMSIVEdit *wk, BOOL visible) {
    func_0204b3dc(wk->cursor[0], visible);
    func_0204b3dc(wk->cursor[1], visible);
    func_ov110_021bc69c(wk, TRUE);
}

// pokebw2's PMSIVEdit_StopArrow
void func_ov110_021bc724(PMSIVEdit *wk) {
}

// pokebw2's PMSIVEdit_ActiveArrow
void func_ov110_021bc728(PMSIVEdit *wk) {
}

// pokebw2's PMSIVEdit_MoveCursor
void func_ov110_021bc72c(PMSIVEdit *wk, u32 pos) {
    ClActorPos actPos;
    ClActorPos cursorPos;

    if (wk->wordCount) {
        func_ov110_021bc578(&wk->wordPos[pos], &cursorPos);
        actPos.x = cursorPos.x;
        actPos.y = cursorPos.y;
    } else {
        actPos.x = 128;
        actPos.y = 24;
    }
    func_0204b404(wk->cursor[0], &actPos, CLACT_SURFACE_MAIN);
    actPos.y += 192;
    func_0204b404(wk->cursor[1], &actPos, CLACT_SURFACE_SUB);
    func_ov110_021bc69c(wk, TRUE);
}
