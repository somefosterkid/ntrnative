// The PC build compiles src/ov091/plist_sys.c through this file. Test only: each per-frame update of the Pokemon list tells
// the route pilot (port/src/route_fly.h, the F:fly token) which state it is in, where the cursor is and the mode it was opened for.
// The DS build never sees this file.
#pragma push_macro("func_ov091_021b68f0")
#define func_ov091_021b68f0 func_ov091_021b68f0_game
#include "../../../../src/ov091/plist_sys.c"
#pragma pop_macro("func_ov091_021b68f0")

#include "port_route.h"

BOOL func_ov091_021b68f0(PokeListWork *wk);
BOOL func_ov091_021b68f0(PokeListWork *wk) {
    BOOL r = func_ov091_021b68f0_game(wk);

    gPortRoutePList.state = wk->state;
    gPortRoutePList.subState = wk->subState;
    gPortRoutePList.input = wk->input;
    gPortRoutePList.cursorPos = wk->cursorPos;
    gPortRoutePList.selectPos = wk->selectPos;
    gPortRoutePList.mode = (int)wk->param->mode;
    gPortRoutePListTeach.ask = wk->state != 9 ? 0 : wk->menuFunc == func_ov091_021b9ec0 ? 1 : wk->menuFunc == func_ov091_021b9f44 ? 2 : 0;   // 9ec0: Yes = the forget page; 9f44: Yes = give up
    gPortRoutePListTeach.msgWait = wk->state == 7 && wk->msgWaitInput;
    gPortRoutePListTeach.result = (int)wk->param->result;
    gPortRoutePListTeach.move = (int)wk->param->move;
    gPortRoutePList.vblank = OS_GetVBlankCount();
    return r;
}
