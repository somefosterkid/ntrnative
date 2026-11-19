#include "global.h"

#include "field_types_def.h"
#include "overlay_01.h"
#include "overlay_02.h"
#include "player_avatar.h"
#include "pokemon_types_def.h"
#include "constants/heap.h"

u32 ov02_0224C4B4(void *param_1, FieldSystem *param_2, FieldMoveTaskEnvironment *param_3);

s32 ov01_022062CC(FieldSystem *);

u32 ov02_0224C4B4(void *param_1, FieldSystem *param_2, FieldMoveTaskEnvironment *param_3) {
    u32 gender;
    void *pvVar1;

    gender = PlayerAvatar_GetGender(param_2->playerAvatar);
    pvVar1 = ov02_02249458(param_2, 0, param_3->mon, gender);
    param_3->cutIn = pvVar1;
    param_3->state = param_3->state + 1;
    return 0;
}

u32 ov02_0224C4D8(void *param_1, void *param_2, FieldMoveTaskEnvironment *param_3);

u32 ov02_0224C4D8(void *param_1, void *param_2, FieldMoveTaskEnvironment *param_3) {
    BOOL BVar1;

    BVar1 = ov02_0224953C(param_3->cutIn);
    if (BVar1 == 0) {
        return 0;
    }
    ov02_02249548(param_3->cutIn);
    ov01_02205D68(param_3->fieldSystem);
    param_3->state = param_3->state + 1;
    return 0;
}

FieldMoveTaskEnvironment *FieldMoveTask_CreateTeleportEnvironment(FieldSystem *fieldSystem, Pokemon *mon, u8 slotno, enum HeapID heapID) {
    FieldMoveTaskEnvironment *fieldMoveTaskEnvironment;

    fieldMoveTaskEnvironment = ov02_0224C660(heapID, sizeof(FieldMoveTaskEnvironment));
    fieldMoveTaskEnvironment->type = 2;
    fieldMoveTaskEnvironment->fieldSystem = fieldSystem;
    fieldMoveTaskEnvironment->mapObject = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
    fieldMoveTaskEnvironment->mon = mon;
    if ((u32)(PlayerAvatar_GetState(fieldSystem->playerAvatar) - 1) <= 1U) {
        fieldMoveTaskEnvironment->followMonActive = 0;
    } else if ((ov01_02206268(fieldSystem) != 0) && (slotno == ov01_022062CC(fieldSystem))) {
        fieldMoveTaskEnvironment->followMonActive = 1;
    } else {
        fieldMoveTaskEnvironment->followMonActive = 0;
    }
    return fieldMoveTaskEnvironment;
}
