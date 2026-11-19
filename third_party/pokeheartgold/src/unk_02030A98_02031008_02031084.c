#include "global.h"

#include "save.h"
#include "unk_02030A98.h"

extern s32 _021D2AFC;

void Save_Frontier_Commit(SaveData *saveData) {
    MI_CpuCopy8(&_021D2AFC, SaveArray_Get(saveData, 0x13), 0x1628U);
}

void Save_Frontier_Load(SaveData *saveData) {
    MI_CpuCopy8(SaveArray_Get(saveData, 0x13), &_021D2AFC, 0x1628U);
}
