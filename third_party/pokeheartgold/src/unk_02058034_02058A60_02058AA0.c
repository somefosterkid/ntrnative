#include "global.h"

#include "field_communication_manager.h"

void sub_0205876C(void);

void sub_020587E8(void);

void sub_0205882C(void);

void sub_02058854(void);

void sub_02058870(void);

u32 sub_02058AA0(void);

u32 sub_02058AA0(void) {
    void (*pcVar1)();

    if (_021D41C8 == 0) {
        return 0;
    }
    pcVar1 = _021D41C8->task;
    if (pcVar1 == sub_02058870 || pcVar1 == sub_02058854 || pcVar1 == sub_0205882C || pcVar1 == sub_020587E8 || pcVar1 == sub_0205876C) {
        return 1;
    }
    return 0;
}
