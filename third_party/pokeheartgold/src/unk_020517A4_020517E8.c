#include "global.h"

#include "trainer_data.h"
#include "overlay_manager.h"

s32 BattleSetup_GetTransitionAndMusicParam(void *arg0);

s32 BattleStartGetTransition(s32, BattleSetup *);

s32 BattleSetup_GetWildTransitionEffect(BattleSetup *setup);

struct OverlayManager;
BOOL sub_020517F8(OverlayManager *appMan);

s32 BattleSetup_GetWildTransitionEffect(BattleSetup *setup) {
    return BattleStartGetTransition(BattleSetup_GetTransitionAndMusicParam(setup), setup);
}

BOOL sub_020517F8(OverlayManager *appMan) {
    return 1;
}
