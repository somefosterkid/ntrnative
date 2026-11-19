#include "global.h"
#include "sprite_resources_header.h"
#include "field_system_def.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "options.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "overlay_85.h"
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

void Sprite_SetDrawFlag(Sprite *sprite, BOOL draw);

void Sprite_SetAnimActiveFlag(Sprite *sprite, BOOL animate);

typedef struct JournalEntry JournalEntry;

typedef struct ListMenu2D ListMenu2D;

extern const u16 ov85_021EA914[6][2];

void ov85_021E9084(UnkStruct_ov109_021D5140 *param0);

s32 ov85_021E9244(void *, String *, s32);
s32 ov85_021E9EC0(void *, s32, s32, void *);

void ov85_021E9084(UnkStruct_ov109_021D5140 *param0) {
    int v0;

    CreateSpriteResourcesHeader(&param0->unk_278, 2, 2, 2, 2, 0xffffffff, 0xffffffff, 0, 1, param0->unk_1F0[0], param0->unk_1F0[1], param0->unk_1F0[2], param0->unk_1F0[3], NULL, NULL);

    {
        SpriteTemplate v1;

        v1.spriteList = param0->unk_60;
        v1.header = &param0->unk_278;
        v1.position.z = 0;
        v1.scale.x = FX32_ONE;
        v1.scale.y = FX32_ONE;
        v1.scale.z = FX32_ONE;
        v1.rotation = 0;
        v1.drawPriority = 1;
        v1.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
        v1.heapID = HEAP_ID_102;

        for (v0 = 0; v0 < 5; v0++) {
            v1.position.x = FX32_ONE * ov85_021EA914[v0 + 1][0];
            v1.position.y = FX32_ONE * ov85_021EA914[v0 + 1][1];

            param0->unk_29C[v0 + 1] = Sprite_CreateAffine(&v1);

            Sprite_SetAnimActiveFlag(param0->unk_29C[v0 + 1], 1);
            Sprite_SetAnimCtrlSeq(param0->unk_29C[v0 + 1], 27 + (v0 - 1) * 2);
            Sprite_SetDrawFlag(param0->unk_29C[v0 + 1], FALSE);
        }

        for (v0 = 0; v0 < 5; v0++) {
            v1.position.x = FX32_ONE * 24;
            v1.position.y = FX32_ONE * (32 + 32 * v0) + (256 * FX32_ONE);
        }
    }
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, 1);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, 1);
}

void ov85_021E9160(UnkStruct_ov109_021D5140 *arg0) {
    AddWindowParameterized(arg0->unk_14, &arg0->unk_36C, 5U, 0x1AU, 0x14U, 6U, 2U, 0xDU, 0x1C3U);
    FillWindowPixelBuffer(&arg0->unk_36C, 0U);
    AddWindowParameterized(arg0->unk_14, &arg0->unk_35C, 0U, 2U, 0x13U, 0x1BU, 4U, 0xDU, 0x28U);
    FillWindowPixelBuffer(&arg0->unk_35C, 0xFU);
    AddWindowParameterized(arg0->unk_14, &arg0->unk_37C, 0U, 3U, 1U, 0x1AU, 2U, 0xFU, 0x94U);
    ov85_021E9244(&arg0->unk_37C, arg0->unk_58, 0);
    AddWindowParameterized(arg0->unk_14, arg0->unk_30C, 0U, 2U, 6U, 0x10U, 0xBU, 0xFU, 0xC8U);
    FillWindowPixelBuffer(arg0->unk_30C, 0U);
    ov85_021E9EC0(arg0->unk_30C, 0, 0x10300, arg0);
}
