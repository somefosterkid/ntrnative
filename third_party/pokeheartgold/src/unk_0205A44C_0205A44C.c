#include "global.h"

#include "field_system.h"
#include "union_room_manager.h"
#include "unk_02037C94.h"

void sub_0205A44C(int param_1, int param_2, void *param_3, void *param_4);

void sub_0205A44C(int param_1, int param_2, void *param_3, void *param_4) {
    FieldSystem *fieldSystem = param_4;

    fieldSystem->unk80->unused_2C = 1;
    fieldSystem->unk80->activityAnswer = *(u8 *)param_3;
    if (*(u8 *)param_3 == 4) {
        sub_020388E0();
    }
}
