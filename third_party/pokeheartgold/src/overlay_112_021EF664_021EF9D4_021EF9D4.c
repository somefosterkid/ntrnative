#include "global.h"

#include "message_format.h"
#include "overlay_112_pokewalker_app.h"

u8 ov112_021EA08C(Ov112PokewalkerApp *, u32, u32);

u32 ov112_021EF9D4(Ov112PokewalkerApp *param_1);

u32 ov112_021EF9D4(Ov112PokewalkerApp *param_1) {
    u32 uVar1;

    if (param_1->unk_1EC78 != 0) {
        BufferPokewalkerCourseName(param_1->msgFormat, 4, param_1->unk_1EC78 & 0xff);
        uVar1 = ov112_021EA08C(param_1, 0, 0x30);
        param_1->unk_1E524 = uVar1;
        param_1->unk_C = 7;
        param_1->unk_1EC78 = 0;
        return 0x23;
    }
    return 8;
}
