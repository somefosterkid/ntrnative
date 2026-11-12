// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov205/codein_pv.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/codein.h"
#include "app/codein_pv.h"
#include "constants/sound.h"
#include "gfl/button_man.h"
#include "gfl/clact.h"
#include "gfl/key.h"
#include "gfl/sound.h"
#include "gfl/str.h"
#include "gfl/touchpanel.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "system/codein_param.h"
#include "system/str_tool.h"
#include "system/wipe.h"

// The code input screen's states and input. The ROM has no name for this file; "codein_pv.c" is a guess from
// pokeplatinum, whose code input screen has the same three files. Our names

#define KEY_COLUMNS 5
#define KEY_ROWS 3
// The frames of a change of focus: the digits move, then finish scaling
#define MOVE_FRAMES 4
#define SCALE_COUNT 6

typedef BOOL (*CodeInStateFunc)(CodeInWork *wk);

// A key's center and half its size
typedef struct {
    s16 x;
    s16 y;
    s16 halfWidth;
    s16 halfHeight;
} CodeInKeyRect;

static void func_ov129_021d8950(CodeInWork *wk, int focus);
static void func_ov129_021d8af4(CodeInWork *wk, int state);
static BOOL func_ov129_021d8b0c(CodeInWork *wk);
static BOOL func_ov129_021d8b98(CodeInWork *wk);
static BOOL func_ov129_021d8be4(CodeInWork *wk);
static BOOL func_ov129_021d8c5c(CodeInWork *wk);
static BOOL func_ov129_021d8c90(CodeInWork *wk);
static void func_ov129_021d8fa0(CodeInWork *wk);
int func_ov129_021d92e4(CodeInWork *wk);
static BOOL func_ov129_021d9320(CodeInWork *wk);
static void func_ov129_021d9348(CodeInWork *wk);
static void func_ov129_021d93d4(CodeInWork *wk);
static void func_ov129_021d9564(CodeInWork *wk, int rect);
static void func_ov129_021d958c(u32 button, u32 event, void *work);
static void func_ov129_021d9760(CodeInWork *wk);
static void func_ov129_021d97b8(CodeInWork *wk);
static int func_ov129_021d97cc(CodeInWork *wk, int group);
static int func_ov129_021d97fc(CodeInWork *wk, int group);

// By CODEIN_STATE_*
// pokebw2's sCodeInStates
static const CodeInStateFunc data_ov129_021da718[] = {
    func_ov129_021d8b0c, func_ov129_021d8c5c, func_ov129_021d8c90, func_ov129_021d8b98, func_ov129_021d8be4,
};

// The column and row of each key, by its touch rectangle
// pokebw2's sCodeInKeyCursorPos
static const u8 data_ov129_021da72c[CODEIN_KEY_COUNT][2] = {
#include "asset/codein_pv_data_ov129_021da72c.inc"
};

// The scales of a digit as it grows and as it shrinks
// pokebw2's sCodeInScalesLarge
static fx32 data_ov129_021da830[] = { 0x800, 0x333, 0x800, FX32_ONE, 0x1333, FX32_ONE, FX32_ONE };
// pokebw2's sCodeInScalesSmall
static fx32 data_ov129_021da84c[] = { 0xccd, 0x99a, 0x666, 0x333, 0xccd, FX32_ONE, FX32_ONE };

// pokebw2's CodeIn_SetFocus
static void func_ov129_021d8950(CodeInWork *wk, int focus) {
    wk->focusOld = wk->focus;
    wk->focus = focus;
    wk->largeStart = 0;
    wk->largeEnd = 0;
    wk->smallStart = 0;
    wk->smallEnd = 0;
    if (focus != 0) {
        wk->largeStart = wk->range[focus - 1].start;
        wk->largeEnd = wk->range[focus - 1].end;
    }
    if (wk->focusOld != 0) {
        wk->smallStart = wk->range[wk->focusOld - 1].start;
        wk->smallEnd = wk->range[wk->focusOld - 1].end;
    }
}

