#include "global.h"
#include "field_system_def.h"
#include "simple3dmodel.h"

#include "constants/heap.h"

#include "struct_defs/resdat_narc_structs.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "heap.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "error_handling.h"
#include "overlay_85.h"
#include "unk_02005D10.h"
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

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    UnkStruct_ov109_021D0F70 *unk_10;
    ManagedSprite *unk_14;
} UnkStruct_ov109_021D3600;

ManagedSprite *ov85_021E7044(UnkStruct_ov109_021D0F70 *param0, const ManagedSpriteTemplate *param1);

void ov85_021E815C(SysTask *param0, void *param1);

SysTask *ov85_021E81E0(UnkStruct_ov109_021D0F70 *param0);

void ov85_021E6DF0(UnkStruct_ov109_021D0F70 *param0);
void ov85_021E6DFC(UnkStruct_ov109_021D0F70 *param0);
void ov85_021E6E08(UnkStruct_ov109_021D0F70 *param0);
void ov85_021E825C(UnkStruct_ov109_021D0F70 *param0, UnkStruct_ov109_021D3700 *param1);
void ov85_021E82F8(UnkStruct_ov109_021D0F70 *param0);
void ov85_021E833C(UnkStruct_ov109_021D0F70 *param0);
BOOL ov85_021E834C(UnkStruct_ov109_021D0F70 *param0);
void ov85_021E8358(UnkStruct_ov109_021D0F70 *param0);

SysTask *ov85_021E81E0(UnkStruct_ov109_021D0F70 *param0) {
    ManagedSpriteTemplate v0;
    UnkStruct_ov109_021D3600 *v1 = Heap_AllocAtEnd(HEAP_ID_102, sizeof(UnkStruct_ov109_021D3600));

    memset(v1, 0, sizeof(UnkStruct_ov109_021D3600));

    v0.x = 128;
    v0.y = 100;
    v0.z = 0;
    v0.animation = 0;
    v0.drawPriority = 0;
    v0.pal = 0;
    v0.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    v0.bgPriority = 0;
    v0.vramTransfer = 0;
    v0.resIdList[0] = 4;
    v0.resIdList[1] = 5;
    v0.resIdList[2] = 6;
    v0.resIdList[3] = 7;
    v0.resIdList[4] = -1;
    v0.resIdList[5] = -1;

    v1->unk_14 = ov85_021E7044(param0, &v0);
    ManagedSprite_TickFrame(v1->unk_14);

    v1->unk_10 = param0;
    {
        SysTask *v2 = SysTask_CreateOnMainQueue(ov85_021E815C, v1, 0);

        ((v2 != 0L) ? (void)0 : GF_AssertFail());
        return v2;
    }
}

void ov85_021E825C(UnkStruct_ov109_021D0F70 *param0, UnkStruct_ov109_021D3700 *param1) {
    switch (param1->unk_00) {
    case 0:
        break;
    case 1:
        ov85_021E6DF0(param0);
        PlaySE(1508);
        param1->unk_04 = 0;
        param1->unk_00++;
    case 2:
        param1->unk_04++;

        if (param1->unk_04 >= 2) {
            param1->unk_04 = 0;
            param1->unk_00++;
            ov85_021E6E08(param0);
        }
        break;
    case 3:
        param1->unk_04++;

        if (param1->unk_04 >= 4) {
            ov85_021E6DF0(param0);
            param1->unk_04 = 0;
            param1->unk_00++;
        }
        break;
    case 4:
        param1->unk_04++;

        if (param1->unk_04 >= 2) {
            ov85_021E6DFC(param0);
            param1->unk_02 = 0;
            param1->unk_00 = 0;
        }
        break;
    }
}

void ov85_021E82E4(SysTask *param0, void *param1) {
    UnkStruct_ov109_021D0F70 *v0 = param1;
    UnkStruct_ov109_021D3700 *v1 = &v0->unk_C44;

    ov85_021E825C(v0, v1);
}

void ov85_021E82F8(UnkStruct_ov109_021D0F70 *param0) {
    UnkStruct_ov109_021D3700 *v0 = &param0->unk_C44;
    memset(v0, 0, sizeof(UnkStruct_ov109_021D3700));
    param0->unk_DC0 = SysTask_CreateOnMainQueue(ov85_021E82E4, param0, 259);
    GF_ASSERT(param0->unk_DC0);
}

void ov85_021E833C(UnkStruct_ov109_021D0F70 *param0) {
    SysTask_Destroy(param0->unk_DC0);
}

BOOL ov85_021E834C(UnkStruct_ov109_021D0F70 *param0) {
    UnkStruct_ov109_021D3700 *v0 = &param0->unk_C44;
    return v0->unk_02;
}

void ov85_021E8358(UnkStruct_ov109_021D0F70 *param0) {
    UnkStruct_ov109_021D3700 *v0 = &param0->unk_C44;
    GF_ASSERT(v0->unk_02 == 0);
    v0->unk_02 = 1;
    v0->unk_00 = 1;
}
