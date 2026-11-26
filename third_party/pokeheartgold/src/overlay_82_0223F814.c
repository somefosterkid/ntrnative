#include "global.h"
struct BattleHallApp;

#include "overlay_82.h"
#include "unk_02035900.h"
#include "message_format.h"
#include "options.h"
#include "yes_no_prompt.h"
#include "bg_window.h"
#include "overlay_82_battle_hall_app.h"
#include "overlay_80_022372D8.h"


void ov82_0223F834(struct BattleHallApp *arg0);
u8 ov82_0223EF7C(void *, s32, u8);
s32 ov82_0223EFB4(void *, s32, s32);
u8 ov82_0223F6C4(u8);
s32 ov82_0223F6E4(void *);
s32 ov82_0223FCB0(void *, void *);
s32 ov82_0223FCFC(void *, s32);
#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov82_0223FD78(Window *, u8);
#else
s32 ov82_0223FD78(Window *, u32);
#endif
s32 ov82_0223FDC8(YesNoPrompt *, BgConfig *, u8);
u8 sub_02030BD0(u8, u8 *);

s32 ov82_0223F5E0(BgConfig *, u8, u8);
s32 ov82_0223F90C(void *arg0);

void ov82_0223F814(s32 netID, s32 unused, void *data, void *context) {
    u16 v0;

    if (netID != sub_0203769C()) {
        v0 = *(u16 *)((u8 *)data + 2);
        if (v0 != 0) {
            ((BattleHallApp *)context)->partnersDecision = v0;
        }
    }
}

void ov82_0223F834(BattleHallApp *arg0) {
    if (arg0->yesNoMenuOpen == 1) {
        arg0->yesNoMenuOpen = 0U;
        YesNoPrompt_Reset(*((YesNoPrompt * *)&arg0->unk_8C));
    }
}

void ov82_0223F84C(BattleHallApp *arg0) {
    s32 v0;
    u8 v1;

    ov82_0223FCB0(*((void **)&arg0->monSprite), 0);
    ov82_0223FD78(&arg0->windows[0], Options_GetFrame(arg0->options));
    BufferTypeName(((MessageFormat *)arg0->strTemplate), 0U, ov80_02237920(arg0->cursorPos));
    v1 = sub_02030BD0(ov82_0223F6C4(arg0->cursorPos), *((void **)&arg0->typeRanks)) + 1;
    if ((u32)v1 > 0xAU) {
        v1 = 0xA;
    }
    ov82_0223EFB4(arg0, 1, v1);
    if (ov82_0223F6E4(arg0) == 1) {
        v0 = 0x1F;
    } else {
        v0 = 0x18;
    }
    *((s8 *)&arg0->printerID) = ov82_0223EF7C(arg0, v0, 1);
    ov82_0223FCFC(*((void **)&arg0->cursorSprite), 0);
    ov82_0223FDC8(*((void **)&arg0->unk_8C), *((void **)&arg0->bgConfig), *((u8 *)&arg0->pad_90[0]));
    *((s8 *)&arg0->yesNoMenuOpen) = 1;
}

void ov82_0223F8E4(BattleHallApp *arg0) {
    ov82_0223F90C(arg0);
    ov82_0223F5E0(arg0->bgConfig, arg0->cursorPos, 0);
    ScheduleBgTilemapBufferTransfer(arg0->bgConfig, 3U);
    ov82_0223FCFC(*((void **)&arg0->cursorSprite), 1);
}
