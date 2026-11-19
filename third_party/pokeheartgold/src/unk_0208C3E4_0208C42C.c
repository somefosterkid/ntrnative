#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "unk_02088288.h"
#include "unk_0208C3E4.h"
#include "pokemon_summary_app.h"

extern const WindowTemplate _02104CC4[];

extern const WindowTemplate _02104D04[];

extern const WindowTemplate _02104C84[];

void sub_0208C42C(PokemonSummary *summary) {
    Window *pWVar1;
    u32 uVar2;
    s32 iVar3;
    const WindowTemplate *unaff_r7;
    unsigned char new_var;
    switch (summary->page) {
    case 0:
        unaff_r7 = _02104CC4;
        summary->numPageWindows = 8;
        break;

    case 1:
        unaff_r7 = _02104D04;
        summary->numPageWindows = 0x12;
        break;

    case 2:
        unaff_r7 = _02104C84;
        summary->numPageWindows = 8;
        break;

    case 3:
        new_var = summary->args->unk12;
        if ((new_var != 3) && (new_var != 4)) {
            return;
        }
        summary->numPageWindows = 3;
        break;

    default:
        return;
    }

    pWVar1 = AllocWindows(HEAP_ID_19, summary->numPageWindows & 0xff);
    summary->pageWindows = pWVar1;
    uVar2 = 0;
    if (uVar2 < summary->numPageWindows) {
        iVar3 = 0;
        do {
#if defined(SDK_PORT) || defined(NONMATCHING)
            AddWindow(summary->bgConfig, &summary->pageWindows[uVar2], unaff_r7);
#else
            AddWindow(summary->bgConfig, (Window *)((u8 *)summary->pageWindows + iVar3), unaff_r7);
#endif
            uVar2 = uVar2 + 1;
            unaff_r7 = unaff_r7 + 1;
            iVar3 = iVar3 + 0x10;
        } while (uVar2 < summary->numPageWindows);
    }
    return;
}
