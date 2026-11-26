#include "global.h"

#include "bag_view.h"
#include "heap.h"
#include "naming_screen.h"
#include "overlay_manager.h"
#include "poke_overlay.h"
#include "pokemon_storage_system.h"
#include "sprite_system.h"
#include "touchscreen.h"
#include "overlay_14_move_owner.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E7960(s16, s16);

s32 ov14_021E65C4(void *);
s32 ov14_021E6CF8(void *, u32, s32);
s32 ov14_021E70B0(void *, s32);
s32 ov14_021E7148(void *, void *);
s32 ov14_021E8434(void *);
s32 ov14_021E8514(s32);
s32 ov14_021E8824(UnkStruct_ov14_boxsys *);
s32 ov14_021F40E8(void *, s32);
s32 ov14_021E8BA4(struct UnkStruct_ov14_boxapp *arg0);

typedef struct UnkStruct_ov14_021E8B3C {
    void *unk_0;
    PCStorage *unk_4;
    u8 unk_8[0x8];
    Options *unk_10;
    OverlayManager *unk_14;
    NamingScreenArgs *unk_18;
    u8 unk_1C[0x9];
    u8 unk_25;
} UnkStruct_ov14_021E8B3C;

typedef struct UnkStruct_ov14_021E8B1C {
    u8 unk_0[0x18];
    BagView *unk_18;
    u16 unk_1C;
} UnkStruct_ov14_021E8B1C;

FS_EXTERN_OVERLAY(OVY_15);

s32 ov14_021E8B1C(UnkStruct_ov14_021E8B1C *arg0);

s32 sub_02019978(void *, s32);

s32 ov14_021E8B1C(UnkStruct_ov14_021E8B1C *arg0) {
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_15));
    arg0->unk_1C = BagView_GetItemId(arg0->unk_18);
    Heap_Free(arg0->unk_18);
    return 0;
}

s32 ov14_021E8B3C(UnkStruct_ov14_021E8B3C *arg0);

s32 ov14_021E8B3C(UnkStruct_ov14_021E8B3C *arg0) {
    arg0->unk_18 = NamingScreen_CreateArgs(HEAP_ID_9, NAME_SCREEN_BOX, 0, 8, arg0->unk_10, *(MenuInputStateMgr **)((u8 *)(arg0->unk_0) + 4));
    PCStorage_GetBoxName(arg0->unk_4, (u32) arg0->unk_25, (arg0->unk_18)->nameInputString);
    arg0->unk_14 = OverlayManager_New(&gOverlayTemplate_NamingScreen, arg0->unk_18, HEAP_ID_9);
    return 0;
}

s32 ov14_021E8B80(UnkStruct_ov14_boxapp *arg0) {
    NamingScreenArgs *namingScreenArgs;

    namingScreenArgs = arg0->unk_18;
    PCStorage_SetBoxName(arg0->storage, (u32) arg0->unk25, namingScreenArgs->nameInputString);
    *((s16 *)&arg0->unk_1C) = (s16)namingScreenArgs->noInput;
    NamingScreen_DeleteArgs(arg0->unk_18);
    return 0;
}

s32 ov14_021E8BA4(UnkStruct_ov14_boxapp *arg0) {
    u32 x;
    u32 y;
    s32 v1;
    s32 v0;
    s32 v7;
    s32 v8;
    s32 v9;
    u16 v2;
    UnkStruct_ov14_boxsys *v3;
    UnkStruct_ov14_boxsys *v4;
    UnkStruct_ov14_boxsys *v5;
    UnkStruct_ov14_boxsys *v6;

    v6 = arg0->sys;
    v7 = ov14_021E8514(*((s32 *)&v6->unk_2F0));
    v8 = sub_02019978(((u8 *)((UnkStruct_ov14_boxsys *)arg0->sys)->unk_2F0), 8);
    v0 = sub_02019978(((u8 *)((UnkStruct_ov14_boxsys *)arg0->sys)->unk_2F0), 9);
    v1 = sub_02019978(((u8 *)((UnkStruct_ov14_boxsys *)arg0->sys)->unk_2F0), 0xA);
    v2 = v6->unk_10;
    switch (v2) {
    case 0:
        if (System_GetTouchHeldCoords(&x, &y) == 0) {
            v9 = 0xFF;
            if (*((u8 *)&arg0->unk23[1]) != 0) {
                v4 = arg0->sys;
                v9 = ov14_021E79D8((s16) *((u32 *)&v4->unk_40B8), (s16) *((u32 *)&v4->unk_40BC));
            }
            if (v9 == 0xFF) {
                v5 = arg0->sys;
                v9 = ov14_021E7960((s16) *((u32 *)&v5->unk_40B8), (s16) *((u32 *)&v5->unk_40BC));
            }
            ov14_021E6CF8(arg0, arg0->unk21, v9);
            ov14_021F40E8(arg0, 0);
            if (!(0x80 & v9)) {
                arg0->unk21 = ov14_021E70B0(arg0, arg0->unk21);
            }
            ov14_021E8434(((u8 *)((UnkStruct_ov14_boxsys *)arg0->sys)->unk_2F0));
            ov14_021E8824(arg0->sys);
            v6->unk_10 = 1U;
        } else {
            v3 = arg0->sys;
            ManagedSprite_SetPositionXY(v3->sprites[v3->unk_4094[arg0->unk21]], (s16)x, (s16)((s16)y - 8));
            ov14_021F4174(arg0);
            *((u32 *)&((UnkStruct_ov14_boxsys *)arg0->sys)->unk_40B8) = x;
            *((u32 *)&((UnkStruct_ov14_boxsys *)arg0->sys)->unk_40BC) = y;
        }
    default:
        break;
    case 1:
        if ((ov14_021E65C4(arg0) == 0) && (v7 == 0) && (v8 == 0) && (v0 == 0) && (v1 == 0)) {
            ov14_021E7148(arg0, v6->unk_C);
            ov14_021F4174(arg0);
            ov14_021F40E8(arg0, *((s32 *)&((UnkStruct_ov14_boxsys *)arg0->sys)->unk_40C4));
            v6->unk_10 = 0U;
            return 0;
        }
        break;
    }
    return 1;
}
