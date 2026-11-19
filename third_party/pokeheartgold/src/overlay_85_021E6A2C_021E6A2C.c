#include "global.h"
#include "field_system_def.h"
#include "simple3dmodel.h"
#include "constants/heap.h"
#include "gf_gfx_planes.h"
#include "unk_struct_0209_bdf8.h"
#include "save.h"
#include "options.h"
#include "pm_string.h"
#include "filesystem.h"
#include "palette.h"
#include "pokemon_types_def.h"
#include "sprite_system.h"
#include "player_data.h"
#include "game_stats.h"
#include "overlay_85_ov109_021D5140.h"
#include "camera.h"
#include "msgdata.h"
#include "simple3danimation.h"
#include "overlay_85_ov109_021D0F70.h"
#include "unk_struct_0209_c194.h"

struct BgConfig;

struct BgTemplate;

struct Camera;

struct CameraAngle;

struct GameStats;

struct GraphicsModes;

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

struct Window;

typedef struct UnkStruct_0205C22C_t UnkStruct_0205C22C;

typedef struct JournalEntry JournalEntry;

void SetBothScreensModesAndDisable(const GraphicsModes *graphicsModes);

BgConfig *BgConfig_Alloc(enum HeapID heapID);

void InitBgFromTemplate(BgConfig *bgConfig, u8 bgLayer, const BgTemplate *bgTemplate, u8 bgType);

void BG_ClearCharDataRange(u8 bgLayer, u32 size, u32 offset, enum HeapID heapID);

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);

void ov85_021E6A2C(UnkStruct_ov109_021D0F70 *param0);

extern const GraphicsModes ov85_021EA538;

extern const BgTemplate ov85_021EA60C;

extern const BgTemplate ov85_021EA628;

extern const BgTemplate ov85_021EA644;

extern const BgTemplate ov85_021EA660;

extern const BgTemplate ov85_021EA67C;

void ov85_021E6A2C(UnkStruct_ov109_021D0F70 *param0) {
    param0->unk_D84 = BgConfig_Alloc(HEAP_ID_102);
    GX_SetDispSelect(GX_DISP_SELECT_MAIN_SUB);

    {
        GraphicsModes v0 = ov85_021EA538;

        SetBothScreensModesAndDisable(&v0);
    }

    {
        BgTemplate v1 = ov85_021EA60C;

        InitBgFromTemplate(param0->unk_D84, GF_BG_LYR_MAIN_1, &v1, 0);
        BgClearTilemapBufferAndCommit(param0->unk_D84, GF_BG_LYR_MAIN_1);
        BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 32, 0, HEAP_ID_102);
    }

    {
        BgTemplate v2 = ov85_021EA628;

        InitBgFromTemplate(param0->unk_D84, GF_BG_LYR_MAIN_2, &v2, 0);
        BgClearTilemapBufferAndCommit(param0->unk_D84, GF_BG_LYR_MAIN_2);
    }

    {
        BgTemplate v3 = ov85_021EA644;

        InitBgFromTemplate(param0->unk_D84, GF_BG_LYR_MAIN_3, &v3, 0);
        BgClearTilemapBufferAndCommit(param0->unk_D84, GF_BG_LYR_MAIN_3);
    }

    {
        BgTemplate v4 = ov85_021EA660;

        InitBgFromTemplate(param0->unk_D84, GF_BG_LYR_SUB_2, &v4, 0);
        BgClearTilemapBufferAndCommit(param0->unk_D84, GF_BG_LYR_SUB_2);
    }

    {
        BgTemplate v5 = ov85_021EA67C;

        InitBgFromTemplate(param0->unk_D84, GF_BG_LYR_SUB_3, &v5, 0);
        BgClearTilemapBufferAndCommit(param0->unk_D84, GF_BG_LYR_SUB_3);
    }
}
