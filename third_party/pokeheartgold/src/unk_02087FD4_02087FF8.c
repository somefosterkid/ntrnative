#include "global.h"
#include "constants/heap.h"
#include "overlay_40_unk_struct_0208_b878.h"
#include "filesystem.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "unk_02013534.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "pokepic.h"
#include "overlay_39_unk_struct_ov62_022349_a8.h"
#include "overlay_40_unk_struct_ov62_02233310.h"
#include "overlay_40_unk_struct_ov62_0223118c.h"

struct BgConfig;

struct CharTransferAllocation;

struct MailMessage;

struct TextOBJ;

struct G3DPipelineBuffers;

struct ManagedSprite;

struct MsgData;

struct NARC;

struct PaletteData;

struct PokepicManager;

struct SaveData;

struct SpriteManager;

struct SpriteSystem;

struct String;

struct MessageFormat;

struct SysTask;

struct SysTaskQueue;

struct TouchScreenActions;

struct UnkStruct_02012744_t;

struct UnkStruct_02012B20_t;

struct UnkStruct_0202F41C_t;

struct UnkStruct_02030A80_t;

struct UnkStruct_020F3DCC_t;

struct Window;

typedef struct UnkStruct_020F3DCC_t UnkStruct_020F3DCC;

typedef struct UnkStruct_0202F41C_t UnkStruct_0202F41C;

typedef struct UnkStruct_02030A80_t UnkStruct_02030A80;

typedef struct CharTransferAllocation {
    u32 size;
    u32 offset;
    u16 vramType;
    u16 atEnd;
} CharTransferAllocation;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

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

typedef union {
    struct {
        u8 top;
        u8 bottom;
        u8 left;
        u8 right;
    } rect;

    struct {
        u8 code;
        u8 x;
        u8 y;
        u8 r;
    } circle;
} TouchScreenRect, TouchscreenHitbox;