// pokebw2's CodeIn_InitParam
void func_ov129_021d89c4(CodeInWork *wk) {
    int i;
    int j;
    int count;

    wk->touchMode = TRUE;
    wk->range[0].start = 0;
    wk->range[0].end = wk->param.blocks[0];
    wk->range[1].start = wk->param.blocks[0];
    wk->range[1].end = wk->param.blocks[0] + wk->param.blocks[1];
    wk->range[2].start = wk->param.blocks[0] + wk->param.blocks[1];
    wk->range[2].end = wk->param.blocks[0] + wk->param.blocks[1] + wk->param.blocks[2];
    func_ov129_021d8950(wk, 1);
    for (i = 0; i < CODEIN_BLOCK_COUNT; i++) {
        wk->codeMax += wk->param.blocks[i];
    }
    {
        int kind = 0;
        // For a Friend Code and for a single number
        int focusX[2][CODEIN_BLOCK_COUNT] = { { 56, 20, 8 }, { 56, 12, 0 } };

        if (wk->param.blocks[0] != 4) {
            kind = 1;
        }
        for (i = 0; i < CODEIN_BLOCK_COUNT; i++) {
            wk->focusX[i] = focusX[kind][i];
        }
        wk->focusX[i] = focusX[kind][i - 1];
    }
    count = 0;
    for (i = 0; i < CODEIN_BAR_COUNT; i++) {
        count += wk->param.blocks[i];
        wk->bar[i].state = count - 1;
    }
    i = 0;
    count = 0;
    do {
        for (j = 0; j < wk->param.blocks[i]; j++) {
            wk->code[count].group = i + 1;
            count++;
        }
        i++;
    } while (count < wk->codeMax);
}

// pokebw2's CodeIn_SetState
static void func_ov129_021d8af4(CodeInWork *wk, int state) {
    wk->state = state;
    wk->unk230 = 0;
    wk->seq = 0;
    wk->count = 0;
}

// pokebw2's CodeIn_StateInit
static BOOL func_ov129_021d8b0c(CodeInWork *wk) {
    func_ov129_021d9890(wk);
    func_ov129_021da164(wk);
    func_ov129_021d9ab0(wk);
    func_ov129_021da1a8(wk, FALSE);
    func_ov129_021d9bec(wk);
    func_ov129_021d9d44(wk);
    func_ov129_021da320(wk);
    func_ov129_021da3b8(wk);
    func_ov129_021da42c(wk);
    func_ov129_021da524(&wk->window, wk->param.mode, 4, 2, 19, 27, 4, wk->font, wk->param.unk1C);
    func_ov129_021d8af4(wk, CODEIN_STATE_INPUT);
    GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_IN, WIPE_TYPE_FADE_IN, WIPE_COLOR_BLACK, 6, 1, HEAPID_CODEIN);
    return FALSE;
}

// pokebw2's CodeIn_StateEnd
static BOOL func_ov129_021d8b98(CodeInWork *wk) {
    switch (wk->seq) {
    case 0:
        GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_OUT, WIPE_TYPE_FADE_OUT, WIPE_COLOR_BLACK, 6, 1, HEAPID_CODEIN);
        wk->seq++;
        break;
    case 1:
        if (GFL_WipeIsFinished() == TRUE) {
            wk->seq++;
        }
        break;
    default:
        return TRUE;
    }
    return FALSE;
}

// pokebw2's CodeIn_StateError
static BOOL func_ov129_021d8be4(CodeInWork *wk) {
    switch (wk->seq) {
    case 0:
        func_ov129_021da610(wk->window, wk->param.mode, wk->font, wk->param.unk1C);
        wk->seq++;
        break;
    case 1:
        if ((GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) || (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) || func_02035c60()) {
            func_ov129_021da578(wk->window, wk->param.mode, wk->font, wk->param.unk1C);
            func_ov129_021d8af4(wk, CODEIN_STATE_INPUT);
        }
        break;
    }
    return FALSE;
}

// pokebw2's CodeIn_StateInput
static BOOL func_ov129_021d8c5c(CodeInWork *wk) {
    if (wk->seq == 0) {
        if (GFL_WipeIsFinished() == TRUE) {
            wk->seq++;
        }
    } else {
        func_ov129_021d9760(wk);
        func_02051c88(wk->buttons);
        func_ov129_021d8fa0(wk);
    }
    return FALSE;
}

