#include "global.h"

#include "constants/heap.h"

#include "bag.h"
#include "bag_types_def.h"
#include "heap.h"
#include "options.h"
#include "overlay_manager.h"
#include "party.h"
#include "player_data.h"
#include "pokemon_storage_system.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "save_arrays.h"
#include "overlay_14.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

BOOL PCBox_Init(OverlayManager *man, int *state) {
    UnkStruct_ov14_boxapp *work;
    PCBoxArgs *args;
    PCStorage *storage;

    Heap_Create(HEAP_ID_3, HEAP_ID_9, 0x10000);
    work = OverlayManager_CreateAndGetData(man, sizeof(UnkStruct_ov14_boxapp), HEAP_ID_9);
    MI_CpuFill8(work, 0, sizeof(UnkStruct_ov14_boxapp));
    args = OverlayManager_GetArgs(man);
    work->unk_0 = args;
    storage = SaveArray_PCStorage_Get(args->saveData);
    work->storage = storage;
    work->unk1F = PCStorage_GetActiveBox(storage);
    work->party = SaveArray_Party_Get(work->unk_0->saveData);
    work->bag = Save_Bag_Get(work->unk_0->saveData);
    work->options = Save_PlayerData_GetOptionsAddr(work->unk_0->saveData);
    work->unk21 = 0xff;
    work->unk2C = 0;
    work->unk30 = 0xb;
    return 1;
}

BOOL PCBox_Main(OverlayManager *man, int *state) {
    if (ov14_021EAF8C(OverlayManager_GetData(man), state) == 0) {
        return 1;
    }
    return 0;
}

BOOL PCBox_Exit(OverlayManager *man, int *state) {
    UnkStruct_ov14_boxapp *work = OverlayManager_GetData(man);

    PCStorage_SetActiveBox(work->storage, work->unk1F);
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_9);
    return 1;
}
