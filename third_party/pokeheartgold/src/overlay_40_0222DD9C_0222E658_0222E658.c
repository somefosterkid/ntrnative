#include "global.h"

#include "error_handling.h"
#include "sys_task.h"
#include "overlay_39_unk_struct_ov62_022349_a8.h"
#include "overlay_40_unk_struct_ov62_02233310.h"
#include "overlay_40_unk_struct_ov62_0223118c.h"
#include "overlay_40_unk_struct_0208_c06_c.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "palette.h"
#include "struct_defs/resdat_narc_structs.h"

struct String;

struct MessageFormat;

typedef struct String String;

typedef struct MessageFormat MessageFormat;

extern const int ov40_02244FF0[16][4];

int ov40_0222E658(int param0, int param1);

int ov40_0222E658(int param0, int param1) {
    int v0 = param0;

    if ((param0 > 16) || (param0 < 0)) {
        GF_ASSERT(FALSE);
        v0 = 0;
    }

    if (param1 == 4) {
        return 316 + v0;
    }

    if (param1 >= 5) {
        return 0;
    }

    return ov40_02244FF0[v0][param1];
}

struct BgConfig;

struct TextOBJ;

struct G3DPipelineBuffers;

struct ManagedSprite;

struct MsgData;

struct PaletteData;

struct PokepicManager;

struct SaveData;

struct SpriteManager;

struct SpriteSystem;

struct SysTask;

struct SysTaskManager;

struct TouchHitboxController;

struct UnkStruct_0202F41C_t;

struct UnkStruct_02030A80_t;

struct UnkStruct_020F3DCC_t;

typedef struct ManagedSprite ManagedSprite;

typedef struct SpriteSystem SpriteSystem;

typedef struct SpriteManager SpriteManager;

typedef struct SpriteTemplate {
    s16 x;
    s16 y;
    s16 z;
    u16 animIdx;
    int priority;
    int plttIdx;
    enum NNS_G2D_VRAM_TYPE vramType;
    int resources[GF_GFX_RES_TYPE_MAX];
    int bgPriority;
    BOOL vramTransfer;
} SpriteTemplate;

u8 SpriteSystem_LoadPaletteBuffer(PaletteData *paletteData, enum PaletteBufferId bufferID, SpriteSystem *spriteSys, SpriteManager *spriteMan, enum NarcId narcID, int memberIdx, BOOL compressed, int paletteIdx, enum NNS_G2D_VRAM_TYPE vramType, int resourceID);

BOOL SpriteSystem_LoadCellResObj(SpriteSystem *spriteSys, SpriteManager *spriteMan, enum NarcId narcID, int memberIdx, BOOL compressed, int resourceID);

BOOL SpriteSystem_LoadAnimResObj(SpriteSystem *spriteSys, SpriteManager *spriteMan, enum NarcId narcID, int memberIdx, BOOL compressed, int resourceID);

ManagedSprite *SpriteSystem_NewSprite(SpriteSystem *spriteSys, SpriteManager *spriteMan, const SpriteTemplate *template);

void ManagedSprite_TickFrame(ManagedSprite *managedSprite);

void ManagedSprite_SetAnim(ManagedSprite *managedSprite, u32 animID);

BOOL SpriteSystem_LoadCharResObjAtEndWithHardwareMappingType(SpriteSystem *spriteSys, SpriteManager *spriteMan, enum NarcId narcID, int memberIdx, BOOL compressed, enum NNS_G2D_VRAM_TYPE vramType, int resourceID);

typedef struct SysTaskManager SysTaskManager;

struct SysTaskManager {
    u16 maxTasks;
    u16 stackPointer;
    SysTask sentinelTask;
    SysTask **taskStack;
    SysTask *tasks;
    BOOL locked;
    SysTask *currentTask;
    SysTask *nextTask;
};

typedef struct UnkStruct_02030A80_t UnkStruct_02030A80;

typedef struct UnkStruct_020F3DCC_t UnkStruct_020F3DCC;

struct UnkStruct_020F3DCC_t {
    BOOL unk_00;
    BOOL unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    const UnkStruct_020F3DCC *unk_20;
};

typedef struct TextOBJ TextOBJ;

typedef struct MailMessage {
    u16 type;
    u16 id;
    u16 words[2];
} MailMessage;

typedef struct SaveData SaveData;

typedef struct MsgData MsgData;

typedef struct PokepicManager PokepicManager;

typedef struct BgConfig BgConfig;

