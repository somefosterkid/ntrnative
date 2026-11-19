#include "global.h"

#include "overlay_49.h"
#include "constants/heap.h"
#include "struct_defs/resdat_narc_structs.h"
#include "bg_window.h"
#include "filesystem.h"
#include "gf_3d_loader.h"
#include "gf_3d_vramman.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "player_data.h"
#include "pm_string.h"
#include "sprite.h"
#include "touch_hitbox_controller.h"
#include "touchscreen.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_49_unk_struct_ov70_0225_e9_c8.h"
#include "overlay_49_unk_struct_ov70_0225_e4_ec.h"
#include "overlay_45_wifi_plaza_system.h"
#include "overlay_49_unk_struct_ov70_0225f208.h"
#include "overlay_49_unk_struct_ov70_0225f350.h"


typedef u16 charcode_t;
void ov45_0222A844(const WiFiPlazaProfile *profile, PlayerProfile *trainerInfo, enum HeapID heapID);
void ov49_0225BB14(UnkStruct_ov70_0225E4EC *param0, NARC *param1, u32 param2, u32 param3, u32 param4, u32 heapID);
void ov49_0225BEA0(UnkStruct_ov70_0225F350 *param0, UnkStruct_ov70_0225FA84 *param1, UnkStruct_ov70_0225E4EC *param2, u32 heapID, const WiFiPlazaProfile *param4);
void ov49_0225BFF0(UnkStruct_ov70_0225FA84 *param0, UnkStruct_ov70_0225E4EC *param1, NARC *param2, u32 heapID, u32 param4, BOOL param5);
void ov49_0225C180(UnkStruct_ov70_0225FA84 *param0, UnkStruct_ov70_0225E4EC *param1, NARC *param2, u32 heapID, u32 param4);
void ov49_0225C4CC(UnkStruct_ov70_0225FA84 *param0, UnkStruct_ov70_0225E4EC *param1, NARC *param2, u32 heapID, const WiFiPlazaProfile *param4, const WiFiPlazaProfile *param5);
s32 ov45_0222A9CC(WiFiPlazaProfile *);
u32 ov45_0222AA5C(WiFiPlazaProfile *);
s32 ov45_0222AA84(WiFiPlazaProfile *);
u32 ov45_0222AAC8(WiFiPlazaProfile *);
s32 ov49_0225B3A8(UnkStruct_ov70_0225F208 *, s32, s32, s32, s32);
s32 ov49_0225B438(UnkStruct_ov70_0225F208 *);
s32 ov49_0225BA40(UnkStruct_ov70_0225F350 *, UnkStruct_ov70_0225E4EC *, s32, u32);
s32 ov49_0225C3C0(UnkStruct_ov70_0225FA84 *);
s32 ov49_0225C3DC(UnkStruct_ov70_0225FA84 *, UnkStruct_ov70_0225F208 *, s32, s32, u8, u8, s32);
s32 ov49_0225C414(UnkStruct_ov70_0225FA84 *, UnkStruct_ov70_0225F208 *, s32, s32, u8, u8, s32);
s32 ov49_0225C460(UnkStruct_ov70_0225FA84 *, s32);
s32 ov49_0225C470(UnkStruct_ov70_0225FA84 *, UnkStruct_ov70_0225F208 *, PlayerProfile *);
s32 ov49_0225C480(UnkStruct_ov70_0225FA84 *, UnkStruct_ov70_0225F208 *, WiFiPlazaProfile *);
s32 ov49_0225C4B0(UnkStruct_ov70_0225FA84 *, UnkStruct_ov70_0225F208 *, UnkStruct_ov66_02230914 *);
void ov49_0225BBD0(UnkStruct_ov70_0225F350 *param0, UnkStruct_ov70_0225E9C8 *param1, UnkStruct_ov70_0225FA84 *param2, UnkStruct_ov70_0225E4EC *param3, UnkStruct_ov70_0225F208 *param4, s32 param5, NARC *param6, u32 heapID, WiFiPlazaProfile *param8, WiFiPlazaProfile *param9, s32 param10, s32 param11, UnkStruct_ov66_02230914 *param12, s32 param13);

void ov49_0225BBCC(void) {
}

