#include "global.h"

#include "pokewalker.h"
#include "overlay_112_pokewalker_app.h"

void ov112_021F3244(SaveData *, void *);

void ov112_021E7DC0(Ov112PokewalkerApp *param_1) {
    void *srcp;

    ov112_021F3244(param_1->saveData, param_1->unk_F200);
    param_1->unk_F1D0 = 1;
    param_1->unk_F1D4 = GAME_VERSION;
    srcp = sub_02032728(param_1->pokewalker);
    MIi_CpuCopyFast(srcp, param_1->unk_F1D8, 0x28);
    return;
}
