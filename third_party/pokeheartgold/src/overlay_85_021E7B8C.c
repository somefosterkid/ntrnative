#include "global.h"
#include "field_system_def.h"
#include "simple3dmodel.h"

#include "heap.h"
#include "overlay_85.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "bg_window.h"
#include "camera.h"
#include "error_handling.h"
#include "filesystem.h"
#include "math_util.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "sprite_system.h"
#include "unk_02005D10.h"
#include "unk_struct_0209_bdf8.h"
#include "overlay_85_ov109_021D5140.h"
#include "simple3danimation.h"
#include "overlay_85_ov109_021D0F70.h"
#include "player_data.h"
#include "game_stats.h"
#include "msgdata.h"
#include "unk_struct_0209_c194.h"

extern u32 ov85_021EA51C;

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
    fx32 unk_0C;
    UnkStruct_ov109_021D2D78 *unk_10;
    UnkStruct_ov109_021D0F70 *unk_14;
} UnkStruct_ov109_021D2FE0;
void ov85_021E776C(UnkStruct_ov109_021D0F70 *param0, UnkStruct_ov109_021D2D78 *param1);
int ov85_021E7BC0(UnkStruct_ov109_021D2FE0 *param0);
void ov85_021E73D4(UnkStruct_ov109_021D0F70 *param0, String *param1, int param2, int param3);
int ov85_021E7C2C(UnkStruct_ov109_021D2FE0 *param0);
void ov85_021E7E88(UnkStruct_ov109_021D0F70 *param0, UnkStruct_ov109_021D2D78 *param1);
void ov85_021E7FC0(UnkStruct_ov109_021D0F70 *param0, UnkStruct_ov109_021D2D78 *param1);
void ov85_021E7C70(UnkStruct_ov109_021D0F70 *param0, UnkStruct_ov109_021D3110 *param1);
void ov85_021E7D08(UnkStruct_ov109_021D0F70 *param0);
void ov85_021E7D40(UnkStruct_ov109_021D0F70 *param0);
void ov85_021E7D50(UnkStruct_ov109_021D0F70 *param0, UnkStruct_ov109_021D2D78 *param1);
void ov85_021E7D78(UnkStruct_ov109_021D0F70 *param0);
BOOL ov85_021E7DA8(UnkStruct_ov109_021D0F70 *param0);
void ov85_021E8530(fx32 *param0, fx32 param1);
void ov85_021E7DC8(UnkStruct_ov109_021D3268 *param0);
void ov85_021E7E3C(UnkStruct_ov109_021D0F70 *param0);
void ov85_021E7E78(UnkStruct_ov109_021D0F70 *param0);
void ov85_021E7ED0(UnkStruct_ov109_021D3370 *param0);
void ov85_021E7F74(UnkStruct_ov109_021D0F70 *param0);
void ov85_021E7FB0(UnkStruct_ov109_021D0F70 *param0);

void ov85_021E7B8C(SysTask *param0, void *param1) {
    s32 iVar1;

    do {
        iVar1 = (**(s32 (**)(void *))(&ov85_021EA51C + *(s32 *)param1))(param1);
    } while (iVar1 == 1);
    if (iVar1 == 2) {
        *(u32 *)(*(u8 **)((u8 *)param1 + 0x10) + 8) = 1;
        Heap_Free(param1);
        SysTask_Destroy(param0);
    }
    return;
}

int ov85_021E7BC0(UnkStruct_ov109_021D2FE0 *param0) {
    UnkStruct_ov109_021D2D78 *v0 = param0->unk_10;

    v0->unk_38.x = FX32_ONE;
    v0->unk_38.y = FX32_ONE;
    v0->unk_38.z = FX32_ONE;
    v0->unk_50.y = (FX32_ONE * 192);

    ov85_021E776C(param0->unk_14, v0);

    param0->unk_0C = (FX32_ONE * 8);
    param0->unk_00 = 1;

    return 0;
}

s32 ov85_021E7BEC(void *arg0) {
    void *v4;

    v4 = *(void **)((u8 *)arg0 + 0x10);
    *(s32 *)((u8 *)v4 + 0x54) = (s32)(*(s32 *)((u8 *)v4 + 0x54) - *(s32 *)((u8 *)arg0 + 0xC));
    if ((*(s32 *)((u8 *)arg0 + 8) == 0) && ((s32) * (s32 *)((u8 *)v4 + 0x54) <= 0x68000)) {
        *(s32 *)((u8 *)arg0 + 8) = 1;
        PlaySE(0x64FU);
    }
    if ((s32) * (s32 *)((u8 *)v4 + 0x54) <= 0x20000) {
        *(s32 *)((u8 *)v4 + 0x54) = 0x20000;
        *(s32 *)arg0 = 2;
    }
    return 0;
}

