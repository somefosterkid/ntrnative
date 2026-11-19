#include "global.h"

#include "overlay_70_gtsapplication_state.h"

#include "list_menu_items.h"
#include "party.h"
#include "pokemon_types_def.h"
#include "touchscreen_list_menu.h"
#include "text.h"

void ov70_02238D60(GTSApplicationState *);

void ov70_02238D84(GTSApplicationState *, u32, u32);

void ov70_02238E50(GTSApplicationState *, u32, u32);

void ov70_0223A4F4(GTSApplicationState *, u32, u32, u32, u32, Pokemon *);

void ov70_0223A874(GTSApplicationState *);

s32 ov70_0223E76C(Pokemon *);

u32 ov70_0223A3DC(GTSApplicationState *param_1);

s32 ov70_0223A4A0(GTSApplicationState *arg0);
s32 ov70_0223A4C0(GTSApplicationState *arg0);

u32 ov70_0223A3DC(GTSApplicationState *param_1) {
    s32 iVar1;
    Pokemon *piVar2;

    iVar1 = TouchscreenListMenu_HandleInput((TouchscreenListMenu *)param_1->unk_11D0);
    if (iVar1 == 1) {
        ov70_02238D60(param_1);
        ListMenuItems_Delete(param_1->unk_11AC);
        piVar2 = (Pokemon *)&param_1->receivedListing.pokemon;
        iVar1 = ov70_0223E76C(piVar2);
        if ((iVar1 != 0) && (iVar1 = Party_GetCount(*(Party **)((u8 *)param_1->playerData + 8)), iVar1 == 6)) {
            ov70_0223A4F4(param_1, 0x24, 1, 0, 0xf0f, piVar2);
            ov70_02238D84(param_1, 3, 1);
            return 3;
        }
        ov70_0223A4F4(param_1, 6, 1, 0, 0xf0f, piVar2);
        ov70_02238D84(param_1, 3, 5);
        ov70_0223A874(param_1);
    } else if ((iVar1 == 2) || (iVar1 == -2)) {
        ov70_02238D60(param_1);
        ListMenuItems_Delete(param_1->unk_11AC);
        param_1->currentScreenInstruction = 2;
        ov70_02238E50(param_1, 1, 0);
        ov70_0223A874(param_1);
    }
    return 3;
}

s32 ov70_0223A4A0(GTSApplicationState *arg0) {
    if (TextPrinterCheckActive((u8)arg0->textPrinter) == 0) {
        arg0->currentScreenInstruction = (s32)arg0->nextScreenInstruction;
    }
    return 3;
}

s32 ov70_0223A4C0(GTSApplicationState *arg0) {
    if (TextPrinterCheckActive((u8)arg0->textPrinter) == 0) {
        arg0->unk_11C0 = (s32)(arg0->unk_11C0 + 1);
        if ((s32)arg0->unk_11C0 > 0x2D) {
            arg0->unk_11C0 = 0;
            arg0->currentScreenInstruction = (s32)arg0->nextScreenInstruction;
        }
    }
    return 3;
}
