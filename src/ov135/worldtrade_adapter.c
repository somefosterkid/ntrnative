// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov214/worldtrade_adapter.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcbl.h"
#include "gfl/touchpanel.h"
#include "pml/poke_party.h"
#include "system/app_keycursor.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/text_speed.h"
#include "system/wordset.h"
#include "text/system/0415.h"
#include "worldtrade_local.h"

#include "save/trainer_data_savedata.h"

// func_020223b4's states




// The Global Trade Station's helpers over the game's systems: Pokémon copies, window clearing, the text printing
// through the print queue and the message stream, and a small printer of numbers. The names of its functions are
// ours, guessed

#define NUM_FONT_ENTRY_COUNT 8

// The color the number printer prints in
#define NUM_FONT_COLOR 0x3dc8
// The color func_ov135_02202858 prints in
#define PRINT_COLOR_DEFAULT 0x44f

struct WorldTradeNumFont {
    Font *font;
    PrintQueue *printQueue;
    WordSet *wordSet;
    MsgData *msgData;
    PrintWindow printWins[NUM_FONT_ENTRY_COUNT];
    BOOL active[NUM_FONT_ENTRY_COUNT];
    HeapID heapId;
};

// pokebw2's WorldTrade_SetBoxPkmNickname
void func_ov135_022025e4(WordSet *wordSet, u32 index, BoxPkm *pkm) {
    PartyPkm *partyPkm = boxPkmRegenToPartyPkm(pkm, HEAPID_WORLDTRADE);

    loadPokemonNicknameToStrbuf(wordSet, index, partyPkm);
    GFL_HeapFree(partyPkm);
}

// pokebw2's WorldTrade_AllocPartyPkm
PartyPkm *func_ov135_02202608(HeapID heapId) {
    return GFL_HeapAllocate(heapId, PokeParty_GetPkmRawSize(), FALSE, "worldtrade_adapter.c", 71);
}

// pokebw2's WorldTrade_CopyPartyPkm
void func_ov135_0220262c(PartyPkm *src, PartyPkm *dest) {
    MI_CpuCopy8(src, dest, PokeParty_GetPkmRawSize());
}

// pokebw2's WorldTrade_ClearWindow
void func_ov135_02202644(BmpWin *win, int mode) {
    func_020456f8(win);
    switch (mode) {
    case 0:
        GFL_BGSysLoadScr(BmpWin_GetBGIndex(win));
        break;
    case 1:
        GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(win));
        break;
    }
}

// pokebw2's WorldTrade_ExpandMessage
StrBuf *func_ov135_02202670(WordSet *wordSet, MsgData *msgData, u32 msgNo, HeapID heapId) {
    StrBuf *src = GFL_MsgDataLoadStrbufNew(msgData, msgNo);
    StrBuf *dest = GFL_StrBufCreate(0x200, heapId);

    GFL_WordSetFormatStrbuf(wordSet, dest, src);
    GFL_StrBufFree(src);
    return dest;
}

// pokebw2's WorldTrade_BoxPkmToPartyPkm
void func_ov135_022026a0(BoxPkm *pkm, PartyPkm *dest) {
    PartyPkm *partyPkm = boxPkmRegenToPartyPkm(pkm, HEAPID_WORLDTRADE);

    func_ov135_0220262c(partyPkm, dest);
    GFL_HeapFree(partyPkm);
}

// pokebw2's WorldTrade_GetBoxPkm
BoxPkm *func_ov135_022026bc(PartyPkm *pkm) {
    return func_0201d624(pkm);
}

// pokebw2's WorldTrade_GetStrWidth
int func_ov135_022026c4(WorldTradePrint *print, u8 font, StrBuf *str, int spacing) {
    return GFL_FontGetBlockWidth(str, print->font, (u16)spacing);
}

