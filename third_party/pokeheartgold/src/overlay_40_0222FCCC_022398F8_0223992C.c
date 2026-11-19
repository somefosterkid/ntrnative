#include "global.h"
#include "sprite.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_40_unk_struct_ov62_022312b0.h"

struct BgConfig;

struct CharTransferAllocation;

struct TextOBJ;

struct ManagedSprite;

struct MsgData;

struct String;

struct TouchScreenActions;

struct UnkStruct_02012B20_t;

struct Window;

typedef struct CharTransferAllocation {
    u32 size;
    u32 offset;
    u16 vramType;
    u16 atEnd;
} CharTransferAllocation;

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
    u8 unk_00;
    u8 unk_01[3];
    u32 unk_04;
} UnkStruct_ov61_0222B2D8;

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

typedef struct TouchScreenActions TouchScreenActions;

typedef struct {
    u8 unk_00;
    u8 unk_01[3];
    u8 unk_04[16];
    u64 unk_14[16];
    u8 unk_94[12];
    u64 unk_A0[12];
    u16 unk_100[20];
    u64 unk_128[20];
} UnkStruct_ov61_0222BDAC_sub1;

typedef struct {
    UnkStruct_ov61_0222BDAC_sub1 unk_00[3];
} UnkStruct_ov61_0222BDAC;

typedef struct {
    u8 unk_00;
    u8 unk_01[3];
    u8 unk_04[16];
    u8 unk_14[12];
    u16 unk_20[20];
} UnkStruct_ov61_0222BDAC_1_sub1;

typedef struct {
    UnkStruct_ov61_0222BDAC_1_sub1 unk_00[3];
} UnkStruct_ov61_0222BDAC_1;

typedef struct {
    u8 unk_00[3];
    u8 unk_03;
} UnkStruct_ov62_0223DF10;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    int unk_20;
    Window unk_24[6];
    Window unk_84[2];
    UnkStruct_ov62_022312B0 unk_A4[2];
    TouchScreenActions *unk_DC;
    UnkStruct_ov61_0222BDAC unk_E0;
    UnkStruct_ov61_0222BDAC_1 unk_638;
    UnkStruct_ov62_0223DF10 unk_710;
    UnkStruct_ov61_0222BDAC *unk_714;
    UnkStruct_ov61_0222BDAC_1 *unk_718;
    UnkStruct_ov62_0223DF10 *unk_71C;
    UnkStruct_ov62_0223D518_sub1 unk_720;
    MsgData *unk_744;
    String *unk_748;
    UnkStruct_ov62_02249380 unk_74C[3][20];
    UnkStruct_ov62_02248CDC unk_B0C;
} UnkStruct_ov62_0223D518;

void ov40_0223992C(UnkStruct_ov62_0223D518 *param0);

void ov40_0223992C(UnkStruct_ov62_0223D518 *param0) {
    int v0[] = { 16, 12, 20 };

    param0->unk_20 = v0[param0->unk_14];
}
