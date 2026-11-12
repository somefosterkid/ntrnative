// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov310/percentage.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/research_radar/percentage.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"

enum {
    ACTOR_PERCENT,
    ACTOR_HUNDREDS,
    ACTOR_TENS,
    ACTOR_ONES,
    ACTOR_COUNT,
};

struct Percentage {
    HeapID heapId;
    u8 value;
    int x;
    int y;
    ClActUnit *unit;
    ClActor *actors[ACTOR_COUNT];
    PercentageResources resources;
};

static void func_ov217_021c2748(Percentage *percentage, BOOL visible);
static void func_ov217_021c278c(Percentage *percentage);
static void func_ov217_021c2888(Percentage *percentage, u8 value);
static void func_ov217_021c288c(Percentage *percentage, int x, int y);
static u8 func_ov217_021c2894(Percentage *percentage);
static u8 func_ov217_021c28a4(Percentage *percentage);
static u8 func_ov217_021c28bc(Percentage *percentage);
static void func_ov217_021c28cc(const Percentage *percentage, s16 *x, s16 *y);
static void func_ov217_021c28dc(const Percentage *percentage, s16 *x, s16 *y);
static void func_ov217_021c28ec(const Percentage *percentage, s16 *x, s16 *y);
static Percentage *func_ov217_021c28fc(HeapID heapId);
static void func_ov217_021c2914(Percentage *percentage);
static void func_ov217_021c291c(Percentage *percentage, HeapID heapId);
static void func_ov217_021c2934(Percentage *percentage, const PercentageResources *resources, ClActUnit *unit);
static void func_ov217_021c2950(Percentage *percentage);
static void func_ov217_021c2958(Percentage *percentage, ClActUnit *unit);
static void func_ov217_021c295c(Percentage *percentage);
static void func_ov217_021c299c(Percentage *percentage);
static void func_ov217_021c29b4(Percentage *percentage, const PercentageResources *resources);

// pokebw2's Percentage_Create
Percentage *func_ov217_021c26e8(HeapID heapId, const PercentageResources *resources, ClActUnit *unit) {
    Percentage *percentage = func_ov217_021c28fc(heapId);

    func_ov217_021c291c(percentage, heapId);
    func_ov217_021c2934(percentage, resources, unit);
    return percentage;
}

// pokebw2's Percentage_Delete
void func_ov217_021c270c(Percentage *percentage) {
    func_ov217_021c2950(percentage);
    func_ov217_021c2914(percentage);
}

// pokebw2's Percentage_SetPos
void func_ov217_021c271c(Percentage *percentage, int x, int y) {
    func_ov217_021c288c(percentage, x, y);
    func_ov217_021c278c(percentage);
}

// pokebw2's Percentage_SetValue
void func_ov217_021c272c(Percentage *percentage, u32 value) {
    func_ov217_021c2888(percentage, value);
    func_ov217_021c278c(percentage);
}

// pokebw2's Percentage_SetVisible
void func_ov217_021c2740(Percentage *percentage, BOOL visible) {
    func_ov217_021c2748(percentage, visible);
}

// The hundreds are shown only when they aren't 0, and the tens only when they or the hundreds aren't
// pokebw2's Percentage_SetVisibleCore
static void func_ov217_021c2748(Percentage *percentage, BOOL visible) {
    func_0204b3dc(percentage->actors[ACTOR_PERCENT], visible);
    if (func_ov217_021c28bc(percentage)) {
        func_0204b3dc(percentage->actors[ACTOR_HUNDREDS], visible);
    }
    if (func_ov217_021c28bc(percentage) || func_ov217_021c28a4(percentage)) {
        func_0204b3dc(percentage->actors[ACTOR_TENS], visible);
    }
    func_0204b3dc(percentage->actors[ACTOR_ONES], visible);
}

// pokebw2's Percentage_UpdateActors
static void func_ov217_021c278c(Percentage *percentage) {
    ClActorPos pos;
    ClActor *actor = percentage->actors[ACTOR_PERCENT];
    PercentageResources *resources = &percentage->resources;

    pos.x = percentage->x;
    pos.y = percentage->y;
    func_0204b404(actor, &pos, resources->surface);
    func_0204b878(actor, resources->percentSequence);
    func_0204b8c4(actor, 0);
    func_0204b974(actor);
    func_0204b3dc(actor, FALSE);

    actor = percentage->actors[ACTOR_HUNDREDS];
    func_ov217_021c28ec(percentage, &pos.x, &pos.y);
    func_0204b404(actor, &pos, resources->surface);
    func_0204b878(actor, resources->digitSequence);
    func_0204b8c4(actor, func_ov217_021c28bc(percentage) * FX32_ONE);
    func_0204b974(actor);
    func_0204b3dc(actor, FALSE);

    actor = percentage->actors[ACTOR_TENS];
    func_ov217_021c28dc(percentage, &pos.x, &pos.y);
    func_0204b404(actor, &pos, resources->surface);
    func_0204b878(actor, resources->digitSequence);
    func_0204b8c4(actor, func_ov217_021c28a4(percentage) * FX32_ONE);
    func_0204b974(actor);
    func_0204b3dc(actor, FALSE);

    actor = percentage->actors[ACTOR_ONES];
    func_ov217_021c28cc(percentage, &pos.x, &pos.y);
    func_0204b404(actor, &pos, resources->surface);
    func_0204b878(actor, resources->digitSequence);
    func_0204b8c4(actor, func_ov217_021c2894(percentage) * FX32_ONE);
    func_0204b974(actor);
    func_0204b3dc(actor, FALSE);
}

