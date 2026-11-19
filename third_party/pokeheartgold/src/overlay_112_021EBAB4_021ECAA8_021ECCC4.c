#include "global.h"

#include "bg_window.h"
#include "error_handling.h"
#include "overlay_112_pokewalker_app.h"

void ov112_021EA570(void *arg0);

s8 ov112_021ECA88(void *arg0);

void ov112_021E9A78(void *, u8);

void ov112_021E9C10(void *, u32);

void ov112_021EA51C(void *, u32);

extern void *ov112_021FF0C4;

u32 ov112_021ECCC4(Ov112PokewalkerApp *param_1);

u32 ov112_021ECCC4(Ov112PokewalkerApp *param_1) {
    s8 sVar1;
    s32 iVar2;

    ov112_021EA570(param_1);
    iVar2 = param_1->unk_1EC50;
    switch (iVar2) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        ov112_021EA51C(param_1, *(u32 *)(&ov112_021FF0C4 + iVar2));
        sVar1 = ov112_021ECA88(param_1);
        if (sVar1 >= 0) {
            ov112_021EA570(param_1);
            FillBgTilemapRect(param_1->bgConfig, 1, 0, 0, 0x10, 0x20, 8, 0x10);
            sVar1 = ov112_021ECA88(param_1);
            param_1->unk_1D798 = (s32)sVar1;
            return 0xc;
        }
        break;
    case 5:
        ov112_021E9C10(param_1, 2);
        ov112_021E9C10(param_1, 3);
        ov112_021E9A78(param_1, 1);
        ov112_021E9A78(param_1, 0);
        param_1->unk_1D77C = 0;
        ov112_021EA570(param_1);
        FillBgTilemapRect(param_1->bgConfig, 1, 0, 0, 0x10, 0x20, 8, 0x10);
        return 0;
    default:
        if (iVar2 >= 6) {
            GF_AssertFail();
        }
    }
    return 10;
}
