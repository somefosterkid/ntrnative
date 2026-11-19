#include "global.h"

#include "unk_020932E0.h"

s32 sub_020935E0(UnkStruct_020932E0 *arg0) {
#if defined(SDK_PORT) || defined(NONMATCHING)
    // DS: cases 3 and default fall out with arg0 still in r0 (no return value); the port returns 0 (keep running)
    switch (arg0->unk_14) {
    case 0:
        return sub_02093630(arg0);
    case 1:
        sub_0209389C(arg0);
        return 0;
    case 2:
        sub_02093908(arg0);
        return 0;
    case 4:
        sub_0209396C(arg0);
        return 0;
    case 5:
        sub_020939B8(arg0);
        return 0;
    case 6:
        sub_0209511C(arg0);
        return 2;
    default:
        return 0;
    }
#else
    u32 v0;
    s32 v1;

    v1 = (s32)arg0;
    v0 = arg0->unk_14;
    switch (v0) {
    case 0:
        return sub_02093630(arg0);
    case 1:
        sub_0209389C(arg0);
        return 0;
    case 2:
        sub_02093908(arg0);
        return 0;
    case 4:
        sub_0209396C(arg0);
        return 0;
    case 5:
        sub_020939B8(arg0);
        return 0;
    case 6:
        sub_0209511C(arg0);
        v1 = 2;
    default:
        return v1;
    }
#endif
}
