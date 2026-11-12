// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/zknsearch_main.h: the lines in use so
// far.
#ifndef BWPORT_APP_ZKNSEARCH_MAIN_H
#define BWPORT_APP_ZKNSEARCH_MAIN_H

#include "types.h"
#include "app/ui/frame_list.h"
#include "app/zukan_search_engine.h"
#include "gfl/clact.h"
#include "system/printsys.h"
#include "struct_decls.h"

// Not from swan: the search screen's heap
#define HEAPID_ZUKAN_SEARCH 0x7b

#define ZKNSEARCH_SHAPE_COUNT 14

// The actors
enum {
    ZKNSEARCH_OBJ_SCROLL_BAR,
    ZKNSEARCH_OBJ_SCROLL_RAIL,
    ZKNSEARCH_OBJ_RETURN,
    ZKNSEARCH_OBJ_EXIT,
    ZKNSEARCH_OBJ_SHORTCUT,
    ZKNSEARCH_OBJ_PAGE_DOWN,
    ZKNSEARCH_OBJ_PAGE_UP,
    ZKNSEARCH_OBJ_SHAPE,
    ZKNSEARCH_OBJ_LOADING_BAR,
    // The marks of the list's chosen items: two on the first BG's rows, and two on the second's, with their windows
    ZKNSEARCH_OBJ_MARK,
    ZKNSEARCH_OBJ_MARK_SUB = ZKNSEARCH_OBJ_MARK + 2,
    ZKNSEARCH_OBJ_MARK_WINDOW = ZKNSEARCH_OBJ_MARK_SUB + 2,
    // The icons of the shapes, in the same three kinds
    ZKNSEARCH_OBJ_SHAPE_ICON = ZKNSEARCH_OBJ_MARK_WINDOW + 2,
    ZKNSEARCH_OBJ_SHAPE_ICON_SUB = ZKNSEARCH_OBJ_SHAPE_ICON + ZKNSEARCH_SHAPE_COUNT,
    ZKNSEARCH_OBJ_SHAPE_ICON_WINDOW = ZKNSEARCH_OBJ_SHAPE_ICON_SUB + ZKNSEARCH_SHAPE_COUNT,
    ZKNSEARCH_OBJ_COUNT = ZKNSEARCH_OBJ_SHAPE_ICON_WINDOW + ZKNSEARCH_SHAPE_COUNT,
};

#define ZKNSEARCH_CHAR_RES_COUNT 6
#define ZKNSEARCH_PALETTE_RES_COUNT 6
#define ZKNSEARCH_CELL_RES_COUNT 4

// The windows
enum {
    ZKNSEARCH_WIN_MODE,
    ZKNSEARCH_WIN_INFO,
    ZKNSEARCH_WIN_ORDER_LABEL,
    ZKNSEARCH_WIN_INITIAL_LABEL,
    ZKNSEARCH_WIN_TYPE_LABEL,
    ZKNSEARCH_WIN_COLOR_LABEL,
    ZKNSEARCH_WIN_SHAPE_LABEL,
    ZKNSEARCH_WIN_ORDER,
    ZKNSEARCH_WIN_INITIAL,
    ZKNSEARCH_WIN_TYPE,
    ZKNSEARCH_WIN_COLOR,
    ZKNSEARCH_WIN_START,
    // On the top screen while a list is open: the criterion's name and its values
    ZKNSEARCH_WIN_TOP_LABEL,
    ZKNSEARCH_WIN_TOP_VALUE,
    ZKNSEARCH_WIN_TOP_VALUE2,
    ZKNSEARCH_WIN_MESSAGE,
    ZKNSEARCH_WIN_COUNT,
};

#define ZKNSEARCH_ITEM_NAME_COUNT 50

