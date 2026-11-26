#include "global.h"
#include "overlay_14_box_funcs.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov14_021F6698(UnkStruct_ov14_boxapp *arg0, s32 arg1, s32 arg2);
#else
void ov14_021F6698(UnkStruct_ov14_boxapp *arg0, s32 arg1, s32 arg2);
#endif
void ov14_021F6844(void *arg0, s32 arg1, s32 arg2);

void ov14_021F6844(void *arg0, s32 arg1, s32 arg2) {
    s32 v0;

    v0 = arg1;
    switch (v0) {
    case 0:
        v0 = 7;
        break;
    case 1:
        v0 = 0xA;
        break;
    }
    ov14_021F6698(arg0, v0, arg2);
}
