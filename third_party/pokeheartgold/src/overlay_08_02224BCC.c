#include "global.h"

#include "constants/heap.h"

#include "sprite.h"
#include "sys_task.h"
#include "unk_02005D10.h"
#include "unk_02009D48.h"
#include "unk_02020A0C.h"
#include "screen_fade_internal.h"
#include "overlay_08_battle_sub_menu_cursor.h"

struct BattleCursor;
struct SysTaskQueue;
void ov12_0226BAFC(struct BattleCursor *cursorRenderer, int x1, int x2, int y1, int y2);
void ov08_02224BC0(BattleSubMenuCursor *cursor);
typedef void (*UnkCallback)(void);
enum ButtonMode {
    BUTTON_MODE_NORMAL = 0,
    BUTTON_MODE_START_IS_X = 1,
    BUTTON_MODE_SWAP_XY = 2,
    BUTTON_MODE_L_IS_A = 3,
};
typedef struct System {
    Callback vBlankIntr;
    void *vBlankIntrArg;
    Callback hBlankIntr;
    void *hBlankIntrArg;
    UnkCallback unk10;
    UnkCallback unk14;
    SysTaskQueue *mainTaskQueue;
    SysTaskQueue *vblankTaskQueue;
    SysTaskQueue *vwaitTaskQueue;
    SysTaskQueue *printTaskQueue;
    u32 *unk28;
    u32 vblankCounter;
    u32 frameCounter;
    int buttonMode;
    int heldKeysRaw;
    int newKeysRaw;
    int newAndRepeatedKeysRaw;
    int heldKeys;
    int newKeys;
    int newAndRepeatedKeys;
    int keyRepeatCounter;
    int keyRepeatContinueDelay;
    int keyRepeatStartDelay;
    int simulatedInputs;
    u16 touchX;
    u16 touchY;
    u16 touchNew;
    u16 touchHeld;
    u8 touchpadReadAuto;
    u8 screensFlipped;
    u8 unk6A;
    u8 lidClosedPauseDisabled;
    u8 softResetDisabled;
    BOOL unk70;
    u32 *unk74;
} System;
extern System gSystem;
enum GridMenuCursorPositionDirection {
    GRID_MENU_CURSOR_POSITION_DIRECTION_UP = 0,
    GRID_MENU_CURSOR_POSITION_DIRECTION_DOWN,
    GRID_MENU_CURSOR_POSITION_DIRECTION_LEFT,
    GRID_MENU_CURSOR_POSITION_DIRECTION_RIGHT,
    GRID_MENU_CURSOR_POSITION_DIRECTION_NONE,
};
static u8 ov08_02224BFC(BattleSubMenuCursor *cursor);
static BOOL ov08_02224C48(const DpadMenuBox *newPosition, u8 incomingDirection);

void ov08_02224BCC(BattleSubMenuCursor *cursor, const DpadMenuBox *positions);
void ov08_02224BF8(BattleSubMenuCursor *cursor, u32 mask);
static u8 ov08_02224BFC(BattleSubMenuCursor *cursor);
static BOOL ov08_02224C48(const DpadMenuBox *newPosition, u8 incomingDirection);
u32 ov08_02224C94(BattleSubMenuCursor *cursor);

void ov08_02224BCC(BattleSubMenuCursor *cursor, const DpadMenuBox *positions) {
    ov08_02224BC0(cursor);

    cursor->positions = positions;
    cursor->enabledPositionsMask = 0xffffffff;

    if (cursor->isVisible == 1) {
        ov12_0226BAFC(cursor->sprites, cursor->positions[0].left, cursor->positions[0].width, cursor->positions[0].top, cursor->positions[0].height);
    }
}

void ov08_02224BF8(BattleSubMenuCursor *cursor, u32 mask) {
    cursor->enabledPositionsMask = mask;
}

static u8 ov08_02224BFC(BattleSubMenuCursor *cursor) {
    if (cursor->isVisible == 1) {
        return 1;
    }

    if (gSystem.newKeys & ((0x0040 | 0x0080 | 0x0020 | 0x0010) | 0x0002 | 0x0001)) {
        cursor->isVisible = 1;

        ov12_0226BAFC(cursor->sprites, cursor->positions[cursor->currentPositionIndex].left, cursor->positions[cursor->currentPositionIndex].width, cursor->positions[cursor->currentPositionIndex].top, cursor->positions[cursor->currentPositionIndex].height);
        PlaySE(1500);
    }

    return 0;
}

