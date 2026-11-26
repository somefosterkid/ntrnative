#include "global.h"

#include "unk_02005D10.h"
#include "overlay_112_pokewalker_app.h"

u32 ov112_021EF958(Ov112PokewalkerApp *param_1) {
    u32 uVar1;
    if (((param_1->unk_1F374) & 0xFFFFu) == 0) {
        return 5;
    }
    uVar1 = ov112_021EA08C(param_1, 0, 0x34);
    param_1->unk_1E524 = uVar1;
    param_1->unk_C = 5;
    PlayFanfare(0x4a0);
    param_1->unk_1F374 = 0;
    return 0x23;
}
