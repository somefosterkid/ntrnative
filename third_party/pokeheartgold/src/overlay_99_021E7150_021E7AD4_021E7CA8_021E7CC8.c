#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_99_pokeathlon_medals.h"

void ov99_021E7CC8(void);

void ov99_021E7CC8(void) {
    return;
}

static void ov99_021E7CE4(void *param_1, u32 param_2, u32 param_3, s32 param_4);

void ov99_021E7CCC(void *arg0, s32 arg1, s32 arg2);

void ov99_021E7CCC(void *arg0, s32 arg1, s32 arg2) {
    ov99_021E7CE4(arg0, arg1, arg2, 0);
}

void ov99_021E7CD8(void *arg0, s32 arg1, s32 arg2);

void ov99_021E7CD8(void *arg0, s32 arg1, s32 arg2) {
    ov99_021E7CE4(arg0, arg1, arg2, 1);
}

void ov99_021E77D8(void *);

void ov99_021E7C58(void *param_1, u32 param_2, u32 param_3, u32 param_4);

static void ov99_021E7CE4(void *param_1, u32 param_2, u32 param_3, s32 param_4) {
    s32 seqno;

    if ((param_2 != param_3) && (param_2 < 0x20)) {
        ov99_021E7C58(param_1, param_2, param_3, param_4);
        if ((s32)param_2 < 0x1e) {
            *((u32 *)&((PokeathlonMedals *)param_1)->flags) = *((u32 *)&((PokeathlonMedals *)param_1)->flags) | 0x20000000;
        } else {
            ov99_021E77D8(param_1);
        }
        if (param_2 == 0x1e) {
            seqno = 0xc;
        } else {
            seqno = 0xe;
        }
        if (param_4 != 0) {
            seqno = seqno + 1;
        }
        ManagedSprite_SetAnim(((PokeathlonMedals *)param_1)->sprites[0], seqno);
    }
    return;
}