// pokebw2's CodeIn_StateMove
static BOOL func_ov129_021d8c90(CodeInWork *wk) {
    int i;
    ClActorScale scale;
    ClActorPos pos;
    ClActorPos barPos;

    switch (wk->seq) {
    case 0:
        func_ov129_021d9e48(wk, CODEIN_CURSOR_CODE, FALSE);
        for (i = 0; i < wk->codeMax; i++) {
            if (wk->code[i].move.wait == 0) {
                continue;
            }
            func_0204b454(wk->code[i].actor, &pos, 0);
            pos.x += wk->code[i].move.x;
            pos.y += wk->code[i].move.y;
            func_0204b404(wk->code[i].actor, &pos, 0);
            wk->code[i].move.wait--;
            if (i >= wk->largeStart && i < wk->largeEnd) {
                scale.x = data_ov129_021da830[wk->code[i].move.scale];
                scale.y = data_ov129_021da830[wk->code[i].move.scale];
                func_0204b5ac(wk->code[i].actor, &scale);
                wk->code[i].move.scale++;
            }
            if (i >= wk->smallStart && i < wk->smallEnd) {
                scale.x = data_ov129_021da84c[wk->code[i].move.scale];
                scale.y = data_ov129_021da84c[wk->code[i].move.scale];
                func_0204b5ac(wk->code[i].actor, &scale);
                wk->code[i].move.scale++;
            }
        }
        for (i = 0; i < CODEIN_BAR_COUNT; i++) {
            if (wk->bar[i].move.wait == 0) {
                continue;
            }
            func_0204b454(wk->bar[i].actor, &barPos, 0);
            barPos.x += wk->bar[i].move.x;
            barPos.y += wk->bar[i].move.y;
            func_0204b404(wk->bar[i].actor, &barPos, 0);
            wk->bar[i].move.wait--;
        }
        if (wk->code[0].move.wait == 0) {
            for (i = wk->largeStart; i < wk->largeEnd; i++) {
                func_0204b878(wk->code[i].actor, func_ov129_021da158(wk->code[i].state, wk->code[i].size));
            }
            for (i = wk->smallStart; i < wk->smallEnd; i++) {
                func_0204b878(wk->code[i].actor, func_ov129_021da158(wk->code[i].state, wk->code[i].size));
            }
            wk->seq++;
        }
        wk->count++;
        break;
    case 1:
        for (i = wk->largeStart; i < wk->largeEnd; i++) {
            if (wk->code[i].move.scale == SCALE_COUNT) {
                continue;
            }
            scale.x = data_ov129_021da830[wk->code[i].move.scale];
            scale.y = data_ov129_021da830[wk->code[i].move.scale];
            func_0204b5ac(wk->code[i].actor, &scale);
            wk->code[i].move.scale++;
        }
        for (i = wk->smallStart; i < wk->smallEnd; i++) {
            if (wk->code[i].move.scale == SCALE_COUNT) {
                continue;
            }
            scale.x = data_ov129_021da84c[wk->code[i].move.scale];
            scale.y = data_ov129_021da84c[wk->code[i].move.scale];
            func_0204b5ac(wk->code[i].actor, &scale);
            wk->code[i].move.scale++;
        }
        wk->count++;
        if (wk->count == SCALE_COUNT) {
            wk->seq++;
        }
        break;
    default:
        func_ov129_021da320(wk);
        if (wk->nextLast == FALSE) {
            i = func_ov129_021d97cc(wk, wk->nextWork);
        } else {
            i = func_ov129_021d97fc(wk, wk->nextWork);
        }
        func_ov129_021d9e60(wk, i);
        if (wk->focus != 0) {
            func_ov129_021d9e48(wk, CODEIN_CURSOR_CODE, TRUE);
        }
        func_ov129_021d97b8(wk);
        func_ov129_021d8af4(wk, CODEIN_STATE_INPUT);
        break;
    }
    return FALSE;
}

// pokebw2's CodeIn_Main
BOOL func_ov129_021d8f78(CodeInWork *wk) {
    BOOL done = data_ov129_021da718[wk->state](wk);

    func_ov129_021d9f40(wk);
    func_ov129_021da004(wk);
    return done;
}

