#include "global.h"
#include "sprite_resources_header.h"
#include "field_system_def.h"

#include "bg_window.h"
#include "options.h"
#include "overlay_85.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"
#include "vram_transfer_manager.h"
#include "overlay_85_ov109_021D5140.h"
#include "overlay_41_sprite_resource_collection.h"
#include "player_data.h"
#include "msgdata.h"
#include "game_stats.h"
#include "overlay_85_ov109_021D0F70.h"
#include "unk_struct_0209_c194.h"

struct GameStats;

struct JournalEntry;

struct ListMenu2D;

struct MsgData;

struct SpriteList;

struct SpriteResourceCollection;

struct MessageFormat;

struct PlayerProfile;

struct UnkStruct_0205C22C_t;

struct UnkStruct_ov109_021D0F70_t;

typedef struct UnkStruct_0205C22C_t UnkStruct_0205C22C;

typedef struct JournalEntry JournalEntry;

typedef struct ListMenu2D ListMenu2D;

void ov85_021E9E58(u16 *param0);

extern const u8 ov85_021EA8EA[5][2];

void ov85_021E8BB0(SysTask *param0, void *param1) {
    UnkStruct_ov109_021D5140 *v0 = (UnkStruct_ov109_021D5140 *)param1;
    UnkStruct_ov109_021D5140_sub1 *v1 = &v0->unk_1C;

    if (v1->unk_00) {
        if (v1->unk_08 > ov85_021EA8EA[v1->unk_04][0]) {
            u16 *v2;

            v1->unk_08 = 0;
            v1->unk_04++;

            if (ov85_021EA8EA[v1->unk_04][1] == 0xff) {
                v1->unk_04 = 0;
            }

            v2 = (u16 *)v1->unk_10->pRawData;
            GX_LoadOBJPltt(&v2[16 * ov85_021EA8EA[v1->unk_04][1]], 0, 32);
        } else {
            v0->unk_1C.unk_08++;
        }

        ov85_021E9E58(&v0->unk_418);
    }
}

void ov85_021E8C14(void *param0) {
    GF_RunVramTransferTasks();
    OamManager_ApplyAndResetBuffers();
    DoScheduledBgGpuUpdates((BgConfig *)param0);

    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}
