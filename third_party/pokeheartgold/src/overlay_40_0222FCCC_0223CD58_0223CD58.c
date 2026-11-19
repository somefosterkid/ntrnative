#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "font_types_def.h"
#include "obj_char_transfer.h"
#include "overlay_40.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "sprite.h"
#include "sprite_system.h"
#include "text.h"
#include "touch_hitbox_controller.h"
#include "touchscreen.h"
#include "unk_0202FBCC.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_39_unk_struct_ov62_022349_a8.h"
#include "overlay_40_unk_struct_ov62_02233310.h"
#include "overlay_40_unk_struct_ov62_0223118c.h"
#include "overlay_40_unk_struct_0208_c06_c.h"
#include "overlay_40_unk_struct_ov62_022312b0.h"

struct FontOAM;

struct G3DPipelineBuffers;

struct MessageLoader;

struct PokemonSpriteManager;

struct StringTemplate;

struct TouchScreenActions;

struct UnkStruct_02012744_t;

struct UnkStruct_02012B20_t;

struct UnkStruct_0202F41C_t;

struct UnkStruct_020F3DCC_t;

typedef struct UnkStruct_0202F41C_t UnkStruct_0202F41C;

typedef struct {
    u16 unk_00;
    u16 unk_02;
} UnkStruct_0202F298_sub1;

typedef struct UnkStruct_02030A80_t {
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
} UnkStruct_02030A80;

typedef struct {
    int unk_00;
    enum HeapID heapID;
    int unk_08;
    int unk_0C;
    fx32 unk_10;
} UnkStruct_0208B284;

typedef struct {
    NARC *unk_00;
    SpriteSystem *unk_04;
    SpriteManager *unk_08;
    PaletteData *unk_0C;
} UnkStruct_0208B878_sub1;

typedef struct {
    ManagedSprite *unk_00;
    s16 unk_04;
    s16 unk_06;
    int unk_08;
    int unk_0C;
} UnkStruct_0208B878_sub2;

typedef struct {
    UnkStruct_0208B878_sub1 unk_00;
    UnkStruct_0208B284 unk_10;
    UnkStruct_0208B878_sub2 unk_24[12];
    int unk_E4;
    int unk_E8;
    int unk_EC;
    BOOL unk_F0;
    SysTask *unk_F4;
    int unk_F8;
    int unk_FC;
    int unk_100;
    int unk_104;
} UnkStruct_0208B878;

typedef struct {
    BOOL unk_00;
    int unk_04;
    u8 padding_08[8];
} UnkStruct_0208BA84;

typedef struct {
    BOOL unk_00;
    int unk_04;
    int unk_08[8];
} UnkStruct_0208C06C_sub1;

typedef struct {
    BOOL unk_00;
    int unk_04;
    UnkStruct_0208B878 *unk_08;
    SysTask *unk_0C;
} UnkStruct_0208C06C_sub2;

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

typedef struct UnkStruct_02012B20_t UnkStruct_02012B20;

typedef struct {
    ManagedSprite *unk_00;
    u8 padding_04[8];
    BOOL unk_0C;
    TouchscreenHitbox *unk_10;
    FontOAM *unk_14;
    UnkStruct_02012B20 *unk_18;
    UnkStruct_02021AC8 unk_1C;
} UnkStruct_ov62_022307C0_sub1;

typedef struct {
    UnkStruct_ov62_022307C0_sub1 unk_00[5];
    UnkStruct_ov62_022307C0_sub1 unk_C8[5];
    const UnkStruct_020F3DCC *unk_190[5];
    int unk_1A4;
    u8 padding_1A8[4];
    int unk_1AC;
    int unk_1B0;
    u8 padding_1B4[8];
} UnkStruct_ov62_022307C0;

typedef struct {
    ManagedSprite *unk_00[2];
    SysTask *unk_08;
    fx32 unk_0C;
    f32 unk_10;
    s16 unk_14;
    s16 unk_16;
    s8 unk_18;
    u8 unk_19;
    u8 padding_1A[2];
    int unk_1C;
    int unk_20;
    int unk_24;
    int unk_28;
    int unk_2C;
} UnkStruct_ov62_02230C28;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
} UnkStruct_ov61_0222B138;

