// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/field_encount_st.c, its first five
// functions: Black's file ends there, before move_pokemon.c. Black's encounter state is smaller and made without a
// terrain, and a zone without encounters has the encounter ID 0xffff.
#include "types.h"
#include "field/encounter.h"
#include "field/field.h"
#include "field/zone.h"
#include "gfl/arc.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "system/game_data.h"

BOOL EncData_Load(EncData *encData, ArcTool *arc, u16 zoneId, u8 season) {
    u32 fileId;
    u32 count;

    MI_CpuFill8(encData, 0, sizeof(EncData));
    fileId = GetZoneEncID(zoneId);
    if (fileId == 0xffff) {
        return FALSE;
    }
    count = GFL_ArcToolGetDataLength(arc, fileId) / sizeof(EncData);
    if (count != 1 && count != 4) {
        return FALSE;
    }
    if (count == 1) {
        season = 0;
    }
    GFL_ArcToolReadRange(arc, fileId, season * sizeof(EncData), sizeof(EncData), encData);
    encData->fishingEnable = TRUE;
    return TRUE;
}

EncountState *EncountState_Create(HeapID heapId) {
    return GFL_HeapAllocate(heapId, sizeof(EncountState), TRUE, "field_encount_st.c", 0x2d);
}

void EncountState_Free(EncountState *state) {
    GFL_HeapFree(state);
}

// pokebw2's func_ov012_0215917c
void func_ov010_0215e33c(Field *field) {
    EncountSystem *system = Field_GetEncountSystem(field);

    func_ov021_021aa7dc(system, system->effectEncountState);
}

// pokebw2's func_ov012_021591b4
void func_ov010_0215e34c(GameData *gameData) {
    EncountState *state = GameData_GetEncountState(gameData);

    state->unkC = 0;
}