typedef struct {
    ManagedSprite *unk_00;
    u8 padding_04[8];
    BOOL unk_0C;
    TouchScreenRect *unk_10;
    TextOBJ *unk_14;
    UnkStruct_02012B20 *unk_18;
    CharTransferAllocation unk_1C;
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
    const TouchScreenRect *unk_28;
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
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int *unk_10;
    u32 unk_14_;
    struct { struct NARC *unk_00; struct SpriteSystem *unk_04; struct SpriteManager *unk_08; u8 padding_0C[4]; struct BgConfig *unk_10; struct PaletteData *unk_14; struct TouchScreenActions *unk_18; u8 unk_1C[20]; BOOL unk_30; struct MsgData *unk_34; struct MsgData *unk_38; struct UnkStruct_02012744_t *unk_3C; int unk_40; int unk_44; u8 unk_48; u8 padding_49[3]; struct G3DPipelineBuffers *unk_4C; struct PokepicManager *unk_50; u8 unk_58_pad[4]; struct { struct SaveData *saveData; void *unk_04; void *unk_08; struct { s32 unk_00; u8 unk_04; u8 language; s8 unk_06[301]; u8 padding[5]; } unk_0C; u32 heapID; int unk_148; int unk_14C; struct { void *unk_00; void *unk_04; void *unk_08; void *unk_0C; void *unk_10; void *unk_14; void *unk_18; void *unk_1C; void *unk_20; void *unk_24; void *unk_28; } unk_150; struct { BOOL unk_00; int unk_04; int unk_08; int unk_0C; } unk_17C; void *unk_18C; struct { u8 unk_0_anon[540]; u8 unk_220_pad[4]; u8 unk_21C[8]; } unk_190; void *unk_3B4; int unk_3B8; int unk_3BC; int unk_3C0; BOOL unk_3C4; BOOL unk_3C8; struct { int unk_00; int unk_04; int unk_08; } unk_3CC; int unk_3D8; int unk_3DC; int unk_3E0; int unk_3E4; int unk_3E8; int unk_3EC; int unk_3F0; struct MsgData *unk_3F4; struct MessageFormat *unk_3F8; struct String *unk_3FC; struct { void *unk_00; u16 unk_04; u16 unk_06; u8 unk_08; u8 unk_C_pad[3]; void *unk_0C; } unk_400; u8 unk_40D; u8 unk_40E; u8 unk_416_tail[2]; } unk_54; struct { int unk_00; BOOL unk_04; u32 unk_08; u32 unk_0C; s16 unk_10; u8 padding_12[2]; BOOL unk_14; struct ManagedSprite *unk_18[2]; } unk_46C; struct { int unk_00; int unk_04; s16 unk_08; u8 padding_0A[2]; int unk_0C; int unk_10; int unk_14; Window unk_18; void *unk_28; struct ManagedSprite *unk_2C[2]; struct MsgData *unk_34; struct { int unk_00; int unk_04; int unk_08; int unk_0C; } unk_38; BOOL unk_48; u8 padding_4C[4]; } unk_48C; u8 padding_4D8[24]; struct { int unk_00; int unk_04; int unk_08; int unk_0C; int unk_10; int unk_14; BOOL unk_18; u8 padding_1C[4]; } unk_4F0; BOOL unk_510; BOOL unk_514; } unk_14;
    UnkStruct_ov62_02233750 unk_52C;
    UnkStruct_ov62_022307C0 unk_534;
    void *unk_6F0;
    void *unk_6F4;
    UnkStruct_ov62_02230C28 unk_6F8[6];
    struct UnkStruct_020F3DCC_t *unk_818;
    struct UnkStruct_020F3DCC_t *unk_81C[5];
    struct SaveData *saveData;
    u16 unk_834;
    u16 unk_836;
    UnkStruct_0208C06C_sub1 unk_838;
    void *unk_860;
    void *unk_870_;
    void *unk_868;
    int unk_86C;
    BOOL unk_870;
    u32 unk_874;
    u8 *unk_878;
    struct UnkStruct_0202F41C_t *unk_87C[4];
    struct UnkStruct_02030A80_t *unk_88C[4];
    BOOL unk_89C;
    BOOL unk_8A0;
    Window unk_8A4;
    struct { void *unk_00; u8 unk_8_pad[4]; struct { struct { u16 unk_00[8]; u32 unk_10; u8 unk_14; u8 unk_15; u8 unk_16; u8 unk_17; u8 unk_18; u8 unk_19; u8 unk_1A; u8 unk_1B_0 : 1; u8 unk_1B_1 : 7; u16 unk_1C; u8 unk_1E; u8 unk_1F; u8 unk_20_anon[80]; u8 unk_70[12]; struct { u16 unk_00; u16 unk_02; } unk_7C; } unk_00; struct { u16 unk_00[12]; u8 unk_18[12]; u16 unk_24; u8 unk_26; u8 unk_27; u8 unk_28[32]; u16 unk_48; u8 unk_4A[14]; u64 unk_58; struct { u16 unk_00; u16 unk_02; } unk_60; u8 unk_64_tail[4]; } unk_80; struct { struct { u32 unk_00; int unk_04; int unk_08[4]; struct { u8 unk_00; u8 unk_01; u8 unk_02; u8 unk_03; u16 unk_04[4]; u32 unk_0C; u32 unk_10; u16 unk_14[8]; struct { u16 type; u16 id; u16 words[2]; } unk_24; struct { u16 type; u16 id; u16 words[2]; } unk_2C; } unk_18[4]; int unk_E8; int unk_EC; int unk_F0; int unk_F4; int unk_F8; int unk_FC; int unk_100; int unk_104; int unk_108; int unk_10C; int unk_110; u32 unk_114[4]; u32 unk_124; int unk_128; u32 unk_12C; u32 unk_130; int unk_134[4]; u16 unk_144; u16 unk_146; int unk_148; u8 unk_14C[4]; } unk_00; struct { u8 unk_00[1024][4]; } unk_150; struct { u16 unk_00; u16 unk_02; struct { u32 unk_00; u16 unk_04_0 : 1; u16 unk_04_1 : 1; u16 unk_04_2 : 1; u16 unk_04_3 : 13; u16 unk_06; u16 unk_08; u8 unk_C_pad[2]; u32 unk_0C; u32 unk_10; u8 unk_14; u8 unk_15; u8 unk_16; u8 unk_17; u8 unk_18; u8 unk_19; u8 unk_1A; u8 unk_1B; u16 unk_1C[4]; u8 unk_24[4]; u8 unk_28[4]; u32 unk_2C_0 : 5; u32 unk_2C_5 : 5; u32 unk_2C_10 : 5; u32 unk_2C_15 : 5; u32 unk_2C_20 : 5; u32 unk_2C_25 : 5; u32 unk_2C_30 : 1; u32 unk_2C_31 : 1; u8 unk_30_0 : 1; u8 unk_30_1 : 2; u8 unk_30_3 : 5; u16 unk_32[11]; u16 unk_48[8]; u8 unk_58; u8 unk_59; u8 unk_5C_pad[2]; u32 unk_5C; u8 unk_60; u8 unk_61; u16 unk_62; u16 unk_64; u16 unk_66; u16 unk_68; u16 unk_6A; u16 unk_6C; u16 unk_6E; } unk_04[6]; } unk_1150[4]; struct { u16 unk_00[8]; u32 unk_10; u32 unk_14; u8 unk_18; u8 unk_19; u8 unk_1A; u8 unk_1B; u8 unk_1C; u8 unk_1D_0 : 1; u8 unk_1D_1 : 1; u8 unk_1D_2 : 6; u8 unk_1E; u8 unk_1F; } unk_1B68[4]; struct { u16 unk_00_0 : 4; u16 unk_00_4 : 2; u16 unk_00_6 : 1; u16 unk_00_7 : 1; u16 unk_00_8 : 2; u16 unk_00_10 : 5; u16 unk_00_15 : 1; } unk_1BE8; u16 unk_1BEA; struct { u16 unk_00; u16 unk_02; } unk_1BEC; } unk_E4; } unk_04; u8 unk_1D54; u8 padding_1D55[3]; void *unk_1D58[30]; u8 unk_1DD8_pad[4]; struct { struct { u16 unk_00[8]; u32 unk_10; u8 unk_14; u8 unk_15; u8 unk_16; u8 unk_17; u8 unk_18; u8 unk_19; u8 unk_1A; u8 unk_1B_0 : 1; u8 unk_1B_1 : 7; u16 unk_1C; u8 unk_1E; u8 unk_1F; u8 unk_20_anon[80]; u8 unk_70[12]; struct { u16 unk_00; u16 unk_02; } unk_7C; } unk_00; struct { u16 unk_00[12]; u8 unk_18[12]; u16 unk_24; u8 unk_26; u8 unk_27; u8 unk_28[32]; u16 unk_48; u8 unk_4A[14]; u64 unk_58; struct { u16 unk_00; u16 unk_02; } unk_60; u8 unk_64_tail[4]; } unk_80; } unk_1DD0[30]; int unk_3900; u8 unk_3904[32]; u8 unk_392C_tail[4]; } unk_8B4;
    UnkStruct_0208C06C_sub2 unk_41D8;
    struct SysTask *unk_41E8;
    u8 pad_4204[4];
} UnkStruct_0208C06C;

const UnkStruct_020F3DCC *sub_02087FD4(int param0);

BOOL sub_02087E1C(UnkStruct_0208C06C *param0);

extern const UnkStruct_020F3DCC _0210357C[5];

extern const UnkStruct_020F3DCC _02102DC0[5];

const UnkStruct_020F3DCC *sub_02087FF8(UnkStruct_0208C06C *param0, int param1);

const UnkStruct_020F3DCC *sub_02087FF8(UnkStruct_0208C06C *param0, int param1) {
    if (param1 == 0) {
        if (sub_02087E1C(param0) == 0) {
            return &_0210357C[0];
        }
    }

    if (param1 == 3) {
        if (sub_02087E1C(param0) == 0) {
            return &_02102DC0[0];
        }
    }

    return sub_02087FD4(param1);
}
