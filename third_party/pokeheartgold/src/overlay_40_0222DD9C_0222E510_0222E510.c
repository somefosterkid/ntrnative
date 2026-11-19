#include "global.h"
#include "sys_task.h"
#include "overlay_39_unk_struct_ov62_022349_a8.h"
#include "overlay_40_unk_struct_ov62_02233310.h"
#include "overlay_40_unk_struct_ov62_0223118c.h"
#include "overlay_40_unk_struct_0208_c06_c.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "palette.h"
#include "struct_defs/resdat_narc_structs.h"

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

struct String;

struct MessageFormat;

struct SysTask;

struct SysTaskManager;

struct TouchHitboxController;

struct UnkStruct_0202F41C_t;

struct UnkStruct_02030A80_t;

struct UnkStruct_020F3DCC_t;

typedef struct ManagedSprite ManagedSprite;

typedef struct SpriteSystem SpriteSystem;

typedef struct SpriteManager SpriteManager;

typedef struct ManagedSpriteTemplate {
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
} ManagedSpriteTemplate;

u8 SpriteSystem_LoadPaletteBuffer(PaletteData *paletteData, enum PaletteBufferId bufferID, SpriteSystem *spriteSys, SpriteManager *spriteMan, enum NarcId narcID, int memberIdx, BOOL compressed, int paletteIdx, enum NNS_G2D_VRAM_TYPE vramType, int resourceID);

BOOL SpriteSystem_LoadCellResObj(SpriteSystem *spriteSys, SpriteManager *spriteMan, enum NarcId narcID, int memberIdx, BOOL compressed, int resourceID);

BOOL SpriteSystem_LoadAnimResObj(SpriteSystem *spriteSys, SpriteManager *spriteMan, enum NarcId narcID, int memberIdx, BOOL compressed, int resourceID);

ManagedSprite *SpriteSystem_NewSprite(SpriteSystem *spriteSys, SpriteManager *spriteMan, const ManagedSpriteTemplate *template);

void ManagedSprite_SetAnim(ManagedSprite *managedSprite, u32 animID);

void ManagedSprite_SetPaletteOverrideOffset(ManagedSprite *managedSprite, int paletteOffset);

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

typedef struct String String;

typedef struct MailMessage {
    u16 type;
    u16 id;
    u16 words[2];
} MailMessage;

typedef struct SaveData SaveData;

typedef struct MsgData MsgData;

typedef struct PokepicManager PokepicManager;

typedef struct MessageFormat MessageFormat;

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

u32 GetMonIconNaixEx(u32 species, u32 isEgg, u32 form);

const u8 GetMonIconPaletteEx(u32 species, u32 form, u32 isEgg);

u32 sub_02074490(void);

u32 sub_0207449C(void);

u32 sub_020744A8(void);

u16 sub_020315E0(const UnkStruct_02030A80 *param0);

int sub_020315F0(const UnkStruct_02030A80 *param0);

int sub_02031610(const UnkStruct_02030A80 *param0);

void ov40_0222E510(UnkStruct_ov62_022323B8 *param0, UnkStruct_0208C06C *param1);

void ov40_0222E510(UnkStruct_ov62_022323B8 *param0, UnkStruct_0208C06C *param1) {
    int v0;
    int v1;
    int v2;
    int v3;
    u8 v4;
    SpriteSystem *v5;
    SpriteManager *v6;
    PaletteData *v7;
    ManagedSpriteTemplate v8;

    v5 = param1->unk_14.unk_04;
    v6 = param1->unk_14.unk_08;
    v7 = param1->unk_14.unk_14;

    v4 = SpriteSystem_LoadPaletteBuffer(v7, PLTTBUF_MAIN_OBJ, v5, v6, (enum NarcId)20, sub_02074490(), 0, 3, NNS_G2D_VRAM_TYPE_2DMAIN, 100000);

    {
        int v9 = sub_0207449C();
        SpriteSystem_LoadCellResObj(v5, v6, (enum NarcId)20, v9, 0, 100000);
    }

    {
        int v10 = sub_020744A8();
        SpriteSystem_LoadAnimResObj(v5, v6, (enum NarcId)20, v10, 0, 100000);
    }

    {
        int v11;

        v1 = sub_020315E0(param0->unk_00);
        v3 = sub_020315F0(param0->unk_00);
        v0 = sub_02031610(param0->unk_00);

        if (v1 == 0) {
            return;
        }

        v11 = GetMonIconNaixEx(v1, v0, v3);

        SpriteSystem_LoadCharResObjAtEndWithHardwareMappingType(v5, v6, (enum NarcId)20, v11, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 100000);

        v8.x = 72 + 4;
        v8.y = 64 - 4;
        v8.z = 0;
        v8.animIdx = 0;
        v8.priority = 0;
        v8.plttIdx = 0;
        v8.vramType = NNS_G2D_VRAM_TYPE_2DMAIN;
        v8.bgPriority = 0;
        v8.vramTransfer = 0;
        v8.resources[0] = 100000;
        v8.resources[1] = 100000;
        v8.resources[2] = 100000;
        v8.resources[3] = 100000;
        v8.resources[4] = -1;
        v8.resources[5] = -1;

        param0->unk_04 = SpriteSystem_NewSprite(param1->unk_14.unk_04, param1->unk_14.unk_08, &v8);
        v2 = GetMonIconPaletteEx(v1, v3, v0);

        ManagedSprite_SetPaletteOverrideOffset(param0->unk_04, v4 + v2);
        ManagedSprite_SetAnim(param0->unk_04, 1);
    }
}