void ov49_0225BBD0(UnkStruct_ov70_0225F350 *param0, UnkStruct_ov70_0225E9C8 *param1, UnkStruct_ov70_0225FA84 *param2, UnkStruct_ov70_0225E4EC *param3, UnkStruct_ov70_0225F208 *param4, s32 param5, NARC *param6, u32 heapID, WiFiPlazaProfile *param8, WiFiPlazaProfile *param9, s32 param10, s32 param11, UnkStruct_ov66_02230914 *param12, s32 param13) {
    PlayerProfile *ptr;
    UnkStruct_ov70_0225F350 *v0;
    PlayerProfile *playerProfile;
    s32 v2;
    u32 v1;

    v0 = param0;
    ov49_0225B438(param4);
    if (ov45_0222A9CC(param8) == 0) {
        v2 = 0x70800;
        if (param10 == 0) {
            ov49_0225BA40(v0, param3, 0x56, heapID);
        } else {
            ov49_0225BA40(v0, param3, 0x58, heapID);
        }
    } else {
        v2 = 0x30400;
        if (param10 == 0) {
            ov49_0225BA40(v0, param3, 0x57, heapID);
        } else {
            ov49_0225BA40(v0, param3, 0x58, heapID);
        }
    }
    BgFillTilemapBufferAndSchedule(param3->unk_00, 4U, 0U);
    BgFillTilemapBufferAndSchedule(param3->unk_00, 5U, 0U);
    BgFillTilemapBufferAndSchedule(param3->unk_00, 6U, 0U);
    ov49_0225C3C0(param2);
    ov49_0225BB14(param3, param6, 0x5CU, 4U, 0U, heapID);
    playerProfile = PlayerProfile_New((enum HeapID)heapID);
    ptr = playerProfile;
    ov45_0222A844(param8, (PlayerProfile *)ptr, (enum HeapID)heapID);
    ov49_0225C3DC(param2, param4, 0, 0x2C, 0, 0, v2);
    ov49_0225B3A8(param4, PlayerProfile_GetTrainerID_VisibleHalf(playerProfile), 5, 0, 2);
    ov49_0225C414(param2, param4, 0, 0x31, 0x7A, 0, 0x10200);
    ov49_0225C3DC(param2, param4, 0, 0x2D, 0, 0x10, v2);
    ov49_0225C470(param2, param4, ptr);
    ov49_0225C414(param2, param4, 0, 0x32, 0x7A, 0x10, 0x10200);
    ov49_0225C460(param2, 0);
    ov49_0225C3DC(param2, param4, 1, 0x2E, 0, 0, v2);
    if (ov45_0222AA84(param8) == 0) {
        ov49_0225C3DC(param2, param4, 1, 0x37, 0x20, 0x10, 0x10200);
    } else {
        ov49_0225C480(param2, param4, param8);
        ov49_0225C3DC(param2, param4, 1, 0x33, 0x20, 0x10, 0x10200);
    }
    ov49_0225C460(param2, 1);
    if (param11 != 0) {
        param2->unk_00 = 1;
        ov49_0225C470(param2, param4, ptr);
        ov49_0225C3DC(param2, param4, 4, 0x38, 8, 0, v2);
        ov49_0225C4B0(param2, param4, param12);
        ov49_0225C3DC(param2, param4, 4, 0x3B, 6, 0x18, 0x10200);
        ov49_0225C3DC(param2, param4, 4, 0x3C, 0x5A, 0x18, 0x10200);
        ov49_0225C3DC(param2, param4, 4, 0x3D, 6, 0x2C, 0x10200);
        ov49_0225C3DC(param2, param4, 4, 0x3E, 0x5A, 0x2C, 0x10200);
        ov49_0225C460(param2, 4);
    } else {
        param2->unk_00 = 0;
        ov49_0225C3DC(param2, param4, 4, 0x30, 8, 0, v2);
        ov49_0225C460(param2, 4);
        ov49_0225C4CC(param2, param3, param6, heapID, param8, param9);
    }
    ov49_0225BFF0(param2, param3, param6, heapID, ov45_0222AAC8(param8), param13);
    if (param5 == 1) {
        if (ov45_0222A9CC(param8) == 1) {
            v1 = 0x61;
        } else {
            v1 = 0;
        }
    } else {
        v1 = ov45_0222AA5C(param8);
    }
    ov49_0225C180(param2, param3, param6, heapID, v1);
    ov49_0225BEA0(v0, param2, param3, heapID, param8);
    Heap_Free(ptr);
}
