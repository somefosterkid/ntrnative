// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/trainer_card_savedata.c, ported by tools/port_file.py.
#include "types.h"
#include "save/records.h"
#include "gfl/std.h"
#include "nitro/os.h"
#include "save/save_control.h"
#include "nitro/mi.h"

// The save block of the records, which the trainer card shows (the ROM has no string for the file, so the name is a
// guess). Block 0x26.

#define SAVE_BLOCK_TRAINER_CARD 0x26

u32 func_02008c20(void) {
    return sizeof(GameRecords);
}

void initTrainerCardInfoBlk(GameRecords *records) {
    MIi_CpuClear32(0, records, sizeof(GameRecords));
    records->seedHigh = OS_GetVBlankCount() | (OS_GetVBlankCount() << 8);
    EncryptRecordStorage(records, 1);
}

GameRecords *getTrainerCardInfoBlkAddress(SaveControl *save) {
    return SaveControl_GetBlockPtr(save, SAVE_BLOCK_TRAINER_CARD);
}
