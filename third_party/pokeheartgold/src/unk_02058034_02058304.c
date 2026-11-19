#include "global.h"

#include "unk_02034354.h"
#include "unk_02035900.h"

typedef void (*FieldCommTask)(void);
void ov03_02255C54(void);
void sub_020582F4(FieldCommTask task, u16 time);
void sub_02058328(void);

void sub_02058304(void);

void sub_02058304(void) {
    if (!sub_020373B4(sub_0203769C())) {
        return;
    }

    ov03_02255C54();

    sub_02034434();
    sub_020582F4(sub_02058328, 0);
}
