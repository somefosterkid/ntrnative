#include "global.h"

#include "frontier/overlay_80.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "obj_char_transfer.h"
#include "options.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "touchscreen.h"
#include "unk_0202FBCC.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_40_unk_struct_ov62_02233310.h"
#include "overlay_40_unk_struct_ov62_022312b0.h"
#include "overlay_40_unk_struct_ov62_02246bf4.h"
#include "pokepic.h"
#include "overlay_40_unk_struct_ov62_02241204.h"
#include "touch_hitbox_controller.h"

struct TextOBJ;

struct G3DPipelineBuffers;

struct MsgData;

struct PokepicManager;

struct MessageFormat;

struct TouchHitboxController;

struct UnkStruct_02012744_t;

struct UnkStruct_02012B20_t;

struct UnkStruct_0202F41C_t;

struct UnkStruct_020F3DCC_t;

typedef struct {
    BOOL unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
} UnkStruct_ov62_0223CAA4;

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

typedef struct {
    SaveData *saveData;
    UnkFuncPtr_ov62_022349A8 unk_04;
    void *unk_08;
    UnkStruct_ov62_022349A8_sub1 unk_0C;
    enum HeapID heapID;
    int unk_148;
    int unk_14C;
    UnkStruct_ov62_022349A8_sub2 unk_150;
    UnkStruct_ov62_0223CAA4 unk_17C;
    void *unk_18C;
    UnkStruct_ov62_022349A8_sub3 unk_190;
    void *unk_3B4;
    int unk_3B8;
    int unk_3BC;
    int unk_3C0;
    BOOL unk_3C4;
    BOOL unk_3C8;
    UnkStruct_ov61_0222B138 unk_3CC;
    int unk_3D8;
    int unk_3DC;
    int unk_3E0;
    int unk_3E4;
    int unk_3E8;
    int unk_3EC;
    int unk_3F0;
    MsgData *unk_3F4;
    MessageFormat *unk_3F8;
    String *unk_3FC;
    UnkStruct_ov61_0222B920 unk_400;
    u8 unk_40D;
    u8 unk_40E;
} UnkStruct_ov62_022349A8;

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
    BOOL unk_04;
    u32 unk_08;
    u32 unk_0C;
    s16 unk_10;
    u8 padding_12[2];
    BOOL unk_14;
    ManagedSprite *unk_18[2];
} UnkStruct_ov62_0223359C;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

typedef struct {
    NARC *unk_00;
    SpriteSystem *unk_04;
    SpriteManager *unk_08;
    u8 padding_0C[4];
    BgConfig *unk_10;
    PaletteData *unk_14;
    TouchHitboxController *unk_18;
    TouchscreenHitbox unk_1C[5];
    BOOL unk_30;
    MsgData *unk_34;
    MsgData *unk_38;
    UnkStruct_02012744 *unk_3C;
    int unk_40;
    int unk_44;
    u8 unk_48;
    u8 padding_49[3];
    G3DPipelineBuffers *unk_4C;
    PokepicManager *unk_50;
    UnkStruct_ov62_022349A8 unk_54;
    UnkStruct_ov62_0223359C unk_46C;
    UnkStruct_ov62_02233310 unk_48C;
    u8 padding_4D8[24];
    UnkStruct_ov62_0223118C_sub1 unk_4F0;
    BOOL unk_510;
    BOOL unk_514;
} UnkStruct_ov62_0223118C;

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
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int *unk_10;
    UnkStruct_ov62_0223118C unk_14;
    UnkStruct_ov62_02233750 unk_52C;
    UnkStruct_ov62_022307C0 unk_534;
    UnkStruct_0208B878 *unk_6F0;
    UnkStruct_0208B878 *unk_6F4;
    UnkStruct_ov62_02230C28 unk_6F8[6];
    const UnkStruct_020F3DCC *unk_818;
    const UnkStruct_020F3DCC *unk_81C[5];
    SaveData *saveData;
    u16 unk_834;
    u16 unk_836;
    UnkStruct_0208C06C_sub1 unk_838;
    void *unk_860;
    u8 padding_864[4];
    UnkStruct_0208BA84 *unk_868;
    int unk_86C;
    BOOL unk_870;
    u32 unk_874;
    u8 *unk_878;
    UnkStruct_0202F41C *unk_87C[4];
    UnkStruct_02030A80 *unk_88C[4];
    BOOL unk_89C;
    BOOL unk_8A0;
    Window unk_8A4;
    UnkStruct_ov62_02233828 unk_8B4;
    UnkStruct_0208C06C_sub2 unk_41D8;
    SysTask *unk_41E8;
} UnkStruct_0208C06C;

