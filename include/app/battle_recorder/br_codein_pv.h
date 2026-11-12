// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/battle_recorder/br_codein_pv.h: the
// lines in use so far.
#ifndef BWPORT_APP_BATTLE_RECORDER_BR_CODEIN_PV_H
#define BWPORT_APP_BATTLE_RECORDER_BR_CODEIN_PV_H

#include "types.h"
#include "app/battle_recorder/br_codein.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/touchpanel.h"
#include "struct_decls.h"

// The most digits a code has
#define BR_CODEIN_CODE_MAX 12

#define BR_CODEIN_BAR_COUNT 2

// Black 2's, not checked against Black
#define BR_CODEIN_BUTTON_COUNT 2

// The lower screen's keys: the ten digits, then Back and OK
#define BR_CODEIN_KEY_COUNT 12

#define BR_CODEIN_KEY_BACK 10
#define BR_CODEIN_KEY_OK 11

// The touch rectangles: the digits of the code, then the keys
#define BR_CODEIN_RECT_KEYS BR_CODEIN_CODE_MAX

#define BR_CODEIN_RECT_BACK (BR_CODEIN_RECT_KEYS + BR_CODEIN_KEY_BACK)
#define BR_CODEIN_RECT_OK (BR_CODEIN_RECT_KEYS + BR_CODEIN_KEY_OK)
#define BR_CODEIN_RECT_COUNT (BR_CODEIN_RECT_KEYS + BR_CODEIN_KEY_COUNT)

// The cursors: the one under the code's digit, the one on the keys, and the flash of a key that was pressed. Nothing
// shows the last two here
enum {
    BR_CODEIN_CURSOR_CODE,
    BR_CODEIN_CURSOR_KEY,
    BR_CODEIN_CURSOR_PRESS,
    BR_CODEIN_CURSOR_COUNT,
};

// Black 2's; twinned code confirms the values of BR_CODEIN_RES_CODE, BR_CODEIN_RES_BUTTON and BR_CODEIN_RES_CURSOR in
// Black
// The sets of OBJ resources
enum {
    BR_CODEIN_RES_CODE,
    BR_CODEIN_RES_BUTTON,
    BR_CODEIN_RES_CURSOR,
    BR_CODEIN_RES_COUNT,
};

// An actor's movement: the step of each frame, the frames left and the index of its next scale
typedef struct {
    s16 x;
    s16 y;
    u8 wait;
    u8 scale;
} BrCodeInMove;

typedef struct {
    // A digit: 0 while empty, or its value plus 1. A bar: the index of the digit before it. The code's cursor: the
    // index of its digit. Another cursor: its animation. A button: 1 once it was touched
    int state;
    // The group of a digit, from 1
    int group;
    // Whether a digit is shown large, as the focused group's are
    int size;
    ClActor *actor;
    TouchRect *rect;
    // The key's cursor keeps its column and row in x and y
    BrCodeInMove move;
} BrCodeInObj;

// The digits of a group, from start up to end
typedef struct {
    u16 start;
    u16 end;
} BrCodeInRange;

// Black 2's; matching code confirms the offsets of code, bar, cur, btn, focusX, range, state, unk230, seq, count,
// codeMax, focus, focusOld, largeStart, largeEnd, smallStart, smallEnd, arc, unit, plttRes, charRes, cellAnimRes,
// buttons, rects, touchMode, bmpOam, btnText, font, vblankTask, next, nextWork, nextLast, param, heapId, result,
// blockNumber and number in Black
struct BrCodeInWork {
    BrCodeInObj code[BR_CODEIN_CODE_MAX];
    BrCodeInObj bar[BR_CODEIN_BAR_COUNT];
    BrCodeInObj cur[BR_CODEIN_CURSOR_COUNT];
    BrCodeInObj btn[BR_CODEIN_BUTTON_COUNT];
    // Where the code starts for each focus, none and the three groups
    s16 focusX[BR_CODEIN_BLOCK_COUNT + 1];
    BrCodeInRange range[BR_CODEIN_BLOCK_COUNT];
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
    u32 plttRes[BR_CODEIN_RES_COUNT];
    u32 charRes[BR_CODEIN_RES_COUNT];
    u32 unk278[BR_CODEIN_RES_COUNT];
    u32 cellAnimRes[BR_CODEIN_RES_COUNT];
    u32 unk290;
    ButtonMan *buttons;
    TouchRect rects[BR_CODEIN_RECT_COUNT + 1];
    // Set at the start and never cleared: there is no input by keys
    BOOL touchMode;
    BmpOamSys *bmpOam;
    BmpOamActor *btnText[BR_CODEIN_BUTTON_COUNT];
    u32 unk30C;
    Font *font;
    TCB *vblankTask;
    // BR_CODEIN_NEXT_*, its group or digit, and whether the cursor goes to the group's last digit
    int next;
    int nextWork;
    BOOL nextLast;
    BrCodeInParam param;
    HeapID heapId;
    // BR_CODEIN_RESULT_*
    int result;
    // The number of each group, and the whole number
    u32 blockNumber[BR_CODEIN_BLOCK_COUNT];
    u64 number;
};