int ov85_021E7C2C(UnkStruct_ov109_021D2FE0 *param0) {
    UnkStruct_ov109_021D2D78 *v0 = param0->unk_10;

    if (param0->unk_0C > (FX32_ONE * 1)) {
        param0->unk_0C -= 0x1000;

        if (param0->unk_0C < (FX32_ONE * 1)) {
            param0->unk_0C = (FX32_ONE * 1);
        }
    }

    v0->unk_50.y -= param0->unk_0C;

    if (v0->unk_50.y <= 0) {
        v0->unk_50.y = 0;

        ov85_021E73D4(param0->unk_14, param0->unk_14->unk_2C.unk_6C[v0->unk_0C], v0->unk_14, v0->unk_0C);

        return 2;
    }

    return 0;
}

void ov85_021E7C70(UnkStruct_ov109_021D0F70 *param0, UnkStruct_ov109_021D3110 *param1) {
    UnkStruct_ov109_021D2D78 *v0 = param1->unk_1C;

    switch (param1->unk_04) {
    case 0:
        ov85_021E7E88(param0, v0);
        param1->unk_10 = (FX32_ONE * 4);
        param1->unk_14 = -((FX32_ONE * 4) / (30 / 2));
        param1->unk_04++;
    case 1:
        v0->unk_50.y += param1->unk_10;
        param1->unk_10 += param1->unk_14;
        param1->unk_08++;

        if (param1->unk_08 < (30 / 2)) {
            break;
        }

        param1->unk_08 = 0;
        param1->unk_10 = 0;

        if (param1->unk_0C == 0) {
            param1->unk_0C = 1;
            break;
        }

        v0->unk_50.y = 0;
        ov85_021E7FC0(param0, v0);
        memset(param1, 0, sizeof(UnkStruct_ov109_021D3110));
    }
}

void ov85_021E7CE0(SysTask *param0, void *param1) {
    int v0;
    UnkStruct_ov109_021D0F70 *v1 = param1;
    UnkStruct_ov109_021D31A8 *v2 = &v1->unk_AB4;
    UnkStruct_ov109_021D3110 *v3 = v2->unk_00;

    for (v0 = 0; v0 < 5; v0++, v3++) {
        if (v3->unk_00) {
            ov85_021E7C70(v1, v3);
        }
    }
}

void ov85_021E7D08(UnkStruct_ov109_021D0F70 *param0) {
    UnkStruct_ov109_021D31A8 *v0 = &param0->unk_AB4;
    memset(v0, 0, sizeof(UnkStruct_ov109_021D31A8));

    param0->unk_DB0 = SysTask_CreateOnMainQueue(ov85_021E7CE0, param0, 256);
    GF_ASSERT(param0->unk_DB0);
}

void ov85_021E7D40(UnkStruct_ov109_021D0F70 *param0) {
    SysTask_Destroy(param0->unk_DB0);
}

void ov85_021E7D50(UnkStruct_ov109_021D0F70 *param0, UnkStruct_ov109_021D2D78 *param1) {
    int v0;
    UnkStruct_ov109_021D31A8 *v1 = &param0->unk_AB4;
    UnkStruct_ov109_021D3110 *v2 = v1->unk_00;

    for (v0 = 0; v0 < 5; v0++, v2++) {
        if (v2->unk_00 == 0) {
            v2->unk_00 = 1;
            v2->unk_1C = param1;
            return;
        }
    }

    GF_ASSERT(FALSE);
}

void ov85_021E7D78(UnkStruct_ov109_021D0F70 *param0) {
    UnkStruct_ov109_021D1334 *v0 = &param0->unk_190;
    int v1 = 0, v2 = param0->unk_2C.unk_04;

    do {
        if (v0->unk_140[v1].unk_00) {
            ov85_021E7D50(param0, &v0->unk_140[v1]);
        }

        v1++;
    } while (v1 < v2);
}

BOOL ov85_021E7DA8(UnkStruct_ov109_021D0F70 *param0) {
    int v0;
    UnkStruct_ov109_021D31A8 *v1 = &param0->unk_AB4;
    UnkStruct_ov109_021D3110 *v2 = v1->unk_00;

    for (v0 = 0; v0 < 5; v0++, v2++) {
        if (v2->unk_00) {
            return 1;
        }
    }

    return 0;
}

void ov85_021E7DC8(UnkStruct_ov109_021D3268 *param0) {
    fx32 v0;
    UnkStruct_ov109_021D2D78 *v1 = param0->unk_10;

    switch (param0->unk_04) {
    case 0:
        param0->unk_0C = (((fx32)0x0000000000001000L) * 24);
        v1->unk_2C.y = 0;
        param0->unk_04++;
    case 1:
        v0 = v1->unk_2C.y;
        ov85_021E8530(&v1->unk_2C.y, param0->unk_0C);

        if (v1->unk_2C.y < (((fx32)0x0000000000001000L) * 4)) {
            param0->unk_0C = (u32)(param0->unk_0C) >> 1;

            if (param0->unk_0C < (((fx32)0x0000000000001000L) * 4)) {
                v1->unk_2C.y = 0;
                memset(param0, 0, sizeof(UnkStruct_ov109_021D3268));
            }
        }
    }
}

