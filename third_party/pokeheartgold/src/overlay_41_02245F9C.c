#include "global.h"

#include "constants/heap.h"

#include "touchscreen.h"
#include "gf_gfx_planes.h"
#include "overlay_90_unk_struct_ov70_0225_ca20_t.h"
#include "overlay_41_unk_struct_ov22_0225500c.h"
#include "overlay_45_unk_struct_ov66_02234798.h"

struct SoftwareSprite;
struct SoftwareSpriteChars;
struct SoftwareSpriteManager;
struct SoftwareSpritePalette;
struct UnkStruct_ov63_0222BE18_t;
struct UnkStruct_ov63_0222BEC0_t;
struct UnkStruct_ov63_0222D848_t;
struct UnkStruct_ov66_02234548_t;
struct UnkStruct_ov66_02234798_t;
struct UnkStruct_ov70_0225C894_t;
struct UnkStruct_ov70_0225CA20_t;
struct UnkStruct_ov70_0225CC54_t;
typedef struct UnkStruct_ov70_0225C894_t UnkStruct_ov70_0225C894;
typedef struct UnkStruct_ov70_0225CA20_t UnkStruct_ov70_0225CA20;
typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;
typedef struct UnkStruct_ov63_0222BE18_t UnkStruct_ov63_0222BE18;
typedef struct UnkStruct_ov63_0222D848_t UnkStruct_ov63_0222D848;
typedef struct UnkStruct_ov66_02234548_t UnkStruct_ov66_02234548;
void sub_02015FC4(UnkStruct_ov66_02234548 *param0);
typedef struct {
    fx32 unk_00;
    fx32 unk_04;
    fx32 unk_08;
    u32 unk_0C;
} UnkStruct_ov70_0225D220;
typedef struct {
    u8 unk_00;
    u8 unk_01;
    s8 unk_02;
    s8 unk_03;
    s16 unk_04;
    s16 unk_06;
} UnkStruct_ov70_0225D93C;
typedef union {
    struct {
        s16 unk_00;
        s16 unk_02;
    } val1;
    struct {
        UnkStruct_ov70_0225D220 unk_00;
        u32 unk_10;
    } val2;
    struct {
        UnkStruct_ov70_0225D220 unk_00;
        u32 unk_10;
        fx32 unk_14;
    } val3;
    struct {
        s32 unk_00;
        VecFx32 unk_04;
    } val4;
} UnkStruct_ov70_0225CC54_sub1;
struct UnkStruct_ov70_0225CC54_t {
    UnkStruct_ov63_0222BEC0 *unk_00;
    UnkStruct_ov66_02234798 *unk_04;
    u16 unk_08;
    u8 unk_0A;
    u8 unk_0B;
    UnkStruct_ov70_0225CC54_sub1 unk_0C;
    void (*unk_24)(struct UnkStruct_ov70_0225CC54_t *, UnkStruct_ov70_0225CA20 *);
};
typedef struct SoftwareSprite SoftwareSprite;
NNSG2dSVec2 sub_02015FCC(SoftwareSprite *sprite);
typedef struct {
    int unk_00;
    SoftwareSprite *unk_04;
} UnkStruct_ov22_02255040;
NNSG2dSVec2 sub_02015FE8(SoftwareSprite *sprite);
void sub_02015FB0(SoftwareSprite *sprite, BOOL visible);
void sub_02015FF4(SoftwareSprite *sprite, s32 arg1);
void ov41_02245FD8(UnkStruct_ov22_02255040 *param0, int *param1, int *param2);
typedef struct {
    UnkStruct_ov22_02255040 *unk_00;
    int unk_04;
} UnkStruct_ov22_02254DE0;
typedef struct SoftwareSpriteChars SoftwareSpriteChars;
typedef struct SoftwareSpritePalette SoftwareSpritePalette;
typedef struct SoftwareSpriteManager SoftwareSpriteManager;
typedef struct SoftwareSpriteTemplate {
    SoftwareSpriteManager *softSpriteMan;
    SoftwareSpriteChars *chars;
    SoftwareSpritePalette *palette;
    s16 xPos;
    s16 yPos;
    u16 rotation;
    fx32 alpha;
    int priority;
    u16 paletteSlot;
} SoftwareSpriteTemplate;
SoftwareSprite *sub_02015F8C(const SoftwareSpriteTemplate *template);

