#include "global.h"

#include "message_format.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "bg_window.h"
#include "overlay_83_battle_castle_self_app.h"

u8 ov83_0223FCB4(void *app, Window *window, int entryID, u32 xOffset, u32 yOffset, u32 renderDelay, u8 fgColor, u8 shadowColor, u8 bgColor, u8 font);

void ov83_02240C48(BattleCastleSelfApp *, u32, u32, u32, u32);

void ov83_0223FFD8(BattleCastleSelfApp *param_1, Window *param_2, u32 param_3, u32 param_4, u32 param_5, Pokemon *param_6, s32 param_7, s32 param_8, s32 param_9);

void ov83_0223FFD8(BattleCastleSelfApp *param_1, Window *param_2, u32 param_3, u32 param_4, u32 param_5, Pokemon *param_6, s32 param_7, s32 param_8, s32 param_9) {
    u8 uVar1;
    u32 uVar2;
    u32 uVar3;
    u32 uVar4;
    uVar4 = param_4;
    uVar2 = GetMonData(param_6, param_7, (void *)0x0);
    BufferMoveName(param_1->strTemplate, param_3, uVar2);
    uVar3 = ((param_3 * 0x18) + 0xc) & 0xffff;
    uVar1 = ov83_0223FCB4(param_1, param_2, param_4, 0x18, uVar3, 0xff, 1, 2, uVar4 = 0, 0);
    param_1->printerID = uVar1;
    uVar2 = GetMonData(param_6, param_8, (void *)0x0);
    ov83_02240C48(param_1, 4, uVar2, 3, 1);
    uVar2 = GetMonData(param_6, param_9, (void *)0x0);
    ov83_02240C48(param_1, 5, uVar2, 3, 0);
    uVar1 = ov83_0223FCB4(param_1, param_2, param_5, 0x60, uVar3, 0xff, 1, 2, 0, 0);
    param_1->printerID = uVar1;
    return;
}
