#include "global.h"
#include "field_system_def.h"
#include "simple3dmodel.h"

#include "bg_window.h"
#include "camera.h"
#include "error_handling.h"
#include "filesystem.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "sprite_system.h"
#include "sys_task_api.h"
#include "heap.h"
#include "overlay_85.h"
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

void ov85_021E83C0(SysTask *param0, void *param1);

void ov85_021E83E0(UnkStruct_ov109_021D0F70 *param0);

void ov85_021E8418(UnkStruct_ov109_021D0F70 *param0);
void ov85_021E8428(UnkStruct_ov109_021D0F70 *param0, UnkStruct_ov109_021D2D78 *param1);
void ov85_021E8454(UnkStruct_ov109_021D0F70 *param0);
typedef struct {
    UnkStruct_ov109_021D0F70 *unk_00;
    int unk_04;
    u32 unk_08;
} UnkStruct_ov109_021D3948;
void ov85_021E6EA8(UnkStruct_ov109_021D0F70 *param0, u32 param1);

void ov85_021E83E0(UnkStruct_ov109_021D0F70 *param0) {
    UnkStruct_ov109_021D3884 *v0 = &param0->unk_C4C;
    memset(v0, 0, sizeof(UnkStruct_ov109_021D3884));
    param0->unk_DC4 = SysTask_CreateOnMainQueue(ov85_021E83C0, param0, 260);
    GF_ASSERT(param0->unk_DC4);
}

void ov85_021E8418(UnkStruct_ov109_021D0F70 *param0) {
    SysTask_Destroy(param0->unk_DC4);
}

void ov85_021E8428(UnkStruct_ov109_021D0F70 *param0, UnkStruct_ov109_021D2D78 *param1) {
    int v0 = param1->unk_0C;
    UnkStruct_ov109_021D3884 *v1 = &param0->unk_C4C;
    UnkStruct_ov109_021D3818 *v2 = &v1->unk_00[v0];

    GF_ASSERT(v0 < 5);
    GF_ASSERT(v2->unk_00 == 0);

    v2->unk_00 = 1;
    v2->unk_0C = param1;
}

void ov85_021E8454(UnkStruct_ov109_021D0F70 *param0) {
    u32 v0 = param0->unk_20;
    int v1 = 0, v2 = param0->unk_2C.unk_04;
    UnkStruct_ov109_021D1334 *v3 = &param0->unk_190;

    while (v1 < v2) {
        if (v3->unk_140[v1].unk_00 && (v0 & (1 << v3->unk_140[v1].unk_0C))) {
            ov85_021E8428(param0, &v3->unk_140[v1]);
        }

        v1++;
    }
}

void ov85_021E84A4(SysTask *param0, void *param1) {
    int v0 = 0;
    UnkStruct_ov109_021D3948 *v1 = param1;

    if (v1->unk_04 == 1) {
        v1->unk_08--;

        if ((int)v1->unk_08 <= 0) {
            v1->unk_08 = 0;
            v0 = 1;
        }
    } else {
        v1->unk_08++;

        if ((int)v1->unk_08 >= 8) {
            v1->unk_08 = 8;
            v0 = 1;
        }
    }

    ov85_021E6EA8(v1->unk_00, v1->unk_08);

    if (v0 == 1) {
        Heap_Free(v1);
        SysTask_Destroy(param0);
    }
}
