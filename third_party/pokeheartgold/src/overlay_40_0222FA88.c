#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "touchscreen.h"
#include "game_stats.h"
#include "unk_0202FBCC.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "filesystem.h"
#include "obj_char_transfer.h"
#include "overlay_40.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "frontier/overlay_80.h"
#include "heap.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_40_unk_struct_ov62_02233310.h"
#include "pokepic.h"

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

s32 ov40_0222FA2C(UnkStruct_ov62_0223359C *, u32);

extern TouchscreenHitbox ov40_02244E08;

extern TouchscreenHitbox ov40_02244E0C;

void ov40_0222FA88(UnkStruct_ov62_0223359C *param0);

void ov40_0222FB28(void *arg0, s32 arg1);
s32 sub_02030814(void *, void *, void *, void *, SaveData *);
void ov40_0222FB40(void *arg0);

struct TextOBJ;
struct G3DPipelineBuffers;
struct MsgData;
struct PokepicManager;
struct MessageFormat;
struct TouchHitboxController;
struct UnkStruct_02012744_t;
struct UnkStruct_02012B20_t;
struct UnkStruct_0202F41C_t;
struct UnkStruct_02030A80_t;
struct UnkStruct_020F3DCC_t;
typedef struct {
    int unk_00;
    BOOL unk_04;
} UnkStruct_ov62_02233750;
void ov40_0222BC44(int param0);
void sub_0203A86C(void);
typedef struct UnkStruct_0202F41C_t UnkStruct_0202F41C;
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
typedef struct UnkStruct_02030A80_t UnkStruct_02030A80;
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
void PokepicManager_HandleLoadImgAndOrPltt(PokepicManager *monSpriteMan);
BOOL ov40_02230958(UnkStruct_0208C06C *param0);
int ov40_02244AB0(SaveData *saveData, int param1, u8 param2, u16 *param3, u16 *param4);
int ov39_0222801C(UnkStruct_ov62_022349A8 *param0, UnkStruct_ov61_0222BE20 **param1, int param2);
UnkStruct_ov62_022349A8 *ov40_0223D540(UnkStruct_0208C06C *param0);
void ov40_0222FB90(UnkStruct_0208C06C *param0, int param1);
BOOL ov40_0222FBB4(UnkStruct_0208C06C *param0);
void ov40_0222FBF8(UnkStruct_0208C06C *param0);
BOOL ov40_0222FC14(UnkStruct_0208C06C *param0, int param1, u8 param2);
void ov40_0222FC40(UnkStruct_0208C06C *param0, BOOL param1);
void ov40_0222FC4C(void *param0, const UnkStruct_ov62_0223CAA4 *param1);

s32 sub_0202FC90(void *, s32, u32 *, void *, s32);
void *sub_0203077C(s32);
void *sub_020307AC(s32);
UnkStruct_02030A80 *sub_020314A4(s32);
s32 sub_020314C4(UnkStruct_02030A80 *, SaveData *);
void ov40_0222FCCC(void *arg0);
void ov40_0222FDC4(void *arg0);

void ov40_0222FA88(UnkStruct_ov62_0223359C *param0) {
    u32 x;
    u32 y;
    s32 v0;
    s32 v1;
    s32 v2;
    v2 = 0;
    v1 = System_GetTouchHeldCoords(&x, &y);
    if (param0->unk_14 != 0) {
        ManagedSprite_TickTwoFrames(param0->unk_18[0]);
        ManagedSprite_TickTwoFrames(param0->unk_18[1]);
    }
    if ((param0->unk_04 == 0) && (v1 == 0)) {
        param0->unk_04 = 0;
        param0->unk_00 = 0;
        param0->unk_08 = 0;
        param0->unk_0C = 0;
        param0->unk_10 = 0;
        return;
    }
    if ((TouchscreenHitbox_TouchHeldIsIn(&ov40_02244E08) != 0) || (TouchscreenHitbox_TouchHeldIsIn(&ov40_02244E0C) != 0)) {
        v2 = 1;
    }
    v0 = param0->unk_04;
    if (v2 != 0) {
        if (param0->unk_04 == 0) {
            param0->unk_04 = 1;
            param0->unk_08 = x;
            param0->unk_0C = y;
            param0->unk_00 = 2;
            return;
        }
        param0->unk_00 -= 1;
        ov40_0222FA2C(param0, y);
        return;
    }
    if (param0->unk_04 != 0) {
        v1 = 0;
        param0->unk_04 = v1;
        param0->unk_00 = v1;
        param0->unk_08 = v1;
        param0->unk_0C = 0;
    }
}

