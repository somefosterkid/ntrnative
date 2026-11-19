#include "global.h"
#include "field_system_def.h"
#include "simple3dmodel.h"

#include "constants/heap.h"

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
#include "math_util.h"
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

void sub_020696C4(Simple3DModel *model, u32 modelIdx, NARC *narc, u32 modelSetNarcIdx, u32 heapID, BOOL allocAtEnd);

void sub_02069714(Simple3DModel *model);

void sub_020697DC(Simple3DAnimation *anim, u32 unused, NARC *narc, u32 animSetNarcIdx, u32 heapID, BOOL allocAtEnd);

void sub_02069864(Simple3DAnimation *anim, const Simple3DModel *model, u32 heapID);

void sub_02069884(Simple3DAnimation *anim, const Simple3DModel *model);

extern const u32 ov85_021EA558[5];

extern const u32 ov85_021EA580[5];

void ov85_021E7660(UnkStruct_ov109_021D0F70 *param0);

void sub_02069784(Simple3DModel *model);
void sub_02069818(Simple3DAnimation *anim);
void ov85_021E7734(UnkStruct_ov109_021D0F70 *param0);
void sub_02069978(Simple3DRenderObj *renderObj, Simple3DModel *model);
void ov85_021E7A20(UnkStruct_ov109_021D0F70 *param0, UnkStruct_ov109_021D2D78 *param1);
void ov85_021E7AC8(UnkStruct_ov109_021D0F70 *param0, UnkStruct_ov109_021D2D78 *param1);
void ov85_021E776C(UnkStruct_ov109_021D0F70 *param0, UnkStruct_ov109_021D2D78 *param1);
void ov85_021E78D8(UnkStruct_ov109_021D2D78 *param0, const VecFx32 *param1);
void ov85_021E7A54(UnkStruct_ov109_021D0F70 *param0, UnkStruct_ov109_021D2EF4 *param1);
void ov85_021E7B04(UnkStruct_ov109_021D0F70 *param0, UnkStruct_ov109_021D2FA4 *param1);
void ov85_021E7798(UnkStruct_ov109_021D0F70 *param0);
void sub_020699BC(Simple3DRenderObj *renderObj, const VecFx32 *position);
void sub_020699D0(Simple3DRenderObj *renderObj, const VecFx32 *position, const VecFx32 *scale, const Simple3DRotationAngles *rotation);
void ov85_021E782C(UnkStruct_ov109_021D0F70 *param0);
void ov85_021E8530(fx32 *param0, fx32 param1);
void ov85_021E78A4(UnkStruct_ov109_021D0F70 *param0, fx32 param1);

void ov85_021E7660(UnkStruct_ov109_021D0F70 *param0) {
    int v0;
    const u32 *v1, *v2;
    UnkStruct_ov109_021D1334 *v3 = &param0->unk_190;

    sub_020696C4(&v3->unk_00, 0, param0->unk_D80, 0, HEAP_ID_102, 0);
    sub_02069714(&v3->unk_00);
    sub_020696C4(&v3->unk_14, 0, param0->unk_D80, 5, HEAP_ID_102, 0);
    sub_02069714(&v3->unk_14);

    v1 = ov85_021EA558;
    v2 = ov85_021EA580;

    for (v0 = 0; v0 < 5; v0++, v1++, v2++) {
        sub_020696C4(&v3->unk_28[v0], 0, param0->unk_D80, *v1, HEAP_ID_102, 0);
        sub_02069714(&v3->unk_28[v0]);
        sub_020697DC(&v3->unk_8C[v0], 0, param0->unk_D80, *v2, HEAP_ID_102, 0);
        sub_02069864(&v3->unk_8C[v0], &v3->unk_28[v0], HEAP_ID_102);
        sub_02069884(&v3->unk_8C[v0], &v3->unk_28[v0]);
    }
}

