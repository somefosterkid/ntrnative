#include "global.h"

#include "comm_player_manager.h"
#include "unk_02035900.h"

s32 sub_020572AC(void *);

void sub_020572DC(SysTask *task, void *arg1);

void sub_020572DC(SysTask *task, void *arg1) {
    if (sub_02037474() != 0) {
        sub_020572AC(arg1);
        if (sub_0203769C() == 0) {
            sub_0205712C(arg1);
            sub_020570C8(arg1);
        } else {
            sub_020570C8(arg1);
        }
        sub_02057314(arg1);
    }
}
