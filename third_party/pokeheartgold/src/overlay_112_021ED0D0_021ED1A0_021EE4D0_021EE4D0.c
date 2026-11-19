#include "global.h"

#include "math_util.h"
#include "overlay_112_pokewalker_app.h"

void ov112_021EA7D0(void *work);

void ov112_021ED314(int a0);

int ov112_021EE4D0(Ov112PokewalkerApp *work);

int ov112_021EE4D0(Ov112PokewalkerApp *work) {
    (work->unk_1F2D0)++;
    if (work->unk_1F2D0 > 0xB4) {
        return 0x37;
    }
    if ((u32) * &work->unk_1F2D0 > MTRandom() % 0xB4) {
        ov112_021EA7D0(work);
    }
    ov112_021ED314(work->unk_1F2D0);
    return 0x36;
}
