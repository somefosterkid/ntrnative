#include "global.h"

#include "error_handling.h"
#include "overlay_112_pokewalker_app.h"

void ov112_021EA570(Ov112PokewalkerApp *arg0, s32);

u32 ov112_021EB780(Ov112PokewalkerApp *param_1, s32 arg1) {
    Ov112PokewalkerApp *new_var;
    ov112_021EA570(param_1, arg1);
    new_var = param_1;
    switch (param_1->unk_1EC50) {
    case 0:
        param_1->unk_4 = 3;
        param_1->unk_10 = 0;
        break;

    case 1:
        param_1->unk_4 = 3;
        param_1->unk_10 = 1;
        break;

    case 2:
        param_1->unk_4 = 3;
        param_1->unk_10 = 2;
        break;

    case 3:
        param_1->unk_4 = 0xb;
        break;

    default:
        if ((3 + 1) <= (new_var->unk_1EC50)) {
            GF_AssertFail();
        }
    }

    return 0x1a;
}
