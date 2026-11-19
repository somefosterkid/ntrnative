#include "global.h"
#include "bg_window.h"
#include "pokemon_summary_app.h"
#include "unk_0208C3E4.h"
#include "unk_02088288.h"

void sub_0208C4E0(PokemonSummary *param_1) {
    u8 cVar1;
    s32 iVar2;
    u32 uVar3;

    switch (param_1->page) {
    case 0:
    case 1:
        uVar3 = 0;
        if (uVar3 < param_1->numPageWindows) {
            iVar2 = 0;
            do {
#if defined(SDK_PORT) || defined(NONMATCHING)
                ClearWindowTilemapAndCopyToVram(&param_1->pageWindows[uVar3]);
#else
                ClearWindowTilemapAndCopyToVram((Window *)((u8 *)param_1->pageWindows + iVar2));
#endif
                uVar3 = uVar3 + 1;
                iVar2 = iVar2 + 0x10;
            } while (uVar3 < param_1->numPageWindows);
        }
        break;
    case 2:
        ClearWindowTilemapAndCopyToVram(param_1->pageWindows);
        break;
    case 3:
        cVar1 = param_1->args->unk12;
        if ((cVar1 != 3) && (cVar1 != 4)) {
            return;
        }
        break;
    default:
        return;
    }
    WindowArray_Delete(param_1->pageWindows, param_1->numPageWindows & 0xff);
    return;
}
