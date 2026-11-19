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
#include "heap.h"
#include "sys_task_api.h"
#include "unk_struct_0209_bdf8.h"
#include "overlay_85_ov109_021D5140.h"
#include "simple3danimation.h"
#include "overlay_85_ov109_021D0F70.h"
#include "player_data.h"
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

extern const u16 ov85_021EA7C4[5 + 1][5];

UnkStruct_ov109_021D2D78 *ov85_021E7958(UnkStruct_ov109_021D0F70 *param0);

void sub_02069978(Simple3DRenderObj *renderObj, Simple3DModel *model);
void ov85_021E7A54(UnkStruct_ov109_021D0F70 *param0, UnkStruct_ov109_021D2EF4 *param1);
void ov85_021E7A20(UnkStruct_ov109_021D0F70 *param0, UnkStruct_ov109_021D2D78 *param1);
void sub_02069998(Simple3DRenderObj *renderObj, Simple3DModel *model, Simple3DAnimation *anim);
void ov85_021E7AC8(UnkStruct_ov109_021D0F70 *param0, UnkStruct_ov109_021D2D78 *param1);
BOOL sub_020698E8(Simple3DAnimation *anim, fx32 frameDelta, BOOL loop);
void ov85_021E7B04(UnkStruct_ov109_021D0F70 *param0, UnkStruct_ov109_021D2FA4 *param1);

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    fx32 unk_0C;
    UnkStruct_ov109_021D2D78 *unk_10;
    UnkStruct_ov109_021D0F70 *unk_14;
} UnkStruct_ov109_021D2FE0;
void ov85_021E7B8C(SysTask *param0, void *param1);
SysTask *ov85_021E7B40(UnkStruct_ov109_021D0F70 *param0, int param1, int param2, int param3, int param4, int param5, UnkStruct_ov109_021D2D78 *param6);

UnkStruct_ov109_021D2D78 *ov85_021E7958(UnkStruct_ov109_021D0F70 *param0) {
    int v0, v1, v2, v3;
    fx32 v4, v5, v6;
    int v7 = param0->unk_2C.unk_04;
    int v8[5], v9[5];
    UnkStruct_ov109_021D1334 *v10 = &param0->unk_190;

    v0 = 0;
    v4 = (FX32_ONE * (ov85_021EA7C4[v7][param0->unk_2C.unk_00]));

    do {
        v9[v0] = v0, v8[v0] = 0xffff;
    } while (++v0 < v7);

    for (v0 = 0; v0 < v7; v0++) {
        v5 = v10->unk_140[v0].unk_1C;
        v6 = v4 - v5;

        if (v6 < 0) {
            v6 = (v4 + (FX32_ONE * 360)) - v5;
        }

        v8[v0] = ((v6) / FX32_ONE);
    }

    for (v0 = 1; v0 < v7; v0++) {
        v3 = v9[v0];
        v2 = v8[v3];

        for (v1 = v0 - 1; v1 >= 0 && v8[v9[v1]] > v2; v1--) {
            v9[v1 + 1] = v9[v1];
        }

        v9[v1 + 1] = v3;
    }

    return &v10->unk_140[v9[0]];
}

void ov85_021E7A20(UnkStruct_ov109_021D0F70 *param0, UnkStruct_ov109_021D2D78 *param1) {
    UnkStruct_ov109_021D1334 *v0 = &param0->unk_190;
    UnkStruct_ov109_021D2EF4 *v1 = &v0->unk_4B0[param1->unk_10];

    v1->unk_00 = 1;
    v1->unk_78 = param1;
    sub_02069978(&v1->unk_24, &v0->unk_14);
    ov85_021E7A54(param0, v1);
}

void ov85_021E7A54(UnkStruct_ov109_021D0F70 *param0, UnkStruct_ov109_021D2EF4 *param1) {
    fx32 v0;

    param1->unk_04.alpha = 0;
    param1->unk_04.beta = 0;
    param1->unk_04.gamma = 0;
    param1->unk_18 = param1->unk_78->unk_44;
    param1->unk_18.x -= param1->unk_78->unk_50.x;
    param1->unk_18.y -= param1->unk_78->unk_50.y;
    param1->unk_18.z -= param1->unk_78->unk_50.z;
    param1->unk_18.y += (FX32_ONE * 2);

    v0 = param1->unk_78->unk_50.y;

    if (v0 < 0) {
        v0 = -v0;
    }

    v0 /= 0x80;

    if (v0 > 0x800) {
        v0 = 0x800;
    }

    param1->unk_0C.x = FX32_ONE - v0;
    param1->unk_0C.y = FX32_ONE - v0;
    param1->unk_0C.z = FX32_ONE - v0;
}

void ov85_021E7AC8(UnkStruct_ov109_021D0F70 *param0, UnkStruct_ov109_021D2D78 *param1) {
    UnkStruct_ov109_021D1334 *v0 = &param0->unk_190;
    UnkStruct_ov109_021D2FA4 *v1 = &v0->unk_71C[param1->unk_10];

    v1->unk_00 = 1;
    v1->unk_64 = param1;

    sub_02069998(&v1->unk_10, &v0->unk_28[param1->unk_10], &v0->unk_8C[param1->unk_10]);
}

void ov85_021E7B04(UnkStruct_ov109_021D0F70 *param0, UnkStruct_ov109_021D2FA4 *param1) {
    UnkStruct_ov109_021D1334 *v0 = &param0->unk_190;

    param1->unk_04 = param1->unk_64->unk_44;
    param1->unk_04.y += (FX32_ONE * 20);

    sub_020698E8(&v0->unk_8C[param1->unk_64->unk_10], FX32_ONE, 1);
}

SysTask *ov85_021E7B40(UnkStruct_ov109_021D0F70 *param0, int param1, int param2, int param3, int param4, int param5, UnkStruct_ov109_021D2D78 *param6) {
    SysTask *v0;
    UnkStruct_ov109_021D2FE0 *v1;

    param6->unk_0C = param1;
    param6->unk_10 = param2;
    param6->unk_14 = param3;
    param6->unk_18 = param2;
    param6->unk_1C = (FX32_ONE * (param4));
    param6->unk_20 = (FX32_ONE * (param5));
    param6->unk_00 = 1;
    param6->unk_08 = 0;

    v1 = Heap_AllocAtEnd(HEAP_ID_102, sizeof(UnkStruct_ov109_021D2FE0));
    memset(v1, 0, sizeof(UnkStruct_ov109_021D2FE0));
    v1->unk_14 = param0;
    v1->unk_10 = param6;
    v0 = SysTask_CreateOnMainQueue(ov85_021E7B8C, v1, 0);

    return v0;
}
