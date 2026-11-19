#include "global.h"

#include "bg_window.h"
#include "overlay_112_pokewalker_app.h"

extern u8 ov112_021FEC80[];

void ov112_021E9C10(Ov112PokewalkerApp *arg0, s32 arg1);

void ov112_021E9C10(Ov112PokewalkerApp *arg0, s32 arg1) {
    u16 var_r4;
    u16 var_r4_2;
    if (ov112_021FEC80[arg1] == 0) {
        for (var_r4 = 0; ((s32)var_r4) < ((s32)arg0->unk_1EC48); var_r4 += 1) {
            RemoveWindow(&arg0->windows[var_r4]);
        }
        arg0->unk_1EC4A = 0xFF;
        return;
    }
    var_r4_2 = 0;
    if (((s32)arg0->unk_1EC49) > 0) {
        do {
            RemoveWindow(&arg0->windows2[var_r4_2]);
            var_r4_2 += 1;
        } while (((s32)var_r4_2) < ((s32)arg0->unk_1EC49));
    }
    arg0->unk_1EC4B = 0xFF;
}
