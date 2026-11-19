#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_bg_switch.h"

BattleBgSwitch *ov07_0221DF1C(BattleAnimSystem *system);

BattleBgSwitch *ov07_0221DF1C(BattleAnimSystem *system) {
    int i;
    BattleBgSwitch *bgSwitch;

    bgSwitch = Heap_Alloc((enum HeapID)system->heapID, sizeof(BattleBgSwitch));
    if (bgSwitch == NULL) {
        GF_AssertFail();
        return NULL;
    }
    memset(bgSwitch, 0, sizeof(BattleBgSwitch));
    bgSwitch->unread_00 = 0;
    bgSwitch->state = 0;
    bgSwitch->unk_0F = 0;
    bgSwitch->unk_0E = 0;
    bgSwitch->battleAnimSystem = system;
    bgSwitch->blendCoeffA = 0;
    bgSwitch->blendCoeffB = 0x1F;
    bgSwitch->targetBlendCoeffA = 0x1D;
    bgSwitch->targetBlendCoeffB = 2;
    if (ov07_0221C4A8(system, 5) == 1) {
        bgSwitch->blendCoeffA = 0;
        bgSwitch->blendCoeffB = 0x1F;
        bgSwitch->targetBlendCoeffA = 0xF;
        bgSwitch->targetBlendCoeffB = 7;
    }
    if (ov07_0221C4A8(system, 5) == 2) {
        bgSwitch->blendCoeffA = 7;
        bgSwitch->blendCoeffB = 0xF;
        bgSwitch->targetBlendCoeffA = 0x1D;
        bgSwitch->targetBlendCoeffB = 2;
    }
    for (i = 0; i < 10; i++) {
        bgSwitch->scriptVars[i] = system->scriptVars[i];
    }
    system->bgSwitchState = 1;
    return bgSwitch;
}
