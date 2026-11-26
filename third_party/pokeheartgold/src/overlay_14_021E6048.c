#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "menu_input_state.h"
#include "party.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "unk_02019BA4.h"
#include "overlay_14.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

void ov14_021E6094(struct UnkStruct_ov14_boxapp *arg0, u32 arg1, s32 arg2, void *arg3);

void ov14_021E6100(struct UnkStruct_ov14_boxapp *arg0, u32 arg1, u32 arg2);

BoxPokemon *ov14_021E60C0(struct UnkStruct_ov14_boxapp *arg0, u32 arg1, u32 arg2);
u32 ov14_021E6070(struct UnkStruct_ov14_boxapp *arg0, s32 arg1, s32 arg2, void *arg3);

void ov14_021E6048(UnkStruct_ov14_boxapp *param_1) {
    BOOL BVar1;

    BVar1 = GridInputHandler_IsButtonInputMode(param_1->sys->unk_2C);
    if (BVar1 == 1) {
        MenuInputStateMgr_SetState(param_1->unk_0->menuInputStatePtr, MENU_INPUT_STATE_BUTTONS);
        return;
    }
    MenuInputStateMgr_SetState(param_1->unk_0->menuInputStatePtr, MENU_INPUT_STATE_TOUCH);
    return;
}

u32 ov14_021E6070(UnkStruct_ov14_boxapp *arg0, s32 arg1, s32 attr, void *ptr) {
    BoxPokemon *boxMon;

    boxMon = ov14_021E60C0(arg0, arg0->unk1F, arg1);
    if (boxMon == NULL) {
        return 0U;
    }
    return GetBoxMonData(boxMon, attr, ptr);
}

void ov14_021E6094(UnkStruct_ov14_boxapp *arg0, u32 arg1, s32 attr, void *ptr) {
    BoxPokemon *boxMon;

    boxMon = ov14_021E60C0(arg0, arg0->unk1F, arg1);
    if (boxMon != NULL) {
        SetBoxMonData(boxMon, attr, ptr);
        if (arg1 < 0x1EU) {
            PCStorage_SetBoxModified(arg0->storage, arg0->unk1F);
        }
    }
}

BoxPokemon *ov14_021E60C0(UnkStruct_ov14_boxapp *arg0, u32 boxno, u32 slotno) {
    u32 slot;

    slot = slotno;
    if ((boxno == 0xFF) || (slot >= 0x1EU)) {
        if (slot >= 0x1EU) {
            slot -= 0x1E;
        }
        if ((u32)Party_GetCount(arg0->party) > slot) {
            return Mon_GetBoxMon(Party_GetMonByIndex(arg0->party, (s32)slot));
        }
        return NULL;
    }
    if (slot == 0xFF) {
        return NULL;
    }
    return PCStorage_GetMonByIndexPair(arg0->storage, boxno, slotno);
}

void ov14_021E6100(UnkStruct_ov14_boxapp *arg0, u32 boxno, u32 slotno) {
    if (slotno < 0x1EU) {
        PCStorage_DeleteBoxMonByIndexPair(arg0->storage, boxno, slotno);
        return;
    }
    Party_RemoveMon(arg0->party, slotno - 0x1E);
}
