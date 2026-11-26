#include "global.h"

#include "battle_castle_app_sprite.h"
#include "pokemon_icon_idx.h"
#include "pokemon_types_def.h"
#include "seal_case.h"
#include "unk_02005D10.h"
#include "overlay_90_unk_struct_ov70_0225_ca20_t.h"
#include "overlay_45_unk_struct_ov66_02234798.h"

struct Sprite;
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
void ov80_0222A3D4(Sprite *sprite, u8 animID);
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
void ov80_0222A400(Sprite *sprite, s16 x, s16 y, u8 isSelected);

void ov83_022475EC(BattleCastleAppSprite *sprite, Pokemon *mon);
void ov83_02247600(BattleCastleAppSprite *, u8);
void ov83_0224760C(BattleCastleAppSprite *sprite, u8 isSelected);
BOOL ov83_02247624(BattleCastleAppSprite *sprite);

void ov83_0224755C(BattleCastleAppSprite *sprite, BOOL flag);
#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov83_02247568(void *, u16, u16);
#else
s32 ov83_02247568(void *, s32, s32);
#endif
s32 ov83_022475D4(void *, s32);
void ov83_02247630(void *arg0, s32 arg1, s32 arg2);

void ov83_022475EC(BattleCastleAppSprite *sprite, Pokemon *mon) {
    Sprite_SetPalOffsetRespectVramOffset(sprite->sprite, Pokemon_GetIconPalette(mon));
}

void ov83_02247600(BattleCastleAppSprite *sprite, u8 animID) {
    ov80_0222A3D4(sprite->sprite, animID);
}

void ov83_0224760C(BattleCastleAppSprite *sprite, u8 isSelected) {
    ov80_0222A400(sprite->sprite, sprite->x, sprite->y, isSelected);
}

BOOL ov83_02247624(BattleCastleAppSprite *sprite) {
    return Sprite_IsAnimated(sprite->sprite);
}

void ov83_02247630(void *arg0, s32 arg1, s32 arg2) {
    ov83_022475D4(arg0, 0xB);
    ov83_02247568(arg0, arg1, arg2);
    ov83_0224755C(arg0, 1);
    PlaySE(0x5E3U);
    PlaySE(0x655U);
}
