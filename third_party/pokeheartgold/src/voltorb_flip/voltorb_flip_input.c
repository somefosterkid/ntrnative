#include "voltorb_flip/voltorb_flip_input.h"

#include "global.h"

#include "constants/sndseq.h"

#include "heap.h"
#include "sprite_system.h"
#include "system.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_020192D0.h"

static void VoltorbFlipInputHandler_HandleDpad(VoltorbFlipInputHandler *inputHandler);
static void VoltorbFlipInputHandler_MoveMemoCursorToExit(VoltorbFlipInputHandler *inputHandler);
static int VoltorbFlipInputHandler_HandleInput_Memo(VoltorbFlipInputHandler *inputHandler);
static int VoltorbFlipInputHandler_HandleInput_NoMemo(VoltorbFlipInputHandler *inputHandler);
static void VoltorbFlipInputHandler_SetFocus(VoltorbFlipInputHandler *inputHandler, int newFocus);
static void VoltorbFlipInputHandler_SetMemoCursorSpritePos(VoltorbFlipInputHandler *inputHandler);
static void VoltorbFlipInputHandler_UpdateBoardCursorAnim(VoltorbFlipInputHandler *inputHandler, int newFocus);
static void VoltorbFlipInputHandler_HandleChangeFocus(VoltorbFlipInputHandler *inputHandler, int newFocus, int oldFocus, MenuInputState inputMode);
static void VoltorbFlipInputCB_OnButtonPress(void *data, int nextInput, int prevInput);
static void VoltorbFlipInputCB_OnTouchSwitch(void *data, int nextInput, int prevInput);
static void VoltorbFlipInputCB_OnKeyMove(void *data, int nextInput, int prevInput);
static void VoltorbFlipInputCB_OnTouch(void *data, int nextInput, int prevInput);

const GridCallbacks sInputCallbacks = {
    VoltorbFlipInputCB_OnButtonPress,
    VoltorbFlipInputCB_OnTouchSwitch,
    VoltorbFlipInputCB_OnKeyMove,
    VoltorbFlipInputCB_OnTouch,
};

// clang-format off
const TouchscreenHitbox sMemoTouchscreenHitboxes[] = {
    { .rect = {  80, 104, 200, 224 } },
    { .rect = {  80, 104, 224, 248 } },
    { .rect = { 104, 128, 200, 224 } },
    { .rect = { 104, 128, 224, 248 } },
    { .rect = { 128, 152, 224, 248 } },
    { .rect = {   8,  72, 192, 256 } },
    { .rect = { 160, 192, 192, 256 } },
    { .rect = TOUCHSCREEN_RECTLIST_END },
};

TouchscreenHitbox sTouchscreenHitboxes[] __attribute__((section(BWPORT_DATA_SECTION))) = {
    { .rect = {   0,  0,   0,  0 } },
    { .rect = {   0,  0,  0,  0 } },
    { .rect = {   0,  0,  0,  0 } },
    { .rect = {   0,  0, 0, 0 } },
    { .rect = {   0,  0, 0, 0 } },
    { .rect = {  0,  0,   0,  0 } },
    { .rect = {  0,  0,  0,  0 } },
    { .rect = {  0,  0,  0,  0 } },
    { .rect = {  0,  0, 0, 0 } },
    { .rect = {  0,  0, 0, 0 } },
    { .rect = {  0,  0,   0,  0 } },
    { .rect = {  0,  0,  0,  0 } },
    { .rect = {  0,  0,  0,  0 } },
    { .rect = {  0,  0, 0, 0 } },
    { .rect = {  0,  0, 0, 0 } },
    { .rect = { 0, 0,   0,  0 } },
    { .rect = { 0, 0,  0,  0 } },
    { .rect = { 0, 0,  0,  0 } },
    { .rect = { 0, 0, 0, 0 } },
    { .rect = { 0, 0, 0, 0 } },
    { .rect = { 0, 0,   0,  0 } },
    { .rect = { 0, 0,  0,  0 } },
    { .rect = { 0, 0,  0,  0 } },
    { .rect = { 0, 0, 0, 0 } },
    { .rect = { 0, 0, 0, 0 } },
    { .rect = {   0,  0, 0,   0 } },
    { .rect = { 0, 0, 0,   0 } },
    { .rect = TOUCHSCREEN_RECTLIST_END },
};

