#include "global.h"

#include "list_menu_items.h"
#include "party.h"
#include "pokemon.h"
#include "render_window.h"
#include "touchscreen_list_menu.h"
#include "overlay_70_gtsapplication_state.h"

typedef struct UnkStruct_ov70_0223DB94_temp_r1 {
    u8 unk_0[0x8];
    Party *unk_8;
    void *unk_C;
} UnkStruct_ov70_0223DB94_temp_r1;

s32 ov70_02238D60(void *);

s32 ov70_02238D84(void *, s32, s32);

s32 ov70_02238E50(void *, s32, s32);

s32 ov70_0223DE6C(void *);

s32 ov70_0223E01C(void *, s32, s32, s32, u16, s32);

s32 ov70_0223E490(s32);

BoxPokemon *ov70_0223E49C(Party *, PCStorage *, u32, s32);

s32 ov70_0223E4FC(BoxPokemon *boxMon);

s32 ov70_0223E538(void *);

s32 ov70_0223E59C(void *);

s32 ov70_0223DB94(GTSApplicationState *arg0);

s32 ov70_0223DB94(GTSApplicationState *arg0) {
    s32 temp_r0_2;
    void *temp_r4;
    s32 var_r4;
    u32 temp_r0;
    UnkStruct_ov70_0223DB94_temp_r1 *temp_r1;

    temp_r0 = (u32)TouchscreenListMenu_HandleInput(arg0->unk_11D0);
    switch (temp_r0) {
    case 1:
        ov70_02238D60(arg0);
        ListMenuItems_Delete(arg0->unk_11AC);
        arg0->currentScreenInstruction = 2;
        ov70_02238E50(arg0, 8, 6);
        break;
    case 2:
        ov70_02238D60(arg0);
        ListMenuItems_Delete(arg0->unk_11AC);
        ClearFrameAndWindow2(&arg0->bottomInstructionWindow, 0);
        temp_r1 = *(void **)arg0;
        temp_r4 = ov70_0223E49C(temp_r1->unk_8, temp_r1->unk_C, arg0->selectedBoxId, arg0->partySlotIndex);
        if (ov70_0223E4FC(temp_r4) != 0) {
            ov70_0223E01C(arg0, 0x25, 1, 0, 0xF0F, 1);
            ov70_02238D84(arg0, 4, 1);
        } else {
            temp_r0_2 = ov70_0223E538(temp_r4);
            if (temp_r0_2 != 0) {
                if (temp_r0_2 == 1) {
                    ov70_0223E01C(arg0, 0xB1, 1, 0, 0xF0F, 1);
                } else {
                    ov70_0223E01C(arg0, 0xB2, 1, 0, 0xF0F, 1);
                }
                ov70_02238D84(arg0, 4, 1);
            } else if (ov70_0223E59C(temp_r4) != 0) {
                ov70_0223E01C(arg0, 0xB3, 1, 0, 0xF0F, 1);
                ov70_02238D84(arg0, 4, 1);
            } else {
                var_r4 = 0;
                if ((ov70_0223E490(arg0->selectedBoxId) != 0) && (GetMonData(Party_GetMonByIndex(*(Party **)((u8 *)(*(void **)arg0) + 8), (s32) arg0->partySlotIndex), 0xA2, NULL) != 0)) {
                    var_r4 = 1;
                    arg0->currentScreenInstruction = 0xB;
                }
                if (var_r4 == 0) {
                    ov70_0223DE6C(arg0);
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
