#include "global.h"

#include "constants/heap.h"

#include "battle/battle.h"

#include "bg_window.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "heap.h"
#include "overlay_manager.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "overlay_18.h"
#include "overlay_18_pokedex_entry_popup.h"

PokedexEntryPopup *ov18_021F8974(UnkStruct_50C *unkStruct) {
    PokedexEntryPopup *data;

    data = Heap_Alloc(unkStruct->heapID, sizeof(PokedexEntryPopup));
    memset(data, 0, sizeof(PokedexEntryPopup));
    memcpy(data, unkStruct, sizeof(UnkStruct_50C));
    data->gfxNarc = NARC_New(NARC_graphic_zukan_gra, data->heapId);
    data->unk_240 = 0;
    data->unk_254 = 0;
    data->task = SysTask_CreateOnMainQueue(ov18_021F89F8, data, 0);
    return data;
}

s32 ov18_021F89C8(PokedexEntryPopup *popup) {
    return popup->unk_254;
}

void ov18_021F89D0(PokedexEntryPopup *popup) {
    ov18_021F91F0(popup);
    ov18_021F8F10(popup);
    ov18_021F8BEC(popup);
    NARC_Delete(popup->gfxNarc);
    SysTask_Destroy(popup->task);
    Heap_Free(popup);
}