// Corrected struct layout for Black (entries at 0x18, keyCursor at 0x138)
void func_ov135_022026d8(WorldTradePrint *print, TrainerDataSave *config) {
    int i;

    MI_CpuFill8(print, 0, sizeof(WorldTradePrint));
    print->tcbManager = GFL_TCBExMgrCreate(HEAPID_WORLDTRADE, HEAPID_WORLDTRADE, 32, 32);
    print->config = config;
    print->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, HEAPID_WORLDTRADE);
    print->printQueue = func_0201c440(HEAPID_WORLDTRADE);
    print->keyCursor = func_02028590(15, 1, 1, HEAPID_WORLDTRADE);
    for (i = 0; i < PRINT_ENTRY_COUNT; i++) {
        WorldTradePrintEntry *entry = &print->entries[i];

        MI_CpuFill8(&entry->printWin, 0, sizeof(PrintWindow));
        entry->active = FALSE;
    }
}

void func_ov135_02202744(WorldTradePrint *print) {
    func_02028604(print->keyCursor);
    func_0201c4c0(print->printQueue);
    GFL_FontFree(print->font);
    GFL_TCBExMgrFree(print->tcbManager);
    MI_CpuFill8(print, 0, sizeof(WorldTradePrint));
}

void func_ov135_02202770(WorldTradePrint *print) {
    int i;

    for (i = 0; i < PRINT_ENTRY_COUNT; i++) {
        WorldTradePrintEntry *entry = &print->entries[i];

        if (entry->active) {
            PrintQueue *queue = print->printQueue;
            if (entry->printWin.flushPending) {
                if (!func_0201c6c4(queue, BmpWin_GetBitmap(entry->printWin.window))) {
                    BmpWin_FlushChar(entry->printWin.window);
                    entry->printWin.flushPending = FALSE;
                }
            }
            if (!entry->printWin.flushPending) {
                entry->active = FALSE;
            }
        }
    }

    if (print->stream != NULL) {
        func_020286c4(print->keyCursor, print->stream, print->streamWin);
        switch (func_0201ce30(print->stream)) {
        case PRINT_STREAM_RUNNING:
            if ((GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) || func_02035c60()) {
                func_0201ce5c(print->stream, 0);
            }
            break;
        case PRINT_STREAM_PAUSED:
            if ((GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) || func_02035c60()) {
                GFL_SndSEPlay(SEQ_SE_MESSAGE);
                func_0201ce38(print->stream);
            }
            break;
        case PRINT_STREAM_DONE:
            func_0201ce48(print->stream);
            print->stream = NULL;
            print->streamWin = NULL;
            break;
        }
    }

    func_0201c4e4(print->printQueue);
    GFL_TCBExMgrUpdate(print->tcbManager);
}




// pokebw2's WorldTrade_PrintIsBusy
BOOL func_ov135_02202840(WorldTradePrint *print) {
    BOOL busy = FALSE;

    if (print->stream != NULL && func_0201ce30(print->stream) != PRINT_STREAM_DONE) {
        busy = TRUE;
    }
    return busy;
}

void func_ov135_022028c8(BmpWin *win, u8 font, StrBuf *str, int x, int y, u8 align, u16 color, WorldTradePrint *print);
void func_ov135_02202858(BmpWin *win, u8 font, StrBuf *str, int x, int y, WorldTradePrint *print) {
    func_ov135_022028c8(win, font, str, x, y, 0, PRINT_COLOR_DEFAULT, print);
}


// pokebw2's WorldTrade_StreamPrint
void func_ov135_02202878(BmpWin *win, u8 font, StrBuf *str, int x, int y, WorldTradePrint *print) {
    if (print->stream != NULL) {
        func_0201ce48(print->stream);
        print->stream = NULL;
    }
    print->stream =
        func_0201cce4(win, x, y, str, print->font, func_02012ffc(), print->tcbManager, 0, HEAPID_WORLDTRADE, 15);
    print->streamWin = win;
}

void func_ov135_022028c8(BmpWin *win, u8 font, StrBuf *str, int x, int y, u8 align, u16 color, WorldTradePrint *print) {
    WorldTradePrintEntry *entry = NULL;
    PrintQueue *queue;
    int i;

    for (i = 0; i < PRINT_ENTRY_COUNT; i++) {
        if (!print->entries[i].active) {
            entry = &print->entries[i];
        }
    }
    GFL_ASSERT(entry != NULL);

    entry->printWin.window = win;
    entry->printWin.flushPending = FALSE;
    queue = print->printQueue;
    func_0201c724(queue, BmpWin_GetBitmap(win), x, y, str, print->font, color);
    entry->printWin.flushPending = TRUE;
    entry->active = TRUE;
}


