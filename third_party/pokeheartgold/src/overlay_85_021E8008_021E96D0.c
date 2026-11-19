#include "global.h"

#include "error_handling.h"
#include "list_menu_2d.h"
#include "overlay_85.h"
#include "unk_02005D10.h"
#include "unk_02035900.h"
#include "unk_0205A44C.h"
#include "overlay_85_ov109_021D5140.h"
#include "overlay_41_sprite_resource_collection.h"
#include "unk_struct_0209_bdf8.h"
#include "game_stats.h"
#include "overlay_85_ov109_021D0F70.h"
#include "unk_struct_0209_c194.h"

s32 ov85_021E943C(void *);

extern WindowTemplate ov85_021EA8D8;

s32 ov85_021E96D0(UnkStruct_ov109_021D5140 *arg0, s32 arg1) {
    arg0->unk_394 = Std_CreateYesNoMenu(arg0->unk_14, &ov85_021EA8D8, 0x1FU, 0xBU, HEAP_ID_102);
    arg0->unk_3B8 = 0x17;
    ov85_021E943C(arg0);
    return arg1;
}

s32 ov85_021E9458(void *, s32);

int ov85_021E9FD0(void);

s32 ov85_021EA39C(void *, s32);

s32 sub_02096D4C(void *, s32, s32 *, s32);

s32 ov85_021E9700(UnkStruct_ov109_021D5140 *arg0, s32 arg1) {
    s32 v0;
    s32 v1;
    UnkStruct_0209BDF8 *v2;

    v2 = arg0->unk_10;
    if ((v2->unk_2C != ov85_021E9FD0()) || ((s32)v2->unk_30 != 0)) {
        if (0xC3 & gSystem.newKeys) {
            PlaySE(0x5F2U);
        }
        ov85_021E943C(arg0);
        return arg1;
    }
    v1 = Handle2dMenuInput_DeleteOnFinish(arg0->unk_394, HEAP_ID_102);
    if (v1 != -1) {
        if (v1 == -2) {
            v0 = 0;
            sub_02096D4C(arg0->unk_10, 7, &v0, 1);
            ov85_021EA39C(arg0, 1);
            ov85_021E9458(arg0, 0);
        } else if (sub_0203769C() == 0) {
            arg0->unk_3B8 = 0x18;
            arg0->unk_4AB6 = ov85_021E9FD0();
            sub_0205A904(0xC);
        } else {
            GF_AssertFail();
        }
        arg0->unk_394 = NULL;
    }
    ov85_021E943C(arg0);
    return arg1;
}

struct GameStats;

struct JournalEntry;

struct ListMenu2D;

struct MsgData;

struct SpriteResourceCollection;

struct MessageFormat;

struct PlayerProfile;

struct UnkStruct_0205C22C_t;

struct UnkStruct_ov109_021D0F70_t;

typedef struct UnkStruct_0205C22C_t UnkStruct_0205C22C;

typedef struct JournalEntry JournalEntry;

typedef struct ListMenu2D ListMenu2D;

int ov85_021E97B8(UnkStruct_ov109_021D5140 *param0, int param1);

int ov85_021E97B8(UnkStruct_ov109_021D5140 *param0, int param1) {
    if (param0->unk_4AB7 == 0) {
        if (sub_02096D4C(param0->unk_10, 5, NULL, 0) == 1) {
            param0->unk_3B8 = 31;
        }
    }

    return param1;
}

s32 ov85_021E97E8(UnkStruct_ov109_021D5140 *arg0, s32 arg1) {
    if (sub_0203769C() == 0) {
        sub_02037454();
    }
    if (arg0->unk_10->unk_20 == sub_02037454()) {
        arg0->unk_3B8 = 0x1F;
    }
    return arg1;
}