typedef struct {
    u8 unk_00;
    u8 unk_01[3];
    u32 unk_04;
} UnkStruct_ov61_0222B2D8;

typedef struct {
    BOOL unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
} UnkStruct_ov62_0223CAA4;

typedef void (*UnkFuncPtr_ov61_0222B920)(void *, const UnkStruct_ov62_0223CAA4 *);

typedef BOOL (*UnkFuncPtr_ov61_0222B920_1)(void *, void *);

typedef struct {
    UnkFuncPtr_ov61_0222B920 unk_00;
    u16 unk_04;
    u16 unk_06;
    u8 unk_08;
    UnkFuncPtr_ov61_0222B920_1 unk_0C;
} UnkStruct_ov61_0222B920;

typedef void (*UnkFuncPtr_ov62_022349A8)(void *, String *);

typedef struct {
    void *unk_00;
    UnkFuncPtr_ov61_0222B920 unk_04;
    UnkFuncPtr_ov61_0222B920 unk_08;
    UnkFuncPtr_ov61_0222B920 unk_0C;
    UnkFuncPtr_ov61_0222B920 unk_10;
    UnkFuncPtr_ov61_0222B920 unk_14;
    UnkFuncPtr_ov61_0222B920 unk_18;
    UnkFuncPtr_ov61_0222B920 unk_1C;
    UnkFuncPtr_ov61_0222B920 unk_20;
    UnkFuncPtr_ov61_0222B920 unk_24;
    UnkFuncPtr_ov61_0222B920 unk_28;
} UnkStruct_ov62_022349A8_sub2;

typedef struct {
    u32 personality;
    u32 otID;
    u16 species;
    u16 trainerName[7 + 1];
    s8 priority;
    u8 xPos;
    u8 yPos;
    u8 form;
} UnkStruct_ov61_0222AE80_sub1;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    s8 unk_03;
} UnkStruct_ov61_0222AE80_sub2;

typedef struct {
    u16 unk_00;
    u16 unk_02;
} UnkStruct_ov62_02239DA4_sub1;

typedef struct {
    u32 integrity;
    UnkStruct_ov61_0222AE80_sub1 unk_04;
    u32 unk_24;
    MailMessage title;
    UnkStruct_ov61_0222AE80_sub2 unk_30[10];
    u8 unk_58;
    u8 language;
    u8 unk_5A[2];
    UnkStruct_ov62_02239DA4_sub1 unk_5C;
} UnkStruct_ov61_0222AE80;

typedef struct {
    u16 unk_00[8];
    u32 unk_10;
    u8 unk_14;
    u8 unk_15;
    u8 unk_16;
    u8 unk_17;
    u8 unk_18;
    u8 unk_19;
    u8 unk_1A;
    u8 unk_1B_0 : 1;
    u8 unk_1B_1 : 7;
    u16 unk_1C;
    u8 unk_1E;
    u8 unk_1F;
    union {
        MailMessage unk_20_val1;
        u16 unk_20_val2[40];
    };
    u8 unk_70[12];
    UnkStruct_ov62_02239DA4_sub1 unk_7C;
} UnkStruct_ov61_0222AFC0;

typedef struct {
    UnkStruct_ov61_0222AFC0 unk_00;
    UnkStruct_ov61_0222AE80 unk_80;
} UnkStruct_ov62_022349A8_sub3_sub1;

typedef struct {
    u16 unk_00[20];
    u16 unk_28[30];
    u32 unk_64[30];
    u32 unk_DC[30];
    u32 unk_154;
    u8 unk_158[30];
    u8 unk_176;
    u8 unk_177;
    u8 unk_178[32];
    UnkStruct_ov62_02239DA4_sub1 unk_198;
} UnkStruct_ov62_02239DA4;

typedef struct {
    UnkStruct_ov61_0222AFC0 unk_00;
    UnkStruct_ov62_02239DA4 unk_80;
} UnkStruct_ov62_022349A8_sub3_sub2;

