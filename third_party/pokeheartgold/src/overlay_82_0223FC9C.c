#include "global.h"

#include "heap.h"
#include "pokemon_icon_idx.h"
#include "pokemon_types_def.h"
#include "seal_case.h"

struct Sprite;
struct UnkStruct_ov63_0222BE18_t;
struct UnkStruct_ov63_0222BEC0_t;
struct UnkStruct_ov63_0222D848_t;
struct UnkStruct_ov66_02234548_t;
struct UnkStruct_ov66_02234798_t;
struct UnkStruct_ov70_0225C894_t;
struct UnkStruct_ov70_0225CA20_t;
struct UnkStruct_ov70_0225CC54_t;
typedef struct Sprite Sprite;
void Sprite_Delete(Sprite *gfxElem);
typedef struct BattleArcadeAppSprite {
    u32 unk_00;
    u32 unk_04;
    u16 unk_08;
    u8 unk_0A;
    u8 onSubScreen;
    u16 x;
    u16 y;
    Sprite *sprite;
} BattleArcadeAppSprite;
typedef struct {
    u16 unk_00;
    u8 unk_02;
    u8 unk_03;
} UnkStruct_ov63_0222D894;
typedef struct UnkStruct_ov70_0225C894_t UnkStruct_ov70_0225C894;
typedef struct UnkStruct_ov70_0225CA20_t UnkStruct_ov70_0225CA20;
typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;
typedef struct UnkStruct_ov63_0222BE18_t UnkStruct_ov63_0222BE18;
typedef struct UnkStruct_ov63_0222BEC0_t UnkStruct_ov63_0222BEC0;
typedef struct UnkStruct_ov63_0222D848_t UnkStruct_ov63_0222D848;
void Sprite_SetDrawFlag(UnkStruct_ov63_0222D848 *param0, const UnkStruct_ov63_0222D894 *param1);
typedef struct UnkStruct_ov66_02234548_t UnkStruct_ov66_02234548;
typedef struct UnkStruct_ov66_02234798_t UnkStruct_ov66_02234798;
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
struct UnkStruct_ov70_0225CA20_t {
    UnkStruct_ov63_0222BE18 *unk_00;
    UnkStruct_ov66_02234548 *unk_04;
    UnkStruct_ov70_0225CC54 *unk_08;
    u16 unk_0C;
    u16 unk_0E;
    UnkStruct_ov63_0222D848 *unk_10;
    const UnkStruct_ov70_0225C894 *unk_14;
    UnkStruct_ov70_0225D93C unk_18;
    UnkStruct_ov70_0225CC54 *unk_20;
    UnkStruct_ov70_0225CC54 *unk_24;
};
void Sprite_SetMatrix(Sprite *sprite, const VecFx32 *position);
const VecFx32 *Sprite_GetMatrixPtr(const Sprite *sprite);
void Sprite_TryChangeAnimSeq(Sprite *sprite, u32 animID);
typedef struct BattleHallAppSprite {
    u32 unused[3];
    const u8 *isAlwaysNull;
    Sprite *sprite;
} BattleHallAppSprite;
void Sprite_SetAnimSpeed(Sprite *sprite, fx32 speed);
void Sprite_SetPalOffsetRespectVramOffset(Sprite *sprite, u8 paletteOffset);

void *ov82_0223FC9C(BattleArcadeAppSprite *appSprite);
void ov82_0223FCB0(UnkStruct_ov70_0225CA20 *param0, const UnkStruct_ov63_0222D894 *param1);
void ov82_0223FCBC(BattleHallAppSprite *sprite, u16 x, u16 y);
void ov82_0223FCFC(BattleArcadeAppSprite *appSprite, u32 animID);
void ov82_0223FD18(BattleArcadeAppSprite *appSprite, Pokemon *mon);

void *ov82_0223FC9C(BattleArcadeAppSprite *appSprite) {
    Sprite_Delete(appSprite->sprite);
    Heap_Free(appSprite);

    return 0L;
}

void ov82_0223FCB0(UnkStruct_ov70_0225CA20 *param0, const UnkStruct_ov63_0222D894 *param1) {
    Sprite_SetDrawFlag(param0->unk_10, param1);
}

void ov82_0223FCBC(BattleHallAppSprite *sprite, u16 x, u16 y) {
    if (sprite->isAlwaysNull != 0L) {
        Sprite_TryChangeAnimSeq(sprite->sprite, sprite->isAlwaysNull[0]);
    }

    VecFx32 position = *Sprite_GetMatrixPtr(sprite->sprite);

    position.x = x * ((fx32)0x0000000000001000L);
    position.y = y * ((fx32)0x0000000000001000L);

    Sprite_SetMatrix(sprite->sprite, &position);
}

void ov82_0223FCFC(BattleArcadeAppSprite *appSprite, u32 animID) {
    Sprite_SetAnimSpeed(appSprite->sprite, ((fx32)0x0000000000001000L));
    Sprite_TryChangeAnimSeq(appSprite->sprite, animID);
}

void ov82_0223FD18(BattleArcadeAppSprite *appSprite, Pokemon *mon) {
    Sprite_SetPalOffsetRespectVramOffset(appSprite->sprite, Pokemon_GetIconPalette(mon));
}
