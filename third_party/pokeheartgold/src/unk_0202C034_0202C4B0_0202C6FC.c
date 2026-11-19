#include "global.h"

#include "save.h"
#include "unk_0202C034.h"

extern s32 _021D2230;

void sub_0202C6FC(SaveData *saveData) {
    MI_CpuCopy8(SaveArray_Get(saveData, 0x19), &_021D2230, 0x8C0U);
}

void sub_0202C714(SaveData *saveData) {
    MI_CpuCopy8(&_021D2230, SaveArray_Get(saveData, 0x19), 0x8C0U);
}