static BOOL ov08_02224C48(const DpadMenuBox *newPosition, u8 incomingDirection) {
    switch (incomingDirection) {
    case GRID_MENU_CURSOR_POSITION_DIRECTION_UP:
        if (newPosition->buttonDown & 0x80) {
            return 1;
        }
        break;
    case GRID_MENU_CURSOR_POSITION_DIRECTION_DOWN:
        if (newPosition->buttonUp & 0x80) {
            return 1;
        }
        break;
    case GRID_MENU_CURSOR_POSITION_DIRECTION_LEFT:
        if (newPosition->buttonRight & 0x80) {
            return 1;
        }
        break;
    case GRID_MENU_CURSOR_POSITION_DIRECTION_RIGHT:
        if (newPosition->buttonLeft & 0x80) {
            return 1;
        }
        break;
    }

    return 0;
}

u32 ov08_02224C94(BattleSubMenuCursor *cursor) {
    u8 nextPositionIndex;
    u8 pressedDirection;

    if (ov08_02224BFC(cursor) == 0) {
        return 0xffffffff;
    }

    if (gSystem.newKeys & (0x0040)) {
        nextPositionIndex = DpadMenuBox_GetNeighborInDirection(cursor->positions, 0L, 0L, 0L, 0L, cursor->currentPositionIndex, GRID_MENU_CURSOR_POSITION_DIRECTION_UP);
        pressedDirection = GRID_MENU_CURSOR_POSITION_DIRECTION_UP;
    } else if (gSystem.newKeys & (0x0080)) {
        nextPositionIndex = DpadMenuBox_GetNeighborInDirection(cursor->positions, 0L, 0L, 0L, 0L, cursor->currentPositionIndex, GRID_MENU_CURSOR_POSITION_DIRECTION_DOWN);
        pressedDirection = GRID_MENU_CURSOR_POSITION_DIRECTION_DOWN;
    } else if (gSystem.newKeys & (0x0020)) {
        nextPositionIndex = DpadMenuBox_GetNeighborInDirection(cursor->positions, 0L, 0L, 0L, 0L, cursor->currentPositionIndex, GRID_MENU_CURSOR_POSITION_DIRECTION_LEFT);
        pressedDirection = GRID_MENU_CURSOR_POSITION_DIRECTION_LEFT;
    } else if (gSystem.newKeys & (0x0010)) {
        nextPositionIndex = DpadMenuBox_GetNeighborInDirection(cursor->positions, 0L, 0L, 0L, 0L, cursor->currentPositionIndex, GRID_MENU_CURSOR_POSITION_DIRECTION_RIGHT);
        pressedDirection = GRID_MENU_CURSOR_POSITION_DIRECTION_RIGHT;
    } else {
        nextPositionIndex = 0xffffffff;
    }

    if (nextPositionIndex != 0xff) {
        u8 nextPositionIsEnabled = 1;

        if (nextPositionIndex & 0x80) {
            if (cursor->previousPositionIndex != 0xff) {
                nextPositionIndex = cursor->previousPositionIndex;
            } else {
                nextPositionIndex ^= 0x80;
            }
        }

        while (1) {
            u8 replacementPositionIndex;

            if (cursor->enabledPositionsMask & (1 << nextPositionIndex)) {
                break;
            }

            nextPositionIsEnabled = 0;
            replacementPositionIndex = DpadMenuBox_GetNeighborInDirection(cursor->positions, 0L, 0L, 0L, 0L, nextPositionIndex, pressedDirection) & (0xff ^ 0x80);

            if ((replacementPositionIndex == nextPositionIndex) || (replacementPositionIndex == cursor->currentPositionIndex)) {
                nextPositionIndex = cursor->currentPositionIndex;
                break;
            }

            nextPositionIndex = replacementPositionIndex;
        }

        if (cursor->currentPositionIndex != nextPositionIndex) {
            u8 x1, y1, x2, y2;

            DpadMenuBox_GetPosition(&cursor->positions[nextPositionIndex], &x1, &y1);
            DpadMenuBox_GetDimensions(&cursor->positions[nextPositionIndex], &x2, &y2);

            if ((ov08_02224C48(&cursor->positions[nextPositionIndex], pressedDirection) == 1) && (nextPositionIsEnabled != 0)) {
                cursor->previousPositionIndex = cursor->currentPositionIndex;
            } else {
                cursor->previousPositionIndex = 0xff;
            }

            cursor->currentPositionIndex = nextPositionIndex;

            ov12_0226BAFC(cursor->sprites, x1, x2, y1, y2);
            PlaySE(1500);
        }

        return 0xffffffff;
    }

    if (gSystem.newKeys & (0x0001)) {
        return cursor->currentPositionIndex;
    }

    if (gSystem.newKeys & (0x0002)) {
        PlaySE(1501);
        return 0xfffffffe;
    }

    return 0xffffffff;
}
