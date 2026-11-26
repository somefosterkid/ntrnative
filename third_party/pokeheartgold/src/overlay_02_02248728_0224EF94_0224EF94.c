#include "global.h"
struct FieldSystem;

#include "error_handling.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "map_header.h"
#include "field_system.h"
#include "follow_mon_interact_work.h"

s32 ov02_0224F058(void *, FollowMonInteractWork *);

s32 ov02_0224F108(void *, void *, FollowMonInteractWork *);

u32 ov02_0224EF94(struct FieldSystem *arg0);

u32 ov02_0224EF94(FieldSystem *arg0) {
    s32 var_r2;
    FollowMonInteractWork sp4;
    s32 sp20[100];
    void *sp0;
    s32 *var_r1;
    s32 *var_r3;
    s32 *var_r4;
    FieldSystemUnkSub120 *temp_r0;
    s32 temp_r1;
    s32 var_r0;
    s32 var_r3_2;
    s32 var_r3_3;
    s32 var_r5;
    void *temp_r6;
    ReadWholeNarcMemberByIdPair(arg0->unk120->unk14, NARC_a_2_2_0, MapHeader_GetMapSec(arg0->location->mapId) + 1);
    ReadWholeNarcMemberByIdPair(&arg0->unk120->unk14[0x258], NARC_a_2_2_0, 0);
    ov02_0224F058(arg0, &sp4);
    temp_r0 = arg0->unk120;
    var_r3 = &sp20[0];
    sp0 = temp_r0;
    sp0 = temp_r0->unk14;
    var_r0 = 0;
    var_r2 = 0;
    do {
        temp_r1 = var_r2 + 0x1E;
        var_r2 += 1;
        var_r0 += 1;
        *var_r3 = temp_r1;
        var_r3 += 1;
    } while (var_r2 < 0xC);
    var_r3_2 = 0;
    var_r1 = &(&sp20[0])[var_r0];
    do {
        *var_r1 = var_r3_2;
        var_r1 += 1;
        var_r3_2 += 1;
        var_r0 += 1;
    } while (var_r3_2 < 0x1E);
    var_r3_3 = 0x2A;
    var_r1 = &(&sp20[0])[var_r0];
    do {
        *var_r1 = var_r3_3;
        var_r1 += 1;
        var_r3_3 += 1;
        var_r0 += 1;
    } while (var_r3_3 < 0x64);
    var_r5 = 0;
    var_r4 = &sp20[0];
loop_7:
    temp_r6 = (void *)(sp0 + ((*var_r4) * 0x14));

    if (((((u32)((*((u16 *)(((u8 *)temp_r6) + 0xA))) << 0x10)) >> 0x16) != 0) && (ov02_0224F108(arg0, temp_r6, &sp4) != 0)) {
        return ((u32)((*((u16 *)(((u8 *)temp_r6) + 0xA))) << 0x10)) >> 0x16;
    }
    var_r5 += 1;
    var_r4 += 1;
    if (var_r5 >= 0x64) {
        GF_AssertFail();
        return 0U;
    }
    goto loop_7;
}