// pokebw2's BrCodeInDisp_Free
void func_ov183_021e08e4(BrCodeInWork *wk);
// pokebw2's BrCodeInDisp_AddCodeActors
void func_ov183_021e0954(BrCodeInWork *wk);
// pokebw2's BrCodeInDisp_AddCursorActors
void func_ov183_021e0a88(BrCodeInWork *wk);
// pokebw2's BrCodeInDisp_AddButtonActors
void func_ov183_021e0c04(BrCodeInWork *wk);
// pokebw2's BrCodeInDisp_SetCodeCursor
void func_ov183_021e0d00(BrCodeInWork *wk, int digit);
// pokebw2's BrCodeInDisp_SetKeyCursor
void func_ov183_021e0d38(BrCodeInWork *wk, int key);
// pokebw2's BrCodeInDisp_SetCodePositions
// Puts the digits and bars where the focus has them, at once or by starting their movement
void func_ov183_021e0e90(BrCodeInWork *wk, BOOL move);
// pokebw2's BrCodeInDisp_UpdateCodeRects
void func_ov183_021e1008(BrCodeInWork *wk);
// pokebw2's BrCodeInDisp_InitBmpOam
void func_ov183_021e10a0(BrCodeInWork *wk);
// pokebw2's BrCodeInDisp_AddButtonTexts
void func_ov183_021e10f4(BrCodeInWork *wk);

// Black 2's; twinned code confirms the values of BR_CODEIN_STATE_INPUT, BR_CODEIN_STATE_MOVE and BR_CODEIN_STATE_END in
// Black
// The states of the code input
enum {
    BR_CODEIN_STATE_INIT,
    BR_CODEIN_STATE_INPUT,
    BR_CODEIN_STATE_MOVE,
    BR_CODEIN_STATE_END,
};

// What the input state does next
enum {
    BR_CODEIN_NEXT_NONE,
    // Moves the focus to the group work
    BR_CODEIN_NEXT_FOCUS,
    // Moves the cursor to the digit work
    BR_CODEIN_NEXT_CURSOR,
    BR_CODEIN_NEXT_WAIT = 0xff,
};

// pokebw2's BrCodeIn_InitParam
// br_codein_pv.c
void func_ov183_021e1244(BrCodeInWork *wk);
// pokebw2's BrCodeIn_MainPv
BOOL func_ov183_021e16e4(BrCodeInWork *wk);
// pokebw2's BrCodeIn_InitButtons
void func_ov183_021e1910(BrCodeInWork *wk);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BrCodeInDisp_GetResources
// br_codein_disp.c
void func_ov183_021e0840(BrCodeInWork *wk);
// pokebw2's BrCodeInDisp_SetCursorVisible
void func_ov183_021e0ce8(BrCodeInWork *wk, int cursor, BOOL visible);
// pokebw2's BrCodeInDisp_UpdateCursors
void func_ov183_021e0d90(BrCodeInWork *wk);
// pokebw2's BrCodeInDisp_GetCodeSequence
// The animation of a digit's actor
int func_ov183_021e0e40(int state, int size);
// pokebw2's BrCodeInDisp_SetCodeSizes
void func_ov183_021e0e4c(BrCodeInWork *wk);

#endif // BWPORT_APP_BATTLE_RECORDER_BR_CODEIN_PV_H
