#include "global.h"

#include "overlay_70_gtsapplication_state.h"

#include "unk_02005D10.h"

typedef unsigned short undefined2;

typedef unsigned int undefined4;

void ov70_02238D84(GTSApplicationState *arg0, s32 arg1, s32 arg2);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov70_0223CAC4(GTSApplicationState *arg0, s32 strno, u32 textSpeed);
#else
undefined4 ov70_0223CAC4();
#endif

undefined4 ov70_0223C23C(GTSApplicationState *param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4);

undefined4 ov70_0223C23C(GTSApplicationState *param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4)

{
    param_1->submittedRequirements.species = 0;
#if defined(SDK_PORT) || defined(NONMATCHING)
    ov70_0223CAC4(param_1, 0x26, 1);
#else
    ov70_0223CAC4(param_1, 0x26, 1, 0, 0xf0f);
#endif
    ov70_02238D84(param_1, 0x15, 1);
    PlaySE(0x5f3);
    return 3;
}
