#include "global.h"
#include "overlay_112_pokewalker_app.h"

void ov112_021E7620(u8 *, u8 *);

void ov112_021E7CC8(Ov112PokewalkerApp *);

u32 ov112_021ED8DC(Ov112PokewalkerApp *param_1) {
    MIi_CpuClearFast(0, &param_1->unk_AABC, 0x6c8);
    ov112_021E7620(param_1->unk_9D70, param_1->unk_B184);
    ov112_021E7CC8(param_1);
    ov112_021ED25C(param_1);
    return 0x1d;
}
