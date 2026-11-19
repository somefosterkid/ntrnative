// The PC build compiles src/ov072/townmap.c through this file: an external definition of the game's plain `inline`
// function (see port/extern/src/ov119/pokemontrade_save.c).
// Test only: the proc's table is ours (below the include), so each frame of the Town Map tells the route pilot (port/src/route_fly.h,
// the F:fly token) where its cursor and places are. The game's main, init and end run as always.
#pragma push_macro("data_ov072_021bbb80")
#define data_ov072_021bbb80 data_ov072_021bbb80_game
#include "../../../../src/ov072/townmap.c"
#pragma pop_macro("data_ov072_021bbb80")

#include "port_route.h"

static int PortTM_Index(const TownMapPlaces *places, const TownMapPlace *place) {
    return place == NULL ? -1 : (int)(place - places->places);
}

static BOOL PortTM_Main(GameProc *proc, u32 *state, void *paramPtr, void *workPtr) {
    BOOL r = func_ov072_021b98f8(proc, state, paramPtr, workPtr);
    TownMapWork *work = workPtr;
    PortRouteTownMap *t = &gPortRouteTownMap;
    u32 i;

    if (work == NULL || work->param == NULL || work->places.places == NULL) {
        return r;
    }
    if (!t->alive) {
        t->zone = work->param->zoneId;   // the player's zone: the param's zoneId becomes the zone picked on the pick
    }
    t->alive = 1;
    t->mode = (int)work->param->mode;
    t->result = (int)work->param->result;
    t->zoomed = work->zoomed;
    t->moving = func_ov072_021bb0e0(&work->map);
    t->flyZone = work->param->zoneId;
    t->warpX = work->param->warpX;
    t->warpZ = work->param->warpZ;
    t->cx = work->cursor.pos.x;
    t->cy = work->cursor.pos.y;
    t->sx = work->places.scroll.x;
    t->sy = work->places.scroll.y;
    t->hit = PortTM_Index(&work->places, func_ov072_021ba6c8(&work->places, &work->cursor));
    t->ring = PortTM_Index(&work->places, work->cursor.place);
    t->selected = PortTM_Index(&work->places, work->selected);
    t->count = work->places.count < 64 ? (int)work->places.count : 64;
    for (i = 0; i < (u32)t->count; i++) {
        TownMapPlace *pl = &work->places.places[i];

        t->p[i].zone = func_ov072_021ba774(pl, 0);
        t->p[i].x = func_ov072_021ba774(pl, 4);
        t->p[i].y = func_ov072_021ba774(pl, 5);
        t->p[i].canFly = pl->canFly;
        t->p[i].hidden = pl->hidden;
    }
    t->vblank = OS_GetVBlankCount();
    return r;
}

static BOOL PortTM_End(GameProc *proc, u32 *state, void *paramPtr, void *workPtr) {
    TownMapParam *param = paramPtr;

    // the zone picked and the pick are in the param, which outlives the proc: keep them for the pilot
    gPortRouteTownMap.result = (int)param->result;
    gPortRouteTownMap.flyZone = param->zoneId;
    gPortRouteTownMap.alive = 0;
    return func_ov072_021b9858(proc, state, paramPtr, workPtr);
}

const GameProcFunctions data_ov072_021bbb80 = { func_ov072_021b95a0, PortTM_Main, PortTM_End };

extern __typeof__(TownMapMap_SetLimits) TownMapMap_SetLimits;
