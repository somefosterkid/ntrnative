#include "global.h"

#include "field_system.h"
#include "map_matrix.h"
#include "overlay_01_021F4704.h"

u8 ov01_021F635C(s32, const u8, void *);

// the definition (src/overlay_01_021FAD1C_021FAE50.c) types it as its file-local Mesh copy, which has the layout of Bdhc
int ov01_021FAE50(int mode, fx32 arg1, fx32 x, fx32 z, const Bdhc *bdhc, fx32 *out);

BOOL ov01_021FB42C(const int tileX, const int tileZ, const DynamicTerrainHeightManager *dynamicTerrainHeightMan, u8 *index);

fx32 ov01_021FB474(const u8 index, const DynamicTerrainHeightManager *dynamicTerrainHeightMan);

s32 sub_02054648(s32, s32);

s32 sub_02054654(const FieldSystem *fieldSystem, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5);

s32 sub_02054654(const FieldSystem *fieldSystem, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5) {
    VecFx32 pos;
    u8 sp2C;
    s32 sp28;
    s32 sp24;
    s32 sp20;
    u32 sp1C;
    u32 sp18;
    s32 var_r4;
    s32 sp14;
    s32 spC;
    s32 sp8;
    s32 temp_r0;
    s32 temp_r4;
    s32 temp_r5;
    s32 temp_r7;
    MapLoadManager *sp10;
    s32 var_r6;
    s8 var_r1;
    u32 temp_r0_2;
    u32 temp_r0_3;
    u32 temp_r0_4;

    var_r4 = 0;
    pos.x = arg3;
    pos.z = arg4;
    pos.y = 0;
    sp8 = arg1;
    sp10 = fieldSystem->mapLoadManager;
    spC = arg2;
    temp_r0 = (s32)MapMatrix_GetWidth(fieldSystem->mapMatrix);
    sp28 = temp_r0;
    sp14 = temp_r0 << 5;
    temp_r7 = (s32)(arg3 + ((u32)(arg3 >> 0xF) >> 0x10)) >> 0x10;
    sp20 = (arg4 / 0x10000);
    sp24 = ov01_021FB42C(temp_r7, sp20, fieldSystem->dynamicTerrainHeightManager, &sp2C);
    temp_r0_2 = sp1C = (u32)temp_r7 >> 5;
    temp_r0_3 = sp18 = (u32)sp20 >> 5;
    pos.x = arg3 - (((temp_r0_2 << 5) + 0x10) << 0x10);
    pos.z = arg4 - (((temp_r0_3 << 5) + 0x10) << 0x10);
    temp_r0_4 = ov01_021F635C(temp_r0_2 + (sp18 * sp28), ov01_021F6328(temp_r7 + (sp20 * sp14), sp14), sp10);
    if (temp_r0_4 > 3U) {
        var_r6 = 0;
    } else {
        var_r6 = ov01_021FAE50(sp8, spC, pos.x, pos.z, ov01_021F65D0(sp10, (u8)temp_r0_4), &pos.y);
    }
    if (sp24 != 0) {
        temp_r5 = ov01_021FB474(sp2C, fieldSystem->dynamicTerrainHeightManager);
        if (var_r6 != 0) {
            var_r4 = pos.y;
            if (temp_r5 <= var_r4) {
                var_r1 = 1;
            } else {
                temp_r4 = sub_02054648(var_r4, spC);
                if (temp_r4 <= sub_02054648(temp_r5, spC)) {
                    var_r4 = pos.y;
                    var_r1 = 1;
                } else {
                    var_r4 = temp_r5;
                    var_r1 = 2;
                }
            }
        } else {
            var_r1 = 2;
            var_r4 = temp_r5;
        }
    } else if (var_r6 != 0) {
        var_r1 = 1;
        var_r4 = pos.y;
    } else {
        var_r1 = 0;
    }
    if (arg5 != NULL) {
        *arg5 = var_r1;
    }
    return var_r4;
}