void ov40_0222D6D0(UnkStruct_ov62_022312B0 *param0);

void ov40_0222D7DC(UnkStruct_0208C06C *param0);

typedef struct {
    int unk_00;
    int unk_04[4];
    u8 padding_14[4];
    String *unk_18;
    Options *options;
} UnkStruct_ov62_02246B00;

typedef struct {
    s16 unk_00;
    s16 unk_02;
    u8 unk_04;
    u8 unk_05;
} UnkStruct_ov62_02246BF4_sub1_sub1;

typedef struct {
    int unk_00;
    int unk_04;
    BOOL unk_08;
    ManagedSprite *unk_0C;
    TouchscreenHitbox *unk_10;
    UnkStruct_ov62_02246BF4_sub1_sub1 unk_14;
    u8 padding_1A[2];
} UnkStruct_ov62_02246BF4_sub1;

typedef struct {
    NARC *unk_00;
    SpriteSystem *unk_04;
    SpriteManager *unk_08;
    BgConfig *unk_0C;
    PaletteData *unk_10;
    TouchHitboxController *unk_14;
    TouchscreenHitbox unk_18[24];
    BOOL unk_78;
} UnkStruct_ov62_02246BF4_sub2;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
} UnkStruct_ov62_02246BF4_sub3;

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

void ov40_02241114(UnkStruct_0208C06C *param0);

void ov40_02241114(UnkStruct_0208C06C *param0) {
    UnkStruct_ov62_02241204 *v0 = param0->unk_860;
    int v1;

    for (v1 = 0; v1 < 4; v1++) {
        ov40_0222D6D0(&v0->unk_10[v1]);
        Sprite_DeleteAndFreeResources(v0->unk_10[v1].unk_08);
    }

    ov40_0222D7DC(param0);
}

void TouchHitboxController_Destroy(TouchHitboxController *buttonAction);

void ov40_0222BF64(UnkStruct_0208C06C *param0, int param1, BOOL param2, int *param3);

void ov40_0222BF80(UnkStruct_0208C06C *param0, int param1);

void ov40_0222D88C(UnkStruct_0208C06C *param0);

BOOL ov40_0222DA00(int *param0, int *param1, int param2, int param3);

BOOL ov40_0222DA84(int *param0, int param1);

void ov40_0222DAA8(int *param0);

void ov40_0222DD08(UnkStruct_0208C06C *param0);

void ov40_0222DED0(UnkStruct_0208C06C *param0, int param1);

void ov40_0222DFB0(UnkStruct_0208C06C *param0);

void ov40_0222E7B8(UnkStruct_ov62_022323B8 *param0, UnkStruct_0208C06C *param1);

void ov40_0222FB90(UnkStruct_0208C06C *param0, int param1);

BOOL ov40_0222FBB4(UnkStruct_0208C06C *param0);

void ov40_0223064C(UnkStruct_ov62_02233F74 *param0, UnkStruct_0208C06C *param1);

void ov40_02230964(UnkStruct_0208C06C *param0, BOOL param1);

BOOL ov40_02230D94(UnkStruct_0208C06C *param0, u64 param1);

UnkStruct_0202F41C *sub_020307F8(void);

u64 sub_0203088C(UnkStruct_0202F41C *param0, int param1, int param2);

void ov40_02240910(UnkStruct_0208C06C *param0);

void ov40_02241054(UnkStruct_0208C06C *param0);

void ov40_02241A34(UnkStruct_0208C06C *param0);

void ov40_022421FC(UnkStruct_0208C06C *param0);

BOOL ov40_02242378(UnkStruct_0208C06C *param0);

BOOL ov40_02241144(UnkStruct_0208C06C *param0);

