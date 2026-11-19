#include "global.h"

#include "sound_02004A44.h"
#include "sprite.h"
#include "unk_02005D10.h"
#include "unk_02009D48.h"
#include "sys_task.h"
#include "overlay_18_pokedex_entry_popup.h"










void ov18_021F89F8(SysTask *task, void *param) {
    PokedexEntryPopup *param_2 = param;
    s32 iVar1;
    BOOL BVar2;

    switch (param_2->unk_240) {
    case 0:
        reg_G2_BLDCNT = 0;
        ov18_021F8AB8(param_2);
        ov18_021F8B10(param_2);
        ov18_021F8CCC(param_2);
        ov18_021F8FA0(param_2);
        ov18_021F95CC(param_2);
        ov18_021F8C0C(param_2);
        param_2->unk_240 = 1;
        break;
    case 1:
        iVar1 = ov18_021F8C48(param_2);
        if (iVar1 == 1) {
            param_2->unk_240 = 2;
        }
        break;
    case 2:
        PlayCryEx(0xe, param_2->species & 0xffff, 0x1ff, 0x1ff, 0x1ff, 0);
        param_2->unk_240 = 3;
        break;
    case 3:
        BVar2 = IsCryFinished();
        if (BVar2 == 0) {
            param_2->unk_254 = 1;
            param_2->unk_240 = 4;
        }
        break;
    case 4:
        break;
    }
    SpriteList_RenderAndAnimateSprites(param_2->spriteList);
    ov18_021F8C68(param_2);
    return;
}
