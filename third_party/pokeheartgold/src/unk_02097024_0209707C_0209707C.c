#include "global.h"

#include "constants/heap.h"

#include "field_system.h"
#include "field_types_def.h"
#include "game_stats.h"
#include "heap.h"
#include "options.h"
#include "player_data.h"
#include "save.h"
#include "unk_struct_0209_c1ec.h"

void *sub_0209707C(FieldSystem *fieldSystem);

void *sub_0209707C(FieldSystem *fieldSystem) {
    UnkStruct_0209C1EC *temp_r0;

    temp_r0 = Heap_Alloc(HEAP_ID_FIELD2, sizeof(UnkStruct_0209C1EC));
    memset(temp_r0, 0, sizeof(UnkStruct_0209C1EC));
    temp_r0->fieldSystem = fieldSystem;
    temp_r0->unk_08.saveData = fieldSystem->saveData;
    temp_r0->unk_08.unk_0C = (struct UnkStruct_0205C22C_t *)fieldSystem->unk84;
    temp_r0->unk_08.options = Save_PlayerData_GetOptionsAddr(fieldSystem->saveData);
    temp_r0->unk_08.records = Save_GameStats_Get(fieldSystem->saveData);
    temp_r0->unk_08.unk_04 = Options_GetFrame(temp_r0->unk_08.options);
    temp_r0->unk_08.fieldSystem = fieldSystem;
    return temp_r0;
}
