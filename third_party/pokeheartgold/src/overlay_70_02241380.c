#include "global.h"
#include "overlay_manager.h"
#include "save_special_ribbons.h"
#include "unk_02088288.h"
#include "overlay_70_gtsapplication_state.h"

typedef struct UnkStruct_ov70_022413AC_v0 {
    u8 unk_0[0x8];
    void *unk_8;
    void *unk_C;
} UnkStruct_ov70_022413AC_v0;

s32 ov70_02238F9C(void *, int, s32);

void ov70_02241380(GTSApplicationState *arg0);

void *ov70_0223E49C(void *, void *, u32, s32);
extern u8 ov70_02245D48;
s32 ov70_022413AC(GTSApplicationState *arg0);
s32 ov70_02238E50(void *, s32, s32);
s32 ov70_02241468(GTSApplicationState *arg0);
void ov70_02238E58(GTSApplicationState *arg0);
s32 ov70_02241494(GTSApplicationState *arg0);

void ov70_02241380(GTSApplicationState *arg0) {
    s32 v0;

    v0 = 0;
    do {
        ov70_02238F9C(arg0->avatarSprites[v0], arg0->avatarPositions[v0][0], arg0->avatarPositions[v0][1] + 0x20);
        v0 += 1;
    } while (v0 < 8);
}

s32 ov70_022413AC(GTSApplicationState *arg0) {
    UnkStruct_ov70_022413AC_v0 *v0;

    v0 = *(void **)arg0;
    arg0->pokemonSummary.party = ov70_0223E49C(v0->unk_8, v0->unk_C, arg0->selectedBoxId, arg0->partySlotIndex);
    arg0->pokemonSummary.unk11 = 2;
    arg0->pokemonSummary.partyCount = 1;
    arg0->pokemonSummary.partySlot = 0;
    arg0->pokemonSummary.unk12 = 1;
    arg0->pokemonSummary.moveToLearn = 0;
    arg0->pokemonSummary.unk2C = sub_02088288(*(SaveData **)((u8 *)arg0->playerData + 0x20));
    arg0->pokemonSummary.natDexEnabled = (s32) * (s32 *)((u8 *)arg0->playerData + 0x30);
    arg0->pokemonSummary.options = *(Options **)((u8 *)arg0->playerData + 0x24);
    arg0->pokemonSummary.ribbons = Save_SpecialRibbons_Get(*(SaveData **)((u8 *)arg0->playerData + 0x20));
    arg0->pokemonSummary.isFlag982Set = sub_0208828C(*(SaveData **)((u8 *)arg0->playerData + 0x20));
    sub_02089D40(&arg0->pokemonSummary, &ov70_02245D48);
    sub_0208AD34(&arg0->pokemonSummary, *(PlayerProfile **)((u8 *)arg0->playerData + 0x1C));
    arg0->appMan = OverlayManager_New(&gOverlayTemplate_PokemonSummary, &arg0->pokemonSummary, HEAP_ID_61);
    arg0->appManActive = 1;
    return 2;
}

s32 ov70_02241468(GTSApplicationState *arg0) {
    s32 v0;

    v0 = 3;
    if (OverlayManager_Run(arg0->appMan) != 0) {
        OverlayManager_Delete(arg0->appMan);
        ov70_02238E50(arg0, 5, arg0->screenArgument);
        v0 = 4;
    }
    return v0;
}

s32 ov70_02241494(GTSApplicationState *arg0) {
    ov70_02238E58(arg0);
    return 1;
}
