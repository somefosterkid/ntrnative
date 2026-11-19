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
#include "unk_02035900.h"
#include "unk_020379A0.h"
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

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
} UnkStruct_ov109_021D122C;

SysTask *ov85_021E7B40(UnkStruct_ov109_021D0F70 *param0, int param1, int param2, int param3, int param4, int param5, UnkStruct_ov109_021D2D78 *param6);

void *ov85_021E8610(UnkStruct_ov109_021D0F70 *param0);

extern const u16 ov85_021EA7C4[5 + 1][5];

int ov85_021E5E30(UnkStruct_ov109_021D0F70 *param0);

int ov85_021E5EB4(UnkStruct_ov109_021D0F70 *param0);
int ov85_021E5EE8(UnkStruct_ov109_021D0F70 *param0);
BOOL sub_02096D4C(UnkStruct_0209BDF8 *param0, u32 param1, const void *param2, u32 param3);
int ov85_021E5F10(UnkStruct_ov109_021D0F70 *param0);
BOOL ov85_021E8614(UnkStruct_ov109_021D0F70 *param0, u16 param1);
int ov85_021E5F3C(UnkStruct_ov109_021D0F70 *param0);
int ov85_021E5F58(UnkStruct_ov109_021D0F70 *param0);
int ov85_021E5F6C(UnkStruct_ov109_021D0F70 *param0);
SysTask *ov85_021E81E0(s32 *);
s32 ov85_021E84EC(s32 *, s32);
void ov85_021E7400(UnkStruct_ov109_021D0F70 *param0, int param1);
int ov85_021E5F9C(UnkStruct_ov109_021D0F70 *param0);
int ov85_021E5FD0(UnkStruct_ov109_021D0F70 *param0);

int ov85_021E5E30(UnkStruct_ov109_021D0F70 *param0) {
    param0->unk_0C--;

    if (param0->unk_0C > 0) {
        return 0;
    }

    param0->unk_0C = 15;

    {
        UnkStruct_ov109_021D122C *v0 = ov85_021E8610(param0);
        u16 v1 = ov85_021EA7C4[v0->unk_14][v0->unk_10];
        int v2 = param0->unk_2C.unk_44[v0->unk_10].unk_02;

        ov85_021E7B40(param0, v2, v0->unk_10, v0->unk_00, v1, v0->unk_04, &param0->unk_190.unk_140[v0->unk_10]);

        v0->unk_10++;
        v0->unk_10 %= v0->unk_14;
        v0->unk_00++;

        if (v0->unk_00 == v0->unk_14) {
            param0->unk_0C = 0;
            param0->unk_00 = 18;
        }
    }

    return 0;
}

int ov85_021E5EB4(UnkStruct_ov109_021D0F70 *param0) {
    UnkStruct_ov109_021D1334 *v0 = &param0->unk_190;
    int v1 = 0, v2 = 0, v3 = param0->unk_2C.unk_04;

    do {
        if (v0->unk_140[v1].unk_08 == 1) {
            v2++;
        }

        v1++;
    } while (v1 < v3);

    if (v2 == v3) {
        param0->unk_00 = 19;
        return 1;
    }

    return 0;
}

int ov85_021E5EE8(UnkStruct_ov109_021D0F70 *param0) {
    param0->unk_0C++;

    if (param0->unk_0C > 30) {
        param0->unk_0C = 0;

        if (sub_0203769C() == 0) {
            param0->unk_00 = 20;
        } else {
            param0->unk_00 = 21;
        }
    }

    return 0;
}

int ov85_021E5F10(UnkStruct_ov109_021D0F70 *param0) {
    u16 v0 = (1 << 2);

    if (sub_02096D4C(param0->unk_D0, 8, &v0, 2) == 1) {
        param0->unk_00 = 22;
    }

    return 0;
}

int ov85_021E5F3C(UnkStruct_ov109_021D0F70 *param0) {
    if (ov85_021E8614(param0, 1 << 2) == 1) {
        param0->unk_00 = 22;

        return 1;
    }

    return 0;
}

int ov85_021E5F58(UnkStruct_ov109_021D0F70 *param0) {
    sub_02037AC0(202);
    param0->unk_00 = 23;
    return 0;
}

int ov85_021E5F6C(UnkStruct_ov109_021D0F70 *param0) {
    if (sub_02037B38(202)) {
        param0->unk_00 = 24;
    }

    return 0;
}

s32 ov85_021E5F84(s32 *arg0) {
    ov85_021E84EC(arg0, 1);
    ov85_021E81E0(arg0);
    *arg0 = 0x19;
    return 0;
}

int ov85_021E5F9C(UnkStruct_ov109_021D0F70 *param0) {
    int v0;

    param0->unk_0C++;

    if (param0->unk_0C < (30 * 3 + 5)) {
        return 0;
    }

    for (v0 = 0; v0 < param0->unk_2C.unk_04; v0++) {
        ov85_021E7400(param0, v0);
    }

    param0->unk_0C = 0;
    param0->unk_00 = 26;

    return 0;
}

int ov85_021E5FD0(UnkStruct_ov109_021D0F70 *param0) {
    if (sub_0203769C() == 0) {
        param0->unk_00 = 27;
    } else {
        param0->unk_00 = 27;
    }

    return 0;
}