// pokebw2's CodeIn_HandleKeys
static void func_ov129_021d8fa0(CodeInWork *wk) {
    int keys[KEY_ROWS][KEY_COLUMNS] = {
#include "asset/codein_pv_keys.inc"
};
    BOOL moved = FALSE;
    int key = keys[wk->cur[CODEIN_CURSOR_KEY].move.y][wk->cur[CODEIN_CURSOR_KEY].move.x];

    // The first key press after a touch only shows the cursor
    if (wk->touchMode == TRUE && GCTX_HIDGetPressedKeys()) {
        wk->touchMode = FALSE;
        func_ov129_021d9e98(wk, key);
        if (key == CODEIN_KEY_BACK || key == CODEIN_KEY_OK) {
            if (wk->cur[CODEIN_CURSOR_KEY].state != 2) {
                wk->cur[CODEIN_CURSOR_KEY].state = 2;
            }
        } else {
            if (wk->cur[CODEIN_CURSOR_KEY].state != 1) {
                wk->cur[CODEIN_CURSOR_KEY].state = 1;
            }
        }
        return;
    }
    if (GCTX_HIDGetTypedKeys() & PAD_KEY_UP) {
        if (wk->cur[CODEIN_CURSOR_KEY].move.y > 0) {
            wk->cur[CODEIN_CURSOR_KEY].move.y--;
        } else {
            wk->cur[CODEIN_CURSOR_KEY].move.y = KEY_ROWS - 1;
        }
        moved = TRUE;
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_DOWN) {
        wk->cur[CODEIN_CURSOR_KEY].move.y++;
        wk->cur[CODEIN_CURSOR_KEY].move.y %= KEY_ROWS;
        moved = TRUE;
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_RIGHT) {
        if (key == CODEIN_KEY_BACK) {
            wk->cur[CODEIN_CURSOR_KEY].move.x = 3;
        } else if (key == CODEIN_KEY_OK) {
            wk->cur[CODEIN_CURSOR_KEY].move.x = 0;
        } else {
            wk->cur[CODEIN_CURSOR_KEY].move.x++;
            wk->cur[CODEIN_CURSOR_KEY].move.x %= KEY_COLUMNS;
        }
        moved = TRUE;
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_LEFT) {
        if (key == CODEIN_KEY_BACK) {
            wk->cur[CODEIN_CURSOR_KEY].move.x = 3;
        } else if (key == CODEIN_KEY_OK) {
            wk->cur[CODEIN_CURSOR_KEY].move.x = 0;
        } else if (wk->cur[CODEIN_CURSOR_KEY].move.x > 0) {
            wk->cur[CODEIN_CURSOR_KEY].move.x--;
        } else {
            wk->cur[CODEIN_CURSOR_KEY].move.x = KEY_COLUMNS - 1;
        }
        moved = TRUE;
    } else if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
        if (key == CODEIN_KEY_BACK) {
            func_ov129_021d93d4(wk);
            GFL_SndSEPlay(SEQ_SE_CANCEL3);
        } else if (key == CODEIN_KEY_OK) {
            if (func_ov129_021d9320(wk)) {
                func_ov129_021d9348(wk);
                GFL_SndSEPlay(SEQ_SE_DECIDE2);
            } else {
                func_ov129_021d8af4(wk, CODEIN_STATE_ERROR);
            }
        } else {
            int digit;
            int group;
            int next;
            int nextGroup;

            if (wk->focus == 0) {
                return;
            }
            digit = wk->cur[CODEIN_CURSOR_CODE].state;
            wk->code[digit].state = key + 1;
            func_ov129_021d9e48(wk, CODEIN_CURSOR_KEY, FALSE);
            func_ov129_021d9e48(wk, CODEIN_CURSOR_PRESS, TRUE);
            func_ov129_021d9ee8(wk, key, CODEIN_CURSOR_PRESS);
            func_0204b878(wk->code[digit].actor,
                          func_ov129_021da158(wk->code[digit].state, wk->code[digit].size));
            func_0204b878(wk->cur[CODEIN_CURSOR_PRESS].actor, 3);
            group = wk->code[digit].group;
            next = digit + 1;
            if (next == wk->codeMax) {
                wk->next = CODEIN_NEXT_FOCUS;
                wk->nextWork = 0;
                wk->cur[CODEIN_CURSOR_KEY].move.x = 3;
                wk->cur[CODEIN_CURSOR_KEY].move.y = 2;
                moved = TRUE;
            } else {
                nextGroup = wk->code[next].group;
                if (group != nextGroup) {
                    wk->next = CODEIN_NEXT_FOCUS;
                    wk->nextWork = nextGroup;
                } else {
                    wk->next = CODEIN_NEXT_CURSOR;
                    wk->nextWork = next;
                }
                GFL_SndSEPlay(SEQ_SE_DECIDE1);
            }
        }
    } else if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
        func_ov129_021d93d4(wk);
        GFL_SndSEPlay(SEQ_SE_CANCEL3);
    } else if (GCTX_HIDGetTypedKeys() & PAD_BUTTON_L) {
        int digit;

        if (wk->cur[CODEIN_CURSOR_CODE].state == 0) {
            wk->cur[CODEIN_CURSOR_CODE].state = wk->codeMax - 1;
        } else {
            wk->cur[CODEIN_CURSOR_CODE].state--;
        }
        digit = wk->cur[CODEIN_CURSOR_CODE].state;
        if (wk->code[digit].size == 1) {
            wk->next = CODEIN_NEXT_CURSOR;
            wk->nextWork = digit;
        } else {
            wk->next = CODEIN_NEXT_FOCUS;
            wk->nextWork = wk->code[digit].group;
            wk->nextLast = TRUE;
        }
        GFL_SndSEPlay(SEQ_SE_SELECT1);
    } else if (GCTX_HIDGetTypedKeys() & PAD_BUTTON_R) {
        int digit;

        if (wk->cur[CODEIN_CURSOR_CODE].state == wk->codeMax - 1) {
            wk->cur[CODEIN_CURSOR_CODE].state = 0;
        } else {
            wk->cur[CODEIN_CURSOR_CODE].state++;
        }
        digit = wk->cur[CODEIN_CURSOR_CODE].state;
        if (wk->code[digit].size == 1) {
            wk->next = CODEIN_NEXT_CURSOR;
            wk->nextWork = digit;
        } else {
            wk->next = CODEIN_NEXT_FOCUS;
            wk->nextWork = wk->code[digit].group;
        }
        GFL_SndSEPlay(SEQ_SE_SELECT1);
    }
    if (moved == TRUE) {
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        key = keys[wk->cur[CODEIN_CURSOR_KEY].move.y][wk->cur[CODEIN_CURSOR_KEY].move.x];
        func_ov129_021d9e98(wk, key);
        if (key == CODEIN_KEY_BACK || key == CODEIN_KEY_OK) {
            if (wk->cur[CODEIN_CURSOR_KEY].state != 2) {
                wk->cur[CODEIN_CURSOR_KEY].state = 2;
            }
        } else {
            if (wk->cur[CODEIN_CURSOR_KEY].state != 1) {
                wk->cur[CODEIN_CURSOR_KEY].state = 1;
            }
        }
    }
}

