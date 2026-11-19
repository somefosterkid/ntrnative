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

void sub_020696C4(Simple3DModel *model, u32 modelIdx, NARC *narc, u32 modelSetNarcIdx, u32 heapID, BOOL allocAtEnd);

void sub_02069714(Simple3DModel *model);

void sub_02069978(Simple3DRenderObj *renderObj, Simple3DModel *model);

extern const u32 ov85_021EA5C4[5 + 1];

extern const fx32 ov85_021EA5DC[5 + 1];

void ov85_021E752C(UnkStruct_ov109_021D0F70 *param0);

void sub_02069784(Simple3DModel *model);
void ov85_021E75B8(UnkStruct_ov109_021D0F70 *param0);
void ov85_021E8530(fx32 *param0, fx32 param1);
void ov85_021E75C8(UnkStruct_ov109_021D0F70 *param0);
void sub_020699D0(Simple3DRenderObj *renderObj, const VecFx32 *position, const VecFx32 *scale, const Simple3DRotationAngles *rotation);
void ov85_021E7628(UnkStruct_ov109_021D0F70 *param0);
void ov85_021E7644(UnkStruct_ov109_021D2AE4 *param0, fx32 param1);
SysTask *ov85_021E7660(SysTaskFunc callback, void *param, u32 priority);
SysTask *ov85_021E7734(SysTaskFunc callback, void *param, u32 priority);
SysTask *ov85_021E7658(SysTaskFunc callback, void *param, int priority);

void ov85_021E752C(UnkStruct_ov109_021D0F70 *param0) {
    int v0;
    u32 v1;
    UnkStruct_ov109_021D2AE4 *v2 = &param0->unk_D4;
    NARC *v3 = param0->unk_D80;

    v0 = param0->unk_CC->unk_08;
    v1 = ov85_021EA5C4[v0];

    sub_020696C4(&v2->unk_54, 0, v3, v1, HEAP_ID_102, 0);
    sub_02069714(&v2->unk_54);
    sub_02069978(&v2->unk_68, &v2->unk_54);

    v2->unk_3C = (FX32_ONE * 0);
    v2->unk_44 = ov85_021EA5DC[v0];
    v2->unk_18.x = FX32_ONE;
    v2->unk_18.y = FX32_ONE;
    v2->unk_18.z = FX32_ONE;
    v2->unk_4C.alpha = 0;
    v2->unk_4C.beta = 0;
    v2->unk_4C.gamma = 0;
    v2->unk_00.x = (FX32_ONE * 0);
    v2->unk_00.y = (FX32_ONE * -36);
    v2->unk_00.z = (FX32_ONE * 0);
    v2->unk_0C = v2->unk_00;
}

void ov85_021E75B8(UnkStruct_ov109_021D0F70 *param0) {
    UnkStruct_ov109_021D2AE4 *v0 = &param0->unk_D4;
    sub_02069784(&v0->unk_54);
}

void ov85_021E75C8(UnkStruct_ov109_021D0F70 *param0) {
    UnkStruct_ov109_021D2AE4 *v0 = &param0->unk_D4;
    fx32 v1 = v0->unk_3C;

    ov85_021E8530(&v1, v0->unk_40);
    ov85_021E8530(&v1, v0->unk_44);

    v0->unk_4C.beta = (360 - ((v1) / ((fx32)0x0000000000001000L))) % 360;
    v0->unk_0C.x = v0->unk_00.x + v0->unk_30.x + v0->unk_24.x;
    v0->unk_0C.y = v0->unk_00.y + v0->unk_30.y + v0->unk_24.y;
    v0->unk_0C.z = v0->unk_00.z + v0->unk_30.z + v0->unk_24.z;
}

void ov85_021E7628(UnkStruct_ov109_021D0F70 *param0) {
    UnkStruct_ov109_021D2AE4 *v0 = &param0->unk_D4;
    sub_020699D0(&v0->unk_68, &v0->unk_0C, &v0->unk_18, &v0->unk_4C);
}

void ov85_021E7644(UnkStruct_ov109_021D2AE4 *param0, fx32 param1) {
    ov85_021E8530(&param0->unk_3C, param1);
}

SysTask *ov85_021E7650(SysTaskFunc callback, void *param, int priority) {
    return ov85_021E7660(callback, param, priority);
}

SysTask *ov85_021E7658(SysTaskFunc callback, void *param, int priority) {
    return ov85_021E7734(callback, param, priority);
}
