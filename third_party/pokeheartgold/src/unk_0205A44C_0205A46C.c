#include "global.h"

#include "field_system.h"
#include "union_room_manager.h"
#include "unk_0205A44C.h"

void sub_0205A46C(int arg0, int arg1, void *arg2, void *arg3) {
    FieldSystem *fieldSystem = arg3;

    fieldSystem->unk80->chatEnded = 1;
}
