#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "menu_input_state.h"
#include "overlay_manager.h"
#include "sound_02004A44.h"
#include "system.h"
#include "unk_020210A0.h"
#include "yes_no_prompt.h"
#include "overlay_41_unk_struct_ov22_0225_a0_e4.h"
#include "overlay_41_unk_struct_ov22_02254de0.h"

s32 ov41_02247240(void *);

s32 ov41_02247288(void *, void *, s32, s32);

s32 ov41_02247334(void *);

void ov41_02247478(void *param0);

s32 ov41_02247480(void *, s32);

s32 ov41_022474D4(void *);

s32 ov41_0224765C(void *, void *);

s32 ov41_02248E84(void *, void *);

s32 ov41_02248F18(void *, void *, void *, void *, void *, s32);

s32 ov41_022499B4(void *, s32, s32);

s32 ov41_02246DE0(OverlayManager *man, s32 *state);

s32 ov41_02246DE0(OverlayManager *man, s32 *state) {
    MenuInputStateMgr *stateMgr;
    enum MenuInputState v1;
    void *arg;
    void *v0;

    Heap_Create(HEAP_ID_3, HEAP_ID_13, 0x20000U);
    Heap_Create(HEAP_ID_3, HEAP_ID_14, 0x40000U);
    arg = OverlayManager_CreateAndGetData(man, sizeof(UnkStruct_ov22_0225A0E4), HEAP_ID_13);
    memset(arg, 0, sizeof(UnkStruct_ov22_0225A0E4));
    Main_SetVBlankIntrCB(ov41_02247478, arg);
    HBlankInterruptDisable();
    v0 = OverlayManager_GetArgs(man);
    *((s32 *)&((UnkStruct_ov22_0225A0E4 *)arg)->unk_6DC) = (s32) * (s32 *)((u8 *)v0 + 0xC);
    stateMgr = *(MenuInputStateMgr **)((u8 *)v0 + 0x20);
    if (stateMgr != NULL) {
        v1 = MenuInputStateMgr_GetState(stateMgr);
        *((enum MenuInputState *)&((UnkStruct_ov22_0225A0E4 *)arg)->unk_6EC) = v1;
    } else {
        v1 = MENU_INPUT_STATE_BUTTONS;
        *((enum MenuInputState *)&((UnkStruct_ov22_0225A0E4 *)arg)->unk_6EC) = v1;
    }
    EnableTouchPad();
    InitializeTouchPad(4);
    ov41_02248E84(*(void **)((u8 *)v0 + 8), arg + 0x184);
    ov41_02247240(arg);
    ov41_022499B4(arg + 0x35C, 0x2CF, 0xD);
    ((UnkStruct_ov22_0225A0E4 *)arg)->unk_364 = ov41_02245EA0(0x2BC, HEAP_ID_13);
    ov41_02247288(arg, *(void **)v0, 0xA, 0);
    ov41_02247334(arg);
    ov41_02247480(arg, 0);
    ov41_022474D4(arg);
    ov41_0224765C(arg, *(void **)((u8 *)v0 + 0xC));
    ov41_02248F18(arg + 0x498, arg + 0x3F4, arg + 0x368, arg, arg + 0x568, 1);
    *((YesNoPrompt **)&((UnkStruct_ov22_0225A0E4 *)arg)->unk_6B8) = YesNoPrompt_Create(HEAP_ID_13);
    *((Window **)&((UnkStruct_ov22_0225A0E4 *)arg)->unk_6BC) = AllocWindows(HEAP_ID_13, 1);
    *((s32 *)&((UnkStruct_ov22_0225A0E4 *)arg)->unk_6B0) = 0;
    Sound_SetSceneAndPlayBGM(0x35U, 0U, 0);
    return 1;
}
