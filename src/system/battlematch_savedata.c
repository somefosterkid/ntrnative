// Black's code at main 0x0200e9dc-0x0200ea0c. The name is pokebw2's file between livematch_savedata.c and
// hall_of_fame_savedata.c (16 functions there, 6 here).

#include "types.h"
#include "app/livebattlematch_flow.h"
#include "save/records.h"
#include "struct_decls.h"
#include "save/save_control.h"

#define SAVE_BLOCK_RECORD 0x3c

u32 func_0200e9dc(void) {
    return sizeof(RecordSave);
}

// The save table's init of the block
void func_0200e9e0(void *block) {
    RecordSave *record = block;

    func_0200e904(&record->liveMatch);
    func_0200dfcc(&record->rndMatch);
}

RecordSave *getRecordBlkAddress(SaveControl *save) {
    return SaveControl_GetBlockPtr(save, SAVE_BLOCK_RECORD);
}

void *func_0200ea00(RecordSave *record) {
    return &record->rndMatch;
}

void *func_0200ea04(RecordSave *record) {
    return &record->rndMatch;
}

void *func_0200ea08(RecordSave *record) {
    return &record->liveMatch;
}
