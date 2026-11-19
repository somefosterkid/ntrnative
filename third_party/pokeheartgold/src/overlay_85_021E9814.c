#include "global.h"
#include "field_system_def.h"

#include "game_stats.h"
#include "overlay_85.h"
#include "unk_02037C94.h"
#include "unk_02005D10.h"
#include "unk_02035900.h"
#include "overlay_85_ov109_021D5140.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_85_ov109_021D0F70.h"
#include "unk_struct_0209_c194.h"

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

void ov85_021EA4EC(SaveData *saveData, const UnkStruct_ov109_021D5140_sub2 *param1);

int ov85_021E9814(UnkStruct_ov109_021D5140 *param0, int param1);

s32 ov85_021E9458(void *, s32);
s32 ov85_021EA0EC(void *, s32, s32);

int ov85_021E9814(UnkStruct_ov109_021D5140 *param0, int param1) {
    ov85_021EA4EC(param0->unk_0C->unk_14.saveData, param0->unk_FE4);

    param0->unk_3B8 = 27;

    return param1;
}

s32 ov85_021E9834(UnkStruct_ov109_021D5140 *arg0, s32 arg1) {
    gSystem.softResetDisabled = 1;
    GameStats_AddScore(arg0->unk_0C->unk_14.records, 0x13);
    sub_02039EAC(&arg0->unk_414);
    arg0->unk_3B8 = 0x1C;
    return arg1;
}

s32 ov85_021E9864(UnkStruct_ov109_021D5140 *arg0, s32 arg1) {
    if (sub_0203769C() == 0) {
        sub_02037454();
    }
    if (sub_02039EB4(arg0->unk_0C->unk_14.saveData, 2, &arg0->unk_414) != 0) {
        StopSE(0x657U, 8);
        ov85_021EA0EC(arg0, 0xD, 0);
        ov85_021E9458(arg0, 0x1D);
        arg0->unk_1C.unk_00 = 0;
        arg0->unk_3C4 = 0;
        gSystem.softResetDisabled = 0;
        arg0->unk_10->unk_24 = 0;
    }
    return arg1;
}