// The number the digits make, empty ones counting as 0
// pokebw2's CodeIn_GetNumber
int func_ov129_021d92e4(CodeInWork *wk) {
    int i;
    int number = 0;
    int place = 1;

    for (i = wk->codeMax - 1; i > -1; i--) {
        if (wk->code[i].state != 0) {
            number += (wk->code[i].state - 1) * place;
        }
        place *= 10;
    }
    return number;
}

// Whether the code may be confirmed: always in mode 0, else when its number is at most the parameter's limit
static BOOL func_ov129_021d9320(CodeInWork *wk) {
    if (wk->param.mode == 0) {
        return TRUE;
    }
    if (func_ov129_021d92e4(wk) <= wk->param.unk1C) {
        return TRUE;
    }
    return FALSE;
}

// Writes the digits to the parameter's string, empty ones as 0, and ends the screen
// pokebw2's CodeIn_Confirm
static void func_ov129_021d9348(CodeInWork *wk) {
    int i;
    StrBuf *digit = GFL_StrBufCreate(100, HEAPID_CODEIN);

    wk->btn[1].state = 1;
    wk->btn[1].move.wait = 0;
    for (i = 0; i < wk->codeMax; i++) {
        if (wk->code[i].state == 0) {
            wk->code[i].state = 1;
            func_0204b878(wk->code[i].actor, func_ov129_021da158(wk->code[i].state, wk->code[i].size));
        }
        GFL_WordSetFormatNumber(digit, wk->code[i].state - 1, 1, 1, TRUE);
        func_02045aa8(wk->param.str, digit);
    }
    GFL_StrBufFree(digit);
    func_ov129_021d8af4(wk, CODEIN_STATE_END);
}