BOOL ov40_02241144(UnkStruct_0208C06C *param0) {
    UnkStruct_ov62_02241204 *v0 = param0->unk_860;

    if (ov40_02230D94(param0, sub_0203088C(sub_020307F8(), 4, 0))) {
        switch (param0->unk_08) {
        case 0:
            ov40_02230964(param0, 1);

            if (v0->unk_1CC == 0) {
                ov40_0223064C(&v0->unk_10C, param0);
            } else {
                ov40_0222E7B8(&v0->unk_80, param0);
            }

            ov40_02230964(param0, 0);
            ov40_02241A34(param0);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, 0);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, 0);
            param0->unk_08++;
            break;
        case 1:
            ov40_0222DA84(&v0->unk_08, 1);
            if (ov40_0222DA00(&v0->unk_00, &v0->unk_04, 1, 0)) {
                ov40_02230964(param0, 1);
                ov40_022421FC(param0);
                ov40_02241054(param0);
                ov40_02230964(param0, 0);
                BgClearTilemapBufferAndCommit(param0->unk_14.unk_10, 3);
                BgClearTilemapBufferAndCommit(param0->unk_14.unk_10, 7);
                param0->unk_08++;
            }

            PaletteData_BlendPalettes(param0->unk_14.unk_14, PLTTBUF_SUB_OBJ, 0xC, v0->unk_08, param0->unk_14.unk_44);
            break;
        case 2:
            ov40_0222DED0(param0, 277);
            param0->unk_08++;
            break;
        case 3:
            if (!System_GetTouchNew()) {
                break;
            }

            ov40_0222DFB0(param0);
            BgClearTilemapBufferAndCommit(param0->unk_14.unk_10, 2);
            BgClearTilemapBufferAndCommit(param0->unk_14.unk_10, 6);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, 0);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, 0);
            param0->unk_08++;
            break;
        case 4:
            ov40_0222DA84(&v0->unk_08, 1);

            if (ov40_0222DA00(&v0->unk_00, &v0->unk_04, 1, 0)) {
                ov40_02241114(param0);
                param0->unk_08++;
            }
            PaletteData_BlendPalettes(param0->unk_14.unk_14, PLTTBUF_SUB_OBJ, 0xC, v0->unk_08, param0->unk_14.unk_44);
            break;
        case 5:
            TouchHitboxController_Destroy(v0->unk_608[0]);
            TouchHitboxController_Destroy(v0->unk_608[1]);
            TouchHitboxController_Destroy(v0->unk_608[2]);
            ov40_0222DAA8(&v0->unk_08);
            ov40_02230964(param0, 1);
            ov40_02240910(param0);
            ov40_0222D88C(param0);
            ov40_02230964(param0, 0);
            ov40_0222FB90(param0, 1);
            param0->unk_08++;
            break;
        default:
            if (ov40_0222FBB4(param0) == 0) {
                break;
            }

            if (ov40_0222DA84(&v0->unk_08, 0)) {
                ov40_0222DD08(param0);
                ov40_0222DAA8(&v0->unk_08);

                PaletteData_BlendPalettes(param0->unk_14.unk_14, PLTTBUF_MAIN_OBJ, 0xC, 16, param0->unk_14.unk_44);

                ov40_0222BF64(param0, 1, 1, param0->unk_10);
                ov40_0222BF80(param0, 5);

                Heap_Free(v0);
                BgClearTilemapBufferAndCommit(param0->unk_14.unk_10, 2);
                BgClearTilemapBufferAndCommit(param0->unk_14.unk_10, 6);
                BgClearTilemapBufferAndCommit(param0->unk_14.unk_10, 3);
                BgClearTilemapBufferAndCommit(param0->unk_14.unk_10, 7);

                if (sub_0202FC48() == 1) {
                    sub_0202FC24();
                }
            } else {
                PaletteData_BlendPalettes(param0->unk_14.unk_14, PLTTBUF_SUB_BG, 0x2, v0->unk_08, param0->unk_14.unk_44);
                PaletteData_BlendPalettes(param0->unk_14.unk_14, PLTTBUF_SUB_OBJ, 0xC, v0->unk_08, param0->unk_14.unk_44);
            }

            break;
        }
    } else {
        if (ov40_02242378(param0)) {
            ov40_0222BF80(param0, 13);
        }
    }

    return 0;
}
