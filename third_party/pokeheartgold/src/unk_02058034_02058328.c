#include "global.h"

void sub_02058328(void);

typedef void (*FieldCommTask)(void);
void ov03_02255BEC(void);
BOOL sub_02034DB8(void);
void sub_020582F4(FieldCommTask task, u16 time);
void sub_02058348(void);
void sub_0205832C(void);

void sub_02058328(void) {
}

void sub_0205832C(void) {
    if (!sub_02034DB8()) {
        return;
    }

    ov03_02255BEC();
    sub_020582F4(sub_02058348, 0);
}

void sub_02058348(void) {
}
