#include "types.h"
#include "constants/arc.h"
#include "field/field_lens_flare.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/heap.h"
#include "system/aeabi.h"

// The lens flares of each effect set, ended by LENS_FLARE_NONE
// clang-format off
static const u16 LENS_FLARE_IDS[10][4] = {
#include "asset/field_goout_effect_data_LENS_FLARE_IDS.inc"
};

// clang-format on

// The resources of each lens flare: its model, then its animations
// clang-format off
static const u16 LENS_FLARE_RESOURCE_IDS[8][6] = {
#include "asset/field_goout_effect_data_LENS_FLARE_RESOURCE_IDS.inc"
};

// clang-format on

FieldLensFlareData *FieldLensFlareData_Create(HeapID heapId) {
    FieldLensFlareData *data;

    data = GFL_HeapAllocate(heapId, sizeof(FieldLensFlareData), TRUE, "field_goout_effect_data.c", 57);
    data->entries = GFL_ArcSysReadHeapNewLZGetLen(ARCID_FIELD_GOOUT_EFFECT_DATA, 0, 0, heapId, &data->byteCount);
    return data;
}

void FieldLensFlareData_Free(FieldLensFlareData *data) {
    GFL_HeapFree(data->entries);
    GFL_HeapFree(data);
}

u16 FieldLensFlare_GetEffectSetID(FieldLensFlareData *data, u32 entryIndex, u32 effectIndex, u32 subIndex) {
    FieldLensFlareEntry *entry;

    entry = FieldLensFlareData_GetEntry(data, entryIndex);
    return entry->effectSetIds[effectIndex][subIndex];
}

u32 FieldLensFlareData_BytesToEntryCount(FieldLensFlareData *data) {
    return _u32_div_f(data->byteCount, sizeof(FieldLensFlareEntry));
}

u16 FieldLensFlareData_GetLensFlareID(FieldLensFlareData *data, u32 effectSet, u32 index) {
    return LENS_FLARE_IDS[effectSet][index];
}

u8 FieldLensFlareData_GetEffectSetSize(FieldLensFlareData *data, u32 effectSet) {
    int index;

    for (index = 0; index < 4; index++) {
        if (FieldLensFlareData_GetLensFlareID(data, effectSet, index) == LENS_FLARE_NONE) {
            break;
        }
    }
    return index;
}

u16 FieldLensFlareData_GetResDatID(FieldLensFlareData *data, u32 effectSet, u32 index) {
    switch (index) {
    case 0:
        return LENS_FLARE_RESOURCE_IDS[effectSet][0];
    case 1:
        return LENS_FLARE_RESOURCE_IDS[effectSet][1];
    case 2:
        return LENS_FLARE_RESOURCE_IDS[effectSet][2];
    case 3:
        return LENS_FLARE_RESOURCE_IDS[effectSet][3];
    case 4:
        return LENS_FLARE_RESOURCE_IDS[effectSet][4];
    default:
        return 0;
    }
}

FieldLensFlareEntry *FieldLensFlareData_GetEntry(FieldLensFlareData *data, u32 index) {
    return &data->entries[index];
}

u32 FieldLensFlareData_GetIdxForZoneTransit(FieldLensFlareData *data, u16 zoneId, u16 transitId) {
    u32 index;
    FieldLensFlareEntry *entry;

    index = 0;
    while (index < FieldLensFlareData_BytesToEntryCount(data)) {
        entry = FieldLensFlareData_GetEntry(data, index);
        if (entry->zoneId == zoneId && entry->transitId == transitId) {
            break;
        }
        index++;
    }
    return index;
}
