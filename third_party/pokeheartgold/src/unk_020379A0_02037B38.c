#include "global.h"
#include "comm_tool.h"


s32 sub_02037B38(u8 a0);

s32 sub_02037B38(u8 a0) {
    if (_021D414C == NULL) {
        return 1;
    }
    if (_021D414C->syncNoPersonal == a0) {
        return 1;
    }
    return 0;
}

u8 sub_02037B5C(s32 arg0);

u8 sub_02037B5C(s32 arg0) {
    return _021D414C->syncNo[arg0];
}
