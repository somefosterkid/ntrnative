#include "global.h"

#include "sprite_system.h"
#include "sys_task_api.h"
#include "overlay_112_pokewalker_app.h"

s32 ov112_021E9290(void *, void *, void *, s32);

void ov112_021ED13C(SysTask *task, void *data);

void ov112_021ED0D0(Ov112PokewalkerApp *arg0) {
    ov112_021E9290(arg0->unk_1E430, arg0->unk_1D7AC, arg0->unk_1D79C, 0);
    ManagedSprite_SetOamMode(arg0->sprites[8], GX_OAM_MODE_XLU);
    ov112_021EA670(arg0, 8);
    arg0->unk_1F2D6 = 0;
    G2_SetBlendAlpha(0, 8, 0, 0x1F);
    arg0->unk_1F2D8 = SysTask_CreateOnMainQueue(ov112_021ED13C, arg0, 2U);
}
