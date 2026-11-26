#include "global.h"
#include "overlay_112_pokewalker_app.h"
struct Ov112PokewalkerApp;

void ov112_021ECA18(Ov112PokewalkerApp *arg0, s32 arg1) {
    s32 v0;

    v0 = ov112_021EC7B8(arg0);
    if ((u32)arg0->unk_1EC76 > 5U) {
        if (arg1 == 0) {
            ov112_021EA688(arg0, 3);
            ov112_021EA670(arg0, 4);
            return;
        }
        if ((arg1 > 0) && (arg1 < (s32)(v0 - 1))) {
            ov112_021EA670(arg0, 3);
            ov112_021EA670(arg0, 4);
            return;
        }
        if (arg1 == (v0 - 1)) {
            ov112_021EA670(arg0, 3);
            ov112_021EA688(arg0, 4);
        }
    } else {
        ov112_021EA688(arg0, 3);
        ov112_021EA688(arg0, 4);
    }
}

s8 ov112_021ECA88(Ov112PokewalkerApp *arg0) {
    s32 idx = arg0->unk_1EC50 + arg0->unk_1EC54 * 5;
    return arg0->unk_1EC58[idx];
}
