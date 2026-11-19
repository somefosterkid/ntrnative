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
#include "error_handling.h"
#include "unk_02005D10.h"
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

void ov85_021E7644(UnkStruct_ov109_021D2AE4 *param0, fx32 param1);

void ov85_021E78A4(UnkStruct_ov109_021D0F70 *param0, fx32 param1);

void ov85_021E8530(fx32 *param0, fx32 param1);

extern const u16 ov85_021EA7C4[5 + 1][5];

int ov85_021E6224(UnkStruct_ov109_021D0F70 *param0);

int ov85_021E62A0(UnkStruct_ov109_021D0F70 *param0);
int ov85_021E62B4(UnkStruct_ov109_021D0F70 *param0);
BOOL sub_02096D4C(UnkStruct_0209BDF8 *param0, u32 param1, const void *param2, u32 param3);
BOOL ov85_021E8764(UnkStruct_ov109_021D0F70 *param0, fx32 param1, int param2);
int ov85_021E62D8(UnkStruct_ov109_021D0F70 *param0);
void ov85_021E75C8(UnkStruct_ov109_021D0F70 *param0);
void ov85_021E7798(UnkStruct_ov109_021D0F70 *param0);
UnkStruct_ov109_021D2D78 *ov85_021E7958(UnkStruct_ov109_021D0F70 *param0);
int ov85_021E636C(UnkStruct_ov109_021D0F70 *param0);
void ov85_021E73D4(UnkStruct_ov109_021D0F70 *param0, String *param1, int param2, int param3);
int ov85_021E63D8(UnkStruct_ov109_021D0F70 *param0);
s32 ov85_021E71EC(void *, s32, const PlayerProfile *);
s32 ov85_021E7400(void *, s32);
s32 ov85_021E6420(UnkStruct_ov109_021D0F70 *arg0);
int ov85_021E6474(UnkStruct_ov109_021D0F70 *param0);

int ov85_021E6224(UnkStruct_ov109_021D0F70 *param0) {
    int v0, v1;
    fx32 v2, v3, v4, v5;
    UnkStruct_ov109_021D2D78 *v6 = param0->unk_28;

    v1 = param0->unk_2C.unk_00;
    v0 = param0->unk_2C.unk_04;
    v5 = (FX32_ONE * 4);
    v2 = v6->unk_1C;
    v3 = (FX32_ONE * (ov85_021EA7C4[v0][v1]));

    v4 = v3 - v2;

    if (v4 < 0) {
        v4 = (v3 + (FX32_ONE * 360)) - v2;
    }

    if (v4 <= (FX32_ONE * 16)) {
        v5 >>= 1;
    }

    if (v4 < v5) {
        v5 = v4;
    }

    ov85_021E7644(&param0->unk_D4, v5);
    ov85_021E78A4(param0, v5);
    ov85_021E8530(&v2, v5);

    if (((v2) / FX32_ONE) == ((v3) / FX32_ONE)) {
        param0->unk_00 = 32;
    }

    return 0;
}

int ov85_021E62A0(UnkStruct_ov109_021D0F70 *param0) {
    sub_02037AC0(202);
    param0->unk_00 = 33;
    return 0;
}

int ov85_021E62B4(UnkStruct_ov109_021D0F70 *param0) {
    if (sub_02037B38(202)) {
        if (sub_0203769C() == 0) {
            param0->unk_00 = 34;
        } else {
            param0->unk_00 = 35;
        }
    }

    return 0;
}

int ov85_021E62D8(UnkStruct_ov109_021D0F70 *param0) {
    int v0, v1;
    UnkStruct_ov109_021D2D78 *v2;
    UnkStruct_ov109_021D17EC v3;

    v3.unk_00 = ((param0->unk_D4.unk_3C) / FX32_ONE);

    for (v0 = 0, param0->unk_20 = 0; v0 < param0->unk_2C.unk_04; v0++) {
        v2 = &param0->unk_190.unk_140[v0];
        GF_ASSERT(v2->unk_00);
        v3.unk_04[v2->unk_10] = (u16)((v2->unk_1C) / FX32_ONE);

        if (ov85_021E8764(param0, v2->unk_1C, param0->unk_2C.unk_04) == 1) {
            param0->unk_20 |= (1 << v2->unk_0C);
        }
    }

    v3.unk_02 = param0->unk_20;
    v1 = sub_02096D4C(param0->unk_D0, 15, &v3, sizeof(UnkStruct_ov109_021D17EC));

    if (v1 == 1) {
        param0->unk_00 = 36;
    }

    return 0;
}

int ov85_021E636C(UnkStruct_ov109_021D0F70 *param0) {
    if (param0->unk_2C.unk_14 == 1) {
        int v0;
        UnkStruct_ov109_021D2D78 *v1;
        UnkStruct_ov109_021D17EC *v2 = &param0->unk_2C.unk_34;

        param0->unk_D4.unk_3C = (FX32_ONE * (v2->unk_00));

        for (v0 = 0; v0 < param0->unk_2C.unk_04; v0++) {
            v1 = &param0->unk_190.unk_140[v0];
            GF_ASSERT(v1->unk_00);
            v1->unk_1C = (FX32_ONE * (v2->unk_04[v1->unk_10]));
        }

        param0->unk_20 = v2->unk_02;

        ov85_021E75C8(param0);
        ov85_021E7798(param0);

        param0->unk_28 = ov85_021E7958(param0);
        param0->unk_00 = 36;
    }

    return 0;
}

int ov85_021E63D8(UnkStruct_ov109_021D0F70 *param0) {
    int v0, v1;
    int v2 = param0->unk_28->unk_10;

    for (v0 = 0; v0 < param0->unk_2C.unk_04; v0++) {
        v1 = param0->unk_190.unk_140[v2].unk_0C;
        ov85_021E73D4(param0, param0->unk_2C.unk_6C[v1], v0, v1);
        v2++;
        v2 %= param0->unk_2C.unk_04;
    }

    param0->unk_0C = 0;
    param0->unk_00 = 37;

    return 0;
}

s32 ov85_021E6420(UnkStruct_ov109_021D0F70 *arg0) {
    s32 v4;
    s32 v5;

    v4 = arg0->unk_0C + 1;
    arg0->unk_0C = v4;
    if (v4 < 0x78) {
        return 0;
    }
    v5 = 0;
    arg0->unk_0C = 0;
    if (arg0->unk_2C.unk_04 > 0) {
        do {
            ov85_021E7400(arg0, v5);
            v5 += 1;
        } while (v5 < arg0->unk_2C.unk_04);
    }
    ov85_021E71EC(arg0, 2, arg0->unk_2C.unk_58[arg0->unk_28->unk_0C]);
    PlayFanfare(0x4A1U);
    arg0->unk_00 = 0x26;
    return 0;
}

int ov85_021E6474(UnkStruct_ov109_021D0F70 *param0) {
    if (param0->unk_0C < (30 * 4)) {
        param0->unk_0C++;
    } else if (IsFanfarePlaying() == 0) {
        param0->unk_0C = 0;
        param0->unk_00 = 39;
    }

    return 0;
}
