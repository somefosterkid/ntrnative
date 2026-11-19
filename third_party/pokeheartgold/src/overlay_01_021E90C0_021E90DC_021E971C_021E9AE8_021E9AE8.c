#include "global.h"

#include "field/area_data.h"
#include "field/map_prop.h"
#include "field/map_prop_animation.h"
#include "field/overlay_01_021FB878.h"

#include "error_handling.h"
#include "field_system.h"
#include "field_types_def.h"
#include "filesystem.h"
#include "heap.h"

s32 sub_02054A60(s32, s32, s32, s32, s32, s32, s32 *);

s32 sub_02054AE4(FieldSystem *, u32, s32 *, MapProp **);

u32 *sub_02054D10(FieldSystem *, s32, s32, s32 *, s32);

void ov01_021E9AE8(FieldSystem *fieldSystem, s32 a1, s32 a2, u8 a3);

void ov01_021E9AE8(FieldSystem *fieldSystem, s32 a1, s32 a2, u8 a3) {
    s32 sp44[4];
    s32 sp2C[6];
    MapProp *sp28;
    s32 sp24;
    s32 sp20;
    u32 *sp1C;
    s32 sp18;
    NARC *temp_r6;
    NNSG3dRenderObj *temp_r4;
    NNSG3dResTex *temp_r6_2;
    s32 var_r4;
    u32 *temp_r0;
    u32 *var_r5;
    u32 temp_r1;
    u8 temp_r0_2;

    sp18 = (s32)a3;
    sub_02054A60(a1, a2, -1, 0, 3, 1, sp44);
    temp_r6 = ov01_021FB904(fieldSystem->areaDataManager);
    temp_r0 = sub_02054D10(fieldSystem, 4, 4, sp44, 0);
    var_r4 = 0;
    sp1C = temp_r0;
    sp24 = 0;
    var_r5 = temp_r0;
loop_1:
    temp_r1 = *var_r5;
    if ((temp_r1 != 0) && (NARC_ReadWholeMember(temp_r6, temp_r1, sp2C), (*(u8 *)&sp2C[1] != 0))) {
        sub_02054AE4(fieldSystem, sp1C[var_r4], sp44, &sp28);
        sp24 = 1;
        sp20 = (s32)sp1C[var_r4];
    } else {
        var_r4 += 1;
        var_r5 += 1;
        if (var_r4 < 4) {
            goto loop_1;
        }
    }
    Heap_Free(sp1C);
    if (sp24 != 0) {
        temp_r0_2 = MapPropAnimationManager_GetPropAnimationCount(fieldSystem->mapPropAnimationManager, sp20);
        if (temp_r0_2 != 0) {
            temp_r6_2 = ov01_021FB9E0(fieldSystem->areaDataManager);
            temp_r4 = MapProp_GetRenderSurface(sp28);
            MapPropOneShotAnimationManager_LoadPropAnimations(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, sp18, sp20, temp_r4, MapProp_GetResModel(sp28), temp_r6_2, (s32)temp_r0_2, 1U, 0);
        }
    } else {
        GF_AssertFail();
    }
}
