// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/battle_examination.h: the lines in use
// so far.
#ifndef BWPORT_SAVE_BATTLE_EXAMINATION_H
#define BWPORT_SAVE_BATTLE_EXAMINATION_H

#include "types.h"
#include "field/bsubway_scr.h"
#include "struct_decls.h"

// Black 2's, not checked against Black
#define BATTLE_EXAMINATION_TRAINER_COUNT 5

#define BATTLE_EXAMINATION_SIGNATURE 0xd68
#define BATTLE_EXAMINATION_SIGNATURE_USED 0x7da
#define BATTLE_EXAMINATION_NUMBER_MASK 0x7f
#define BATTLE_EXAMINATION_DOUBLE 0x8000

// Black 2's; twinned code confirms the offsets of signature, flags and crc, and its size in Black
struct BattleExaminationSave {
    BSubwayTrainer trainers[BATTLE_EXAMINATION_TRAINER_COUNT];
    // What the Trial House copies into its own save block
    u8 unk5A0[0x22];
    // BATTLE_EXAMINATION_SIGNATURE, or BATTLE_EXAMINATION_SIGNATURE_USED once the examination was taken
    u16 signature;
    // The examination's number and BATTLE_EXAMINATION_DOUBLE
    u16 flags;
    // getCRC16 of everything before it
    u16 crc;
}; // size: 0x5c8

// pokebw2's BattleExamination_Init
void func_0200e590(BattleExaminationSave *examination);

#endif // BWPORT_SAVE_BATTLE_EXAMINATION_H
