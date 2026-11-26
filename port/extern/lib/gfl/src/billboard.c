// The PC build compiles lib/gfl/src/billboard.c through this file (tools/port_sources.py): Smooth motion's tags (libntr's
// simulator/sim_g3tag.h) on the billboards, the field's sprites: the player, NPCs, and the effects the field draws as billboards
// (shaking grass, water ripples, shadows: src/ov021/fldeff_grass.c, fldeff_ripple.c, fldeff_shadow.c). Each actor BlActScene_Draw
// draws is tagged right before its G3_Begin (kind 1 sprite: owner its scene, inst its slot, state the image it shows: face,
// material, flips); a scene's run ends with the empty tag; BlActScene_AddNewActor gives the new actor in a slot a new id. Off
// (the default) each site is one flag test and the command stream is unchanged; BWPORT_VCAPTURE_TAGS=1 or SIM_G3TagEnable(1)
// (Smooth motion on) turns them on. The DS build never sees this file.
#pragma push_macro("BlActScene_Draw")
#pragma push_macro("BlActScene_AddNewActor")
#define BlActScene_Draw BlActScene_Draw_game
#define BlActScene_AddNewActor BlActScene_AddNewActor_game
#include "types.h"   // the game file's headers, before the hook below (their include guards keep them out of it)
#include "gfl/blact.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/fx/fx_cp.h"
#include "nitro/fx/fx_vec.h"
#include "nitro/fx/fx_mtx.h"
#include "nitro/gx.h"
#include "nitro/mi.h"
#include "nnsys/gfd.h"
#include <simulator/sim_g3tag.h>

static void PortBlAct_Tag(const BlActScene *scene, const BlAct *actor);

// The file's one G3_Begin: an actor's quad in BlActScene_Draw's loop (its scene and actor)
#define G3_Begin(primitive) ((SIM_G3TagOn ? PortBlAct_Tag(scene, actor) : (void)0), G3_Begin(primitive))
#include "../../../../../lib/gfl/src/billboard.c"
#undef G3_Begin
#pragma pop_macro("BlActScene_Draw")
#pragma pop_macro("BlActScene_AddNewActor")

static void PortBlAct_Tag(const BlActScene *scene, const BlAct *actor) {
    SIM_G3Tag(SIM_G3TAG_OWNER(SIM_G3TAG_SPRITE, scene), SIM_G3TagId(actor),
              actor->face | (u32)actor->material << 16 | (u32)actor->flipS << 30 | (u32)actor->flipT << 31);
}

void BlActScene_Draw(BlActScene *scene, G3DCamera *camera, G3DLight *lights) {
    BlActScene_Draw_game(scene, camera, lights);
    if (SIM_G3TagOn) {
        SIM_G3Tag(0, 0, 0);
    }
}

u32 BlActScene_AddNewActor(BlActScene *scene, u32 material, s16 scaleX, s16 scaleY, const VecFx32 *pos, u8 alpha,
                           u32 lights, u32 type) {
    // the game takes the first free slot and returns 0 also when none is free
    BOOL free0 = scene->setup.actorCount != 0 && scene->actors[0].material == BLACT_MATERIAL_NONE;
    u32 i = BlActScene_AddNewActor_game(scene, material, scaleX, scaleY, pos, alpha, lights, type);

    if (i != 0 || free0) {
        SIM_G3TagDrop(&scene->actors[i]);
    }
    return i;
}
