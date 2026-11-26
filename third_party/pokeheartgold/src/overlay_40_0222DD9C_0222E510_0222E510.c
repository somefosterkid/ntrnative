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
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02013534.h"
#include "pm_string.h"
#include "mail_message_types.h"
#include "save.h"
#include "msgdata.h"
#include "pokepic.h"
#include "touch_hitbox_controller.h"
#include "overlay_40_unk_struct_ov62_02241204.h"

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

struct SysTaskQueue;

struct TouchHitboxController;

struct UnkStruct_0202F41C_t;

struct UnkStruct_02030A80_t;

struct UnkStruct_020F3DCC_t;


BOOL SpriteSystem_LoadCellResObj(SpriteSystem *spriteSys, SpriteManager *spriteMan, enum NarcId narcID, int memberIdx, BOOL compressed, int resourceID);

BOOL SpriteSystem_LoadAnimResObj(SpriteSystem *spriteSys, SpriteManager *spriteMan, enum NarcId narcID, int memberIdx, BOOL compressed, int resourceID);

ManagedSprite *SpriteSystem_NewSprite(SpriteSystem *spriteSys, SpriteManager *spriteMan, const ManagedSpriteTemplate *template);




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

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

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
        v8.animation = 0;
        v8.drawPriority = 0;
        v8.pal = 0;
        v8.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
        v8.bgPriority = 0;
        v8.vramTransfer = 0;
        v8.resIdList[0] = 100000;
        v8.resIdList[1] = 100000;
        v8.resIdList[2] = 100000;
        v8.resIdList[3] = 100000;
        v8.resIdList[4] = -1;
        v8.resIdList[5] = -1;

        param0->unk_04 = SpriteSystem_NewSprite(param1->unk_14.unk_04, param1->unk_14.unk_08, &v8);
        v2 = GetMonIconPaletteEx(v1, v3, v0);

        ManagedSprite_SetPaletteOverrideOffset(param0->unk_04, v4 + v2);
        ManagedSprite_SetAnim(param0->unk_04, 1);
    }
}