DpadMenuBox sDpadButtonSpecs[] __attribute__((section(BWPORT_DATA_SECTION))) = {
    {   0,   0, 0, 0, 0,  0, 0, 0  },
    {  0,   0, 0, 0, 0,  0,  0, 0  },
    {  0,   0, 0, 0, 0,  0,  0, 0  },
    { 0,   0, 0, 0, 0,  0,  0, 0  },
    { 0,   0, 0, 0, 0,  0,  0, 0 },
    {   0,  0, 0, 0,  0, 0, 0, 0  },
    {  0,  0, 0, 0,  0, 0,  0, 0  },
    {  0,  0, 0, 0,  0, 0,  0, 0  },
    { 0,  0, 0, 0,  0, 0,  0, 0  },
    { 0,  0, 0, 0,  0, 0,  0, 0 },
    {   0,  0, 0, 0,  0, 0, 0, 0 },
    {  0,  0, 0, 0,  0, 0, 0, 0 },
    {  0,  0, 0, 0,  0, 0, 0, 0 },
    { 0,  0, 0, 0,  0, 0, 0, 0 },
    { 0,  0, 0, 0,  0, 0, 0, 0 },
    {   0, 0, 0, 0, 0, 0, 0, 0 },
    {  0, 0, 0, 0, 0, 0, 0, 0 },
    {  0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    {   0, 0, 0, 0, 0,  0, 0, 0 },
    {  0, 0, 0, 0, 0,  0, 0, 0 },
    {  0, 0, 0, 0, 0,  0, 0, 0 },
    { 0, 0, 0, 0, 0,  0, 0, 0 },
    { 0, 0, 0, 0, 0,  0, 0, 0 },
    { 0,  0,  0,  0, 0, 0, 0, 0 },
    { 0, 0,  0,  0, 0, 0, 0, 0 },
};

u8 sMemoCursorPos[5][2] = {
    { 200,  80 },
    { 224,  80 },
    { 200, 104 },
    { 224, 104 },
    { 224, 128 },
};
// clang-format on

VoltorbFlipInputHandler *VoltorbFlipInputHandler_Create(enum HeapID heapID, struct ManagedSprite *boardCursorSprite, struct ManagedSprite *memoCursorSprite) {
    GF_ASSERT(boardCursorSprite != NULL);

    VoltorbFlipInputHandler *ptr = Heap_Alloc(heapID, sizeof(VoltorbFlipInputHandler));
    MI_CpuFill8(ptr, 0, sizeof(VoltorbFlipInputHandler));

    ptr->boardCursorSprite = boardCursorSprite;
    ptr->memoCursorSprite = memoCursorSprite;
    ptr->gridInputHandler = GridInputHandler_Create(sTouchscreenHitboxes, sDpadButtonSpecs, &sInputCallbacks, ptr, 1, ptr->focus, heapID);

    VoltorbFlipInputHandler_SetFocus(ptr, 0);
    return ptr;
}

void VoltorbFlipInputHandler_Free(VoltorbFlipInputHandler *inputHandler) {
    GF_ASSERT(inputHandler->gridInputHandler != 0);
    GridInputHandler_Free(inputHandler->gridInputHandler);
    Heap_Free(inputHandler);
}

int VoltorbFlipInputHandler_HandleInput(VoltorbFlipInputHandler *inputHandler) {
    if (inputHandler->memoFocused) {
        return VoltorbFlipInputHandler_HandleInput_Memo(inputHandler);
    } else {
        return VoltorbFlipInputHandler_HandleInput_NoMemo(inputHandler);
    }
}

void VoltorbFlipInputHandler_SetBoardFocused(VoltorbFlipInputHandler *inputHandler, BOOL draw) {
    if (draw) {
        ManagedSprite_SetDrawFlag(inputHandler->boardCursorSprite, TRUE);
    } else {
        ManagedSprite_SetDrawFlag(inputHandler->boardCursorSprite, FALSE);
        inputHandler->focus = 0;
    }
}

void VoltorbFlipInputHandler_SetMemoOpen(VoltorbFlipInputHandler *inputHandler, BOOL open) {
    inputHandler->memoOpen = open;
    VoltorbFlipInputHandler_UpdateBoardCursorAnim(inputHandler, inputHandler->focus);
}

void VoltorbFlipInputHandler_SetMemoFocused(VoltorbFlipInputHandler *inputHandler, BOOL focused) {
    inputHandler->memoFocused = focused;
    VoltorbFlipInputHandler_UpdateBoardCursorAnim(inputHandler, inputHandler->focus);
    ManagedSprite_SetDrawFlag(inputHandler->memoCursorSprite, focused);
}

int VoltorbFlipInputHandler_GetCursorPos(VoltorbFlipInputHandler *inputHandler) {
    GF_ASSERT(inputHandler != 0);
    GF_ASSERT(inputHandler->gridInputHandler != 0);
    // This seems to return which touch screen hitbox was pressed/clicked
    return GridInputHandler_GetNextInput(inputHandler->gridInputHandler);
}

int VoltorbFlipInputHandler_GetMemoButtonID(VoltorbFlipInputHandler *inputHandler) {
    GF_ASSERT(inputHandler != 0);
    GF_ASSERT(inputHandler->gridInputHandler != 0);
    return inputHandler->memoButtonID;
}

BOOL VoltorbFlipInputHandler_IsCursorInGridRange(VoltorbFlipInputHandler *inputHandler) {
    int input = GridInputHandler_GetNextInput(inputHandler->gridInputHandler);
    return input != VOLTORB_FLIP_INPUT_MEMO && input != VOLTORB_FLIP_INPUT_QUIT;
}

void VoltorbFlipInputHandler_SetGridCursor0(VoltorbFlipInputHandler *inputHandler) {
    GridInputHandler_SetNextInput(inputHandler->gridInputHandler, 0);
    VoltorbFlipInputHandler_SetFocus(inputHandler, 0);
}

void VoltorbFlipInputHandler_SetMemoCursor0(VoltorbFlipInputHandler *inputHandler) {
    inputHandler->memoButtonID = 0;
    inputHandler->lastMemoButtonID = 0;
    VoltorbFlipInputHandler_SetMemoCursorSpritePos(inputHandler);
}

BOOL VoltorbFlipInputHandler_GetTouchNew(VoltorbFlipInputHandler *inputHandler) {
    return inputHandler->touchNew;
}

// Probably for handling focus movement
static void VoltorbFlipInputHandler_HandleDpad(VoltorbFlipInputHandler *inputHandler) {
    int newMemoButtonID;
    u8 lastMemoButtonID = inputHandler->memoButtonID;

    if (gSystem.newAndRepeatedKeys & PAD_KEY_DOWN) {
        newMemoButtonID = lastMemoButtonID + 2;
        if (newMemoButtonID > 4) {
            newMemoButtonID = 4;
        }
        inputHandler->memoButtonID = newMemoButtonID;
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) {
        newMemoButtonID = lastMemoButtonID - 1;
        if (newMemoButtonID < 0) {
            newMemoButtonID = 0;
        }
        inputHandler->memoButtonID = newMemoButtonID;
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_UP) {
        if (lastMemoButtonID == 4 && (inputHandler->lastMemoButtonID == 2 || inputHandler->lastMemoButtonID == 3)) {
            inputHandler->memoButtonID = inputHandler->lastMemoButtonID;
        } else {
            newMemoButtonID = lastMemoButtonID - 2;
            if (newMemoButtonID < 0) {
                newMemoButtonID = 0;
            }
            inputHandler->memoButtonID = newMemoButtonID;
        }
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {
        newMemoButtonID = lastMemoButtonID + 1;
        if (newMemoButtonID > 4) {
            newMemoButtonID = 4;
        }
        inputHandler->memoButtonID = newMemoButtonID;
    }

    if (lastMemoButtonID != inputHandler->memoButtonID) {
        inputHandler->lastMemoButtonID = lastMemoButtonID;
        VoltorbFlipInputHandler_SetMemoCursorSpritePos(inputHandler);
        PlaySE(SEQ_SE_DP_SELECT);
    }
}

static void VoltorbFlipInputHandler_MoveMemoCursorToExit(VoltorbFlipInputHandler *inputHandler) {
    inputHandler->memoButtonID = 4;
    VoltorbFlipInputHandler_SetMemoCursorSpritePos(inputHandler);
}

static int VoltorbFlipInputHandler_HandleInput_Memo(VoltorbFlipInputHandler *inputHandler) {
    inputHandler->touchNew = FALSE;

    if (gSystem.newKeys & PAD_BUTTON_X) {
        return 4;
    }
    if (gSystem.newKeys & PAD_BUTTON_B) {
        VoltorbFlipInputHandler_MoveMemoCursorToExit(inputHandler);
        return 3;
    }
    if (gSystem.newKeys & PAD_BUTTON_A) {
        if (inputHandler->memoButtonID == 4) {
            VoltorbFlipInputHandler_MoveMemoCursorToExit(inputHandler);
            return 3;
        }
        return 1;
    }

    VoltorbFlipInputHandler_HandleDpad(inputHandler);
    inputHandler->touchNew = TRUE;

    u8 input = TouchscreenHitbox_FindRectAtTouchNew(sMemoTouchscreenHitboxes);
    if (input < NELEMS(sMemoTouchscreenHitboxes)) { // bug: limit should be 1 fewer
        switch (input) {
        case 4:
            inputHandler->memoButtonID = input;
            VoltorbFlipInputHandler_SetMemoCursorSpritePos(inputHandler);
            return 3;
        case 5:
            return 4;
        case 6:
            return 2;
        default: // 0-3
            inputHandler->memoButtonID = input;
            VoltorbFlipInputHandler_SetMemoCursorSpritePos(inputHandler);
            return 1;
        }
    }
    return 0;
}

static int VoltorbFlipInputHandler_HandleInput_NoMemo(VoltorbFlipInputHandler *inputHandler) {
    int newFocus = GridInputHandler_GetNextInput(inputHandler->gridInputHandler);
    int elementId = GridInputHandler_HandleInput_AllowHold(inputHandler->gridInputHandler);
    GridInputHandler_GetNextInput(inputHandler->gridInputHandler);

    if (elementId < NELEMS(sTouchscreenHitboxes)) { // bug: limit should be 1 lower
        inputHandler->touchNew = System_GetTouchNew();
        // Open/close Memo button
        if (elementId == VOLTORB_FLIP_INPUT_MEMO) {
            if (inputHandler->touchNew) {
                inputHandler->focus = newFocus;
                GridInputHandler_SetNextInput(inputHandler->gridInputHandler, inputHandler->focus);
            } else {
                VoltorbFlipInputHandler_SetGridCursor0(inputHandler);
            }
            return 4;
        }
        // Quit button
        if (elementId == VOLTORB_FLIP_INPUT_QUIT) {
            return 2;
        }
        return 1;
    }

    inputHandler->touchNew = FALSE;
    if (gSystem.newKeys & PAD_BUTTON_X) {
        if (inputHandler->focus == VOLTORB_FLIP_INPUT_MEMO || inputHandler->focus == VOLTORB_FLIP_INPUT_QUIT) {
            VoltorbFlipInputHandler_SetGridCursor0(inputHandler);
        }
        return 4;
    }
    if (gSystem.newKeys & PAD_BUTTON_B) {
        return 3;
    }

    if (inputHandler->memoOpen && inputHandler->focus != VOLTORB_FLIP_INPUT_MEMO && inputHandler->focus != VOLTORB_FLIP_INPUT_QUIT) {
        u32 whichButton = TouchscreenHitbox_FindRectAtTouchNew(sMemoTouchscreenHitboxes);
        if (whichButton < 8) {
            switch (whichButton) {
            case 0:
                return 5;
            case 1:
                return 6;
            case 2:
                return 7;
            case 3:
                return 8;
            }
        }
    }
    return 0;
}

static void VoltorbFlipInputHandler_SetFocus(VoltorbFlipInputHandler *inputHandler, int newFocus) {
    u8 x;
    u8 y;

    VoltorbFlipInputHandler_UpdateBoardCursorAnim(inputHandler, newFocus);
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(inputHandler->gridInputHandler, newFocus), &x, &y);
    ManagedSprite_SetPositionXY(inputHandler->boardCursorSprite, x, y);
    inputHandler->focus = newFocus;
}

static void VoltorbFlipInputHandler_SetMemoCursorSpritePos(VoltorbFlipInputHandler *inputHandler) {
    ManagedSprite_SetPositionXY(inputHandler->memoCursorSprite, sMemoCursorPos[inputHandler->memoButtonID][0], sMemoCursorPos[inputHandler->memoButtonID][1]);
}

static void VoltorbFlipInputHandler_UpdateBoardCursorAnim(VoltorbFlipInputHandler *inputHandler, int newFocus) {
    int indicatorType;

    if (newFocus < VOLTORB_FLIP_INPUT_GRID_NUM) { // In grid
        if (inputHandler->memoFocused) {
            indicatorType = 24; // Disabled pencil
        } else if (inputHandler->memoOpen) {
            indicatorType = 23; // Pencil
        } else {
            indicatorType = 18; // Card focus
        }
    } else if (newFocus == VOLTORB_FLIP_INPUT_MEMO) { // Memo
        indicatorType = 7;                            // Open/close Memo button
    } else if (newFocus == VOLTORB_FLIP_INPUT_QUIT) { // Quit
        indicatorType = 4;                            // Quit button
    } else {
        GF_ASSERT(FALSE);
    }

    if (inputHandler->memoOpen) {
        ManagedSprite_SetPaletteOverride(inputHandler->boardCursorSprite, 4);
    } else {
        ManagedSprite_SetPaletteOverride(inputHandler->boardCursorSprite, 2);
    }
    ManagedSprite_SetAnimNoRestart(inputHandler->boardCursorSprite, indicatorType);
}

static void VoltorbFlipInputHandler_HandleChangeFocus(VoltorbFlipInputHandler *inputHandler, int newFocus, int oldFocus, MenuInputState inputMode) {
    if ((inputMode == MENU_INPUT_STATE_BUTTONS || newFocus != VOLTORB_FLIP_INPUT_MEMO) && newFocus != oldFocus && newFocus < NELEMS(sTouchscreenHitboxes)) {
        // Updates visual focus indicator
        VoltorbFlipInputHandler_SetFocus(inputHandler, newFocus);
        // Plays when you switch focus
        PlaySE(SEQ_SE_DP_SELECT);
    }
}

// DpadGridCallbacks

static void VoltorbFlipInputCB_OnButtonPress(void *data, int nextInput, int prevInput_unused) {
    VoltorbFlipInputHandler *inputHandler = data;

    GridInputHandler_SetNextInput(inputHandler->gridInputHandler, (u8)nextInput);
    VoltorbFlipInputHandler_SetFocus(inputHandler, nextInput);
    PlaySE(SEQ_SE_DP_SELECT);
}

static void VoltorbFlipInputCB_OnTouchSwitch(void *data, int nextInput, int prevInput_unused) {
    // no-op
}

static void VoltorbFlipInputCB_OnKeyMove(void *data, int nextInput, int prevInput) {
    VoltorbFlipInputHandler *inputHandler = data;

    if (nextInput == VOLTORB_FLIP_COL4_ROWLAST) { // wrap around left
        nextInput = inputHandler->selectedRow * 5 + 4;
        GF_ASSERT(nextInput < VOLTORB_FLIP_INPUT_GRID_NUM);
        GridInputHandler_SetNextInput(inputHandler->gridInputHandler, (u8)nextInput);
    } else if (nextInput == VOLTORB_FLIP_COL0_ROWLAST) { // wrap around right
        nextInput = inputHandler->selectedRow * 5;
        GF_ASSERT(nextInput < VOLTORB_FLIP_INPUT_GRID_NUM);
        GridInputHandler_SetNextInput(inputHandler->gridInputHandler, (u8)nextInput);
    }

    if (nextInput != VOLTORB_FLIP_INPUT_MEMO && nextInput != VOLTORB_FLIP_INPUT_QUIT) {
        inputHandler->selectedRow = nextInput / 5;
    }
    VoltorbFlipInputHandler_HandleChangeFocus(inputHandler, nextInput, prevInput, MENU_INPUT_STATE_BUTTONS);
}

static void VoltorbFlipInputCB_OnTouch(void *data, int nextInput, int prevInput) {
    VoltorbFlipInputHandler *inputHandler = data;

    VoltorbFlipInputHandler_HandleChangeFocus(inputHandler, nextInput, prevInput, MENU_INPUT_STATE_TOUCH);
}

// tools/hg_assets.py: the tables above are blanks, port/src/asset_load.c fills them from the ROM at start
void PORT_AssetAdd(const char *game, const char *name, void *p, unsigned long size, const char *layout);
__attribute__((constructor)) static void hgasset_register_src_voltorb_flip_voltorb_flip_input_c_0(void) {
    PORT_AssetAdd("IPKE", "src/voltorb_flip/voltorb_flip_input.c:sDpadButtonSpecs", (void *)&sDpadButtonSpecs, sizeof(sDpadButtonSpecs), "u8");
    PORT_AssetAdd("IPGE", "src/voltorb_flip/voltorb_flip_input.c:sDpadButtonSpecs", (void *)&sDpadButtonSpecs, sizeof(sDpadButtonSpecs), "u8");
    PORT_AssetAdd("IPKE", "src/voltorb_flip/voltorb_flip_input.c:sTouchscreenHitboxes", (void *)&sTouchscreenHitboxes, sizeof(sTouchscreenHitboxes), "u8");
    PORT_AssetAdd("IPGE", "src/voltorb_flip/voltorb_flip_input.c:sTouchscreenHitboxes", (void *)&sTouchscreenHitboxes, sizeof(sTouchscreenHitboxes), "u8");
}
