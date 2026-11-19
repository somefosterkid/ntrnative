#include "global.h"
#include "comm_tool.h"


int sub_02037BA0(int param0, u8 param1);

int sub_02037BA0(int param0, u8 param1) {
    if (!_021D414C) {
        return -1;
    }

    if (_021D414C->unk_00[param0].unk_00 == param1) {
        return _021D414C->unk_00[param0].unk_01;
    }

    return -1;
}
