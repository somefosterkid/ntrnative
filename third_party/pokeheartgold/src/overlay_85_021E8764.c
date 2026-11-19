#include "global.h"
#include "field_system_def.h"
#include "simple3dmodel.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_85.h"
#include "party.h"
#include "pokemon.h"
#include "unk_struct_0209_bdf8.h"
#include "overlay_85_ov109_021D5140.h"
#include "simple3danimation.h"
#include "overlay_85_ov109_021D0F70.h"
#include "game_stats.h"
#include "msgdata.h"
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

typedef struct {
    u16 unk_00;
    u16 unk_02;
} UnkStruct_ov109_021D59AC;

typedef struct {
    int unk_00;
    const UnkStruct_ov109_021D59AC *unk_04;
} UnkStruct_ov109_021D5C14;

void ov85_021E8530(fx32 *param0, fx32 param1);

extern const UnkStruct_ov109_021D5C14 ov85_021EA758[5 + 1];

BOOL ov85_021E8764(UnkStruct_ov109_021D0F70 *param0, fx32 param1, int param2);

void *sub_02097018(UnkStruct_0209BDF8 *param0, int param1);
BOOL ov85_021E86B0(UnkStruct_ov109_021D0F70 *param0, int param1);
BOOL ov85_021E8834(UnkStruct_ov109_021D0F70 *param0);
int ov85_021E8878(UnkStruct_ov109_021D0F70 *param0);
BOOL ov85_021E8898(UnkStruct_ov109_021D0F70 *param0);

BOOL ov85_021E8764(UnkStruct_ov109_021D0F70 *param0, fx32 param1, int param2) {
    int v0;
    fx32 v1;
    u32 v2, v3;
    const UnkStruct_ov109_021D5C14 *v4;
    const UnkStruct_ov109_021D59AC *v5;

    ov85_021E8530(&param1, -param0->unk_D4.unk_3C);

    v1 = param1;
    ov85_021E8530(&v1, FX32_ONE * -4);
    v2 = ((v1) / FX32_ONE);

    v1 = param1;
    ov85_021E8530(&v1, FX32_ONE * 5);
    v3 = ((v1) / FX32_ONE);

    v4 = &ov85_021EA758[param2];
    v0 = v4->unk_00;
    v5 = v4->unk_04;

    while (v0) {
        if (((v2 >= v5->unk_00) && (v2 <= v5->unk_02)) || ((v3 >= v5->unk_00) && (v3 <= v5->unk_02))) {
            return 1;
        }

        v5++;
        v0--;
    }

    return 0;
}

BOOL ov85_021E87F0(Party *param0) {
    int v0, v1;
    Pokemon *v2;

    v1 = Party_GetCount(param0);

    for (v0 = 0; v0 < v1; v0++) {
        v2 = Party_GetMonByIndex(param0, v0);

        if (GetMonData(v2, MON_DATA_IS_EGG, NULL)) {
            if (GetMonData(v2, MON_DATA_CHECKSUM_FAILED, NULL)) {
                return 1;
            }
        }
    }

    return 0;
}

BOOL ov85_021E8834(UnkStruct_ov109_021D0F70 *param0) {
    if (ov85_021E87F0(param0->unk_24) == 1) {
        return 1;
    }

    {
        int v0;
        Party *v1;

        for (v0 = 0; v0 < 5; v0++) {
            if (ov85_021E86B0(param0, v0)) {
                v1 = sub_02097018(param0->unk_D0, v0);

                if (ov85_021E87F0(v1) == 1) {
                    return 1;
                }
            }
        }
    }

    return 0;
}

int ov85_021E8878(UnkStruct_ov109_021D0F70 *param0) {
    int v0 = 0;
    u32 v1 = param0->unk_D0->unk_4A;

    while (v1) {
        v0 += v1 & 0x1;
        v1 >>= 1;
    }

    return v0;
}

BOOL ov85_021E8898(UnkStruct_ov109_021D0F70 *param0) {
    int v0 = 0;
    u32 v1 = param0->unk_D0->unk_48;

    if (v1) {
        return 1;
    }

    return 0;
}
