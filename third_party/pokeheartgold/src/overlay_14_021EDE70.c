#include "global.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F08BC(u8 *param_1);
void ov14_021EDE70(void *arg0);

s32 ov14_021E91E0(UnkStruct_ov14_boxapp *app);

void ov14_021EDE70(void *arg0) {
    ov14_021F08BC(arg0);
    ov14_021F0234(arg0, ov14_021E91E0, 0x55);
}
