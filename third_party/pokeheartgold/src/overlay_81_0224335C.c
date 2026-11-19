#include "global.h"
#include "overlay_81_battle_factory_app.h"

#include "overlay_81.h"
#include "unk_02019BA4.h"

s32 ov81_0224141C();

s32 ov81_02241650();

BOOL ov81_02242710(BattleFactoryApp *arg0);
void ov81_02243220(GridInputHandler *);
GridInputHandler *ov81_022432AC(void *, u8);
GridInputHandler *ov81_022432DC(void *, u8);
GridInputHandler *ov81_02243370(void *arg0, GridInputHandler *arg1);

void ov81_0224335C(s32 arg0, s32 arg1) {
    if (arg1 >= 4) {
        ov81_0224141C();
        return;
    }
    ov81_02241650();
}

GridInputHandler *ov81_02243370(void *arg0, GridInputHandler *arg1) {
    ov81_02243220(arg1);
    if (ov81_02242710(arg0) == 1) {
        return ov81_022432DC(arg0, 0);
    }
    return ov81_022432AC(arg0, 0);
}
