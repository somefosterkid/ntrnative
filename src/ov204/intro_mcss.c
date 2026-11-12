// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov294/intro_mcss.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "demo/intro.h"
#include "gfl/graphics.h"
#include "gfl/random.h"
#include "nitro/ioreg.h"
#include "pml/poke_party.h"
#include "system/mcss.h"

// The intro's sprites, drawn by MCSS

#define SPRITE_COUNT 8

typedef struct {
    MCSS *mcss;
    BOOL animationEnded;
    // Whether the animation goes back to its first frame when it ends
    BOOL restartAtEnd;
} IntroSprite;

struct IntroMcss {
    HeapID heapId;
    MCSSSystem *system;
    IntroSprite sprites[SPRITE_COUNT];
    // Frames until the Pokémon plays its animation again
    u32 idleTimer;
};

static void func_ov204_021bf7f0(uintptr_t param, fx32 frame);

// pokebw2's sScale
static const VecFx32 data_ov204_021c08f0 = { FX32_CONST(16), FX32_CONST(16), FX32_ONE };
// Pokémon face the other way
// pokebw2's sPokemonScale
static const VecFx32 data_ov204_021c08fc = { -FX32_CONST(16), FX32_CONST(16), FX32_ONE };

// pokebw2's IntroMcss_Create
IntroMcss *func_ov204_021bf594(HeapID heapId, u32 mode) {
    IntroMcss *mcss = GFL_HeapAllocate(heapId, sizeof(IntroMcss), TRUE, "intro_mcss.c", 98);

    mcss->system = func_02014b08(SPRITE_COUNT, heapId);
    mcss->heapId = heapId;
    if (mode != 7) {
        func_02016020(mcss->system, 0x70000);
    }
    func_02016024(mcss->system, 0x4000);
    func_02015bf0(mcss->system);
    return mcss;
}

// pokebw2's IntroMcss_Free
void func_ov204_021bf5e0(IntroMcss *mcss) {
    int i;

    for (i = 0; i < SPRITE_COUNT; i++) {
        if (mcss->sprites[i].mcss != NULL) {
            func_02015ba4(mcss->system, mcss->sprites[i].mcss);
        }
    }
    func_02014ba8(mcss->system);
    GFL_HeapFree(mcss);
}

// pokebw2's IntroMcss_Draw
void func_ov204_021bf610(IntroMcss *mcss) {
    func_02014c08(mcss->system);
    func_02014d2c(mcss->system);
}

// pokebw2's IntroMcss_Add
void func_ov204_021bf624(IntroMcss *mcss, fx32 x, fx32 y, fx32 z, const MCSSLoadInfo *info, u8 index) {
    VecFx32 scale = data_ov204_021c08f0;

    mcss->sprites[index].mcss = func_020159c8(mcss->system, x, y, z, info);
    func_02015cac(mcss->sprites[index].mcss, &scale);
    mcss->sprites[index].animationEnded = FALSE;
    mcss->sprites[index].restartAtEnd = TRUE;
    func_02015f2c(mcss->sprites[index].mcss, (uintptr_t)&mcss->sprites[index], func_ov204_021bf7f0, 0);
}

// pokebw2's IntroMcss_AddPokemon
void func_ov204_021bf67c(IntroMcss *mcss, fx32 x, fx32 y, fx32 z, u32 species, u8 index) {
    MCSSLoadInfo info;
    VecFx32 scale = data_ov204_021c08fc;
    PartyPkm *pkm = PokeParty_NewTempPkm(species, 0, 0, mcss->heapId);

    func_02017100(pkm, &info, 0);
    GFL_HeapFree(pkm);
    mcss->sprites[index].mcss = func_020159c8(mcss->system, x, y, z, &info);
    func_02015cac(mcss->sprites[index].mcss, &scale);
    func_02015e04(mcss->sprites[index].mcss);
    mcss->sprites[index].animationEnded = FALSE;
    mcss->sprites[index].restartAtEnd = TRUE;
    mcss->idleTimer = GFL_RandomMTRange(5) * 20 + 200;
    func_02015f2c(mcss->sprites[index].mcss, (uintptr_t)&mcss->sprites[index], func_ov204_021bf7f0, 0);
}

