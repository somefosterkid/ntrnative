#include "global.h"

#include "field_communication_manager.h"

void sub_020588F8(void);

void sub_02058930(void);

u32 sub_02058258(void);

u32 sub_02058258(void) {
    if (_021D41C8->task == sub_020588F8) {
        return 0;
    }
    if (_021D41C8->task != sub_02058930) {
        return 1;
    }
    return 0;
}
