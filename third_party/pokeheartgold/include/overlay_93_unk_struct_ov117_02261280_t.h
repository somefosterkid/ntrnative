#ifndef POKEHEARTGOLD_OVERLAY_93_UNK_STRUCT_OV117_02261280_T_H
#define POKEHEARTGOLD_OVERLAY_93_UNK_STRUCT_OV117_02261280_T_H

// The overlay 93 app object (Pokeathlon-style minigame app, allocated 0x3850 bytes in ov93_0225C768): one real struct for every
// overlay 93 file. Sub-records are named types so function prototypes and the struct agree.

#include "global.h"

#include "bg_window.h"
#include "camera.h"
#include "field/overlay_01_021FB878.h"
#include "gf_3d_vramman.h"
#include "particle_system.h"
#include "message_format.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "overlay_91_unk_struct_ov66_02230dbc.h"
#include "overlay_90_unk_struct_ov114_0225c76c.h"
#include "overlay_90_unk_struct_ov114_0225c9a8.h"
#include "overlay_manager.h"
#include "palette.h"
#include "player_data.h"
#include "save.h"
#include "sprite_system.h"
#include "sys_task.h"

struct FontOAM;
struct ParticleSystem;
struct UnkStruct_02012744_t;
struct UnkStruct_ov114_02025CAD4_t;
struct UnkStruct_ov117_022626B0_t;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 padding_02[2];
} UnkStruct_ov117_022621D4_sub1;

typedef struct {
    u16 unk_00;
    u8 unk_02;
    u8 unk_03;
} UnkStruct_ov117_022621D4_sub2;

typedef struct {
    u16 unk_00;
    u8 unk_02;
    u8 unk_03;
    u8 unk_04;
    u8 padding_05[3];
} UnkStruct_ov117_022621D4_sub3;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u16 unk_02;
    s32 unk_04;
    s32 unk_08;
    u8 unk_0C;
    u8 padding_0D[3];
} UnkStruct_ov117_02264E84;

typedef struct {
    int unk_00;
    union {
        UnkStruct_ov117_022621D4_sub1 unk_04_val1;
        UnkStruct_ov117_022621D4_sub1 unk_04_val2;
        UnkStruct_ov117_022621D4_sub1 unk_04_val3;
        UnkStruct_ov117_02264E84 unk_04_val4;
        UnkStruct_ov117_022621D4_sub2 unk_04_val5;
        UnkStruct_ov117_022621D4_sub3 unk_04_val6;
    };
} UnkStruct_ov117_022621D4;

typedef struct UnkStruct_ov114_02025CAD4_t UnkStruct_ov114_0225CAD4;

typedef struct UnkStruct_ov117_022626B0_t UnkStruct_ov117_022626B0;

typedef struct UnkStruct_02012744_t UnkStruct_02012744;

typedef struct {
    int unk_00;
    int unk_04;
    s32 unk_08;
    s32 unk_0C;
    s32 unk_10;
    s32 unk_14;
    s32 unk_18[4];
    int unk_28;
    u8 unk_2C;
    u8 unk_2D;
    u8 padding_2E[2];
} UnkStruct_ov117_02263DAC;

typedef struct {
    ManagedSprite *unk_00[4];
} UnkStruct_ov117_0226504C;

typedef struct {
    ManagedSprite *unk_00[2];
    fx32 unk_08[2];
    fx32 unk_10[2];
    u8 unk_18;
    u8 unk_19;
    u8 padding_1A[2];
} UnkStruct_ov117_02266344;

typedef struct {
    ManagedSprite *unk_00;
    ManagedSprite *unk_04;
    ManagedSprite *unk_08;
    fx32 unk_0C;
    u8 unk_10;
    u8 unk_11;
    u8 unk_12;
    u8 unk_13;
    u8 unk_14;
    u8 unk_15;
    u8 padding_16[2];
    s32 unk_18;
    s16 unk_1C;
    s16 unk_1E;
    fx32 unk_20;
    fx32 unk_24;
    fx32 unk_28;
    fx32 unk_2C;
    UnkStruct_ov117_02266344 unk_30;
} UnkStruct_ov117_02265428;

typedef struct {
    UnkStruct_ov117_02265428 unk_00[3];
    fx32 unk_E4;
    fx32 unk_E8;
    u32 unk_EC;
    u8 unk_F0;
    u8 unk_F1;
    u8 unk_F2;
    u8 unk_F3;
    u8 unk_F4;
    u8 padding_F5[3];
} UnkStruct_ov117_022653F4;

typedef struct {
    ManagedSprite *unk_00;
    ManagedSprite *unk_04;
    fx32 unk_08;
    fx32 unk_0C;
    s16 unk_10;
    u8 unk_12;
    u8 unk_13;
    u8 unk_14;
    u8 unk_15;
    s16 unk_16;
} UnkStruct_ov117_02265C1C;

typedef struct {
    UnkStruct_ov117_02265C1C unk_00[3];
} UnkStruct_ov117_02265C3C;

typedef struct FontOAM FontOAM;

typedef struct {
    FontOAM *unk_00;
    UnkStruct_02021AC8 unk_04;
    u16 unk_10;
    u8 padding_12[2];
} UnkStruct_ov117_02265EB0;

typedef struct {
    ManagedSprite *unk_00;
    UnkStruct_ov117_02265EB0 unk_04[6][2];
    UnkStruct_ov117_02265EB0 unk_F4;
    UnkStruct_ov117_02265EB0 unk_108[6];
    u8 unk_180[6];
    u8 unk_186[6];
    u8 unk_18C[6];
    u8 unk_192[6];
    s8 unk_198[6];
    s8 unk_19E;
    u8 unk_19F;
    u8 unk_1A0;
    u8 padding_1A1[3];
} UnkStruct_ov117_02265EC8;

