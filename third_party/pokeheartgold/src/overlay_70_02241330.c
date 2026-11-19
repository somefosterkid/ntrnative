#include "global.h"

#include "overlay_70_gtsapplication_state.h"

void ov70_022410F0(void *, int, u32);

extern u32 ov70_02245D0A;

extern u32 ov70_02245D0C;

void ov70_02241330(GTSApplicationState *param_1, s32 param_2, s32 param_3);

s32 ov70_02238F9C(void *, int, int);
void ov70_02241358(GTSApplicationState *arg0);

void ov70_02241330(GTSApplicationState *param_1, s32 param_2, s32 param_3) {
    ov70_022410F0(param_1->unk_F0C, *(u16 *)(&ov70_02245D0A + param_2), param_3 + (*(u16 *)(&ov70_02245D0C + param_2) + 0x20));
    return;
}

void ov70_02241358(GTSApplicationState *arg0) {
    s32 v0;

    v0 = 0;
    do {
        ov70_02238F9C(arg0->avatarSprites[v0], arg0->avatarPositions[v0][0], arg0->avatarPositions[v0][1]);
        v0 += 1;
    } while (v0 < 8);
}
