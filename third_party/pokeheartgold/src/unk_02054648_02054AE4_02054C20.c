#include "global.h"

#include "field/map_load_manager.h"
#include "field/map_prop.h"

s32 ov01_021F652C(MapLoadManager *, s32);

s32 sub_02054C20(void *arg0, s32 arg1, MapProp **arg2, s32 *arg3);

s32 sub_02054C20(void *arg0, s32 arg1, MapProp **arg2, s32 *arg3) {
    MapPropManager *mapPropManager;
    s32 *v2;
    MapProp **v1;
    void *v0;
    MapProp *mapProp;
    int new_var;
    s32 v3;
    u8 index;
    v0 = arg0;
    v1 = arg2;
    v2 = arg3;
    v3 = 0;
loop_1:
    ov01_021F630C(v3, ((FieldSystem *)v0)->mapLoadManager, &mapPropManager);

    if (mapPropManager != 0L) {
        index = 0;
loop_3:
        mapProp = MapPropManager_GetMapPropByIndex(mapPropManager, index);

        new_var = MapProp_GetBuildModel(mapProp);
        if (new_var == arg1) {
            if (v1 != 0L) {
                *v1 = mapProp;
            }
            if (v2 != 0L) {
                *v2 = ov01_021F652C(((FieldSystem *)v0)->mapLoadManager, v3);
            }
            return 1;
        }
        index += 1;
        if (((u32)index) >= 0x20U) {
            goto block_10;
        }
        goto loop_3;
    }
block_10:
    v3 = (s32)((u8)(v3 + 1));

    if (((u32)v3) >= 4U) {
        return 0;
    }
    goto loop_1;
}