typedef struct {
    ManagedSprite *unk_00;
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    s16 unk_0A;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    BOOL unk_1C;
} UnkStruct_ov117_02266274;

typedef struct {
    UnkStruct_ov114_0225C76C unk_00;
    UnkStruct_ov114_0225C9A8 unk_10;
    OverlayManager *appMan;
    u8 unk_2C[4];
    u8 unk_30;
    u8 unk_31;
    u8 padding_32[2];
    int unk_34;
    BOOL unk_38;
    u8 unk_3C;
    u8 unk_3D;
    u8 unk_3E;
    u8 padding_3F;
} UnkStruct_ov117_022605C0;

typedef struct {
    UnkStruct_ov117_022621D4 unk_00[60];
} UnkStruct_ov117_02261280_sub1;

typedef struct {
    int unk_00;
    u8 padding_04[8];
    int unk_0C;
    int unk_10;
    int unk_14;
    u8 unk_18;
    u8 padding_19[3];
} UnkStruct_ov117_02262664;

typedef struct {
    s16 unk_00;
    s16 unk_02;
    u16 unk_04;
    u8 unk_06;
    u8 unk_07;
    u8 padding_08[2];
} UnkStruct_ov117_02266F10;

typedef struct {
    BOOL unk_00;
    s16 unk_04;
    s16 unk_06;
    s32 unk_08;
    s16 unk_0C;
    u8 unk_0E;
    u8 padding_0F[1];
    ManagedSprite *unk_10;
    const UnkStruct_ov117_02266F10 *unk_14;
} UnkStruct_ov117_02263EF8;

typedef struct {
    ManagedSprite *unk_00;
    fx32 unk_04;
    fx32 unk_08;
    fx32 unk_0C;
    fx32 unk_10;
    s16 unk_14;
    u8 padding_16[2];
} UnkStruct_ov117_02264508;

typedef struct {
    BOOL unk_00;
    UnkStruct_ov117_02264508 unk_04[36];
    UnkStruct_ov117_02264508 unk_364[8];
    UnkStruct_ov117_02264508 unk_424[3];
    u8 padding_46C[4];
} UnkStruct_ov117_02264808;

typedef struct {
    ManagedSprite *unk_00;
    u8 unk_04;
    u8 unk_05;
    u8 unk_06;
    u8 padding_07;
    s16 unk_08;
    u8 padding_0A[2];
} UnkStruct_ov117_02264930;

typedef struct {
    Field3dModel unk_00;
    Field3dObject unk_10;
} UnkStruct_ov117_02261C2C;

typedef struct {
    Field3dModel unk_00;
    Field3dObject unk_10;
    Field3dModel unk_88[4];
    struct {
        int unk_00;
        Field3dObject unk_04;
        Field3DModelAnimation unk_7C;
    } unk_C8[8][4];
} UnkStruct_ov117_02261F08;

typedef struct UnkStruct_ov117_02261280_t {
    UnkStruct_ov117_022605C0 *unk_00;
    int unk_04;
    u32 unk_08;
    u32 unk_0C[4];
    UnkStruct_ov114_0225CAD4 *unk_1C;
    int unk_20;
    SpriteSystem *unk_24;
    SpriteManager *unk_28;
    BgConfig *unk_2C;
    Window unk_30[5];
    MsgData *unk_80;
    MessageFormat *unk_84;
    String *unk_88;
    PaletteData *unk_8C;
    UnkStruct_02012744 *unk_90;
    SysTask *unk_94;
    GF3DVramMan *unk_98;
    Camera *camera1;
    Camera *camera2;
    ParticleSystem *unk_A4;
    NNSFndAllocator unk_A8;
    UnkStruct_ov117_02262664 unk_B8;
    UnkStruct_ov117_022626B0 *unk_D4;
    UnkStruct_ov117_02261F08 unk_D8;
    UnkStruct_ov117_02261C2C unk_13A0;
    UnkStruct_ov117_02263DAC unk_1428;
    UnkStruct_ov117_0226504C unk_1458;
    UnkStruct_ov117_022653F4 unk_1468;
    UnkStruct_ov117_02265C3C unk_1560;
    UnkStruct_ov117_02265EC8 unk_15A8;
    UnkStruct_ov117_02266274 unk_174C;
    UnkStruct_ov117_02261280_sub1 unk_176C;
    UnkStruct_ov117_02261280_sub1 unk_1C1C[4];
    UnkStruct_ov117_022621D4 unk_2EDC;
    UnkStruct_ov117_022621D4 unk_2EF0;
    int unk_2F04[4];
    int unk_2F14[4];
    int unk_2F24;
    int unk_2F28;
    UnkStruct_ov117_02264E84 unk_2F2C[8];
    int unk_2FAC;
    int unk_2FB0;
    u32 unk_2FB4;
    int unk_2FB8;
    int unk_2FBC;
    int unk_2FC0;
    int unk_2FC4;
    int unk_2FC8;
    int unk_2FCC;
    int unk_2FD0;
    int unk_2FD4;
    int unk_2FD8;
    UnkStruct_ov117_02263EF8 unk_2FDC[40];
    UnkStruct_ov117_02264808 unk_339C;
    UnkStruct_ov117_02264930 unk_380C[5];
    s32 unk_3848;
    u32 unk_384C;
} UnkStruct_ov117_02261280_t;

// sizeof(UnkStruct_ov117_02261280_t) == 0x3850

#endif // POKEHEARTGOLD_OVERLAY_93_UNK_STRUCT_OV117_02261280_T_H
