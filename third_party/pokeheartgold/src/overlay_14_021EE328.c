#include "global.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);

s32 ov14_021E9450(UnkStruct_ov14_boxapp *app);

void ov14_021EE328(void *arg0) {
    ov14_021F0234(arg0, ov14_021E9450, 0xE);
}