typedef struct {
    u16 unk_00[12];
    u16 unk_18;
    u8 unk_1A;
    u8 unk_1B;
    s8 unk_1C;
    s8 unk_1D;
    u8 unk_1E_0 : 1;
    u8 unk_1E_1 : 1;
    u8 unk_1E_2 : 1;
    u8 unk_1E_3 : 1;
    u8 unk_1E_4 : 1;
    u8 : 3;
    u8 unk_1F;
} UnkStruct_ov61_0222BED8_sub1_sub1;

typedef struct {
    u16 unk_00[12];
    u8 unk_18[12];
    u16 unk_24;
    u8 unk_26;
    u8 unk_27;
    UnkStruct_ov61_0222BED8_sub1_sub1 unk_28;
    u16 unk_48;
    u8 unk_4A[14];
    u64 unk_58;
    UnkStruct_ov62_02239DA4_sub1 unk_60;
} UnkStruct_ov61_0222BED8_sub1;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
    u16 unk_04[4];
    u32 unk_0C;
    u32 unk_10;
    u16 unk_14[8];
    MailMessage unk_24;
    MailMessage unk_2C;
} UnkStruct_ov61_0222BED8_sub2_sub1_sub1;

typedef struct {
    u32 unk_00;
    int unk_04;
    int unk_08[4];
    UnkStruct_ov61_0222BED8_sub2_sub1_sub1 unk_18[4];
    int unk_E8;
    int unk_EC;
    int unk_F0;
    int unk_F4;
    int unk_F8;
    int unk_FC;
    int unk_100;
    int unk_104;
    int unk_108;
    int unk_10C;
    int unk_110;
    u32 unk_114[4];
    u32 unk_124;
    int unk_128;
    u32 unk_12C;
    u32 unk_130;
    int unk_134[4];
    u16 unk_144;
    u16 unk_146;
    int unk_148;
    u8 unk_14C[4];
} UnkStruct_ov61_0222BED8_sub2_sub1;

typedef struct {
    u8 unk_00[4][1024];
} UnkStruct_ov61_0222BED8_sub2_sub2;

typedef struct {
    u32 unk_00;
    u16 unk_04_0 : 1;
    u16 unk_04_1 : 1;
    u16 unk_04_2 : 1;
    u16 unk_04_3 : 13;
    u16 unk_06;
    u16 unk_08;
    u32 unk_0C;
    u32 unk_10;
    u8 unk_14;
    u8 unk_15;
    u8 unk_16;
    u8 unk_17;
    u8 unk_18;
    u8 unk_19;
    u8 unk_1A;
    u8 unk_1B;
    u16 unk_1C[4];
    u8 unk_24[4];
    u8 unk_28[4];
    u32 unk_2C_0 : 5;
    u32 unk_2C_5 : 5;
    u32 unk_2C_10 : 5;
    u32 unk_2C_15 : 5;
    u32 unk_2C_20 : 5;
    u32 unk_2C_25 : 5;
    u32 unk_2C_30 : 1;
    u32 unk_2C_31 : 1;
    u8 unk_30_0 : 1;
    u8 unk_30_1 : 2;
    u8 unk_30_3 : 5;
    u16 unk_32[11];
    u16 unk_48[8];
    u8 unk_58;
    u8 unk_59;
    u32 unk_5C;
    u8 unk_60;
    u8 unk_61;
    u16 unk_62;
    u16 unk_64;
    u16 unk_66;
    u16 unk_68;
    u16 unk_6A;
    u16 unk_6C;
    u16 unk_6E;
} UnkStruct_ov61_0222BED8_sub2_sub3_sub1;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    UnkStruct_ov61_0222BED8_sub2_sub3_sub1 unk_04[6];
} UnkStruct_ov61_0222BED8_sub2_sub3;

