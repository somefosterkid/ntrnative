#include "unk_020968B0.h"

#include "global.h"

#include "frontier/frontier.h"

#include "field_system.h"

FieldFrontierDTO *sub_020968B0(FieldSystem *fieldSystem, void *param1) {
    FieldFrontierDTO *args = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(FieldFrontierDTO));
    MI_CpuFill8(args, 0, sizeof(FieldFrontierDTO));

    args->facilityData = param1;
    args->options = Save_PlayerData_GetOptionsAddr(fieldSystem->saveData);
    args->saveData = fieldSystem->saveData;
    args->bagCursor = fieldSystem->bagCursor;
    args->subscreenCursorOn = fieldSystem->unkB0;
    args->unk_10 = 0;
    args->unk_14 = 0;
    args->mapHeaderID = fieldSystem->location->mapId;
    args->sceneID = 14;
    args->fieldSystem = fieldSystem;
    FieldSystem_LaunchApplication(fieldSystem, &gOverlayTemplate_Frontier, args);
    return args;
}
