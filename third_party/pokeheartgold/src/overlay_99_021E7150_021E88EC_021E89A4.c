#include "global.h"
#include "overlay_99_pokeathlon_event_record.h"
#include "overlay_98_ov98text_sys.h"

extern u32 ov99_021EA384;

extern u32 ov99_021EA38C;

void ov99_021E89A4(void *param_1);

void ov99_021E89A4(void *param_1) {
    ov98_0221EC08(((PokeathlonEventRecord *)param_1)->textContext, 0, *(u32 *)(&ov99_021EA384 + *((s32 *)&((PokeathlonEventRecord *)param_1)->linkMode)), 1, 0);
    ov98_0221EBEC(((PokeathlonEventRecord *)param_1)->textContext, 1, *(u32 *)(&ov99_021EA38C + *((s32 *)&((PokeathlonEventRecord *)param_1)->linkMode)), 1, 0, 4);
    return;
}
