// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/codein_pv.h: the lines in use so far.
#ifndef BWPORT_APP_CODEIN_PV_H
#define BWPORT_APP_CODEIN_PV_H

#include "types.h"
#include "gfl/clact.h"
#include "gfl/touchpanel.h"
#include "system/codein_param.h"
#include "struct_decls.h"

// The most digits a code has
#define CODEIN_CODE_MAX 12

#define CODEIN_BAR_COUNT 2
#define CODEIN_BUTTON_COUNT 2

// The lower screen's keys: the ten digits, then Back and OK
#define CODEIN_KEY_COUNT 12

#define CODEIN_KEY_BACK 10
#define CODEIN_KEY_OK 11

// The touch rectangles: the digits of the code, then the keys
#define CODEIN_RECT_KEYS CODEIN_CODE_MAX

#define CODEIN_RECT_BACK (CODEIN_RECT_KEYS + CODEIN_KEY_BACK)
#define CODEIN_RECT_OK (CODEIN_RECT_KEYS + CODEIN_KEY_OK)
#define CODEIN_RECT_COUNT (CODEIN_RECT_KEYS + CODEIN_KEY_COUNT)

// The cursors: the one under the code's digit, the one on the keys, and the flash of a key that was pressed
enum {
    CODEIN_CURSOR_CODE,
    CODEIN_CURSOR_KEY,
    CODEIN_CURSOR_PRESS,
    CODEIN_CURSOR_COUNT,
};

// The sets of OBJ resources
enum {
    CODEIN_RES_CODE,
    CODEIN_RES_BUTTON,
    CODEIN_RES_CURSOR,
    CODEIN_RES_COUNT,
};

// Black 2's; twinned code confirms the values of CODEIN_STATE_INPUT, CODEIN_STATE_MOVE, CODEIN_STATE_END and
// CODEIN_STATE_ERROR in Black
// The states of the screen
enum {
    CODEIN_STATE_INIT,
    CODEIN_STATE_INPUT,
    CODEIN_STATE_MOVE,
    CODEIN_STATE_END,
    CODEIN_STATE_ERROR,
};

// What the input state does next
enum {
    CODEIN_NEXT_NONE,
    // Moves the focus to the group work
    CODEIN_NEXT_FOCUS,
    // Moves the cursor to the digit work
    CODEIN_NEXT_CURSOR,
    CODEIN_NEXT_WAIT = 0xff,
};

// An actor's movement: the step of each frame, the frames left and the index of its next scale
typedef struct {
    s16 x;
    s16 y;
    u8 wait;
    u8 scale;
} CodeInMove;

typedef struct {
    // A digit: 0 while empty, or its value plus 1. A bar: the index of the digit before it. The code's cursor: the
    // index of its digit. Another cursor: its animation. A button: 1 while its press animation runs
    int state;
    // The group of a digit, from 1
    int group;
    // Whether a digit is shown large, as the focused group's are
    int size;
    ClActor *actor;
    TouchRect *rect;
    // The key's cursor keeps its column and row in x and y
    CodeInMove move;
} CodeInObj;

// The digits of a group, from start up to end
typedef struct {
    u16 start;
    u16 end;
} CodeInRange;

// Black 2's; twinned code confirms the offsets of code, bar, cur, btn, focusX, range, state, unk230, seq, count,
// codeMax, focus, focusOld, largeStart, largeEnd, smallStart, smallEnd, arc, unit, plttRes, charRes, cellAnimRes, fade,
// buttons, rects, touchMode, bmpOam, btnText, btnTextPltt, window, font, vblankTask, next, nextWork, nextLast and param
// in Black
typedef struct {
    CodeInObj code[CODEIN_CODE_MAX];
    CodeInObj bar[CODEIN_BAR_COUNT];
    CodeInObj cur[CODEIN_CURSOR_COUNT];
    CodeInObj btn[CODEIN_BUTTON_COUNT];
    // Where the code starts for each focus, none and the three groups
    s16 focusX[CODEIN_BLOCK_COUNT + 1];
    CodeInRange range[CODEIN_BLOCK_COUNT];
    u32 unk228;
    int state;
    int unk230;
    int seq;
    int count;
    int codeMax;
    // The group that is shown large, from 1, or 0 for none, and the one before it
    int focus;
    int focusOld;
    // The digits that grow and the digits that shrink
    int largeStart;
    int largeEnd;
    int smallStart;
    int smallEnd;
    ArcTool *arc;
    ClActUnit *unit;
    u32 plttRes[CODEIN_RES_COUNT];
    u32 charRes[CODEIN_RES_COUNT];
    u32 unk278[CODEIN_RES_COUNT];
    u32 cellAnimRes[CODEIN_RES_COUNT];
    PaletteFade *fade;
    ButtonMan *buttons;
    TouchRect rects[CODEIN_RECT_COUNT + 1];
    // Whether the touch screen was used last, which hides the keys' cursor
    BOOL touchMode;
    BmpOamSys *bmpOam;
    BmpOamActor *btnText[CODEIN_BUTTON_COUNT];
    u32 btnTextPltt;
    BmpWin *window;
    Font *font;
    TCB *vblankTask;
    // CODEIN_NEXT_*, its group or digit, and whether the cursor goes to the group's last digit
    int next;
    int nextWork;
    BOOL nextLast;
    CodeInParam param;
} CodeInWork;