void ov40_0222FB28(void *arg0, s32 statIdx) {
    GameStats_AddScore(Save_GameStats_Get(*(SaveData **)((u8 *)arg0 + 0x830)), statIdx);
}

void ov40_0222FB40(void *arg0) {
    u32 out;
    u8 *v0;

    sub_0202FBF0(*(SaveData **)((u8 *)arg0 + 0x830), HEAP_ID_126, &out);
    v0 = *(u8 **)((u8 *)arg0 + 0x8B4);
    sub_02030814(v0, v0 + 0x80, v0 + 0xE4, 0, *(SaveData **)((u8 *)arg0 + 0x830));
}

void ov40_0222FB74(SysTask *param0, void *param1) {
    UnkStruct_ov62_02233750 *v0 = param1;

    ov40_0222BC44(v0->unk_00);
    sub_0203A86C();
    v0->unk_04 = 1;
    SysTask_Destroy(param0);
}

void ov40_0222FB90(UnkStruct_0208C06C *param0, int param1) {
    param0->unk_52C.unk_00 = param1;
    param0->unk_52C.unk_04 = 0;

    SysTask_CreateOnVBlankQueue(ov40_0222FB74, &param0->unk_52C, 4);
}

BOOL ov40_0222FBB4(UnkStruct_0208C06C *param0) {
    return param0->unk_52C.unk_04;
}

void ov40_0222FBBC(SysTask *param0, void *param1) {
    UnkStruct_0208C06C *v0 = param1;

    if (v0->unk_14.unk_08 == 0L) {
        return;
    }

    if (ov40_02230958(v0) == 1) {
        return;
    }

    if (v0->unk_14.unk_510 == 1) {
        return;
    }

    v0->unk_14.unk_510 = 1;

    PokepicManager_HandleLoadImgAndOrPltt(v0->unk_14.unk_50);
    SpriteSystem_DrawSprites(v0->unk_14.unk_08);
    SpriteSystem_TransferOam();

    v0->unk_14.unk_510 = 0;
}

void ov40_0222FBF8(UnkStruct_0208C06C *param0) {
    param0->unk_41E8 = SysTask_CreateOnVBlankQueue(ov40_0222FBBC, param0, 5);
}

BOOL ov40_0222FC14(UnkStruct_0208C06C *param0, int param1, u8 param2) {
    int v0 = ov40_02244AB0(param0->saveData, param1, param2, &param0->unk_834, &param0->unk_836);

    if ((v0 == 0) || (v0 == 1)) {
        return 0;
    }

    return 1;
}

void ov40_0222FC40(UnkStruct_0208C06C *param0, BOOL param1) {
    param0->unk_874 = param1;
}

void ov40_0222FC4C(void *param0, const UnkStruct_ov62_0223CAA4 *param1) {
    UnkStruct_0208C06C *v0 = param0;
    UnkStruct_ov62_02233828 *v1 = &v0->unk_8B4;

    if (param1->unk_00 == 1) {
        (void)0;
    } else {
        v1->unk_3900 = ov39_0222801C(ov40_0223D540(v0), v1->unk_1D58, 30);

        {
            int v2;

            for (v2 = 0; v2 < v1->unk_3900; v2++) {
                ((v0->unk_8B4.unk_1D58[v2] != 0L) ? (void)0 : GF_AssertFail());
                MI_CpuCopy8(v0->unk_8B4.unk_1D58[v2], &v0->unk_8B4.unk_1DD0[v2], sizeof(UnkStruct_ov61_0222BE20));
                v0->unk_8B4.unk_3904[v2] = v0->unk_8B4.unk_1D58[v2]->unk_80.unk_27;
            }
        }
    }
}

