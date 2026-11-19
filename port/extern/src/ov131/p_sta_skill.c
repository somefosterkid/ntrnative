// The PC build compiles src/ov131/p_sta_skill.c through this file. Test only: each update of the summary's move page (p_sta_sys.c
// calls it each frame) tells the route pilot (port/src/route_teach.h, the F:teach token) its cursor, the forget confirmation and
// the moves, so F:teach picks the move to forget. The DS build never sees this file.
#pragma push_macro("func_ov131_021d9740")
#define func_ov131_021d9740 func_ov131_021d9740_game
#include "../../../../src/ov131/p_sta_skill.c"
#pragma pop_macro("func_ov131_021d9740")

#include "port_route.h"

void func_ov131_021d9740(PStatusWork *wk, PStaSkillWork *skill);
void func_ov131_021d9740(PStatusWork *wk, PStaSkillWork *skill) {
    PortRouteSkill *s = &gPortRouteSkill;
    u32 i;

    func_ov131_021d9740_game(wk, skill);
    s->cursor = skill->cursor;
    s->confirming = skill->isConfirming;
    s->exiting = skill->isExiting;
    s->moveCount = skill->moveCount;
    s->newMove = wk->param->move;
    s->mode = wk->param->mode;
    for (i = 0; i < 4; i++) {
        s->moves[i] = (int)PStaSkill_GetMove(wk, i);
    }
    s->vblank = OS_GetVBlankCount();
}
