#include "global.h"

#include "constants/heap.h"

#include "struct_defs/resdat_narc_structs.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "gf_3d_loader.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "message_format.h"
#include "options.h"
#include "pm_string.h"
#include "save.h"
#include "sprite.h"
#include "touch_hitbox_controller.h"
#include "touchscreen.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "wild_encounter.h"
#include "font.h"
#include "render_window.h"
#include "overlay_74_gbamigrator.h"
#include "yes_no_prompt.h"

typedef struct GBAPokemonSubstruct0 GBAPokemonSubstruct0;

typedef struct GBAPokemonSubstruct1 GBAPokemonSubstruct1;

typedef struct GBAPokemonSubstruct2 GBAPokemonSubstruct2;

typedef struct GBAPokemonSubstruct3 GBAPokemonSubstruct3;

typedef struct GBABoxPokemon GBABoxPokemon;

typedef struct YesNoButtonAnimData YesNoButtonAnimData;

typedef struct YesNoButtonSprite YesNoButtonSprite;

struct GBAPokemonSubstruct0 {
    u16 species;
    u16 heldItem;
    u32 experience;
    u8 ppBonuses;
    u8 friendship;
    u16 padding;
};

struct GBAPokemonSubstruct1 {
    u16 moves[4];
    u8 pp[4];
};

struct GBAPokemonSubstruct2 {
    u8 hpEV;
    u8 attackEV;
    u8 defenseEV;
    u8 speedEV;
    u8 spAttackEV;
    u8 spDefenseEV;
    u8 cool;
    u8 beauty;
    u8 cute;
    u8 smart;
    u8 tough;
    u8 sheen;
};

struct GBAPokemonSubstruct3 {
    u32 pokerus : 8;
    u32 metLocation : 8;
    u32 metLevel : 7;
    u32 metGame : 4;
    u32 pokeball : 4;
    u32 otGender : 1;
    u32 hpIV : 5;
    u32 attackIV : 5;
    u32 defenseIV : 5;
    u32 speedIV : 5;
    u32 spAttackIV : 5;
    u32 spDefenseIV : 5;
    u32 isEgg : 1;
    u32 abilityNum : 1;
    u32 coolRibbon : 3;
    u32 beautyRibbon : 3;
    u32 cuteRibbon : 3;
    u32 smartRibbon : 3;
    u32 toughRibbon : 3;
    u32 championRibbon : 1;
    u32 winningRibbon : 1;
    u32 victoryRibbon : 1;
    u32 artistRibbon : 1;
    u32 effortRibbon : 1;
    u32 marineRibbon : 1;
    u32 landRibbon : 1;
    u32 skyRibbon : 1;
    u32 countryRibbon : 1;
    u32 nationalRibbon : 1;
    u32 earthRibbon : 1;
    u32 worldRibbon : 1;
    u32 unusedRibbons : 4;

    u32 modernFatefulEncounter : 1;
};

union GBAPokemonSubstruct {
    GBAPokemonSubstruct0 type0;
    GBAPokemonSubstruct1 type1;
    GBAPokemonSubstruct2 type2;
    GBAPokemonSubstruct3 type3;
    u16 raw[(((sizeof(GBAPokemonSubstruct0)) > (sizeof(GBAPokemonSubstruct1)) ? (sizeof(GBAPokemonSubstruct0)) : (sizeof(GBAPokemonSubstruct1))) > ((sizeof(GBAPokemonSubstruct2)) > (sizeof(GBAPokemonSubstruct3)) ? (sizeof(GBAPokemonSubstruct2)) : (sizeof(GBAPokemonSubstruct3))) ? ((sizeof(GBAPokemonSubstruct0)) > (sizeof(GBAPokemonSubstruct1)) ? (sizeof(GBAPokemonSubstruct0)) : (sizeof(GBAPokemonSubstruct1))) : ((sizeof(GBAPokemonSubstruct2)) > (sizeof(GBAPokemonSubstruct3)) ? (sizeof(GBAPokemonSubstruct2)) : (sizeof(GBAPokemonSubstruct3)))) / sizeof(u16)];
};

struct GBABoxPokemon {
    u32 personality;
    u32 otId;
    u8 nickname[10];
    u8 language;
    u8 isBadEgg : 1;
    u8 hasSpecies : 1;
    u8 isEgg : 1;
    u8 unused : 5;
    u8 otName[7];
    u8 markings;
    u16 checksum;
    u16 unknown;
    union {
        u32 raw[12];
        union GBAPokemonSubstruct substructs[4];
    } secure;
};

struct YesNoButtonAnimData {
    const fx32 *frameThresholds;
    u32 frameCount;
    fx32 frameProgress;
};

struct YesNoButtonSprite {
    BgConfig *bgConfig;
    enum GFBgLayer bgLayer;
    void *nscrBuffers[4];
    NNSG2dScreenData *screenData[4];
    YesNoButtonAnimData animData;
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 animFrame;
};

extern u8 ov74_0223C960;

void ov74_02232AC8(GBAMigrator *migrator);

void ov74_02232B18(u8 *param_1);

void ov74_02232AC8(GBAMigrator *migrator) {
    GfGfxLoader_LoadScrnData(NARC_a_1_1_3, 0x18, migrator->bgConfig, GF_BG_LYR_MAIN_2, 0, 0x600, 1, HEAP_ID_76);
    BgTilemapRectChangePalette(migrator->bgConfig, 2, 0, 0, 0x20, 0x18, (&ov74_0223C960)[migrator->gbaVersion]);
    BgCommitTilemapBufferToVram(migrator->bgConfig, 2);
    return;
}

void ov74_02232B18(u8 *param_1) {
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_14_OFFSET, HEAP_ID_76);
    LoadUserFrameGfx1(*(BgConfig **)(param_1 + 0x20), GF_BG_LYR_MAIN_0, 0x3f0, 0xe, 0, HEAP_ID_76);
    LoadUserFrameGfx2(*(BgConfig **)(param_1 + 0x20), GF_BG_LYR_MAIN_0, 0x3d2, 0xd, (u8) * (u32 *)(param_1 + 0x1c), HEAP_ID_76);
    memset((void *)(param_1 + 0x42c), 0, 0x4c);
    *((void **)&((GBAMigrator *)param_1)->unk_490.unk_00) = param_1 + 0x498;
    *((u32 *)&((GBAMigrator *)param_1)->unk_490.unk_08) = 2;
    *((u32 *)&((GBAMigrator *)param_1)->unk_490.unk_0C) = 1;
    *((u32 *)&((GBAMigrator *)param_1)->unk_490.unk_10) = 0x1b;
    *((u32 *)&((GBAMigrator *)param_1)->unk_490.unk_14) = 4;
    *((u32 *)&((GBAMigrator *)param_1)->unk_490.bgLayer) = 0;
    *((u32 *)&((GBAMigrator *)param_1)->unk_490.unk_28) = 1;
    *((u32 *)&((GBAMigrator *)param_1)->unk_490.unk_20) = 1;
    ((GBAMigrator *)param_1)->unk_490.unk_2C = 0x1020f;
    ((GBAMigrator *)param_1)->unk_490.unk_30 = 0xf020f;
    *((u32 *)&((GBAMigrator *)param_1)->unk_490.unk_38) = 0;
    *((u32 *)&((GBAMigrator *)param_1)->unk_490.unk_3C) = 0;
    return;
}
