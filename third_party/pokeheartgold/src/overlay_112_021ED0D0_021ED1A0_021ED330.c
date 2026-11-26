#include "global.h"
#include "overlay_112_pokewalker_app.h"

void *ov112_021E7668(void);

s32 ov112_021ED330(Ov112PokewalkerApp *arg0, s32 arg1) {
    s32 var_r4;
    u16 temp_r1;
    Ov112PokewalkerApp *var_r5;

    var_r5 = arg0;
    var_r4 = arg1;
    if (ov112_021E7668() != 0) {
        temp_r1 = var_r5->unk_1D760;
        if ((s32)temp_r1 < var_r4) {
            var_r5->unk_1D760 = (u16)var_r4;
        } else {
            var_r4 = (s32)temp_r1;
        }
    } else {
        var_r5->unk_1D760 = 0U;
    }
    return var_r4;
}