void ov85_021E7734(UnkStruct_ov109_021D0F70 *param0) {
    int v0;
    UnkStruct_ov109_021D1334 *v1 = &param0->unk_190;

    sub_02069784(&v1->unk_00);
    sub_02069784(&v1->unk_14);

    for (v0 = 0; v0 < 5; v0++) {
        sub_02069784(&v1->unk_28[v0]);
        sub_02069818(&v1->unk_8C[v0]);
    }
}

void ov85_021E776C(UnkStruct_ov109_021D0F70 *param0, UnkStruct_ov109_021D2D78 *param1) {
    UnkStruct_ov109_021D1334 *v0 = &param0->unk_190;

    sub_02069978(&param1->unk_5C, &v0->unk_00);
    param1->unk_04 = 1;

    ov85_021E7A20(param0, param1);
    ov85_021E7AC8(param0, param1);
}

void ov85_021E7798(UnkStruct_ov109_021D0F70 *param0) {
    int v0 = 0, v1 = param0->unk_2C.unk_04;
    UnkStruct_ov109_021D1334 *v2 = &param0->unk_190;

    while (v0 < v1) {
        if (v2->unk_140[v0].unk_00) {
            ov85_021E78D8(&v2->unk_140[v0], &param0->unk_D4.unk_30);
        }

        if (v2->unk_4B0[v0].unk_00) {
            ov85_021E7A54(param0, &v2->unk_4B0[v0]);
        }

        if (v2->unk_71C[v0].unk_00) {
            ov85_021E7B04(param0, &v2->unk_71C[v0]);
        }

        v0++;
    }
}

void ov85_021E782C(UnkStruct_ov109_021D0F70 *param0) {
    int v0 = 0, v1 = param0->unk_2C.unk_04;
    UnkStruct_ov109_021D1334 *v2 = &param0->unk_190;
    UnkStruct_ov109_021D2D78 *v3 = v2->unk_140;
    UnkStruct_ov109_021D2EF4 *v4 = v2->unk_4B0;
    UnkStruct_ov109_021D2FA4 *v5 = v2->unk_71C;

    while (v0 < v1) {
        if (v3->unk_04) {
            sub_020699D0(&v3->unk_5C, &v3->unk_44, &v3->unk_38, &v3->unk_24);
        }

        if (v4->unk_00) {
            sub_020699D0(&v4->unk_24, &v4->unk_18, &v4->unk_0C, &v4->unk_04);
        }

        if (v5->unk_00) {
            sub_020699BC(&v5->unk_10, &v5->unk_04);
        }

        v3++;
        v4++;
        v5++;
        v0++;
    }
}

void ov85_021E78A4(UnkStruct_ov109_021D0F70 *param0, fx32 param1) {
    int v0 = 0, v1 = param0->unk_2C.unk_04;
    UnkStruct_ov109_021D1334 *v2 = &param0->unk_190;

    while (v0 < v1) {
        if (v2->unk_140[v0].unk_00) {
            ov85_021E8530(&v2->unk_140[v0].unk_1C, param1);
        }

        v0++;
    }
}

void ov85_021E78D8(UnkStruct_ov109_021D2D78 *param0, const VecFx32 *param1) {
    u16 v0;
    fx32 v1;

    v1 = param0->unk_1C;
    ov85_021E8530(&v1, param0->unk_20);
    v0 = (u16)((v1) / ((fx32)0x0000000000001000L));

    param0->unk_44.x = (((fx32)0x0000000000001000L) * 0) + param0->unk_50.x + param1->x + (GF_CosDegNoWrap(v0) * 22);
    param0->unk_44.y = (((fx32)0x0000000000001000L) * -6) + param0->unk_50.y + param1->y;
    param0->unk_44.z = (((fx32)0x0000000000001000L) * 0) + param0->unk_50.z + param1->z + (GF_SinDegNoWrap(v0) * 22);

    param0->unk_24.alpha = ((param0->unk_2C.x) / ((fx32)0x0000000000001000L));
    param0->unk_24.beta = ((param0->unk_2C.y) / ((fx32)0x0000000000001000L));
    param0->unk_24.gamma = ((param0->unk_2C.z) / ((fx32)0x0000000000001000L));
}