// Black 2's; matching code confirms the offsets of param, vblankTask, hblankTask, paletteFade, seq, subSeq, nextSeq,
// buttonNextSeq, bgWinFrame, actorUnit, actors, charRes, paletteRes, cellRes, windows, font, msgData, wordSet, strbuf,
// printQueue, frameList, itemNames, listRows, cursorMove, blinkAnm, bgRequest, page, initSeq, timer, loadingScreen,
// buttonIsActor, buttonId, buttonSeq, buttonTimer, scrollTimer, frameMoveLeft and frameMoveSpeed in Black
// The work, 0x2d4 bytes
typedef struct {
    ZukanSearchParam *param;
    TCB *vblankTask;
    TCB *hblankTask;
    PaletteFade *paletteFade;
    int seq;
    int subSeq;
    // The sequences after the page's end or the list's scroll, and after a button's animation
    int nextSeq;
    int buttonNextSeq;
    BGWinFrame *bgWinFrame;
    ClActUnit *actorUnit;
    ClActor *actors[ZKNSEARCH_OBJ_COUNT];
    u32 charRes[ZKNSEARCH_CHAR_RES_COUNT];
    u32 paletteRes[ZKNSEARCH_PALETTE_RES_COUNT];
    u32 cellRes[ZKNSEARCH_CELL_RES_COUNT];
    PrintWindow windows[ZKNSEARCH_WIN_COUNT];
    Font *font;
    MsgData *msgData;
    WordSet *wordSet;
    StrBuf *strbuf;
    PrintQueue *printQueue;
    FrameList *frameList;
    StrBuf *itemNames[ZKNSEARCH_ITEM_NAME_COUNT];
    u32 listRows;
    CursorMove *cursorMove;
    BlinkPalAnm *blinkAnm;
    u8 bgRequest;
    u8 page;
    u8 initSeq;
    u8 timer;
    // The screen of the search's progress bar
    u16 *loadingScreen;
    // A button's animation: of the cursor's palette (0) or of an actor (1), which one, and its progress
    u8 buttonIsActor;
    u8 buttonId;
    u8 buttonSeq;
    u8 buttonTimer;
    int scrollTimer;
    // The touch bar's slide: the pixels left and the pixels a frame
    u16 frameMoveLeft;
    s16 frameMoveSpeed;
    ZukanSearchEngine *engine;
    BOOL searchDone;
} ZknSearchWork;

#define ZKNSEARCH_NONE 0xff

// The shortcut of the search screen that the Y button registers
#define SHORTCUT_POKEDEX_SEARCH 16

// The pages: the main page's is ZKNSEARCH_NONE before the first, else the criterion it came back from
enum {
    ZKNSEARCH_PAGE_ORDER,
    ZKNSEARCH_PAGE_INITIAL,
    ZKNSEARCH_PAGE_TYPE,
    ZKNSEARCH_PAGE_COLOR,
    ZKNSEARCH_PAGE_SHAPE,
};

// pokebw2's ZknSearchMain_InitBG
void func_ov210_021bc3f0(void);
// pokebw2's ZknSearchMain_InitBGWinFrame
void func_ov210_021bca20(ZknSearchWork *wk);
// pokebw2's ZknSearchMain_InitFramePos
// The touch bar and its buttons, below the screen at first, slide by speed pixels a frame
void func_ov210_021bcda4(ZknSearchWork *wk);
// pokebw2's ZknSearchMain_InitPaletteFade
void func_ov210_021bc790(ZknSearchWork *wk);
// pokebw2's ZknSearchMain_StartFade
void func_ov210_021bc82c(ZknSearchWork *wk, u8 start, u8 end);
// pokebw2's ZknSearchMain_InitWindow
void func_ov210_021bc8ec(void);
// pokebw2's ZknSearchMain_ShapeToListPos
// The criteria's shape is its position in the list while the screen runs
void func_ov210_021bcf0c(ZknSearchWork *wk);
// pokebw2's ZknSearchMain_UpdateFrameMove
BOOL func_ov210_021bce58(ZknSearchWork *wk);
// pokebw2's ZknSearchMain_FreeLoadingScreen
void func_ov210_021bcc1c(ZknSearchWork *wk);
// pokebw2's ZknSearchMain_ExitBlinkAnm
void func_ov210_021bca10(ZknSearchWork *wk);
// pokebw2's ZknSearchMain_ExitMsg
void func_ov210_021bc99c(ZknSearchWork *wk);
// pokebw2's ZknSearchMain_ExitBG
void func_ov210_021bc578(void);
// pokebw2's ZknSearchMain_SetBlend
void func_ov210_021bc8b4(BOOL enabled);
// pokebw2's ZknSearchMain_SetMainBGPriority
// Put the main page's frame over its text, or a list's text over its frame
void func_ov210_021bc5b4(void);
// pokebw2's ZknSearchMain_RequestBGOn
void func_ov210_021bcd30(ZknSearchWork *wk);
// pokebw2's ZknSearchMain_StartFrameMove
void func_ov210_021bce0c(ZknSearchWork *wk, s16 speed);
// pokebw2's ZknSearchMain_ListPosToShape
void func_ov210_021bcf38(ZknSearchWork *wk);
// pokebw2's ZknSearchMain_ClearPage
// Hides the page's BGs and clears their screens
void func_ov210_021bcd3c(ZknSearchWork *wk);
// pokebw2's ZknSearchMain_SetListBGPriority
void func_ov210_021bc5c8(void);

