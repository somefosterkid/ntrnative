#include "global.h"

#include "party.h"
#include "pokemon_types_def.h"
#include "overlay_70_gtsapplication_state.h"

typedef struct UnkStruct_ov70_0223DE6C_temp_r1 {
    u8 unk_0[0x8];
    Party *unk_8;
    void *unk_C;
} UnkStruct_ov70_0223DE6C_temp_r1;

s32 ov70_02238D84(void *, s32, s32);

s32 ov70_02238E50(void *, s32, s32);

s32 ov70_0223E01C(void *, s32, s32, s32, u16, s32);

void *ov70_0223E49C(Party *, void *, u32, s32);

s32 ov70_0223E690(void *, void *);

s32 ov70_0223E76C(void *);

s32 ov70_0223DE6C(GTSApplicationState *arg0);

s32 ov70_0223DE6C(GTSApplicationState *arg0) {
    UnkStruct_ov70_0223DE6C_temp_r1 *temp_r1;

    if ((ov70_0223E76C(&arg0->searchResults[arg0->selectedSearchResult]) != 0) && (arg0->selectedBoxId != 0x12) && (Party_GetCount(*(Party **)((u8 *)(*(void **)arg0) + 8)) == 6)) {
        ov70_0223E01C(arg0, 0x1C, 1, 0, 0xF0F, 1);
        ov70_02238D84(arg0, 4, 1);
        return 0;
    }
    temp_r1 = *(void **)arg0;
    arg0->selectedBoxMon = ov70_0223E49C(temp_r1->unk_8, temp_r1->unk_C, arg0->selectedBoxId, arg0->partySlotIndex);
    arg0->currentScreenInstruction = 2;
    arg0->fadeBothScreens = 1;
    ov70_02238E50(arg0, 7, 9);
    ov70_0223E690(&arg0->receivedListing, arg0);
    arg0->searchResultCount = 0;
    return 1;
}