// Clears the cursor's digit and steps back, or goes to the last digit when nothing is focused
// pokebw2's CodeIn_Backspace
static void func_ov129_021d93d4(CodeInWork *wk) {
    int digit;
    int group;
    int prevGroup;

    wk->btn[0].state = 1;
    wk->btn[0].move.wait = 0;
    if (wk->focus == 0) {
        wk->cur[CODEIN_CURSOR_CODE].state = wk->codeMax - 1;
        group = wk->code[wk->cur[CODEIN_CURSOR_CODE].state].group;
        wk->next = CODEIN_NEXT_FOCUS;
        wk->nextWork = group;
        wk->nextLast = TRUE;
        return;
    }
    digit = wk->cur[CODEIN_CURSOR_CODE].state;
    wk->code[digit].state = 0;
    func_0204b878(wk->code[digit].actor, func_ov129_021da158(wk->code[digit].state, wk->code[digit].size));
    group = wk->code[digit].group;
    if (digit > 0) {
        func_0204b878(wk->code[digit - 1].actor,
                      func_ov129_021da158(wk->code[digit - 1].state, wk->code[digit - 1].size));
        prevGroup = wk->code[digit - 1].group;
        if (group != prevGroup) {
            wk->next = CODEIN_NEXT_FOCUS;
            wk->nextWork = prevGroup;
            wk->nextLast = TRUE;
        } else {
            wk->next = CODEIN_NEXT_CURSOR;
            wk->nextWork = digit - 1;
        }
    }
}

// pokebw2's CodeIn_InitButtons
void func_ov129_021d9490(CodeInWork *wk) {
    int i;

    for (i = 0; i < CODEIN_CODE_MAX; i++) {
        wk->code[i].rect = &wk->rects[i];
    }
    {
        CodeInKeyRect keys[CODEIN_KEY_COUNT] = {
#include "asset/codein_pv_keys_2.inc"
};

        for (; i < CODEIN_RECT_COUNT; i++) {
            wk->rects[i].top = keys[i - CODEIN_RECT_KEYS].y - keys[i - CODEIN_RECT_KEYS].halfHeight;
            wk->rects[i].left = keys[i - CODEIN_RECT_KEYS].x - keys[i - CODEIN_RECT_KEYS].halfWidth;
            wk->rects[i].bottom = keys[i - CODEIN_RECT_KEYS].y + keys[i - CODEIN_RECT_KEYS].halfHeight;
            wk->rects[i].right = keys[i - CODEIN_RECT_KEYS].x + keys[i - CODEIN_RECT_KEYS].halfWidth;
        }
    }
    wk->rects[CODEIN_RECT_COUNT].top = TOUCH_RECT_END;
    wk->buttons = func_02051b84(wk->rects, func_ov129_021d958c, wk, HEAPID_CODEIN);
}

// pokebw2's CodeIn_SetKeyCursorByRect
static void func_ov129_021d9564(CodeInWork *wk, int rect) {
    if (rect >= CODEIN_RECT_KEYS && rect < CODEIN_RECT_COUNT) {
        wk->cur[CODEIN_CURSOR_KEY].move.x = data_ov129_021da72c[rect - CODEIN_RECT_KEYS][0];
        wk->cur[CODEIN_CURSOR_KEY].move.y = data_ov129_021da72c[rect - CODEIN_RECT_KEYS][1];
    }
}

