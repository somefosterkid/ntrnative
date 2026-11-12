// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/zknlist_main.h: the lines in use so far.
#ifndef BWPORT_APP_ZKNLIST_MAIN_H
#define BWPORT_APP_ZKNLIST_MAIN_H

#include "types.h"
#include "app/ui/frame_list.h"
#include "gfl/clact.h"
#include "system/printsys.h"
#include "struct_decls.h"

// Not from swan: the list's heap
#define HEAPID_ZUKAN_LIST 0x6b

// Black 2's, not checked against Black
// How many species the list can hold, and the rows it shows
#define ZKNLIST_SPECIES_COUNT 649

// The icons of the rows that a screen can show while the list scrolls
#define ZKNLIST_ICON_COUNT 16

// Black 2's, not checked against Black
// The actors
enum {
    ZKNLIST_OBJ_SCROLL_BAR,
    ZKNLIST_OBJ_SCROLL_RAIL,
    // Shown with the picture of a species
    ZKNLIST_OBJ_MON_BASE,
    ZKNLIST_OBJ_RETURN,
    ZKNLIST_OBJ_EXIT,
    ZKNLIST_OBJ_SHORTCUT,
    ZKNLIST_OBJ_PAGE_DOWN,
    ZKNLIST_OBJ_PAGE_UP,
    ZKNLIST_OBJ_SELECT,
    ZKNLIST_OBJ_START,
    // The icons of the rows: on the touch screen, on the top screen, and the windows of the top screen's
    ZKNLIST_OBJ_ICON,
    ZKNLIST_OBJ_ICON_SUB = ZKNLIST_OBJ_ICON + ZKNLIST_ICON_COUNT,
    ZKNLIST_OBJ_ICON_WINDOW = ZKNLIST_OBJ_ICON_SUB + ZKNLIST_ICON_COUNT,
    // The picture of the species under the cursor: two, so that one loads while the other shows
    ZKNLIST_OBJ_MON = ZKNLIST_OBJ_ICON_WINDOW + ZKNLIST_ICON_COUNT,
    ZKNLIST_OBJ_COUNT = ZKNLIST_OBJ_MON + 2,
};

#define ZKNLIST_CHAR_RES_COUNT 36
#define ZKNLIST_PALETTE_RES_COUNT 6
#define ZKNLIST_CELL_RES_COUNT 6

// Black 2's; twinned code confirms the values of ZKNLIST_WIN_TITLE in Black
// The windows
enum {
    ZKNLIST_WIN_TITLE,
    ZKNLIST_WIN_SEEN,
    ZKNLIST_WIN_CAUGHT,
    ZKNLIST_WIN_COUNT,
};

// Black 2's; matching code confirms the offsets of param, paletteFade, seq, buttonNextSeq, fadeNextSeq, windows, font,
// msgData, wordSet, strbuf, printQueue, buttonId, shownIcons, shownIconsSub, frameList and scrollTimer in Black
// The work, 0xc44 bytes
typedef struct {
    ZukanListParam *param;
    TCB *vblankTask;
    TCB *hblankTask;
    PaletteFade *paletteFade;
    int seq;
    int subSeq;
    // The sequences after a button's animation, and after a fade
    int buttonNextSeq;
    int fadeNextSeq;
    PrintWindow windows[ZKNLIST_WIN_COUNT];
    Font *font;
    MsgData *msgData;
    WordSet *wordSet;
    StrBuf *strbuf;
    PrintQueue *printQueue;
    // The names of the list's items
    StrBuf *names[ZKNLIST_SPECIES_COUNT];
    ClActUnit *actorUnit;
    ClActor *actors[ZKNLIST_OBJ_COUNT];
    u32 charRes[ZKNLIST_CHAR_RES_COUNT];
    u32 paletteRes[ZKNLIST_PALETTE_RES_COUNT];
    u32 cellRes[ZKNLIST_CELL_RES_COUNT];
    // Which of the two pictures the next species loads into
    u16 monIndex : 1;
    // The actor whose animation the sequence waits for
    u16 buttonId : 15;
    // The flashing of the chosen row
    u8 decideSeq;
    u8 decideTimer;
    // The icons shown on each screen, a bit each
    u16 shownIcons;
    u16 shownIconsSub;
    FrameList *frameList;
    int initSeq;
    u32 listRows;
    // The number of each species in the regional Pokédex
    u16 *regionalNumbers;
    int scrollTimer;
    // The touch bar's slide: the pixels left and the pixels a frame
    u16 frameMoveLeft;
    s16 frameMoveSpeed;
} ZknListWork;

