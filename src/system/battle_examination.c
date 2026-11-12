// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/battle_examination.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include <stddef.h>
#include "types.h"
#include "save/battle_examination.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "nitro/os.h"
#include "save/save_control.h"
#include "save/trial_house.h"
#include "system/game_data.h"
#include "stddef.h"

// The downloaded battle examination that the Trial House plays: five Trainers, a signature and a checksum.
// Extra save block 5.

#define SAVE_EXTRA_BLOCK_BATTLE_EXAMINATION 5
#define BATTLE_EXAMINATION_LOAD_SIZE 0x1000 // Black's

// pokebw2's BattleExamination_GetSize
u32 func_0200e588(void) {
    return sizeof(BattleExaminationSave);
}

// pokebw2's BattleExamination_Init
void func_0200e590(BattleExaminationSave *examination) {
    MI_CpuFill8(examination, 0, sizeof(BattleExaminationSave));
}

// pokebw2's BattleExamination_IsValid
BOOL func_0200e5a0(void *saveBuffer) {
    BattleExaminationSave *examination = saveBuffer;

    if (examination->crc == getCRC16(examination, offsetof(BattleExaminationSave, crc))
        && examination->signature == BATTLE_EXAMINATION_SIGNATURE) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's BattleExamination_IsSingle
BOOL func_0200e5cc(void *extraSave) {
    BattleExaminationSave *examination = extraSave;

    if (!(examination->flags & BATTLE_EXAMINATION_DOUBLE)) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's BattleExamination_GetNumber
u32 func_0200e5e4(void *saveBuffer) {
    BattleExaminationSave *examination = saveBuffer;

    return (u16)(examination->flags & BATTLE_EXAMINATION_NUMBER_MASK);
}

// pokebw2's BattleExamination_GetTrainer
BSubwayTrainer *func_0200e5f8(void *save, u32 index) {
    BattleExaminationSave *examination = save;

    if (index < BATTLE_EXAMINATION_TRAINER_COUNT) {
        return &examination->trainers[index];
    }
    return examination->trainers;
}

// pokebw2's BattleExamination_Load
// pokebw2's BattleExamination_Load
void func_0200e608(GameData *gameData, void *saveBuffer, HeapID heapId) {
    SaveControl *save = GameData_GetSaveControl(gameData);
    void *buffer = GFL_HeapAllocate(heapId, BATTLE_EXAMINATION_LOAD_SIZE, FALSE, "battle_examination.c", 153);

    func_020072cc(save, SAVE_EXTRA_BLOCK_BATTLE_EXAMINATION, heapId, buffer, BATTLE_EXAMINATION_LOAD_SIZE);
    MI_CpuCopy8(saveBuffer, buffer, sizeof(BattleExaminationSave));
    func_02012e4c(gameData, SAVE_EXTRA_BLOCK_BATTLE_EXAMINATION);
    for (;;) {
        if (func_02012e7c(gameData, SAVE_EXTRA_BLOCK_BATTLE_EXAMINATION) == 2) {
            break;
        }
        OS_WaitIrq(TRUE, 1);
    }
    freeIntermediateSaveExtraBlksAfterLoad2(save, SAVE_EXTRA_BLOCK_BATTLE_EXAMINATION);
    GFL_HeapFree(buffer);
}


// pokebw2's BattleExamination_SetUsed
void func_0200e684(void *extraSave) {
    BattleExaminationSave *examination = extraSave;

    if (examination->crc == getCRC16(examination, offsetof(BattleExaminationSave, crc))) {
        examination->signature = BATTLE_EXAMINATION_SIGNATURE_USED;
        examination->crc = getCRC16(examination, offsetof(BattleExaminationSave, crc));
    }
}