// pokebw2's IntroMcss_SetVisible
void func_ov204_021bf720(IntroMcss *mcss, BOOL visible, u8 index) {
    if (visible == TRUE) {
        func_02015eb0(mcss->sprites[index].mcss);
    } else {
        func_02015ea0(mcss->sprites[index].mcss);
    }
}

// pokebw2's IntroMcss_SetAnimation
void func_ov204_021bf744(IntroMcss *mcss, u8 index, u32 animation, BOOL restartAtEnd) {
    func_02016028(mcss->sprites[index].mcss, animation);
    func_02015e4c(mcss->sprites[index].mcss);
    mcss->sprites[index].animationEnded = FALSE;
    mcss->sprites[index].restartAtEnd = restartAtEnd;
    func_02015f2c(mcss->sprites[index].mcss, (uintptr_t)&mcss->sprites[index], func_ov204_021bf7f0, 0);
}

// pokebw2's IntroMcss_IsAnimationEnded
BOOL func_ov204_021bf77c(IntroMcss *mcss, u8 index) {
    return mcss->sprites[index].animationEnded;
}

// pokebw2's IntroMcss_ClearAnimationEnded
void func_ov204_021bf788(IntroMcss *mcss, u8 index) {
    mcss->sprites[index].animationEnded = FALSE;
}

// pokebw2's IntroMcss_SetAlpha
void func_ov204_021bf794(IntroMcss *mcss, u8 index, u32 alpha) {
    G2_SetBlendAlpha(1, 9, 0, 0);
    if (alpha > 31) {
        alpha = 31;
    }
    func_02015fbc(mcss->sprites[index].mcss, alpha);
}

void func_ov204_021bf7cc(IntroMcss *mcss, u8 index, BOOL a2) {
    if (a2) {
        func_02015db0(mcss->sprites[index].mcss);
    } else {
        func_02015dc0(mcss->sprites[index].mcss);
    }
}

// pokebw2's IntroMcss_OnAnimationEnd
static void func_ov204_021bf7f0(uintptr_t param, fx32 frame) {
    IntroSprite *sprite = (IntroSprite *)param;

    sprite->animationEnded = TRUE;
    if (sprite->restartAtEnd == TRUE) {
        func_02016374(sprite->mcss);
    }
}

// Moves a sprite along x by step, and returns TRUE once it has reached target
// pokebw2's IntroMcss_MoveX
BOOL func_ov204_021bf804(IntroMcss *mcss, u8 index, fx32 step, fx32 target) {
    VecFx32 position;
    BOOL reached;

    func_02015c30(mcss->sprites[index].mcss, &position);
    reached = TRUE;
    position.x += step;
    if (position.x != target) {
        if ((step > 0 && position.x > target) || (step < 0 && position.x < target)) {
            position.x = target;
        } else {
            reached = FALSE;
        }
    }
    func_02015c48(mcss->sprites[index].mcss, &position);
    if (mcss->sprites[index].animationEnded == TRUE) {
        func_02015e04(mcss->sprites[index].mcss);
    }
    return reached;
}

// Moves sprite 1 down y by step, and returns TRUE once it has reached min
// pokebw2's IntroMcss_DecreaseY
BOOL func_ov204_021bf864(IntroMcss *mcss, fx32 step, fx32 min) {
    VecFx32 position;
    BOOL reached;

    func_02015c30(mcss->sprites[1].mcss, &position);
    position.y -= step;
    if (position.y <= min) {
        position.y = min;
        reached = TRUE;
    } else {
        reached = FALSE;
    }
    func_02015c48(mcss->sprites[1].mcss, &position);
    return reached;
}

// Plays the animation of sprite 1 again every 200 to 280 frames, keeping it still in between
// pokebw2's IntroMcss_Update
void func_ov204_021bf898(IntroMcss *mcss) {
    if (mcss->sprites[1].animationEnded) {
        if (mcss->idleTimer == 0) {
            mcss->idleTimer = GFL_RandomMTRange(5) * 20 + 200;
            func_ov204_021bf744(mcss, 1, 0, TRUE);
        } else {
            func_02015e04(mcss->sprites[1].mcss);
            mcss->idleTimer--;
        }
    }
}
