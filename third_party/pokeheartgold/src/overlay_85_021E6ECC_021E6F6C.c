#include "global.h"
#include "field_system_def.h"
#include "simple3dmodel.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "palette.h"
#include "unk_struct_0209_bdf8.h"
#include "save.h"
#include "options.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "sys_task.h"
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

struct Camera;

struct GameStats;

struct JournalEntry;

struct MsgData;

struct NARC;

struct Options;

struct PaletteData;

struct Party;

struct SaveData;

struct SpriteManager;

struct SpriteSystem;

struct String;

struct MessageFormat;

struct SysTask;

struct SysTaskQueue;

struct PlayerProfile;

struct UnkStruct_0205C22C_t;

struct UnkStruct_ov109_021D0F70_t;

typedef struct UnkStruct_0205C22C_t UnkStruct_0205C22C;

void Heap_Free(void *ptr);

NARC *NARC_New(enum NarcId narcID, enum HeapID heapID);

void NARC_Delete(NARC *narc);

void G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE vramType);

BOOL SpriteSystem_LoadCellResObjFromOpenNarc(SpriteSystem *spriteSys, SpriteManager *spriteMan, NARC *narc, int memberIdx, BOOL compressed, int resourceID);

BOOL SpriteSystem_LoadAnimResObjFromOpenNarc(SpriteSystem *spriteSys, SpriteManager *spriteMan, NARC *narc, int memberIdx, BOOL compressed, int resourceID);

typedef struct JournalEntry JournalEntry;

void sub_0203A880(void);

void *sub_0203A4AC(enum HeapID heapID);

void ov85_021E6F6C(UnkStruct_ov109_021D0F70 *param0);

void ov85_021E6F6C(UnkStruct_ov109_021D0F70 *param0) {
    SpriteSystem *v0 = param0->unk_D94;
    SpriteManager *v1 = param0->unk_D98;
    PaletteData *v2 = param0->unk_D9C;
    NARC *v3 = param0->unk_D80;

    G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);

    NARC *v4;

    v4 = NARC_New((enum NarcId)217, HEAP_ID_102);

    SpriteSystem_LoadCharResObjFromOpenNarc(v0, v1, v4, 16, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 4);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(v2, PLTTBUF_MAIN_OBJ, v0, v1, v4, 15, 0, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 5);
    SpriteSystem_LoadCellResObjFromOpenNarc(v0, v1, v4, 17, 0, 6);
    SpriteSystem_LoadAnimResObjFromOpenNarc(v0, v1, v4, 18, 0, 7);
    NARC_Delete(v4);

    sub_0203A880();

    NNSG2dPaletteData *v5;
    void *v6 = sub_0203A4AC(HEAP_ID_102);

    NNS_G2dGetUnpackedPaletteData(v6, &v5);
    PaletteData_LoadPalette(v2, v5->pRawData, PLTTBUF_MAIN_OBJ, ((14) * (16)), ((16) * sizeof(u16)));
    Heap_Free(v6);
}
