#include "global.h"

#include "constants/heap.h"

#include "code_entry_args.h"
#include "naming_screen.h"
#include "overlay_43_palpad.h"
#include "overlay_43_unk_struct_ov64_0222_e060.h"
#include "overlay_manager.h"
#include "player_data.h"

void ov43_0222A154(PalPadWork *work, enum HeapID heapID);
void ov43_0222A1DC(PalPadWork *work);
void ov43_0222A318(PalPadShared *param0, s8 param1, s8 param2);
void ov43_0222A338(PalPadShared *param0, const String *param1, const String *param2);
NamingScreenArgs *ov43_0222CD44(PalPadNaming *naming, PalPadShared *param1, enum HeapID heapID);

int ov43_0222CBB4(PalPadNaming *param0, PalPadWork *param1, PalPadShared *param2, PalPadGfx *param3, enum HeapID heapID);

int ov43_0222CBB4(PalPadNaming *param0, PalPadWork *param1, PalPadShared *param2, PalPadGfx *param3, enum HeapID heapID) {
    int v0[4];

    switch (param2->subState) {
    case 0:
        param0->namingArgs = ov43_0222CD44(param0, param2, heapID);

        v0[0] = 4;
        v0[1] = 4;
        v0[2] = 4;
        v0[3] = 0;

        param0->codeEntryArgs = sub_02085400(heapID, 12, v0, Save_PlayerData_GetOptionsAddr(param2->saveData), 0, 0);
        ov43_0222A1DC(param1);
        param0->manager = OverlayManager_New(&gOverlayTemplate_NamingScreen, param0->namingArgs, heapID);
        param2->subState = 1;
        break;
    case 1:
        if (OverlayManager_Run(param0->manager) == 0) {
            break;
        }

        OverlayManager_Delete(param0->manager);

        if (param0->namingArgs->noInput == FALSE) {
            param0->manager = OverlayManager_New(&_02102620, param0->codeEntryArgs, heapID);
            param2->subState = 2;
        } else {
            ov43_0222A318(param2, 0, 0);
            param2->subState = 3;
        }
        break;
    case 2:
        if (OverlayManager_Run(param0->manager) == 0) {
            break;
        }

        OverlayManager_Delete(param0->manager);
        ov43_0222A338(param2, param0->namingArgs->nameInputString, param0->codeEntryArgs->unk_1C);
        ov43_0222A318(param2, 0, 7);
        param2->subState = 3;
        break;
    case 3:
        NamingScreen_DeleteArgs(param0->namingArgs);
        sub_02085438(param0->codeEntryArgs);
        ov43_0222A154(param1, heapID);
        return 1;
    case 4:
        param0->namingArgs = ov43_0222CD44(param0, param2, heapID);
        ov43_0222A1DC(param1);
        param0->manager = OverlayManager_New(&gOverlayTemplate_NamingScreen, param0->namingArgs, heapID);
        param2->subState = 5;
        break;
    case 5:
        if (OverlayManager_Run(param0->manager) == 0) {
            break;
        }

        OverlayManager_Delete(param0->manager);

        if (param0->namingArgs->noInput == FALSE) {
            ov43_0222A318(param2, 1, 12);
            ov43_0222A338(param2, param0->namingArgs->nameInputString, 0L);
            param2->subState = 6;
        } else {
            ov43_0222A318(param2, 1, 13);
            param2->subState = 6;
        }
        break;
    case 6:
        NamingScreen_DeleteArgs(param0->namingArgs);
        ov43_0222A154(param1, heapID);
        return 1;
    }

    return 0;
}
