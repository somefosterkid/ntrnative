#include "global.h"

#include "sprite_system.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F2A74(struct UnkStruct_ov14_boxsys *, s32, s32);

void ov14_021F40E8(void *arg0, s32 arg1);

struct UnkStruct_ov14_boxsys;

void ov14_021F40E8(void *arg0, s32 flag) {
    s32 v0;
    ManagedSprite *managedSprite;
    UnkStruct_ov14_boxsys *v1;
    s32 v3;
    s32 v4;
    u32 v5;
    u8 v2;

    if (flag == 1) {
        flag = 1;
    } else {
        flag = 0;
    }
    v5 = 0;
    v4 = 0;
    do {
        managedSprite = ((UnkStruct_ov14_boxapp *)arg0)->sys->sprites[0x3D + v5];
        if (managedSprite != NULL) {
            ManagedSprite_SetDrawFlag(managedSprite, flag);
            if (flag == 1) {
                v2 = ((UnkStruct_ov14_boxapp *)arg0)->unk21;
                if (v2 != 0xFF) {
                    v1 = ((UnkStruct_ov14_boxapp *)arg0)->sys;
                    v3 = v1->unk_4094[v2];
                    v0 = v3;
                    ov14_021F2A74(*((void **)&((UnkStruct_ov14_boxapp *)arg0)->sys), v5 + 0x3D, ManagedSprite_GetDrawPriority(v1->sprites[v3]) + 1);
                    ManagedSprite_SetPriority(((UnkStruct_ov14_boxapp *)arg0)->sys->sprites[0x3D + v5], ManagedSprite_GetPriority(((UnkStruct_ov14_boxapp *)arg0)->sys->sprites[v0]));
                }
            }
        }
        v5 += 1;
        v4 += 4;
    } while (v5 < 8U);
}
