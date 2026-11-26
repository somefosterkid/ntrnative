#include "global.h"
struct PokeathlonEventRecord;

#include "bg_window.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_99_pokeathlon_event_record.h"
#include "overlay_98_ov98text_sys.h"

s32 ov99_021E87D0(void *);

s32 ov99_021E89A4(void *param_1);

s32 ov99_021E8FEC(void *, s32);

s32 ov99_021E9020(void *);

s32 ov99_021E9038(void *);

extern s32 ov99_021EA35A;

void ov99_021E8828(struct PokeathlonEventRecord *arg0);

void ov99_021E8828(PokeathlonEventRecord *arg0) {
    s32 *var_r6;
    s32 var_r4;

    ov99_021E89A4(arg0);
    ov99_021E9038(arg0);
    ov99_021E87D0(arg0);
    var_r6 = &ov99_021EA35A;
    var_r4 = 0;
    do {
        ov98_0221EE84(arg0->textContext, var_r4 + 7);
        ov98_0221EEDC(arg0->textContext, var_r4 + 7);
        ov98_0221EEEC(arg0->textContext, var_r4 + 7, *(u8 *)(((u8 *)var_r6) + *((s32 *)&arg0->linkMode)));
        var_r4 += 1;
        var_r6 = (void *)((u8 *)var_r6 + (2));
    } while (var_r4 < 5);
    ov99_021E8FEC(arg0, 0);
    BgClearTilemapBufferAndCommit(arg0->bgConfig, 6U);
    ManagedSprite_SetDrawFlag(arg0->sprites[1], 0);
    ov98_0221EE84(arg0->textContext, 3);
    ov98_0221EE84(arg0->textContext, 6);
    ov98_0221EE84(arg0->textContext, 0x11);
    ov98_0221EE84(arg0->textContext, 0x12);
    ov99_021E9020(arg0);
    ov98_0221EC08(arg0->textContext, 2, 0x3F, 1, 1);
    ov98_0221EC08(arg0->textContext, 5, 0x40, 0, 1);
    *((s32 *)&arg0->modeSwitchRequested) = 0;
}
