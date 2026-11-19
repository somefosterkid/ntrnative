#include "global.h"

#include "gf_gfx_planes.h"
#include "system.h"
#include "unk_020210A0.h"
#include "overlay_108_021E8850.h"
#include "bg_window.h"
#include "render_window.h"
#include "sprite.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "palette.h"
#include "yes_no_prompt.h"
#include "overlay_108_safari_decoration_app.h"

u32 ov108_021E9304(SafariDecorationApp *param_1) {
    switch (param_1->initState) {
    case 0:
        Main_SetVBlankIntrCB((GFIntrCB)0x0, (void *)0x0);
        HBlankInterruptDisable();
        ov108_021E9F04(param_1);
        ov108_021E9E80(param_1);
        ov108_021E9DE0(param_1);
        ov108_021E9CD0(param_1);
        ov108_021E9C2C(param_1);
        ov108_021E9BD4(param_1);
        ov108_021E9A08(param_1);
        break;
    case 2:
        DisableTouchPad();
        reg_GX_DISPCNT = reg_GX_DISPCNT & 0xffffe0ff;
        reg_GXS_DB_DISPCNT = reg_GXS_DB_DISPCNT & 0xffffe0ff;
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        param_1->initState = 0;
        return 1;
    }
    param_1->initState = param_1->initState + 1;
    return 0;
}

u32 ov108_021E9388(SafariDecorationApp *arg0) {
    s32 v0;
#if defined(SDK_PORT) || defined(NONMATCHING)
    s32 ret;
#endif
    v0 = 0;
#if defined(SDK_PORT) || defined(NONMATCHING)
    ret = ov108_021E96FC(arg0, &v0);
    if (v0 == 0) {
        ret = ov108_021E95AC(arg0);
    }
    return ret; /* r0 on DS = result of the last call made */
#else
    ov108_021E96FC(arg0, &v0);
    if (v0 == 0) {
        ov108_021E95AC(arg0);
    }
#endif
}

s32 ov108_021E93A8(SafariDecorationApp *arg0) {
    s32 v0;

    v0 = 0;
    if (System_GetTouchNew() != 0) {
        v0 = 1;
        arg0->menuInputState = MENU_INPUT_STATE_TOUCH;
    } else {
        if (0xCF3 & gSystem.newKeys) {
            arg0->menuInputState = MENU_INPUT_STATE_BUTTONS;
        }
        if (3 & gSystem.newKeys) {
            v0 = 1;
        }
    }
    if (v0 != 0) {
        PlaySE(0x5E5U);
        ClearFrameAndWindow2(&arg0->windows[1], 1);
        ClearWindowTilemapAndScheduleTransfer(&arg0->windows[1]);
        return 0;
    }
    return 2;
}

void ov108_021E940C(SafariDecorationApp *arg0, s32 arg1) {
    s32 flag;

    if (arg1 == 1) {
        ov108_021E9F94(arg0, 1);
        ov108_021E9F20(arg0, 2);
        ov108_021EA260(arg0);
        ov108_021EA334(arg0, 0);
        flag = 0;
    } else {
        ov108_021E9F94(arg0, 0);
        ClearWindowTilemapAndScheduleTransfer(&arg0->windows[4]);
        ov108_021E9F20(arg0, 0);
        ov108_021EA040(arg0);
        ov108_021EA334(arg0, 2);
        flag = 1;
    }
    Sprite_SetDrawFlag(arg0->sprites[2], flag);
    Sprite_SetDrawFlag(arg0->sprites[3], flag);
    Sprite_SetDrawFlag(arg0->sprites[0], flag);
}

void ov108_021E9488(SafariDecorationApp *param_1) {
    YesNoPromptTemplate YStack_20;

    MI_CpuFill8(&YStack_20, 0, sizeof(YesNoPromptTemplate));
    YStack_20.bgConfig = param_1->bgConfig;
    YStack_20.tileStart = 0x3a6;
    YStack_20.plttSlot = 0xe;
    YStack_20.bgId = 4;
    YStack_20.x = 0x1a;
    YStack_20.y = 4;
    YStack_20.ignoreTouchFlag = param_1->menuInputState;
    YStack_20.initialCursorPos = 0;
    YStack_20.shapeParam = 0;
    YesNoPrompt_InitFromTemplateWithPalette(param_1->yesNoPrompt, &YStack_20, NULL);
    return;
}
