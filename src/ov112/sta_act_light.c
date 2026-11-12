// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov209/sta_act_light.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/musical/sta_act_light.h"
#include "app/musical/sta_act_poke.h"
#include "app/musical/sta_acting.h"
#include "constants/arc.h"
#include "gfl/arc.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"

// Overlay 209's sta_act_light.c: the stage's spotlights, 2D cell actors over the 3D stage, placed in fixed point
// pixels and scrolled with the stage

// The position of func_ov132_021f9e18 when no Pokémon is in the limelight
#define STA_ACT_LIGHT_NO_POKE 4

static void func_ov112_021f6ce0(StaActLightSys *sys, StaActLight *light);
static void func_ov112_021f6d68(StaActLightSys *sys);

// pokebw2's StaActLight_InitSystem
StaActLightSys *func_ov112_021f6bd8(HeapID heapId, StaActing *stage) {
    u8 i;
    ArcTool *arc;
    StaActLightSys *sys = GFL_HeapAllocate(heapId, sizeof(StaActLightSys), FALSE, "sta_act_light.c", 87);

    sys->heapId = heapId;
    sys->stage = stage;
    for (i = 0; i < 4; i++) {
        sys->lights[i].type = 0;
    }
    sys->followActive = FALSE;
    sys->clactUnit = func_0204b100(5, 0, sys->heapId);
    func_0204b270(sys->clactUnit);
    arc = GFL_ArcSysCreateFileHandle(ARCID_MUSICAL, sys->heapId);
    sys->palette = func_0204abf0(arc, 4, CLACT_VRAM_MAIN, 0, sys->heapId);
    sys->chars = func_0204a6c8(arc, 10, FALSE, CLACT_VRAM_MAIN, sys->heapId);
    sys->cellAnims = func_0204af28(arc, 20, 23, sys->heapId);
    GFL_ArcToolFree(arc);
    return sys;
}

// pokebw2's StaActLight_TermSystem
void func_ov112_021f6c7c(StaActLightSys *sys) {
    func_0204ada4(sys->palette);
    func_0204a8d4(sys->chars);
    func_0204afd8(sys->cellAnims);
    func_0204b1cc(sys->clactUnit);
    GFL_HeapFree(sys);
}

// pokebw2's StaActLight_UpdateSystem
void func_ov112_021f6cb0(StaActLightSys *sys) {
    int i;

    for (i = 0; i < 4; i++) {
        if (sys->lights[i].type != 0) {
            func_ov112_021f6ce0(sys, &sys->lights[i]);
        }
    }
    if (sys->stage != NULL) {
        func_ov112_021f6d68(sys);
    }
}

// pokebw2's StaActLight_UpdateLight
static void func_ov112_021f6ce0(StaActLightSys *sys, StaActLight *light) {
    u16 scroll = sys->stage == NULL ? 128 : func_ov132_021f9e04(sys->stage);

    if (light->type == 1) {
        ClActorPos pos;

        pos.x = FX_FX32_TO_F32(light->pos.x) - scroll;
        pos.y = FX_FX32_TO_F32(light->pos.y);
        func_0204b404(light->actor, &pos, CLACT_SURFACE_MAIN);
        if (sys->stage != NULL && func_ov132_021f9e18(sys->stage) != STA_ACT_LIGHT_NO_POKE) {
            func_0204b3dc(light->actor, FALSE);
        } else {
            func_0204b3dc(light->actor, TRUE);
        }
    }
}

// pokebw2's StaActLight_UpdateFollowLight
static void func_ov112_021f6d68(StaActLightSys *sys) {
    if (sys->followActive == FALSE && func_ov132_021f9e18(sys->stage) != STA_ACT_LIGHT_NO_POKE) {
        ClActorSetup setup;

        sys->followActive = TRUE;
        setup.x = 128;
        setup.y = 96;
        setup.sequence = 0;
        setup.priority = 0;
        setup.bgPriority = 2;
        sys->followActor = func_0204b294(sys->clactUnit, sys->chars, sys->palette, sys->cellAnims, &setup,
                                         CLACT_SURFACE_MAIN, sys->heapId);
        func_0204b3dc(sys->followActor, TRUE);
    }
    if (sys->followActive == TRUE) {
        StaActPokeSys *pokeSys;
        StaActPoke *poke;
        u16 scroll;
        VecFx32 pokePos;
        ClActorPos pos;

        if (func_ov132_021f9e18(sys->stage) == STA_ACT_LIGHT_NO_POKE) {
            func_0204b3b4(sys->followActor);
            sys->followActive = FALSE;
            return;
        }
        pokeSys = func_ov132_021f9da0(sys->stage);
        poke = func_ov132_021f9da4(sys->stage, func_ov132_021f9e18(sys->stage));
        scroll = func_ov132_021f9e04(sys->stage);
        func_ov112_021f8044(pokeSys, poke, &pokePos);
        pos.x = FX_FX32_TO_F32(pokePos.x) - scroll;
        pos.y = FX_FX32_TO_F32(pokePos.y) - 32.0f;
        func_0204b404(sys->followActor, &pos, CLACT_SURFACE_MAIN);
    }
}

// pokebw2's StaActLight_DrawSystem
void func_ov112_021f6e74(StaActLightSys *sys) {
}

// pokebw2's StaActLight_AddLight
StaActLight *func_ov112_021f6e78(StaActLightSys *sys, u32 type) {
    u8 i;
    StaActLight *light;
    ClActorSetup setup;

    for (i = 0; i < 4; i++) {
        if (sys->lights[i].type == 0) {
            break;
        }
    }
    sys->lights[i].type = type;
    light = &sys->lights[i];
    setup.x = 128;
    setup.y = 96;
    setup.sequence = 0;
    setup.priority = 0;
    setup.bgPriority = 2;
    light->actor = func_0204b294(sys->clactUnit, sys->chars, sys->palette, sys->cellAnims, &setup, CLACT_SURFACE_MAIN,
                                 sys->heapId);
    func_0204b3dc(light->actor, TRUE);
    return light;
}

// pokebw2's StaActLight_DelLight
void func_ov112_021f6ee4(StaActLightSys *sys, StaActLight *light) {
    func_0204b3b4(light->actor);
    light->type = 0;
}

// pokebw2's StaActLight_SetPosition
void func_ov112_021f6ef4(StaActLightSys *sys, StaActLight *light, VecFx32 *pos) {
    light->pos.x = pos->x;
    light->pos.y = pos->y;
    light->pos.z = pos->z;
}

// pokebw2's StaActLight_GetPosition
void func_ov112_021f6f04(StaActLightSys *sys, StaActLight *light, VecFx32 *pos) {
    pos->x = light->pos.x;
    pos->y = light->pos.y;
    pos->z = light->pos.z;
}

// pokebw2's StaActLight_SetColor
void func_ov112_021f6f14(StaActLightSys *sys, StaActLight *light, GXRgb color, u8 alpha) {
    light->color = color;
    light->alpha = alpha;
}

void func_ov112_021f6f1c(StaActLightSys *sys, StaActLight *light, u32 a2, u32 a3) {
    light->unk14 = a2;
    light->unk18 = a3;
}
