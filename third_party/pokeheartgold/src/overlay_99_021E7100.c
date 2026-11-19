#include "global.h"
#include "overlay_manager.h"
#include "overlay_99_obj99.h"
#include "overlay_99_pokeathlon_course_summary.h"

extern u32 ov99_021E9E04;

u32 ov99_021E7100(s32 param_1);

u8 ov99_021E7124(Obj99 *arg0);

s32 ov98_0221EF64(u32 arg0);
u16 ov99_021E70E8(const PokeathlonCourseSummary *, s32);

struct OverlayManager;
BOOL ov99_021E714C(OverlayManager *appMan);

s32 ov99_021E7150(OverlayManager *arg0);

u32 ov99_021E7100(s32 param_1) {
    u32 uVar1;

    uVar1 = 0;
    do {
        if (*(s32 *)(&ov99_021E9E04 + uVar1) <= param_1) {
            break;
        }
        uVar1 = uVar1 + 1 & 0xff;
    } while (uVar1 < 10);
    return 10 - uVar1 & 0xff;
}

u8 ov99_021E7124(Obj99 *arg0) {
    u32 v0;
    u8 v1;

    v1 = 0;
    v0 = 0;
    do {
        v1 += ov98_0221EF64(ov99_021E70E8(arg0->summary, (u8)v0));
        v0 += 1;
    } while ((s32)v0 < 5);
    return v1;
}

BOOL ov99_021E714C(OverlayManager *appMan) {
    return 1;
}

s32 ov99_021E7150(OverlayManager *arg0) {
    return *(s32 *)((u8 *)(*((void * *)&arg0->exec_state)) + 0xC);
}
