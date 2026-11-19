#include "global.h"
struct PokeathlonEventRecord;

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_99_pokeathlon_event_record.h"
#include "overlay_98_ov98text_sys.h"

void ov99_021E8FEC(void *param_1, s32 param_2);

void ov99_021E9020(struct PokeathlonEventRecord *arg0);

void ov99_021E8FEC(void *param_1, s32 param_2) {
    s32 iVar1;
    s32 iVar3;
    int new_var;
    unsigned int new_var2;
    void *iVar2;
    s32 uStack_1c;
    uStack_1c = 0;
    iVar1 = 0;
    new_var = 2;
    do {
        iVar3 = 0;
        new_var2 = 2;
        iVar2 = param_1 + ((iVar1 * new_var2) * new_var);
        do {
            ManagedSprite_SetDrawFlag(*((ManagedSprite **)(iVar2 + 0x70)), param_2);
            iVar3 = iVar3 + 1;
            iVar2 = iVar2 + 4;
        } while (iVar3 < 3);
        iVar1 = iVar1 + 3;
        uStack_1c = uStack_1c + 1;
    } while (uStack_1c < 5);
    return;
}

void ov99_021E9020(PokeathlonEventRecord *arg0) {
    s32 v0;

    v0 = 0;
    do {
        ov98_0221EE84(arg0->textContext, v0 + 0xC);
        v0 += 1;
    } while (v0 < 5);
}