// pokebw2's CodeInDisp_SetCodeSizes
void func_ov129_021da164(CodeInWork *wk);
// pokebw2's CodeInDisp_AddCodeActors
void func_ov129_021d9ab0(CodeInWork *wk);
// pokebw2's CodeInDisp_SetCodePositions
// Puts the digits and bars where the focus has them, at once or by starting their movement
void func_ov129_021da1a8(CodeInWork *wk, BOOL move);
// pokebw2's CodeInDisp_AddCursorActors
void func_ov129_021d9bec(CodeInWork *wk);
// pokebw2's CodeInDisp_AddButtonActors
void func_ov129_021d9d44(CodeInWork *wk);
// pokebw2's CodeInDisp_UpdateCodeRects
void func_ov129_021da320(CodeInWork *wk);
// pokebw2's CodeInDisp_InitBmpOam
void func_ov129_021da3b8(CodeInWork *wk);
// pokebw2's CodeInDisp_AddButtonTexts
void func_ov129_021da42c(CodeInWork *wk);
// pokebw2's CodeInDisp_CreateMessageWindow
void func_ov129_021da524(BmpWin **window, u32 messageId, int bg, int x, int y, int width, int height,
                                    Font *font, int number);
// pokebw2's CodeInDisp_DrawErrorMessage
void func_ov129_021da610(BmpWin *window, u32 mode, Font *font, int number);
// pokebw2's CodeInDisp_SetCursorVisible
void func_ov129_021d9e48(CodeInWork *wk, int cursor, BOOL visible);
// pokebw2's CodeInDisp_GetCodeSequence
// The animation of a digit's actor
int func_ov129_021da158(int state, int size);
// pokebw2's CodeInDisp_SetCodeCursor
void func_ov129_021d9e60(CodeInWork *wk, int digit);
// pokebw2's CodeInDisp_UpdateCursors
void func_ov129_021d9f40(CodeInWork *wk);
// pokebw2's CodeInDisp_UpdateButtons
void func_ov129_021da004(CodeInWork *wk);
// pokebw2's CodeInDisp_SetKeyCursor
void func_ov129_021d9e98(CodeInWork *wk, int key);
// pokebw2's CodeInDisp_SetCursorOnKey
void func_ov129_021d9ee8(CodeInWork *wk, int key, int cursor);
// pokebw2's CodeIn_InitParam
// codein_pv.c
void func_ov129_021d89c4(CodeInWork *wk);
// pokebw2's CodeIn_Main
BOOL func_ov129_021d8f78(CodeInWork *wk);
// pokebw2's CodeIn_InitButtons
void func_ov129_021d9490(CodeInWork *wk);

// pokebw2's CodeInDisp_InitActors
// codein_disp.c
void func_ov129_021d9838(CodeInWork *wk, const BGSysVRAMConfig *vramConfig);
// pokebw2's CodeInDisp_Free
void func_ov129_021d99f0(CodeInWork *wk);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's CodeInDisp_LoadResources
void func_ov129_021d9890(CodeInWork *wk);
// pokebw2's CodeInDisp_DrawMessage
void func_ov129_021da578(BmpWin *window, u32 messageId, Font *font, int number);

#endif // BWPORT_APP_CODEIN_PV_H
