#include "global.h"

#include "overlay_49.h"

s32 ov49_02258D54(s32);

s32 ov49_02258D70(s32, s32);

s32 ov49_02258EEC(s32, s32, s32);

s32 ov49_02258F38(s32);

s32 ov49_02259FF0(s32);

s32 ov49_0225A010(s32);

s32 ov49_0225EF88(void *);

s32 ov49_0225EF90(void *);

s32 ov49_0225EF98(s32, s32, s32 *, s32);

extern s32 ov49_02269B78;

s32 ov49_022618C0(void *arg0, s32 arg1, s32 arg2) {
    s32 v0;
    s32 v1;
    s32 v2;
    s32 v3;

    v0 = arg2;
    v1 = ov49_02259FF0(arg1);
    v3 = ov49_02258D70(v1, v0);
    v2 = ov49_0225EF88(arg0);
    switch (v2) {
    case 0:
        ov49_02258EEC(v1, v3, 3);
        ov49_0225EF90(arg0);
        break;
    case 1:
        if (ov49_02258F38(v3) != 0) {
            ov49_0225EF90(arg0);
        }
        break;
    case 2:
        ov49_02258D54(v3);
        ov49_0225EF98(ov49_0225A010(arg1), v0, &ov49_02269B78, 0);
        break;
    }
    return 0;
}