// pokebw2's WorldTrade_PrintClear
void func_ov135_02202940(WorldTradePrint *print) {
    int i;

    for (i = 0; i < PRINT_ENTRY_COUNT; i++) {
        WorldTradePrintEntry *entry = &print->entries[i];

        if (entry->active) {
            entry->active = FALSE;
        }
    }
    if (print->stream != NULL) {
        func_0201ce48(print->stream);
        print->stream = NULL;
        print->streamWin = NULL;
    }
}

typedef struct WorldTradeNumFont WorldTradeNumFont;
WorldTradeNumFont *func_ov135_02202974(u32 unused0, u32 unused1, u32 unused2, HeapID heapId) {
    WorldTradeNumFont *numFont =
        GFL_HeapAllocate(heapId, sizeof(WorldTradeNumFont), FALSE, "worldtrade_adapter.c", 477);

    MI_CpuFill8(numFont, 0, sizeof(WorldTradeNumFont));
    numFont->heapId = heapId;
    numFont->font = GFL_FontCreate(ARCID_FONT, 3, 0, FALSE, heapId);
    numFont->printQueue = func_0201c440(heapId);
    numFont->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_0415, heapId);
    numFont->wordSet = GFL_WordSetSystemCreateDefault(heapId);
    return numFont;
}

void func_ov135_022029d4(WorldTradeNumFont *numFont) {
    GFL_WordSetSystemFree(numFont->wordSet);
    GFL_MsgDataFree(numFont->msgData);
    func_0201c4c0(numFont->printQueue);
    GFL_FontFree(numFont->font);
    GFL_HeapFree(numFont);
}

void func_ov135_022029f8(WorldTradeNumFont *numFont) {
    int i;

    for (i = 0; i < NUM_FONT_ENTRY_COUNT; i++) {
        if (numFont->active[i]) {
            PrintWindow_Flush(&numFont->printWins[i], numFont->printQueue);
            if (PrintWindow_IsPrinted(&numFont->printWins[i])) {
                numFont->active[i] = FALSE;
            }
        }
    }
    func_0201c4e4(numFont->printQueue);
}

void func_ov135_02202a50(WorldTradeNumFont *numFont, int num, int digits, int dispType, BmpWin *win, int x, int y) {
    StrBuf *str = GFL_StrBufCreate(32, numFont->heapId);
    StrBuf *format = GFL_StrBufCreate(32, numFont->heapId);
    int i;

    GFL_MsgDataLoadStrbuf(numFont->msgData, Bank0415_Text_253, format);
    WordSetNumber(numFont->wordSet, 0, num, digits, dispType, TRUE);
    GFL_WordSetFormatStrbuf(numFont->wordSet, str, format);

    for (i = 0; i < NUM_FONT_ENTRY_COUNT; i++) {
        if (!numFont->active[i]) {
            PrintWindow_Init(&numFont->printWins[i], win);
            PrintWindow_PrintColor(&numFont->printWins[i], numFont->printQueue, x, y, str, numFont->font, NUM_FONT_COLOR);
            numFont->active[i] = TRUE;
            GFL_StrBufFree(format);
            GFL_StrBufFree(str);
            return;
        }
    }
    GFL_ASSERT(0);
}

void func_ov135_02202b20(WorldTradeNumFont *numFont, int unused, BmpWin *win, int x, int y) {
    StrBuf *str = GFL_MsgDataLoadStrbufNew(numFont->msgData, Bank0415_Text_254);
    int i;

    for (i = 0; i < NUM_FONT_ENTRY_COUNT; i++) {
        if (!numFont->active[i]) {
            PrintWindow_Init(&numFont->printWins[i], win);
            PrintWindow_PrintColor(&numFont->printWins[i], numFont->printQueue, x, y, str, numFont->font, NUM_FONT_COLOR);
            numFont->active[i] = TRUE;
            GFL_StrBufFree(str);
            return;
        }
    }
    GFL_ASSERT(0);
}






