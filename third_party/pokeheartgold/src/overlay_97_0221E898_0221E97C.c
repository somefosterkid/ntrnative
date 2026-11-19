#include "global.h"
#include "pm_string.h"
#include "pokemon_storage_system.h"

void ov97_0221E97C(String *arg0, PCStorage *arg1, u32 arg2);

void ov97_0221E97C(String *arg0, PCStorage *arg1, u32 arg2) {
    PCStorage_GetBoxName(arg1, arg2, arg0);
}