typedef struct {
    u16 unk_00[8];
    u32 unk_10;
    u32 unk_14;
    u8 unk_18;
    u8 unk_19;
    u8 unk_1A;
    u8 unk_1B;
    u8 unk_1C;
    u8 unk_1D_0 : 1;
    u8 unk_1D_1 : 1;
    u8 unk_1D_2 : 6;
    u8 unk_1E;
    u8 unk_1F;
} UnkStruct_ov61_0222BED8_sub2_sub4;

typedef struct {
    u16 unk_00_0 : 4;
    u16 unk_00_4 : 2;
    u16 unk_00_6 : 1;
    u16 unk_00_7 : 1;
    u16 unk_00_8 : 2;
    u16 unk_00_10 : 5;
    u16 unk_00_15 : 1;
} UnkStruct_ov61_0222BED8_sub2_sub5;

typedef struct {
    UnkStruct_ov61_0222BED8_sub2_sub1 unk_00;
    UnkStruct_ov61_0222BED8_sub2_sub2 unk_150;
    UnkStruct_ov61_0222BED8_sub2_sub3 unk_1150[4];
    UnkStruct_ov61_0222BED8_sub2_sub4 unk_1B68[4];
    UnkStruct_ov61_0222BED8_sub2_sub5 unk_1BE8;
    u16 unk_1BEA;
    UnkStruct_ov62_02239DA4_sub1 unk_1BEC;
} UnkStruct_ov61_0222BED8_sub2;

typedef struct {
    UnkStruct_ov61_0222AFC0 unk_00;
    UnkStruct_ov61_0222BED8_sub1 unk_80;
    UnkStruct_ov61_0222BED8_sub2 unk_E4;
} UnkStruct_ov62_022349A8_sub3_sub3;

typedef struct {
    u16 unk_00;
    u8 unk_02;
    u8 unk_03;
    u8 unk_04;
    u8 unk_05[3];
    u32 unk_08;
    u8 unk_0C[4];
} UnkStruct_ov62_022349A8_sub3_sub4;

typedef struct {
    u32 unk_00;
} UnkStruct_ov62_022349A8_sub3_sub5;

typedef struct {
    u8 version;
    u8 language;
    u8 month;
    u8 appearance;
    u16 species;
    u8 isEgg : 1;
    u8 form : 7;
    u8 unk_07;
} UnkStruct_ov61_0222AFCC;

typedef struct {
    u16 unk_00;
    u8 unk_02;
    u8 unk_03;
} UnkStruct_ov62_0223D518_sub1_sub1;

typedef struct {
    UnkStruct_ov61_0222AFCC unk_00;
    UnkStruct_ov62_0223D518_sub1_sub1 unk_08;
    UnkStruct_ov61_0222B2D8 unk_0C[3];
} UnkStruct_ov62_0223D518_sub1;

typedef struct {
    union {
        UnkStruct_ov62_022349A8_sub3_sub1 unk_00_val1;
        UnkStruct_ov62_022349A8_sub3_sub2 unk_00_val2;
        UnkStruct_ov62_0223D518_sub1 unk_00_val3;
        UnkStruct_ov62_022349A8_sub3_sub3 *unk_00_val4;
        UnkStruct_ov62_022349A8_sub3_sub4 unk_00_val5;
        UnkStruct_ov62_022349A8_sub3_sub5 unk_00_val6;
    };
    union {
        struct {
            u8 unk_00;
            u8 unk_01;
            u8 unk_02[2];
        } val1;
        struct {
            u16 unk_00;
            u16 unk_02;
        } val2;
        struct {
            u64 unk_00;
        } val3;
    } unk_21C;
} UnkStruct_ov62_022349A8_sub3;

typedef struct {
    s32 unk_00;
    u8 unk_04;
    u8 language;
    char unk_06[301];
    u8 padding[5];
} UnkStruct_ov62_022349A8_sub1;

typedef struct MessageLoader MessageLoader;


typedef struct PokemonSpriteManager PokemonSpriteManager;

typedef struct StringTemplate StringTemplate;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    BOOL unk_18;
    u8 padding_1C[4];
} UnkStruct_ov62_0223118C_sub1;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
} UnkStruct_ov62_02233310_sub1;

