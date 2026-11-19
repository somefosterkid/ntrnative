#include "global.h"
#include "field_system_def.h"
#include "simple3dmodel.h"

#include "heap.h"
#include "sprite_system.h"
#include "sys_task_api.h"
#include "unk_02005D10.h"
#include "unk_struct_0209_bdf8.h"
#include "save.h"
#include "options.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "overlay_85_ov109_021D5140.h"
#include "camera.h"
#include "simple3danimation.h"
#include "overlay_85_ov109_021D0F70.h"
#include "player_data.h"
#include "game_stats.h"
#include "msgdata.h"
#include "unk_struct_0209_c194.h"

struct BgConfig;

struct Camera;

struct CameraAngle;

struct GameStats;

struct JournalEntry;

struct ManagedSprite;

struct MsgData;

struct NARC;

struct Options;

struct PaletteData;

struct Party;

struct SaveData;

struct Simple3DAnimation;

struct Simple3DModel;

struct Simple3DRenderObj;

struct Simple3DRotationAngles;

struct SpriteManager;

struct SpriteSystem;

struct String;

struct MessageFormat;

struct SysTask;

struct SysTaskQueue;

struct TouchPadDataBuffer;

struct PlayerProfile;

struct UnkStruct_0205C22C_t;

struct UnkStruct_ov109_021D0F70_t;

struct UnkStruct_ov109_021D5140_t;

struct Window;

typedef struct UnkStruct_0205C22C_t UnkStruct_0205C22C;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

typedef struct JournalEntry JournalEntry;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    UnkStruct_ov109_021D0F70 *unk_10;
    ManagedSprite *unk_14;
} UnkStruct_ov109_021D3600;

void ov85_021E815C(SysTask *param0, void *param1);

typedef struct {
    int a[4];
} ov85_021EA528_Array;

extern const ov85_021EA528_Array ov85_021EA528;

void ov85_021E815C(SysTask *param0, void *param1) {
    UnkStruct_ov109_021D3600 *v0 = param1;

    switch (v0->unk_00) {
    case 0:
        if (v0->unk_0C < 4) {
            ov85_021EA528_Array v1 = ov85_021EA528;

            v0->unk_04--;

            if (v0->unk_04 <= 0) {
                v0->unk_04 = 30;
                PlaySE(v1.a[v0->unk_0C]);
                v0->unk_0C++;
            }
        }

        ManagedSprite_TickNFrames(v0->unk_14, FX32_ONE + (FX32_ONE / 2));

        if (ManagedSprite_IsAnimated(v0->unk_14) == 0) {
            v0->unk_00++;
        }

        break;
    case 1:
        Sprite_DeleteAndFreeResources(v0->unk_14);
        Heap_Free(v0);
        SysTask_Destroy(param0);
    }
}