// Black 2's, not checked against Black
#define ZKNLIST_ROW_COUNT 7
// A list item's value: the species, and above it whether it is seen or caught
#define ZKNLIST_ITEM_SPECIES_MASK 0x0fffffff

// Black 2's, not checked against Black
#define ZKNLIST_ITEM_STATE_MASK 0xf0000000
#define ZKNLIST_ITEM_STATE_SHIFT 28
#define ZKNLIST_ITEM_SEEN (1 << ZKNLIST_ITEM_STATE_SHIFT)
#define ZKNLIST_ITEM_CAUGHT (2 << ZKNLIST_ITEM_STATE_SHIFT)

// pokebw2's ZknListMain_InitBG
void func_ov209_021bc388(void);
// pokebw2's ZknListMain_ExitBG
void func_ov209_021bc52c(void);
// pokebw2's ZknListMain_InitPaletteFade
void func_ov209_021bc6c4(ZknListWork *wk);
// pokebw2's ZknListMain_ExitPaletteFade
void func_ov209_021bc704(ZknListWork *wk);
// pokebw2's ZknListMain_SetBlend
void func_ov209_021bc7b8(void);
// pokebw2's ZknListMain_InitWindow
void func_ov209_021bc7d0(void);
// pokebw2's ZknListMain_ExitWindow
void func_ov209_021bc810(void);

// The shortcut of the Pokédex that the Y button registers
#define SHORTCUT_POKEDEX_LIST 15

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's ZknListMain_InitVBlank
void func_ov209_021bc298(ZknListWork *wk);
// pokebw2's ZknListMain_ExitVBlank
void func_ov209_021bc2b0(ZknListWork *wk);
// pokebw2's ZknListMain_InitHBlank
void func_ov209_021bc2e4(ZknListWork *wk);
// pokebw2's ZknListMain_ExitHBlank
void func_ov209_021bc2fc(ZknListWork *wk);
// pokebw2's ZknListMain_InitVRAM
void func_ov209_021bc370(void);
// pokebw2's ZknListMain_GetVRAMConfig
const BGSysVRAMConfig *func_ov209_021bc380(void);
// pokebw2's ZknListMain_LoadGraphics
void func_ov209_021bc574(void);
// pokebw2's ZknListMain_StartFade
// Fades everything but the touch bar and its buttons, which slide instead
void func_ov209_021bc730(ZknListWork *wk, u8 start, u8 end);
// pokebw2's ZknListMain_InitMsg
void func_ov209_021bc824(ZknListWork *wk);
// pokebw2's ZknListMain_ExitMsg
void func_ov209_021bc868(ZknListWork *wk);
// pokebw2's ZknListMain_LoadRegionalNumbers
void func_ov209_021bc88c(ZknListWork *wk);
// pokebw2's ZknListMain_FreeRegionalNumbers
void func_ov209_021bc8a4(ZknListWork *wk);
// pokebw2's ZknListMain_InitFramePos
// The touch bars of both screens and the buttons, off the screens at first, slide by speed pixels a frame
void func_ov209_021bc8b4(ZknListWork *wk);
// pokebw2's ZknListMain_StartFrameMove
void func_ov209_021bc90c(ZknListWork *wk, s16 speed);
// pokebw2's ZknListMain_UpdateFrameMove
BOOL func_ov209_021bc91c(ZknListWork *wk);
// pokebw2's ZknListMain_InitList
// The list of the species, with the cursor on the species that the Pokédex was last on
void func_ov209_021bc9a8(ZknListWork *wk);
// pokebw2's ZknListMain_ExitList
void func_ov209_021bcb60(ZknListWork *wk);

#endif // BWPORT_APP_ZKNLIST_MAIN_H
