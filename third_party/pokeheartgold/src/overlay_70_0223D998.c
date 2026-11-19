#include "global.h"

#include "list_menu_items.h"
#include "party.h"
#include "pokemon.h"
#include "render_window.h"
#include "touchscreen_list_menu.h"
#include "overlay_70_gtsapplication_state.h"

typedef struct UnkStruct_ov70_0223D998_v3 {
    u8 unk_0[0x8];
    Party *unk_8;
    void *unk_C;
} UnkStruct_ov70_0223D998_v3;

typedef struct UnkStruct_ov70_0223D998_v2 {
    u8 unk_0[0x8];
    Party *unk_8;
    void *unk_C;
} UnkStruct_ov70_0223D998_v2;

s32 ov70_02238D60(void *);

s32 ov70_02238D84(void *, s32, s32);

s32 ov70_02238E50(void *, s32, s32);

s32 ov70_0223E01C(void *, s32, s32, s32, u16, s32);

s32 ov70_0223E490(s32);

BoxPokemon *ov70_0223E49C(Party *, PCStorage *, u32, s32);

s32 ov70_0223E4FC(BoxPokemon *boxMon);

s32 ov70_0223E538(void *);

s32 ov70_0223E59C(void *);

s32 ov70_0223D998(GTSApplicationState *arg0);

TouchscreenListMenu *ov70_02238CAC(GTSApplicationState *, u8, u8);
s32 ov70_0223DB30(GTSApplicationState *arg0);

s32 ov70_0223D998(GTSApplicationState *arg0) {
    s32 v1;
    void *v4;
    s32 v5;
    u32 v0;
    UnkStruct_ov70_0223D998_v2 *v2;
    UnkStruct_ov70_0223D998_v3 *v3;

    v0 = (u32)TouchscreenListMenu_HandleInput(arg0->unk_11D0);
    switch (v0) {
    case 1:
        ov70_02238D60(arg0);
        ListMenuItems_Delete(arg0->unk_11AC);
        ClearFrameAndWindow2(&arg0->bottomInstructionWindow, 0);
        arg0->currentScreenInstruction = 2;
        ov70_02238E50(arg0, 8, 5);
        break;
    case 2:
        ov70_02238D60(arg0);
        ListMenuItems_Delete(arg0->unk_11AC);
        v2 = *(void **)arg0;
        v4 = ov70_0223E49C(v2->unk_8, v2->unk_C, arg0->selectedBoxId, arg0->partySlotIndex);
        if (ov70_0223E4FC(v4) != 0) {
            ov70_0223E01C(arg0, 0x25, 1, 0, 0xF0F, 1);
            ov70_02238D84(arg0, 4, 1);
        } else {
            v1 = ov70_0223E538(v4);
            if (v1 != 0) {
                if (v1 == 1) {
                    ov70_0223E01C(arg0, 0xB1, 1, 0, 0xF0F, 1);
                } else {
                    ov70_0223E01C(arg0, 0xB2, 1, 0, 0xF0F, 1);
                }
                ov70_02238D84(arg0, 4, 1);
            } else if (ov70_0223E59C(v4) != 0) {
                ov70_0223E01C(arg0, 0xB3, 1, 0, 0xF0F, 1);
                ov70_02238D84(arg0, 4, 1);
            } else {
                v5 = 0;
                if ((ov70_0223E490(arg0->selectedBoxId) != 0) && (GetMonData(Party_GetMonByIndex(*(Party **)((u8 *)(*(void **)arg0) + 8), (s32) arg0->partySlotIndex), 0xA2, NULL) != 0)) {
                    v5 = 1;
                    arg0->currentScreenInstruction = 0xE;
                }
                if (v5 == 0) {
                    v3 = *(void **)arg0;
                    arg0->selectedBoxMon = ov70_0223E49C(v3->unk_8, v3->unk_C, arg0->selectedBoxId, arg0->partySlotIndex);
                    arg0->currentScreenInstruction = 2;
                    ov70_02238E50(arg0, 6, 0);
                }
            }
        }
        break;
    case -2:
    case 3:
        ov70_02238D60(arg0);
        ListMenuItems_Delete(arg0->unk_11AC);
        ClearFrameAndWindow2(&arg0->bottomInstructionWindow, 0);
        arg0->currentScreenInstruction = 0;
        break;
    }
    return 3;
}

s32 ov70_0223DB30(GTSApplicationState *arg0) {
    arg0->unk_11AC = ListMenuItems_New(3U, HEAP_ID_61);
    ListMenuItems_AppendFromMsgData(arg0->unk_11AC, arg0->gtsMessageLoader, 0x57, 1);
    ListMenuItems_AppendFromMsgData(arg0->unk_11AC, arg0->gtsMessageLoader, 0x58, 2);
    ListMenuItems_AppendFromMsgData(arg0->unk_11AC, arg0->gtsMessageLoader, 0x59, 3);
    arg0->unk_11D0 = ov70_02238CAC(arg0, 3, 0xA);
    arg0->currentScreenInstruction = 0xA;
    return 3;
}
