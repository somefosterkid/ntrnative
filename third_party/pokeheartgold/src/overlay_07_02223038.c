#include "global.h"

#include "overlay_07_battle_anim_funcs.h"
#include "sprite.h"
#include "sprite_system.h"
#include "heap.h"
#include "sys_task_api.h"
#include "constants/heap.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"

struct BattleAnimSystem;

typedef void (*BattleAnimScriptFunc)(struct BattleAnimSystem *system);

extern const BattleAnimScriptFunc ov07_02236520[84];

BattleAnimScriptFunc ov07_02223038(u32 id);

void ov07_02223060(void);
typedef struct AnimExampleContext {
    u8 state;
    BattleAnimSystem *system;
} AnimExampleContext;
void ov07_02223064(SysTask *task, void *param);
void ov07_0222308C(BattleAnimSystem *system);
void ov07_0221C458(BattleAnimSystem *system, SysTask *task);
void ov07_022230B4(SysTask *task, void *param);
void ov07_022230DC(BattleAnimSystem *system);
typedef struct GenericExampleContext {
    u8 state;
    BattleAnimSystem *system;
} GenericExampleContext;
void ov07_02223104(SysTask *task, void *param);

void ov07_02223128(BattleAnimSystem *system);

BattleAnimScriptFunc ov07_02223038(u32 id) {
    if (id >= NELEMS(ov07_02236520)) {
        return NULL;
    }

    return ov07_02236520[id];
}

typedef void (*BattleAnimScriptSpriteFunc)(struct BattleAnimSystem *, SpriteSystem *, SpriteManager *, ManagedSprite *);

extern const BattleAnimScriptSpriteFunc ov07_0223649C[33];

BattleAnimScriptSpriteFunc ov07_0222304C(u32 id);

BattleAnimScriptSpriteFunc ov07_0222304C(u32 id) {
    if (id >= NELEMS(ov07_0223649C)) {
        return NULL;
    }

    return ov07_0223649C[id];
}

void ov07_02223060(void) {
}

void ov07_02223064(SysTask *task, void *param) {
    AnimExampleContext *ctx = param;

    switch (ctx->state) {
    case 0:
        ctx->state++;
        break;
    case 1:
        ov07_0221C448(ctx->system, task);
        Heap_Free(ctx);
        break;
    }
}

void ov07_0222308C(BattleAnimSystem *system) {
    AnimExampleContext *ctx = Heap_Alloc(HEAP_ID_BATTLE, sizeof(AnimExampleContext));

    ctx->state = 0;
    ctx->system = system;

    ov07_0221C3F4(ctx->system, ov07_02223064, ctx, 0x1000);
}

void ov07_022230B4(SysTask *task, void *param) {
    AnimExampleContext *ctx = param;

    switch (ctx->state) {
    case 0:
        ctx->state++;
        break;
    case 1:
        ov07_0221C458(ctx->system, task);
        Heap_Free(ctx);
        break;
    }
}

void ov07_022230DC(BattleAnimSystem *system) {
    AnimExampleContext *ctx = Heap_Alloc(HEAP_ID_BATTLE, sizeof(AnimExampleContext));

    ctx->state = 0;
    ctx->system = system;

    ov07_0221C42C(ctx->system, ov07_022230B4, ctx, 0x1000);
}

void ov07_02223104(SysTask *task, void *param) {
    GenericExampleContext *ctx = param;

    switch (ctx->state) {
    case 0:
        ctx->state++;
        break;
    case 1:
        SysTask_Destroy(task);
        Heap_Free(ctx);
        break;
    }
}

void ov07_02223128(BattleAnimSystem *system) {
    GenericExampleContext *ctx = Heap_Alloc(HEAP_ID_BATTLE, sizeof(GenericExampleContext));

    ctx->state = 0;
    ctx->system = system;

    SysTask_CreateOnMainQueue(ov07_02223104, ctx, 0x1000);
}
