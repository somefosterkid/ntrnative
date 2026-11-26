#include "global.h"

#include "pokemon_summary_app.h"
#include "unk_02005D10.h"
#include "unk_02088288.h"

BOOL sub_02089E80(PokemonSummary *summaryScreen, s32 badge);

void sub_02089E14(PokemonSummary *, s32);

#if defined(SDK_PORT) || defined(NONMATCHING)
void sub_0208A234(PokemonSummary *, s8);
#else
void sub_0208A234(PokemonSummary *, u32);
#endif

s32 sub_0208A310(PokemonSummary *, s32);

void sub_0208ADB8(PokemonSummary *, enum MenuInputState);

u32 sub_0208B044(PokemonSummary *, u32);

u8 sub_0208E544(PokemonSummary *summaryScreen);

u32 sub_02089E98(PokemonSummary *summaryScreen);

u32 sub_02089E98(PokemonSummary *summaryScreen) {
    s32 iVar1;
    BOOL BVar2;
    u32 uVar3;
    u32 uVar4;

    iVar1 = sub_0208E544(summaryScreen);
    if (iVar1 == 0xff) {
        return 2;
    }
    switch (iVar1) {
    case 0:
        BVar2 = sub_02089E80(summaryScreen, iVar1);
        if (BVar2 != 0) {
            sub_02089E14(summaryScreen, (s32)(s8)iVar1);
        }
        break;
    case 1:
    case 2:
        if (summaryScreen->mon.isEgg == 0 && (BVar2 = sub_02089E80(summaryScreen, iVar1), BVar2 != 0)) {
            sub_02089E14(summaryScreen, (s32)(s8)iVar1);
        }
        break;
    case 3:
        sub_0208ADB8(summaryScreen, MENU_INPUT_STATE_TOUCH);
        PlaySE(0x940);
        uVar3 = sub_0208B044(summaryScreen, 0x15);
        return uVar3;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        uVar4 = iVar1 - 4;
        if ((summaryScreen->args->partySlot != uVar4) && (iVar1 = sub_0208A310(summaryScreen, uVar4), iVar1 != 0)) {
            sub_0208A234(summaryScreen, uVar4 & 0xff);
            return 0x13;
        }
    }
    return 2;
}
