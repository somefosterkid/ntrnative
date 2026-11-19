#include "global.h"

#include "unk_02034354.h"
#include "unk_02037C94.h"

typedef void (*FieldCommTask)(void);
void sub_020582F4(FieldCommTask task, u16 time);
void sub_02058388(void);

void sub_0205836C(void);

void sub_0205836C(void) {
    if (!sub_02037EC8()) {
        return;
    }

    sub_02034434();
    sub_020582F4(sub_02058388, 0);
}