typedef struct {
    int unk_00;
    int unk_04;
    u64 unk_08;
} UnkStruct_ov62_02249380;

typedef struct {
    const UnkStruct_ov62_02249380 *unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    int unk_20;
    u32 unk_24;
    const TouchscreenHitbox *unk_28;
} UnkStruct_ov62_02248CDC;

typedef struct {
    int unk_00;
    BOOL unk_04;
    u32 unk_08;
    u32 unk_0C;
    s16 unk_10;
    u8 padding_12[2];
    BOOL unk_14;
    ManagedSprite *unk_18[2];
} UnkStruct_ov62_0223359C;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

typedef struct TouchScreenActions TouchScreenActions;

typedef struct {
    int unk_00;
    BOOL unk_04;
} UnkStruct_ov62_02233750;

typedef struct {
    UnkStruct_ov61_0222AFC0 unk_00;
    UnkStruct_ov61_0222BED8_sub1 unk_80;
} UnkStruct_ov61_0222BE20;

typedef struct {
    UnkStruct_ov61_0222AFC0 unk_00;
    UnkStruct_ov61_0222BED8_sub1 unk_80;
    UnkStruct_ov61_0222BED8_sub2 unk_E4;
} UnkStruct_ov61_0222BED8;

typedef struct {
    UnkStruct_ov61_0222BED8 *unk_00;
    UnkStruct_ov61_0222BED8 unk_04;
    u8 unk_1D54;
    u8 padding_1D55[3];
    UnkStruct_ov61_0222BE20 *unk_1D58[30];
    UnkStruct_ov61_0222BE20 unk_1DD0[30];
    int unk_3900;
    u8 unk_3904[32];
} UnkStruct_ov62_02233828;

typedef struct {
    UnkStruct_02030A80 *unk_00;
    ManagedSprite *unk_04;
    ManagedSprite *unk_08;
    Window unk_0C[8];
} UnkStruct_ov62_022323B8;

typedef struct {
    UnkStruct_02030A80 *unk_00;
    UnkStruct_0202F41C *unk_04;
    u8 padding_08[4];
    ManagedSprite *unk_0C[12];
    Window unk_3C[8];
} UnkStruct_ov62_02233F74;

int ov40_022306C0(Window *param0, String *param1);

typedef struct {
    int unk_00;
    int unk_04;
    u16 *unk_08;
    MessageLoader *unk_0C;
    BOOL unk_10;
} UnkStruct_ov62_0223E01C_sub1;

typedef struct {
    int unk_00;
    ManagedSprite *unk_04;
} UnkStruct_ov62_0223E01C_sub1_sub1;

typedef struct {
    NARC *unk_00;
    int unk_04;
    int unk_08;
    UnkStruct_ov62_0223E01C_sub1_sub1 unk_0C[5][6];
} UnkStruct_ov62_0223E01C_sub2;

typedef u32 TextColor;

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

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    Window unk_14[8];
    Window unk_94[8];
    UnkStruct_ov62_022312B0 unk_114[4];
    UnkStruct_ov61_0222BED8 *unk_184;
    TouchScreenActions *unk_188[3];
    UnkStruct_ov62_022323B8 unk_194;
    UnkStruct_ov62_02233F74 unk_220;
    UnkStruct_ov61_0222BED8 unk_2DC;
    int unk_1FB0;
    int unk_1FB4;
    int unk_1FB8;
    int unk_1FBC;
    BOOL unk_1FC0;
    UnkStruct_ov62_0223E01C_sub1 unk_1FC4;
    UnkStruct_ov62_02249380 *unk_1FD8;
    UnkStruct_ov62_02248CDC unk_1FDC;
    UnkStruct_ov62_0223E01C_sub2 unk_2008;
} UnkStruct_ov62_0223E01C;

void ov40_0223CD58(UnkStruct_0208C06C *param0);

