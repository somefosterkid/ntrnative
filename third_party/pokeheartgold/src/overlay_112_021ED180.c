#include "global.h"

#include "sys_task_api.h"
#include "bg_window.h"
#include "overlay_112_pokewalker_app.h"
#include "sprite_system.h"
#include "text.h"
#include "unk_02005D10.h"

void ov112_021ED180(SysTask *arg0, void *arg1);

s32 ov112_021E7CA4(void *, s32, s32);
s32 ov112_021E9C10(Ov112PokewalkerApp *, s32);
s32 ov112_021E74A8(void *, void *, void *);
s32 ov112_021E7CC8(Ov112PokewalkerApp *);
void ov112_021ED180(SysTask *task, void *data);
void ov112_021ED314(s32 arg0);

void ov112_021ED180(SysTask *task, void *data) {
    Ov112PokewalkerApp *arg1 = data;

    arg1->unk_1F2E0 = (u16)(arg1->unk_1F2E0 + 0x10);
    if ((u32)arg1->unk_1F2E0 > 0x100U) {
        arg1->unk_1F2E0 = 0x100U;
        SysTask_Destroy(task);
    }
}

s32 ov112_021ED1A0(Ov112PokewalkerApp *arg0) {
    BgClearTilemapBufferAndCommit(arg0->bgConfig, 4U);
    BgClearTilemapBufferAndCommit(arg0->bgConfig, 5U);
    BgClearTilemapBufferAndCommit(arg0->bgConfig, 1U);
    BgClearTilemapBufferAndCommit(arg0->bgConfig, 2U);
    ov112_021E9C10(arg0, 2);
    ov112_021E9C10(arg0, 3);
    ov112_021E9A78(arg0, 0);
    ov112_021E9A78(arg0, 1);
    ov112_021E7CA4(arg0, 2, 0xE);
    ov112_021E9F40(arg0, 1);
    ov112_021EA688(arg0, 3);
    ov112_021EA688(arg0, 4);
    ov112_021ED2F4(arg0);
    arg0->unk_1E524 = ov112_021EA08C(arg0, 0, 5);
    return 0x17;
}

s32 ov112_021ED224(Ov112PokewalkerApp *arg0) {
    if ((TextPrinterCheckActive((u8) * &arg0->unk_1E524) == 0) && (arg0->unk_1F2E0 == 0x100)) {
        ov112_021EA5A4(arg0, 2);
        return 0x1C;
    }
    return 0x17;
}

void ov112_021ED25C(Ov112PokewalkerApp *arg0) {
    arg0->unk_14 = 0;
    arg0->unk_16 = 0;
}

s32 ov112_021ED264(Ov112PokewalkerApp *arg0) {
    MIi_CpuClearFast(0U, &arg0->unk_AABC, 0x6C8U);
    MIi_CpuClearFast(0U, arg0->unk_9D70, 0xD4CU);
    ov112_021E7CA4(arg0, 2, 0xE);
    ov112_021E9F40(arg0, 1);
    ov112_021E7D7C(arg0);
    ov112_021E74A8(arg0->unk_10F4, arg0->unk_C910, &arg0->unk_F1D0);
    ov112_021E7CC8(arg0);
    ov112_021EA5A4(arg0, 2);
    ManagedSprite_SetAnim(arg0->sprites[8], 0x19);
    ManagedSprite_SetAnimationFrame(arg0->sprites[8], 0U);
    ov112_021ED25C(arg0);
    return 0x19;
}

void ov112_021ED2F4(Ov112PokewalkerApp *arg0) {
    ov112_021E98A4(arg0);
    arg0->unk_1F2DC = SysTask_CreateOnMainQueue(ov112_021ED180, arg0, 2U);
}

void ov112_021ED314(s32 arg0) {
    if ((arg0 != 0) && (IsSEPlaying(0x934U) == 0)) {
        PlaySE(0x934U);
    }
}
