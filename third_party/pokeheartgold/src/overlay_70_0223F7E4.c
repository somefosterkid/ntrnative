#include "global.h"

#include "list_menu_items.h"
#include "error_handling.h"
#include "wild_encounter.h"
#include "overlay_70_gtsapplication_state.h"

extern s32 ov70_022459F4;

extern s32 ov70_02245A4C;

u32 ov70_0223F7E4(ListMenuItem **arg0, MsgData *arg1, s32 arg2);

void ov70_0223F828(GTSPokemonRequirements *requirements, s32 levelIndex, s32 isRange);

u32 ov70_0223F7E4(ListMenuItem **arg0, MsgData *arg1, s32 arg2) {
    MsgData *msgData;
    s32 value;
    s32 *v0;
    u32 n;

    msgData = arg1;
    if (arg2 == 0) {
        v0 = &ov70_02245A4C;
        n = 0xC;
    } else {
        v0 = &ov70_022459F4;
        n = 0xB;
    }
    *arg0 = ListMenuItems_New(n, HEAP_ID_61);
    value = 0;
    if ((s32)n > 0) {
        do {
            ListMenuItems_AppendFromMsgData(*arg0, msgData, *v0, value);
            value += 1;
            v0 += 2;
        } while (value < (s32)n);
    }
    return n;
}

void ov70_0223F828(GTSPokemonRequirements *requirements, s32 levelIndex, s32 isRange) {
    s32 *v1;
    void *v0;

    if (isRange == 0) {
        v1 = &ov70_02245A4C;
        if (levelIndex >= 0xC) {
            GF_AssertFail();
        }
    } else {
        v1 = &ov70_022459F4;
        if (levelIndex >= 0xB) {
            GF_AssertFail();
        }
    }
    v0 = v1 + levelIndex * 2;
    requirements->level = (s8) * (s16 *)((u8 *)v0 + 4);
    requirements->level2 = (s8) * (s16 *)((u8 *)v0 + 6);
}