void ov40_0223CD58(UnkStruct_0208C06C *param0) {
    UnkStruct_ov62_0223E01C *v0 = param0->unk_860;
    Window *v1;
    String *v2;
    int v3 = 1;
    int v4;

    v1 = &v0->unk_94[0];

    InitWindow(v1);
    AddWindowParameterized(param0->unk_14.unk_10, v1, 6, 4, 6, 10, 2, 14, v3);
    FillWindowPixelBuffer(v1, 0x0);

    v2 = NewString_ReadMsgData(param0->unk_14.unk_34, 130);
    v4 = ov40_022306C0(v1, v2);

    AddTextPrinterParameterizedWithColor(v1, FONT_SYSTEM, v2, v4, 0, 0xFF, ((TextColor)(((15 & 0xFF) << 16) | ((13 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
    String_Delete(v2);
    ScheduleWindowCopyToVram(v1);

    v3 += (10 * 2);
    v1 = &v0->unk_94[1];

    InitWindow(v1);
    AddWindowParameterized(param0->unk_14.unk_10, v1, 6, 18, 6, 10, 2, 14, v3);
    FillWindowPixelBuffer(v1, 0x0);

    v2 = NewString_ReadMsgData(param0->unk_14.unk_34, 131);
    v4 = ov40_022306C0(v1, v2);

    AddTextPrinterParameterizedWithColor(v1, FONT_SYSTEM, v2, v4, 0, 0xFF, ((TextColor)(((15 & 0xFF) << 16) | ((13 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
    String_Delete(v2);
    ScheduleWindowCopyToVram(v1);
}

void ov40_0223CE38(UnkStruct_0208C06C *param0);

void ov40_0223CE38(UnkStruct_0208C06C *param0) {
    UnkStruct_ov62_0223E01C *v0 = param0->unk_860;

    ClearWindowTilemapAndCopyToVram(&v0->unk_94[0]);
    RemoveWindow(&v0->unk_94[0]);
    ClearWindowTilemapAndCopyToVram(&v0->unk_94[1]);
    RemoveWindow(&v0->unk_94[1]);
}

void ov40_0222BF80(UnkStruct_0208C06C *param0, int param1);

void ov40_02230944(UnkStruct_0208C06C *param0);

void ov40_0223CE64(u32 param0, enum TouchScreenButtonState param1, void *param2) {
    UnkStruct_0208C06C *v0 = param2;
    UnkStruct_ov62_0223E01C *v1 = v0->unk_860;

    if (param1 != TOUCH_BUTTON_PRESSED) {
        return;
    }

    switch (param0) {
    case 0:
        ov40_02230944(v0);
        ov40_0222BF80(v0, 8);
        break;
    case 1:
        ov40_02230944(v0);

        if (sub_0202FC48()) {
            ov40_0222BF80(v0, 9);
        } else {
            v1->unk_1FBC = 9;
            ov40_0222BF80(v0, 18);
        }
        break;
    case 2:
        ov40_02230944(v0);

        if (sub_0202FC48()) {
            ov40_0222BF80(v0, 11);
        } else {
            v1->unk_1FBC = 11;
            ov40_0222BF80(v0, 18);
        }
        break;
    case 3:
        ov40_02230944(v0);
        ov40_0222BF80(v0, 10);
        break;
    }
}

void ov40_0222DEAC(UnkStruct_0208C06C *param0);

void ov40_0223CF00(u32 param0, enum TouchScreenButtonState param1, void *param2) {
    UnkStruct_0208C06C *v0 = param2;
    UnkStruct_ov62_0223E01C *v1 = v0->unk_860;

    if (param1 != TOUCH_BUTTON_PRESSED) {
        return;
    }

    switch (param0) {
    case 0:
    case 1:
    case 2:
        v1->unk_1FB0 = param0 + 1;

        if (v0->unk_88C[v1->unk_1FB0] != 0L) {
            ov40_0222BF80(v0, 14);
        } else {
            ov40_0222BF80(v0, 17);
        }

        ov40_02230944(v0);
        break;
    case 3:
        ov40_02230944(v0);
        ov40_0222DEAC(v0);
        ov40_0222BF80(v0, 13);
        break;
    }
}