void ov41_02245F9C(UnkStruct_ov70_0225CA20 *param0);
void ov41_02245FA8(UnkStruct_ov22_02255040 *param0, int *param1, int *param2);
void ov41_02245FD8(UnkStruct_ov22_02255040 *param0, int *param1, int *param2);
void ov41_02246008(UnkStruct_ov22_02255040 *param0, BOOL param1);
void ov41_02246014(UnkStruct_ov22_02255040 *param0, int param1);
void ov41_02246020(UnkStruct_ov22_02255040 *param0, int *param1, int *param2);
UnkStruct_ov22_02255040 *ov41_0224607C(UnkStruct_ov22_02254DE0 *param0);
SoftwareSprite *ov41_022460A8(UnkStruct_ov22_0225500C *param0);
void ov41_022460DC(UnkStruct_ov22_02255040 *param0, TouchscreenHitbox *hitTable);

s32 ov41_022466D0(void);
s32 ov41_022466F0(void);
s32 ov41_02246778(void);
void ov41_02246130(void);
s32 ov41_022467C8(void);
s32 ov41_022467D4(void);
void ov41_02246150(void);

void ov41_02245F9C(UnkStruct_ov70_0225CA20 *param0) {
    sub_02015FC4(param0->unk_04);
}

void ov41_02245FA8(UnkStruct_ov22_02255040 *param0, int *param1, int *param2) {
    NNSG2dSVec2 v0;

    v0 = sub_02015FCC(param0->unk_04);
    *param1 = v0.x;
    *param2 = v0.y;
}

void ov41_02245FD8(UnkStruct_ov22_02255040 *param0, int *param1, int *param2) {
    NNSG2dSVec2 v0;

    v0 = sub_02015FE8(param0->unk_04);
    *param1 = v0.x;
    *param2 = v0.y;
}

void ov41_02246008(UnkStruct_ov22_02255040 *param0, BOOL param1) {
    sub_02015FB0(param0->unk_04, param1);
}

void ov41_02246014(UnkStruct_ov22_02255040 *param0, int param1) {
    sub_02015FF4(param0->unk_04, param1);
}

void ov41_02246020(UnkStruct_ov22_02255040 *param0, int *param1, int *param2) {
    int v0, v1;

    ov41_02245FD8(param0, &v0, &v1);

    switch (v0) {
    case 16:
        *param1 = 0;
        break;
    case 32:
        *param1 = 10;
        break;
    case 64:
        *param1 = 20;
        break;
    }

    switch (v1) {
    case 16:
        *param2 = 0;
        break;
    case 32:
        *param2 = 10;
        break;
    case 64:
        *param2 = 20;
        break;
    }
}

UnkStruct_ov22_02255040 *ov41_0224607C(UnkStruct_ov22_02254DE0 *param0) {
    int v0;

    for (v0 = 0; v0 < param0->unk_04; v0++) {
        if (param0->unk_00[v0].unk_04 == 0L) {
            return param0->unk_00 + v0;
        }
    }

    return 0L;
}

SoftwareSprite *ov41_022460A8(UnkStruct_ov22_0225500C *param0) {
    SoftwareSpriteTemplate v0;

    v0.softSpriteMan = param0->unk_04;
    v0.chars = param0->unk_08;
    v0.palette = param0->unk_0C;
    v0.xPos = param0->unk_10;
    v0.yPos = param0->unk_14;
    v0.rotation = 0;
    v0.alpha = 31;
    v0.priority = 0;
    v0.paletteSlot = param0->unk_1C;

    return sub_02015F8C(&v0);
}

void ov41_022460DC(UnkStruct_ov22_02255040 *param0, TouchscreenHitbox *hitTable) {
    NNSG2dSVec2 v0 = sub_02015FCC(param0->unk_04);
    NNSG2dSVec2 v1 = sub_02015FE8(param0->unk_04);

    hitTable->rect.top = v0.y;
    hitTable->rect.bottom = v0.y + v1.y;
    hitTable->rect.left = v0.x;
    hitTable->rect.right = v0.x + v1.x;
}

void ov41_02246130(void) {
    ov41_022466D0();
    ov41_022466F0();
    ov41_02246778();
    gSystem.screensFlipped = 1;
    GfGfx_SwapDisplay();
}

void ov41_02246150(void) {
    gSystem.screensFlipped = 0;
    GfGfx_SwapDisplay();
    ov41_022467D4();
    ov41_022467C8();
    GX_ResetBankForTex();
}
