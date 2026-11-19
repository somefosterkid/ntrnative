#include "global.h"
#include "constants/heap.h"
#include "ascii_util.h"
#include "menu_input_state.h"
#include "naming_screen.h"
#include "player_data.h"
#include "pm_string.h"
#include "save.h"
#include "overlay_43_unk_struct_ov64_0222_e060.h"
#include "overlay_43_palpad.h"

void ov43_0222CD3C(PalPadNaming *naming, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);

NamingScreenArgs *ov43_0222CD44(PalPadNaming *naming, struct PalPadShared *param_2, enum HeapID param_3);

void ov43_0222CD3C(PalPadNaming *naming, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId) {
}

void ov43_0222CD40(PalPadNaming *naming, PalPadShared *shared, PalPadGfx *gfx);

void ov43_0222CD40(PalPadNaming *naming, PalPadShared *shared, PalPadGfx *gfx) {
}

NamingScreenArgs *ov43_0222CD44(PalPadNaming *naming, PalPadShared *param_2, enum HeapID param_3) {
    Options *options;
    NamingScreenArgs *pNVar1;
    BOOL BVar2;

    options = Save_PlayerData_GetOptionsAddr(param_2->saveData);
    pNVar1 = NamingScreen_CreateArgs(param_3, NAME_SCREEN_UNK7, 0, 7, options, (MenuInputStateMgr *)0x0);
    if (*((s32 *)&param_2->useInitialName) == 1) {
        BVar2 = sub_02020B78(*param_2->initialName);
        if (BVar2 == 0) {
            CopyU16ArrayToString(pNVar1->nameInputString, param_2->initialName);
        } else {
            String_SetEmpty(pNVar1->nameInputString);
        }
        *((u32 *)&param_2->useInitialName) = 0;
    }
    return pNVar1;
}
