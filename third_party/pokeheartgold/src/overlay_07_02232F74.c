#include "global.h"
struct BallRotation;
#include "overlay_manager.h"
#include "overlay_07_ball_rotation.h"

void ov07_02232F74(struct BallRotation *arg0, s32 arg1);

struct OverlayManager;
BOOL ov07_02232F80(OverlayManager *appMan, int *state);

void ov07_02232F74(BallRotation *arg0, s32 arg1) {
    *((s32 *)&arg0->unk_14) = arg1;
    *((s32 *)&arg0->unk_08) = 0;
    *((s32 *)&arg0->unk_0C) = 0;
    *((s32 *)&arg0->unk_10) = 0;
}

BOOL ov07_02232F80(OverlayManager *appMan, int *state) {
    return 1;
}
