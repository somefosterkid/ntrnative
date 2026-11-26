#include "global.h"

#include "screen_fade.h"
#include "bg_window.h"
#include "msgdata.h"
#include "pm_string.h"
#include "overlay_112_pokewalker_app.h"

s32 ov112_021EAF48(void);

void ov112_021E7CA4(Ov112PokewalkerApp *, u32, u32);
void ov112_021E9C10(Ov112PokewalkerApp *, u32);
void ov112_021E9FA4(Ov112PokewalkerApp *, s32, String *, u32, u32);
extern void *ov112_021FF0AC;

s32 ov112_021EAF48(void) {
    if (IsPaletteFadeFinished() == 1) {
        return 4;
    }
    return 2;
}

u32 ov112_021EAF5C(Ov112PokewalkerApp *param_1) {
    String *string;
    s32 iVar1;

    ov112_021E9C10(param_1, 1);
    ov112_021E9A78(param_1, 3);
    FillBgTilemapRect(param_1->bgConfig, 1, 0, 0, 0, 0x20, 0x18, 0x10);
    ov112_021E7CA4(param_1, 5, 0x11);
    ov112_021E7CA4(param_1, 2, 0x17);
    iVar1 = 0;
    do {
        string = NewString_ReadMsgData(param_1->msgData, iVar1 + 0x98);
        ov112_021E9FA4(param_1, iVar1, string, 4, 0xf0100);
        String_Delete(string);
        iVar1 = iVar1 + 1;
    } while (iVar1 < 4);
    ov112_021E9F40(param_1, 6);
    param_1->unk_1EC50 = 0;
    ov112_021EA51C(param_1, *(u32 *)(&ov112_021FF0AC + param_1->unk_1EC50));
    return 3;
}
