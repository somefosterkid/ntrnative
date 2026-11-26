#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_112_pokewalker_app.h"

void ov112_021E7548(void *, void *, void *, void *);

void ov112_021E7CC8(Ov112PokewalkerApp *);

u32 ov112_021ED4D4(Ov112PokewalkerApp *param_1) {
    MIi_CpuClearFast(0, param_1->unk_9DFC, 0xcc0);
    ov112_021E7548(&param_1->unk_9D44, param_1->unk_9D70, &param_1->unk_AABC, param_1->unk_B184);
    ov112_021E7CC8(param_1);
    ov112_021ED25C(param_1);
    ov112_021EA5A4(param_1, 2);
    ManagedSprite_SetAnimationFrame(param_1->sprites[8], 0);
    return 0x1b;
}
