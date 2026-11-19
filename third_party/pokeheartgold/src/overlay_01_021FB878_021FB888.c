#include "global.h"

#include "constants/heap.h"

#include "field/area_data.h"

#include "filesystem.h"
#include "filesystem_files_def.h"
#include "heap.h"

AreaDataManager *AreaDataManager_Alloc(u8 areaDataBank) {
    AreaDataManager *temp_r4;
    NARC *var_r0;

    temp_r4 = Heap_Alloc(HEAP_ID_FIELD1, sizeof(AreaDataManager));
    temp_r4->loadCtx = Heap_AllocAtEnd(HEAP_ID_FIELD1, sizeof(AreaDataLoadContext));
    temp_r4->loadCtx->areaDataBank = (s32)areaDataBank;
    temp_r4->loadCtx->unk_04 = 0;
    ReadWholeNarcMemberByIdPair(&temp_r4->header, NARC_a_0_4_2, (s32)areaDataBank);
    if (temp_r4->header.buildModelKind != 0) {
        temp_r4->mapPropAnimListNarc = NARC_New(NARC_a_1_0_7, HEAP_ID_FIELD1);
        var_r0 = NARC_New(NARC_a_0_4_0, HEAP_ID_FIELD1);
    } else {
        temp_r4->mapPropAnimListNarc = NARC_New(NARC_a_1_0_8, HEAP_ID_FIELD1);
        var_r0 = NARC_New(NARC_a_1_4_8, HEAP_ID_FIELD1);
    }
    temp_r4->loadCtx->mapPropModelNarc = var_r0;
    return temp_r4;
}
