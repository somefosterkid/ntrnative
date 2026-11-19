#include "global.h"
struct FieldSystem;
#include "error_handling.h"
#include "filesystem.h"
#include "map_object.h"
#include "field_system.h"
#include "overlay_02_follow_app.h"

s32 ov02_0224FFD8(void *arg0);

s32 ov01_021F8F08(LocalMapObject *, s32);
s32 ov01_021F8F68(LocalMapObject *, s8);
s32 ov02_0224FE70(void *, LocalMapObject *, s32);
s32 ov02_0224FF5C(void *, LocalMapObject *);
s32 ov02_0224FFD8(void *);
s32 ov02_02250004(struct FieldSystem *arg0, void *arg1, s32 arg2);

s32 ov02_0224FFD8(void *arg0) {
    u8 v0;

    v0 = *(u8 *)((u8 *)arg0 + 0x86B);
    if ((u32)v0 >= 0xAU) {
        return 0;
    }
    if (*(u8 *)((u8 *)(arg0 + (v0 * 8)) + 0x818) == 0xFF) {
        return 0;
    }
    *(s8 *)((u8 *)arg0 + 0x86A) = 0;
    return 1;
}

s32 ov02_02250004(FieldSystem *arg0, void *arg1, s32 arg2) {
    u8 v0;
    u8 v1;

    if (arg2 == 0) {
        GF_AssertFail();
        return 1;
    }
    v0 = *((u8 *)&((Ov02FollowApp *)arg1)->unk_810[88]);
    switch (v0) {
    case 0:
        ReadWholeNarcMemberByIdPair(arg1 + 0x818, NARC_a_2_2_2, arg2 - 1);
    case 1:
        MapObject_CopyPositionVector(arg0->followMon.mapObject, arg1 + 0x870);
        *((u8 *)&((Ov02FollowApp *)arg1)->unk_870[12]) = (u8)MapObject_GetFacingDirection(arg0->followMon.mapObject);
        ov01_021F8F68(arg0->followMon.mapObject, 0);
        ov01_021F8F08(arg0->followMon.mapObject, 1);
        *((u8 *)&((Ov02FollowApp *)arg1)->unk_810[88]) = 2U;
        *((s8 *)&((Ov02FollowApp *)arg1)->unk_810[91]) = 0;
    default:
        break;
    case 2:
        if (ov02_0224FFD8(arg1) == 0) {
            *((u8 *)&((Ov02FollowApp *)arg1)->unk_810[88]) = 4U;
        } else {
            *((u8 *)&((Ov02FollowApp *)arg1)->unk_810[88]) = 3U;
        case 3:
            if (ov02_0224FF5C(arg1, arg0->followMon.mapObject) != 0) {
                *((u8 *)&((Ov02FollowApp *)arg1)->unk_810[88]) = 2U;
            }
        }
        break;
    case 4:
        v1 = (u8)MapObject_GetFacingDirection(arg0->followMon.mapObject);
        ov01_021F8F68(arg0->followMon.mapObject, 0);
        ov01_021F8F08(arg0->followMon.mapObject, 0);
        MapObject_SetPositionVector(arg0->followMon.mapObject, arg1 + 0x870);
        MapObject_SetFacingDirectionDirect(arg0->followMon.mapObject, (u32) *((u8 *)&((Ov02FollowApp *)arg1)->unk_870[12]));
        ov02_0224FE70(arg1, arg0->followMon.mapObject, v1);
        return 1;
    }
    return 0;
}