void ov85_021E7E18(SysTask *param0, void *param1) {
    int v0;
    UnkStruct_ov109_021D0F70 *v1 = param1;
    UnkStruct_ov109_021D32DC *v2 = &v1->unk_B54;
    UnkStruct_ov109_021D3268 *v3 = v2->unk_00;

    for (v0 = 0; v0 < 5; v0++, v3++) {
        if (v3->unk_00) {
            ov85_021E7DC8(v3);
        }
    }
}

void ov85_021E7E3C(UnkStruct_ov109_021D0F70 *param0) {
    UnkStruct_ov109_021D32DC *v0 = &param0->unk_B54;
    memset(v0, 0, sizeof(UnkStruct_ov109_021D32DC));

    param0->unk_DB4 = SysTask_CreateOnMainQueue(ov85_021E7E18, param0, 257);
    GF_ASSERT(param0->unk_DB4);
}

void ov85_021E7E78(UnkStruct_ov109_021D0F70 *param0) {
    SysTask_Destroy(param0->unk_DB4);
}

void ov85_021E7E88(UnkStruct_ov109_021D0F70 *param0, UnkStruct_ov109_021D2D78 *param1) {
    int v0;
    UnkStruct_ov109_021D32DC *v1 = &param0->unk_B54;
    UnkStruct_ov109_021D3268 *v2 = v1->unk_00;

    for (v0 = 0; v0 < 5; v0++, v2++) {
        if ((v2->unk_00 == 1) && (v2->unk_10 == param1)) {
            v2->unk_04 = 0;
            return;
        }
    }

    for (v0 = 0, v2 = v1->unk_00; v0 < 5; v0++, v2++) {
        if (v2->unk_00 == 0) {
            v2->unk_00 = 1;
            v2->unk_10 = param1;
            return;
        }
    }

    GF_ASSERT(FALSE);
}

void ov85_021E7ED0(UnkStruct_ov109_021D3370 *param0) {
    fx32 v0;
    UnkStruct_ov109_021D2D78 *v1 = param0->unk_14;

    switch (param0->unk_04) {
    case 0:
        v1->unk_2C.z = 0;
        param0->unk_10 = 0;
        param0->unk_08 = (((fx32)0x0000000000001000L) * 30);
        param0->unk_0C = (((fx32)0x0000000000001000L) * 20);
        param0->unk_04++;
    case 1:
        v0 = GF_SinDegNoWrap((param0->unk_10) / ((fx32)0x0000000000001000L)) * ((param0->unk_08) / ((fx32)0x0000000000001000L));
        v1->unk_2C.z = 0;
        ov85_021E8530(&v1->unk_2C.z, v0);

        v0 = param0->unk_10;
        ov85_021E8530(&param0->unk_10, param0->unk_0C);

        if (param0->unk_10 < v0) {
            param0->unk_08 = (u32)(param0->unk_08) >> 1;

            if (param0->unk_08 == 0) {
                v1->unk_2C.z = 0;
                memset(param0, 0, sizeof(UnkStruct_ov109_021D3370));
            }
        }
    }
}

void ov85_021E7F50(SysTask *param0, void *param1) {
    int v0;
    UnkStruct_ov109_021D0F70 *v1 = param1;
    UnkStruct_ov109_021D3414 *v2 = &v1->unk_BB8;
    UnkStruct_ov109_021D3370 *v3 = v2->unk_00;

    for (v0 = 0; v0 < 5; v0++, v3++) {
        if (v3->unk_00) {
            ov85_021E7ED0(v3);
        }
    }
}

void ov85_021E7F74(UnkStruct_ov109_021D0F70 *param0) {
    UnkStruct_ov109_021D3414 *v0 = &param0->unk_BB8;
    memset(v0, 0, sizeof(UnkStruct_ov109_021D3414));

    param0->unk_DB8 = SysTask_CreateOnMainQueue(ov85_021E7F50, param0, 258);
    GF_ASSERT(param0->unk_DB8);
}

void ov85_021E7FB0(UnkStruct_ov109_021D0F70 *param0) {
    SysTask_Destroy(param0->unk_DB8);
}

void ov85_021E7FC0(UnkStruct_ov109_021D0F70 *param0, UnkStruct_ov109_021D2D78 *param1) {
    int v0;
    UnkStruct_ov109_021D3414 *v1 = &param0->unk_BB8;
    UnkStruct_ov109_021D3370 *v2 = v1->unk_00;

    for (v0 = 0; v0 < 5; v0++, v2++) {
        if ((v2->unk_00 == 1) && (v2->unk_14 == param1)) {
            v2->unk_04 = 0;
            return;
        }
    }

    for (v0 = 0, v2 = v1->unk_00; v0 < 5; v0++, v2++) {
        if (v2->unk_00 == 0) {
            v2->unk_00 = 1;
            v2->unk_14 = param1;
            return;
        }
    }

    GF_ASSERT(FALSE);
}
