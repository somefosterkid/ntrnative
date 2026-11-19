#include "global.h"
#include "field_system_def.h"
#include "simple3dmodel.h"
#include "unk_struct_0209_bdf8.h"
#include "save.h"
#include "options.h"
#include "pm_string.h"
#include "filesystem.h"
#include "palette.h"
#include "pokemon_types_def.h"
#include "sys_task.h"
#include "sprite_system.h"
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

void ov85_021E8374(UnkStruct_ov109_021D0F70 *param0, UnkStruct_ov109_021D3818 *param1);

static int hgasset_ov85_021E8374_v0[16] __attribute__((section(BWPORT_DATA_SECTION))) = {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        };

void ov85_021E8374(UnkStruct_ov109_021D0F70 *param0, UnkStruct_ov109_021D3818 *param1) {
    switch (param1->unk_00) {
    case 0:
        break;
    case 1: {
        

        param1->unk_0C->unk_50.y = (FX32_ONE * (hgasset_ov85_021E8374_v0[param1->unk_04]));
        param1->unk_04++;

        if (param1->unk_04 >= 16) {
            param1->unk_04 = 0;
            param1->unk_00++;
        }
    } break;
    case 2:
        param1->unk_08++;

        if (param1->unk_08 >= 15) {
            param1->unk_08 = 0;
            param1->unk_00 = 1;
        }
        break;
    }
}

// tools/hg_assets.py: the tables above are blanks, port/src/asset_load.c fills them from the ROM at start
void PORT_AssetAdd(const char *game, const char *name, void *p, unsigned long size, const char *layout);
__attribute__((constructor)) static void hgasset_register_src_overlay_85_021E8008_021E8374_021E8374_c_0(void) {
    PORT_AssetAdd("IPKE", "src/overlay_85_021E8008_021E8374_021E8374.c:hgasset_ov85_021E8374_v0", (void *)&hgasset_ov85_021E8374_v0, sizeof(hgasset_ov85_021E8374_v0), "u8");
    PORT_AssetAdd("IPGE", "src/overlay_85_021E8008_021E8374_021E8374.c:hgasset_ov85_021E8374_v0", (void *)&hgasset_ov85_021E8374_v0, sizeof(hgasset_ov85_021E8374_v0), "u8");
}
