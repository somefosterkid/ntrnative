#include "global.h"

#include "math_util.h"
#include "overlay_42.h"

struct UnkOv49Ctx;

struct UnkOv49Entity;

struct UnkOv49SpawnEntry;

struct UnkStruct_ov42_02122667;

struct UnkStruct_ov42_022280A8;

struct UnkStruct_ov42_02228110;

typedef struct UnkOv49Entity UnkOv49Entity;

typedef struct UnkOv49Ctx UnkOv49Ctx;

typedef void (*UnkOv49EntityFunc)(UnkOv49Entity *entity, UnkOv49Ctx *ctx);

struct UnkOv49Entity {
    UnkStruct_ov42_02228110 *unk0;
    void *unk4;
    u16 unk8;
    u8 unkA;
    u8 unkB;
    u8 unkC[0x18];
    UnkOv49EntityFunc unk24;
};

struct UnkOv49Ctx {
    UnkStruct_ov42_022280A8 *unk0;
    void *unk4;
    UnkOv49Entity *unk8;
    u16 unkC;
    u8 unkE[2];
    void *unk10;
    void *unk14;
    u8 unk18[8];
    UnkOv49Entity *unk20;
    UnkOv49Entity *unk24;
};

typedef struct UnkOv49SpawnEntry {
    u8 unk0;
    u8 unk1;
    u16 unk2;
    u8 unk4;
    u8 unk5[3];
} UnkOv49SpawnEntry;

extern BOOL ov49_022589D8(void *map, u32 id, u16 *x, u16 *y, u32 index);

extern UnkOv49Entity *ov49_022593C0(UnkOv49Ctx *ctx);

extern void *ov45_0223070C(void *manager, UnkStruct_ov42_02228110 *obj);

extern const UnkOv49SpawnEntry ov49_02269678[4];

void ov49_02258EEC(UnkOv49Ctx *ctx, UnkOv49Entity *entity, int kind);

void ov49_022591D8(UnkOv49Ctx *ctx);

void ov49_022591D8(UnkOv49Ctx *ctx) {
    int i;
    UnkOv49Entity *entity;
    u16 x, y;
    u16 index;
    UnkStruct_ov42_02122667 spawn;

    for (i = 0; (u32)i < 4; i++) {
        index = 0;
        while (ov49_022589D8(ctx->unk14, ov49_02269678[i].unk0, &x, &y, index) == TRUE) {
            entity = ov49_022593C0(ctx);

            spawn.unk0 = x * 16;
            spawn.unk2 = y * 16;
            spawn.unk4 = ov49_02269678[i].unk4;
            spawn.unk6 = 0;
            spawn.unk8 = ov49_02269678[i].unk1;
            spawn.unkA = ov49_02269678[i].unk2;
            entity->unk0 = ov42_022280B8(ctx->unk0, &spawn);
            entity->unk4 = ov45_0223070C(ctx->unk4, entity->unk0);
            ov49_02258EEC(ctx, entity, 0);

            if (ov49_02269678[i].unk0 == 0x61) {
                ctx->unk24 = entity;
            }
            index++;
        }
    }
}