// pokebw2's CodeIn_ButtonCallback
static void func_ov129_021d958c(u32 button, u32 event, void *work) {
    CodeInWork *wk = work;

    if (wk->state != CODEIN_STATE_INPUT) {
        return;
    }
    if (wk->touchMode != TRUE) {
        wk->touchMode = TRUE;
    }
    if (event == BMN_EVENT_TOUCH) {
        if (button <= CODEIN_RECT_KEYS - 1) {
            if (wk->code[button].size == 1) {
                wk->next = CODEIN_NEXT_CURSOR;
                wk->nextWork = button;
            } else {
                wk->next = CODEIN_NEXT_FOCUS;
                wk->nextWork = wk->code[button].group;
            }
            GFL_SndSEPlay(SEQ_SE_SELECT1);
        } else if (button == CODEIN_RECT_BACK) {
            wk->cur[CODEIN_CURSOR_KEY].move.x = 0;
            wk->cur[CODEIN_CURSOR_KEY].move.y = 2;
            GFL_SndSEPlay(SEQ_SE_CANCEL3);
            func_ov129_021d93d4(wk);
        } else if (button == CODEIN_RECT_OK) {
            wk->cur[CODEIN_CURSOR_KEY].move.x = 3;
            wk->cur[CODEIN_CURSOR_KEY].move.y = 2;
            if (func_ov129_021d9320(wk)) {
                func_ov129_021d9348(wk);
                GFL_SndSEPlay(SEQ_SE_DECIDE2);
            } else {
                func_ov129_021d8af4(wk, CODEIN_STATE_ERROR);
            }
        } else if (button >= CODEIN_RECT_KEYS && button <= CODEIN_RECT_BACK - 1) {
            int digit;
            int key;
            int group;
            int next;
            int nextGroup;

            if (wk->focus != 0) {
                GFL_SndSEPlay(SEQ_SE_DECIDE1);
                digit = wk->cur[CODEIN_CURSOR_CODE].state;
                wk->code[digit].state = button - CODEIN_RECT_KEYS + 1;
                func_0204b878(wk->code[digit].actor,
                              func_ov129_021da158(wk->code[digit].state, wk->code[digit].size));
                func_ov129_021d9e48(wk, CODEIN_CURSOR_KEY, TRUE);
                key = button - CODEIN_RECT_KEYS;
                func_ov129_021d9e98(wk, key);
                func_ov129_021d9e48(wk, CODEIN_CURSOR_KEY, FALSE);
                func_ov129_021d9e48(wk, CODEIN_CURSOR_PRESS, TRUE);
                func_ov129_021d9ee8(wk, key, CODEIN_CURSOR_PRESS);
                func_0204b878(wk->cur[CODEIN_CURSOR_PRESS].actor, 3);
                group = wk->code[digit].group;
                next = digit + 1;
                if (next == wk->codeMax) {
                    wk->next = CODEIN_NEXT_FOCUS;
                    wk->nextWork = 0;
                    wk->nextLast = FALSE;
                } else {
                    nextGroup = wk->code[next].group;
                    if (group != nextGroup) {
                        wk->next = CODEIN_NEXT_FOCUS;
                        wk->nextWork = nextGroup;
                        wk->nextLast = FALSE;
                    } else {
                        wk->next = CODEIN_NEXT_CURSOR;
                        wk->nextWork = next;
                    }
                }
                func_ov129_021d9564(wk, button);
            }
        } else {
            wk->cur[CODEIN_CURSOR_KEY].move.x = (button - CODEIN_RECT_KEYS) % KEY_COLUMNS;
            wk->cur[CODEIN_CURSOR_KEY].move.y = (button - CODEIN_RECT_KEYS) / KEY_COLUMNS;
        }
    }
}

// Does what the input asked for: a change of focus, which the move state animates, or a step of the cursor
// pokebw2's CodeIn_RunNext
static void func_ov129_021d9760(CodeInWork *wk) {
    switch (wk->next) {
    case CODEIN_NEXT_NONE:
        break;
    case CODEIN_NEXT_FOCUS:
        func_ov129_021d8950(wk, wk->nextWork);
        func_ov129_021da164(wk);
        func_ov129_021da1a8(wk, TRUE);
        func_ov129_021d8af4(wk, CODEIN_STATE_MOVE);
        wk->next = CODEIN_NEXT_WAIT;
        break;
    case CODEIN_NEXT_CURSOR:
        func_ov129_021d9e60(wk, wk->nextWork);
        func_ov129_021d97b8(wk);
        break;
    case CODEIN_NEXT_WAIT:
        break;
    }
}

// pokebw2's CodeIn_ClearNext
static void func_ov129_021d97b8(CodeInWork *wk) {
    wk->next = CODEIN_NEXT_NONE;
    wk->nextWork = 0;
    wk->nextLast = FALSE;
}

// pokebw2's CodeIn_FindGroupFirst
static int func_ov129_021d97cc(CodeInWork *wk, int group) {
    int i;

    for (i = 0; i < wk->codeMax; i++) {
        if (wk->code[i].group == group) {
            return i;
        }
    }
    return 0;
}

// pokebw2's CodeIn_FindGroupLast
static int func_ov129_021d97fc(CodeInWork *wk, int group) {
    int i;
    BOOL found = FALSE;

    for (i = 0; i < wk->codeMax; i++) {
        if (wk->code[i].group == group) {
            found = TRUE;
        } else if (found == TRUE) {
            return i - 1;
        }
    }
    return wk->codeMax - 1;
}
