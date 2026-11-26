#include "global.h"

#include "field_system.h"
#include "union_room_manager.h"

u8 *sub_0205A4B4(int param_1, void *param_2, int param_3);

u8 *sub_0205A4B4(int param_1, void *param_2, int param_3) {
    FieldSystem *fieldSystem = param_2;

    return (u8 *)fieldSystem->unk80->receivedTrainerCases[param_1];
}

void sub_0205A4C4(int param_1, int param_2, void *param_3, void *param_4);

void sub_0205A4C4(int param_1, int param_2, void *param_3, void *param_4) {
    FieldSystem *fieldSystem = param_4;

    fieldSystem->unk80->partySelectionResults[param_1] = *(u8 *)param_3;
}
