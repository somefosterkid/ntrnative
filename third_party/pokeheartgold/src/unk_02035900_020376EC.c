#include "global.h"
#include "sys_task.h"
#include "unk_02033AE0.h"
#include "unk_02037C94.h"
#include "unk_02035900_work.h"

BOOL sub_02037030(int cmd, const void *data, int size);

BOOL sub_020376EC(int cmd);

BOOL sub_0203567C(void);
BOOL sub_020376F8(void);

s32 sub_0203569C(void);
s32 sub_02037700(void);
u8 sub_02035FF0(void);
s32 sub_0203772C(u8 arg0);

BOOL sub_020376EC(int cmd) {
    return sub_02037030(cmd, 0L, 0);
}

BOOL sub_020376F8(void) {
    return sub_0203567C();
}

s32 sub_02037700(void) {
    if ((_021D4140.sCommunicationSystem != NULL) && (_021D4140.sCommunicationSystem->unk_6B8 != 0)) {
        sub_020398D4(1, 1);
        return 1;
    }
    return sub_0203569C();
}

s32 sub_0203772C(u8 arg0) {
    if ((u32)sub_02033FC4(arg0) >= 5U) {
        return 0xC;
    }
    if (sub_02035FF0() == 0) {
        return 0xC;
    }
    return 0x26;
}