void ov40_0222FCCC(void *arg0) {
    u32 v0;
    s32 v2;
    void **v1;

    v2 = 0;
    v1 = (void **)arg0;
    do {
        sub_0202FC90(*(void **)((u8 *)arg0 + 0x830), 0x6D, &v0, 0, v2);
        switch (v0) {
        case 0:
            *(s32 *)((u8 *)v1 + offsetof(UnkStruct_0208C06C, unk_87C)) = 0;
            *(s32 *)((u8 *)v1 + offsetof(UnkStruct_0208C06C, unk_88C)) = 0;
            if (v2 == 0) {
                *(void **)((u8 *)v1 + offsetof(UnkStruct_0208C06C, unk_88C)) = sub_020314A4(0x6D);
                sub_020314C4(*(void **)((u8 *)v1 + offsetof(UnkStruct_0208C06C, unk_88C)), *(void **)((u8 *)arg0 + 0x830));
            }
            break;
        case 2:
            *(s32 *)((u8 *)v1 + offsetof(UnkStruct_0208C06C, unk_87C)) = 0;
            *(s32 *)((u8 *)v1 + offsetof(UnkStruct_0208C06C, unk_88C)) = 0;
            if (v2 == 0) {
                *(void **)((u8 *)v1 + offsetof(UnkStruct_0208C06C, unk_88C)) = sub_020314A4(0x6D);
                sub_020314C4(*(void **)((u8 *)v1 + offsetof(UnkStruct_0208C06C, unk_88C)), *(void **)((u8 *)arg0 + 0x830));
            }
            break;
        case 3:
            *(s32 *)((u8 *)v1 + offsetof(UnkStruct_0208C06C, unk_87C)) = 0;
            *(s32 *)((u8 *)v1 + offsetof(UnkStruct_0208C06C, unk_88C)) = 0;
            if (v2 == 0) {
                *(void **)((u8 *)v1 + offsetof(UnkStruct_0208C06C, unk_88C)) = sub_020314A4(0x6D);
                sub_020314C4(*(void **)((u8 *)v1 + offsetof(UnkStruct_0208C06C, unk_88C)), *(void **)((u8 *)arg0 + 0x830));
            }
            break;
        case 1:
            *(void **)((u8 *)v1 + offsetof(UnkStruct_0208C06C, unk_87C)) = sub_0203077C(0x6D);
            if (v2 == 0) {
                *(void **)((u8 *)v1 + offsetof(UnkStruct_0208C06C, unk_88C)) = sub_020314A4(0x6D);
                sub_020314C4(*(void **)((u8 *)v1 + offsetof(UnkStruct_0208C06C, unk_88C)), *(void **)((u8 *)arg0 + 0x830));
            } else {
                *(void **)((u8 *)v1 + offsetof(UnkStruct_0208C06C, unk_88C)) = sub_020307AC(0x6D);
            }
            break;
        }
        if (sub_0202FC48() == 1) {
            sub_0202FC24();
        }
        v2 += 1;
        v1 += 1;
    } while (v2 < 4);
}

void ov40_0222FDC4(void *arg0) {
    s32 v1;
    void *ptr;
    void *v0;
    void **v2;

    v1 = 0;
    v2 = (void **)arg0;
    do {
        ptr = *(void **)((u8 *)v2 + offsetof(UnkStruct_0208C06C, unk_87C));
        if (ptr != NULL) {
            Heap_Free(ptr);
            *(void **)((u8 *)v2 + offsetof(UnkStruct_0208C06C, unk_87C)) = NULL;
        }
        v0 = *(void **)((u8 *)v2 + offsetof(UnkStruct_0208C06C, unk_88C));
        if (v0 != NULL) {
            Heap_Free(v0);
            *(void **)((u8 *)v2 + offsetof(UnkStruct_0208C06C, unk_88C)) = NULL;
        }
        v1 += 1;
        v2 += 1;
    } while (v1 < 4);
}
