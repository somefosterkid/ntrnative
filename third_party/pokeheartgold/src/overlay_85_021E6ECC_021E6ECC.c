#include "global.h"
#include "field_system_def.h"
#include "simple3dmodel.h"
#include "unk_struct_0209_c194.h"
#include "overlay_85_ov109_021D0F70.h"
#include "options.h"
#include "player_data.h"
#include "game_stats.h"
#include "overlay_85_ov109_021D5140.h"
#include "msgdata.h"
#include "struct_defs/resdat_narc_structs.h"

struct JournalEntry;

struct SpriteManager;

struct SpriteSystem;

SpriteSystem *SpriteSystem_Alloc(enum HeapID heapID);

SpriteManager *SpriteManager_New(SpriteSystem *spriteSys);

BOOL SpriteSystem_Init(SpriteSystem *spriteSys, const OamManagerParam *oamTemplate, const OamCharTransferParam *transferTemplate, int plttCapacity);

BOOL SpriteSystem_InitSprites(SpriteSystem *spriteSys, SpriteManager *spriteMan, int maxSprites);

BOOL SpriteSystem_InitManagerWithCapacities(SpriteSystem *spriteSys, SpriteManager *spriteMan, const SpriteResourceCountsListUnion *capacities);

typedef struct JournalEntry JournalEntry;

void ov85_021E6ECC(UnkStruct_ov109_021D0F70 *param0);

extern const OamManagerParam ov85_021EA6B8;

extern const OamCharTransferParam ov85_021EA56C;

extern const SpriteResourceCountsListUnion ov85_021EA594;

void ov85_021E6ECC(UnkStruct_ov109_021D0F70 *param0) {
    {
        const OamManagerParam v0 = ov85_021EA6B8;
        const OamCharTransferParam v1 = ov85_021EA56C;

        param0->unk_D94 = SpriteSystem_Alloc(HEAP_ID_102);
        SpriteSystem_Init(param0->unk_D94, &v0, &v1, 16 + 16);
    }

    {
        BOOL v2;
        const SpriteResourceCountsListUnion v3 = ov85_021EA594;

        param0->unk_D98 = SpriteManager_New(param0->unk_D94);
        v2 = SpriteSystem_InitSprites(param0->unk_D94, param0->unk_D98, 255);
        GF_ASSERT(v2);
        v2 = SpriteSystem_InitManagerWithCapacities(param0->unk_D94, param0->unk_D98, &v3);
        GF_ASSERT(v2);
    }
}
