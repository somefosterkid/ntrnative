#include "global.h"

#include "overlay_49.h"
#include "overlay_49_getters.h"
#include "overlay_49_plaza_api.h"

extern const UnkStruct_ov70_0226DA18 ov49_02269B78;

BOOL ov49_022618C0(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2);

BOOL ov49_022618C0(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2) {
    u32 v0;
    UnkStruct_ov70_0225CA20 *v1;
    u32 v2;
    UnkStruct_ov70_0225CC54 *v3;

    v0 = param2;
    v1 = ov49_02259FF0(param1);
    v3 = ov49_02258D70(v1, v0);
    v2 = ov49_0225EF88(param0);
    switch (v2) {
    case 0:
        ov49_02258EEC(v1, v3, 3);
        ov49_0225EF90(param0);
        break;
    case 1:
        if (ov49_02258F38(v3) != 0) {
            ov49_0225EF90(param0);
        }
        break;
    case 2:
        ov49_02258D54(v3);
        ov49_0225EF98(ov49_0225A010(param1), v0, &ov49_02269B78, 0);
        break;
    }
    return 0;
}
