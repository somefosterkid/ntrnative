// The PC build compiles src/ov070/itemmenu.c through this file. Test only: the bag's proc table is ours (below the include), so
// each frame of the Bag tells the route pilot (port/src/route_teach.h, the F:teach token) its state, pocket, cursor, the pocket's
// items and its item menu. The game's init, main and end run as always. The DS build never sees this file.
#pragma push_macro("BAG_PROC_FUNCTIONS")
#define BAG_PROC_FUNCTIONS BAG_PROC_FUNCTIONS_game
#include "../../../../src/ov070/itemmenu.c"
#pragma pop_macro("BAG_PROC_FUNCTIONS")

#include "port_route.h"

static BOOL PortBag_Main(GameProc *proc, u32 *state, void *param, void *data) {
    BOOL r = func_ov070_021b8904(proc, state, param, data);
    ItemMenuWork *work = data;
    PortRouteBag *b = &gPortRouteBag;
    int i;

    if (work == NULL) {
        return r;
    }
    b->alive = 1;
    b->phase = work->state == func_ov070_021b7250 ? 1 : work->state == func_ov070_021b6ef8 ? 2 : work->state == func_ov070_021b75a4 ? 3 :
               work->state == func_ov070_021b7504 ? 4 : work->state == func_ov070_021b7688 ? 5 : 0;
    b->pocket = work->pocket;
    b->index = func_ov070_021b61b8(work);   // the cursor's item: the game reads this index (the list's row 0 is none)
    b->count = (int)func_ov070_021b61c8(work);
    for (i = 0; i < 128; i++) {
        BagItem *it = i < b->count ? func_ov070_021b616c(work, i) : NULL;

        b->items[i] = it != NULL ? it->item : 0;
    }
    b->menuLive = work->taskMenu != NULL;
    b->menuCursor = work->taskMenu != NULL ? (int)func_02027a04(work->taskMenu) : -1;
    for (i = 0; i < 4; i++) {
        b->menuActions[i] = (int)work->itemMenuActions[i];
    }
    b->result = (int)work->result;
    b->vblank = OS_GetVBlankCount();
    return r;
}

static BOOL PortBag_End(GameProc *proc, u32 *state, void *param, void *data) {
    BOOL r = func_ov070_021b8990(proc, state, param, data);

    if (r) {
        gPortRouteBag.alive = 0;
    }
    return r;
}

const GameProcFunctions BAG_PROC_FUNCTIONS = { func_ov070_021b86b8, PortBag_Main, PortBag_End };
