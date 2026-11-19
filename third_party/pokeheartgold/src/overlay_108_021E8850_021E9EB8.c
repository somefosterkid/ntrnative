#include "global.h"
#include "overlay_108_safari_decoration_app.h"

#include "overlay_108_021E8850.h"
#include "sprite.h"
#include "sprite_system.h"
#include "bg_window.h"
#include "font.h"
#include "pm_string.h"
#include "text.h"

extern u32 ov108_021EAC48;

extern u32 ov108_021EAAA4[];
void ov108_021E9EB8(SafariDecorationApp *param_1) {
    Sprite *pSVar1;
    UnmanagedSpriteTemplate *template;
    s32 iVar3;

    template = (UnmanagedSpriteTemplate *)&ov108_021EAC48;
    iVar3 = 0;
    do {
        pSVar1 = SpriteSystem_CreateSpriteFromResourceHeader(param_1->spriteSystem, param_1->spriteManager, template);
        param_1->sprites[iVar3] = pSVar1;
        Sprite_SetDrawFlag(param_1->sprites[iVar3], 1);
        template = template + 1;
        iVar3 = iVar3 + 1;
    } while (iVar3 < 4);
    thunk_Sprite_SetPriority(param_1->sprites[0], 1);
    return;
}

void ov108_021E9F04(SafariDecorationApp *arg0) {
    s32 v0;
    v0 = 0;
    do {
        thunk_Sprite_Delete(arg0->sprites[v0]);
        v0 += 1;
    } while (v0 < 4);
}

void ov108_021E9F20(SafariDecorationApp *param_1, s32 param_2) {
    u32 uVar1;

    Sprite_SetAnimCtrlSeq(param_1->sprites[1], param_2 + 2);
    Sprite_ResetAnimCtrlState(param_1->sprites[1]);
    FillWindowPixelBuffer(&param_1->windows[2], 0);
    uVar1 = FontID_String_GetWidth(FONT_UNK_4, param_1->unk_330, 0);
    AddTextPrinterParameterizedWithColor(&param_1->windows[2], FONT_UNK_4, param_1->unk_330, 0x30 - uVar1 >> 1, 4, 0xff, ov108_021EAAA4[param_2], (PrinterCallback_t)0x0);
    ScheduleWindowCopyToVram(&param_1->windows[2]);
    return;
}
