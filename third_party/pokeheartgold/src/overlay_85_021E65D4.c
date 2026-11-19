#include "global.h"
#include "field_system_def.h"
#include "simple3dmodel.h"

#include "camera.h"
#include "overlay_85.h"
#include "render_window.h"
#include "sprite_system.h"
#include "unk_020379A0.h"
#include "unk_02037C94.h"
#include "unk_struct_0209_bdf8.h"
#include "overlay_85_ov109_021D5140.h"
#include "simple3danimation.h"
#include "overlay_85_ov109_021D0F70.h"
#include "game_stats.h"
#include "unk_struct_0209_c194.h"

struct GameStats;

struct JournalEntry;

struct MsgData;

struct MessageFormat;

struct PlayerProfile;

struct UnkStruct_0205C22C_t;

struct UnkStruct_ov109_021D0F70_t;

struct UnkStruct_ov109_021D5140_t;

typedef struct UnkStruct_0205C22C_t UnkStruct_0205C22C;

typedef struct JournalEntry JournalEntry;

void ov85_021E86CC(UnkStruct_ov109_021D0F70 *param0, int param1);

int ov85_021E65D4(UnkStruct_ov109_021D0F70 *param0);

int ov85_021E65D4(UnkStruct_ov109_021D0F70 *param0) {
    if (sub_02037B38(202)) {
        ov85_021E86CC(param0, param0->unk_28->unk_0C);
        sub_02039EAC(&param0->unk_04);
        param0->unk_DC8 = WaitingIcon_New(&param0->unk_C9C.unk_0C[0], 1 + 9);
        param0->unk_00 = 45;
    }

    return 0;
}

s32 ov85_021E6610(UnkStruct_ov109_021D0F70 *arg0);

s32 ov85_021E6610(UnkStruct_ov109_021D0F70 *arg0) {
    if (sub_02039EB4(arg0->unk_CC->unk_14.saveData, 2, (void *)&arg0->unk_04) != 0) {
        gSystem.softResetDisabled = 0;
        sub_0200F450(arg0->unk_DC8);
        arg0->unk_00 = 0x30;
    }
    return 0;
}

s32 ov85_021E7194(s32 *, s32);

s32 ov85_021E6644(s32 *arg0) {
    ov85_021E7194(arg0, 0xC);
    *arg0 = 0x2F;
    return 0;
}

int ov85_021E6658(UnkStruct_ov109_021D0F70 *param0);

int ov85_021E6658(UnkStruct_ov109_021D0F70 *param0) {
    param0->unk_0C++;

    if (param0->unk_0C >= (30 * 4)) {
        param0->unk_0C = 0;
        param0->unk_00 = 48;

        return 1;
    }

    return 0;
}