// Black 2's, not checked against Black
#define ZKNSEARCH_TYPE_COUNT 17

// What the VBlank task does with the BGs of the lists (bgRequest)
#define ZKNSEARCH_BG_REQUEST_ON 1

#define ZKNSEARCH_BG_REQUEST_OFF 2

// pokebw2's ZknSearchMain_GetListPosTypeMsg
u32 func_ov210_021bcef4(ZknSearchWork *wk, u32 pos);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's ZknSearchMain_GetTypeMsg
u32 func_ov210_021bcf00(ZknSearchWork *wk, u32 type);
// pokebw2's ZknSearchMain_GetVRAMConfig
const BGSysVRAMConfig *func_ov210_021bc3e8(void);
// pokebw2's ZknSearchMain_TypeToListPos
u32 func_ov210_021bcef0(ZknSearchWork *wk, u32 type);
// pokebw2's ZknSearchMain_InitVRAM
void func_ov210_021bc3d4(void);
// pokebw2's ZknSearchMain_LoadGraphics
void func_ov210_021bc5dc(void);
// pokebw2's ZknSearchMain_InitMsg
void func_ov210_021bc940(ZknSearchWork *wk);
// pokebw2's ZknSearchMain_InitBlinkAnm
void func_ov210_021bc9d0(ZknSearchWork *wk);
// pokebw2's ZknSearchMain_LoadLoadingScreen
void func_ov210_021bcbb8(ZknSearchWork *wk);
// pokebw2's ZknSearchMain_InitVBlank
void func_ov210_021bc294(ZknSearchWork *wk);
// pokebw2's ZknSearchMain_InitHBlank
void func_ov210_021bc318(ZknSearchWork *wk);
// pokebw2's ZknSearchMain_ExitHBlank
void func_ov210_021bc330(ZknSearchWork *wk);
// pokebw2's ZknSearchMain_ExitVBlank
void func_ov210_021bc2ac(ZknSearchWork *wk);
// pokebw2's ZknSearchMain_ExitWindow
void func_ov210_021bc92c(void);
// pokebw2's ZknSearchMain_ExitBGWinFrame
void func_ov210_021bca78(ZknSearchWork *wk);
// pokebw2's ZknSearchMain_ExitPaletteFade
void func_ov210_021bc800(ZknSearchWork *wk);
// pokebw2's ZknSearchMain_LoadMainScreen
void func_ov210_021bca84(ZknSearchWork *wk);
// pokebw2's ZknSearchMain_ShowLoading
// Show and hide the search's progress bar and its message
void func_ov210_021bcc2c(ZknSearchWork *wk);
// pokebw2's ZknSearchMain_HideLoading
void func_ov210_021bcca8(ZknSearchWork *wk);
// pokebw2's ZknSearchMain_LoadOrderScreen
void func_ov210_021bcac8(ZknSearchWork *wk);
// pokebw2's ZknSearchMain_LoadInitialScreen
void func_ov210_021bcaf8(ZknSearchWork *wk);
// pokebw2's ZknSearchMain_LoadTypeScreen
void func_ov210_021bcb28(ZknSearchWork *wk);
// pokebw2's ZknSearchMain_LoadColorScreen
void func_ov210_021bcb58(ZknSearchWork *wk);
// pokebw2's ZknSearchMain_LoadShapeScreen
void func_ov210_021bcb88(ZknSearchWork *wk);
// pokebw2's ZknSearchMain_ListPosToType
// A type from its position in the list, its position, and the message of the type at a position or of a type
u32 func_ov210_021bceec(ZknSearchWork *wk, u32 pos);

#endif // BWPORT_APP_ZKNSEARCH_MAIN_H