// pokebw2's Percentage_SetValueCore
static void func_ov217_021c2888(Percentage *percentage, u8 value) {
    percentage->value = value;
}

// pokebw2's Percentage_SetPosCore
static void func_ov217_021c288c(Percentage *percentage, int x, int y) {
    percentage->x = x;
    percentage->y = y;
}

// pokebw2's Percentage_GetOnes
static u8 func_ov217_021c2894(Percentage *percentage) {
    return percentage->value % 10;
}

// pokebw2's Percentage_GetTens
static u8 func_ov217_021c28a4(Percentage *percentage) {
    return percentage->value % 100 / 10;
}

// pokebw2's Percentage_GetHundreds
static u8 func_ov217_021c28bc(Percentage *percentage) {
    return percentage->value / 100;
}

// pokebw2's Percentage_GetOnesPos
static void func_ov217_021c28cc(const Percentage *percentage, s16 *x, s16 *y) {
    *x = percentage->x + 20;
    *y = percentage->y - 8;
}

// pokebw2's Percentage_GetTensPos
static void func_ov217_021c28dc(const Percentage *percentage, s16 *x, s16 *y) {
    *x = percentage->x + 15;
    *y = percentage->y - 8;
}

// pokebw2's Percentage_GetHundredsPos
static void func_ov217_021c28ec(const Percentage *percentage, s16 *x, s16 *y) {
    *x = percentage->x + 10;
    *y = percentage->y - 8;
}

// pokebw2's Percentage_Alloc
static Percentage *func_ov217_021c28fc(HeapID heapId) {
    return GFL_HeapAllocate(heapId, sizeof(Percentage), FALSE, "percentage.c", 420);
}

// pokebw2's Percentage_Free
static void func_ov217_021c2914(Percentage *percentage) {
    GFL_HeapFree(percentage);
}

// pokebw2's Percentage_InitWork
static void func_ov217_021c291c(Percentage *percentage, HeapID heapId) {
    int i;

    percentage->heapId = heapId;
    percentage->value = 0;
    percentage->unit = NULL;
    for (i = 0; i < ACTOR_COUNT; i++) {
        percentage->actors[i] = NULL;
    }
}

// pokebw2's Percentage_Setup
static void func_ov217_021c2934(Percentage *percentage, const PercentageResources *resources, ClActUnit *unit) {
    func_ov217_021c2958(percentage, unit);
    func_ov217_021c29b4(percentage, resources);
    func_ov217_021c295c(percentage);
}

// pokebw2's Percentage_Teardown
static void func_ov217_021c2950(Percentage *percentage) {
    func_ov217_021c299c(percentage);
}

// pokebw2's Percentage_SetUnit
static void func_ov217_021c2958(Percentage *percentage, ClActUnit *unit) {
    percentage->unit = unit;
}

// pokebw2's Percentage_CreateActors
static void func_ov217_021c295c(Percentage *percentage) {
    ClActorSetup setup;
    PercentageResources *resources = &percentage->resources;
    int i;

    setup.x = 0;
    setup.y = 0;
    setup.sequence = 0;
    setup.priority = 0;
    setup.bgPriority = 0;
    for (i = 0; i < ACTOR_COUNT; i++) {
        percentage->actors[i] = func_0204b294(percentage->unit, resources->chars, resources->palette,
                                              resources->cellAnims, &setup, resources->surface, percentage->heapId);
    }
}

// pokebw2's Percentage_DeleteActors
static void func_ov217_021c299c(Percentage *percentage) {
    int i;

    for (i = 0; i < ACTOR_COUNT; i++) {
        func_0204b3b4(percentage->actors[i]);
    }
}

// pokebw2's Percentage_SetResources
static void func_ov217_021c29b4(Percentage *percentage, const PercentageResources *resources) {
    percentage->resources = *resources;
}