typedef struct Window {
    BgConfig *bgConfig;
    u8 bgLayer;
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 width;
    u8 height;
    u8 palette;
    u16 baseTile : 15;
    u16 colorMode : 1;
    void *pixels;
} Window;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

typedef struct TouchHitboxController TouchHitboxController;

typedef struct {
    UnkStruct_02030A80 *unk_00;
    ManagedSprite *unk_04;
    ManagedSprite *unk_08;
    Window unk_0C[8];
} UnkStruct_ov62_022323B8;

typedef struct {
    u16 unk_00;
    u16 unk_02;
} UnkStruct_0202F298_sub1;

struct UnkStruct_02030A80_t {
    u16 unk_00[8];
    u32 id;
    u8 gender;
    u8 month;
    u8 appearance;
    u8 country;
    u8 region;
    u8 version;
    u8 language;
    u8 isEgg : 1;
    u8 form : 7;
    u16 species;
    u8 unk_1E;
    u8 unk_1F;
    union {
        MailMessage unk_20_val1;
        u16 unk_20_val2[40];
    };
    u8 unk_70[12];
    UnkStruct_0202F298_sub1 unk_7C;
};

typedef struct BattleRegulation {
    u16 name[12];
    u16 maxTotalLevel;
    u8 teamSize;
    u8 maxLevel;
    s8 heightRestriction;
    s8 weightRestriction;
    u8 evolutionRestriction : 1;
    u8 specialSpeciesRestriction : 1;
    u8 speciesUniqueness : 1;
    u8 itemUniqueness : 1;
    u8 moveRestrictions : 1;
} BattleRegulation;

struct UnkStruct_0202F41C_t {
    u16 unk_00[12];
    u8 unk_18[12];
    u16 unk_24;
    u8 unk_26;
    u8 unk_27;
    BattleRegulation battleRegulation;
    u16 unk_48;
    u8 unk_4A[14];
    u64 unk_58;
    UnkStruct_0202F298_sub1 unk_60;
};

u8 sub_020315D0(const UnkStruct_02030A80 *param0);

u8 sub_02031700(const UnkStruct_02030A80 *param0);

void ov40_0222E690(UnkStruct_ov62_022323B8 *param0, UnkStruct_0208C06C *param1);

void ov40_0222E690(UnkStruct_ov62_022323B8 *param0, UnkStruct_0208C06C *param1) {
    int v0;
    SpriteSystem *v1;
    SpriteManager *v2;
    PaletteData *v3;
    SpriteTemplate v4;
    int v5 = 13;
    int v6 = 14;
    int v7 = 9;
    int v8 = 10;

    v0 = sub_020315D0(param0->unk_00);
    v1 = param1->unk_14.unk_04;
    v2 = param1->unk_14.unk_08;
    v3 = param1->unk_14.unk_14;

    if (param1->unk_86C == 0) {
        if (v0 == 1) {
            v5 = 15;
            v6 = 16;
        }
    } else {
        int v9 = sub_02031700(param0->unk_00);

        if (v9 >= 16) {
            v9 = 0;
        }

        v5 = ov40_0222E658(v9, 2);
        v6 = ov40_0222E658(v9, 3);
    }

    SpriteSystem_LoadPaletteBuffer(v3, PLTTBUF_MAIN_OBJ, v1, v2, (enum NarcId)179, v5, 0, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 165535);
    SpriteSystem_LoadCellResObj(v1, v2, (enum NarcId)179, v7, 0, 165535);
    SpriteSystem_LoadAnimResObj(v1, v2, (enum NarcId)179, v8, 0, 165535);
    SpriteSystem_LoadCharResObjAtEndWithHardwareMappingType(v1, v2, (enum NarcId)179, v6, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 165535);

    v4.x = 40;
    v4.y = 56 + 4;
    v4.z = 0;
    v4.animIdx = 0;
    v4.priority = 0;
    v4.plttIdx = 0;
    v4.vramType = NNS_G2D_VRAM_TYPE_2DMAIN;
    v4.bgPriority = 0;
    v4.vramTransfer = 0;
    v4.resources[0] = 165535;
    v4.resources[1] = 165535;
    v4.resources[2] = 165535;
    v4.resources[3] = 165535;
    v4.resources[4] = -1;
    v4.resources[5] = -1;

    param0->unk_08 = SpriteSystem_NewSprite(param1->unk_14.unk_04, param1->unk_14.unk_08, &v4);
    ManagedSprite_SetAnim(param0->unk_08, 1);
    ManagedSprite_TickFrame(param0->unk_08);
}
