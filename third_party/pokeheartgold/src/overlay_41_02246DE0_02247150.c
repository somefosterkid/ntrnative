#include "global.h"

#include "bg_window.h"
#include "error_handling.h"
#include "game_stats.h"
#include "heap.h"
#include "menu_input_state.h"
#include "overlay_manager.h"
#include "system.h"
#include "unk_020210A0.h"
#include "yes_no_prompt.h"
#include "overlay_41_unk_struct_ov22_0225_a0_e4.h"
#include "overlay_41_unk_struct_ov22_02254de0.h"

s32 ov41_02247274(void *);

s32 ov41_02247310(void *);

s32 ov41_022473F0(void *);

s32 ov41_022474C4(void *);

s32 ov41_02247568(void *);

s32 ov41_022476A8(void *);

s32 ov41_022479A8(void *, void *, void *);

s32 ov41_02248F6C(void *);

s32 ov41_022499DC(void *);

s32 ov41_02247150(OverlayManager *man, s32 *state);

s32 ov41_02247150(OverlayManager *man, s32 *state) {
    MenuInputStateMgr *stateMgr;
    s32 *v0;
    void *v1;
    void *v2;

    v1 = OverlayManager_GetData(man);
    v2 = OverlayManager_GetArgs(man);
    if (*((s32 *)&((UnkStruct_ov22_0225A0E4 *)v1)->unk_6C0) == 1) {
        GameStats_AddScore(*(GameStats **)((u8 *)v2 + 0x10), 8);
        ov41_022479A8(*(void **)((u8 *)v2 + 4), v1 + 0x3F4, *(void **)((u8 *)v2 + 0x14));
    }
    v0 = *(s32 **)((u8 *)v2 + 0x18);
    if (v0 != NULL) {
        if (*((s32 *)&((UnkStruct_ov22_0225A0E4 *)v1)->unk_6C0) == 1) {
            *v0 = 1;
        } else {
            *v0 = 0;
        }
    }
    stateMgr = *(MenuInputStateMgr **)((u8 *)v2 + 0x20);
    if (stateMgr != NULL) {
        MenuInputStateMgr_SetState(stateMgr, *((enum MenuInputState *)&((UnkStruct_ov22_0225A0E4 *)v1)->unk_6EC));
    }
    YesNoPrompt_Destroy(((UnkStruct_ov22_0225A0E4 *)v1)->unk_6B8);
    WindowArray_Delete(((UnkStruct_ov22_0225A0E4 *)v1)->unk_6BC, 1);
    ov41_022476A8(v1);
    ov41_02248F6C(v1 + 0x498);
    ov41_02247568(v1);
    ov41_022474C4(v1);
    ov41_02247310(v1);
    ov41_022473F0(v1);
    ov41_02245ECC(((UnkStruct_ov22_0225A0E4 *)v1)->unk_364);
    ((UnkStruct_ov22_0225A0E4 *)v1)->unk_364 = NULL;
    ov41_022499DC(v1 + 0x35C);
    ov41_02247274(v1);
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    if (DisableTouchPad() != 1) {
        GF_AssertFail();
    }
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_13);
    Heap_Destroy(HEAP_ID_14);
    return 1;
}
