#include "global.h"

#include "trainer_card_main.h"

s32 ov51_021E6A54(void *);

s32 ov51_021E6AAC(void *, s32 *);

unsigned int ov51_021E6B44(TrainerCardMain *arg0) {
    s32 v0;
    TrainerCardMain *new_var;
#if defined(SDK_PORT) || defined(NONMATCHING)
    u32 ret;
#endif
    v0 = 0;
    arg0->unk_3108 = -1;
    new_var = arg0;
#if defined(SDK_PORT) || defined(NONMATCHING)
    ret = ov51_021E6AAC(arg0, &v0);
    if (v0 != 0) {
        new_var->inTouchMode = 1;
        return ret; /* r0 on DS = result of ov51_021E6AAC() */
    }
    ret = ov51_021E6A54(arg0);
    if (ret != 0) {
        arg0->inTouchMode = 0;
    }
    return ret; /* r0 on DS = result of ov51_021E6A54() */
#else
    ov51_021E6AAC(arg0, &v0);
    if (v0 != 0) {
        new_var->inTouchMode = 1;
        return;
    }
    if (ov51_021E6A54(arg0) != 0) {
        arg0->inTouchMode = 0;
    }
#endif
}
