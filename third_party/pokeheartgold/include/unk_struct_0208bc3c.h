#ifndef POKEHEARTGOLD_UNK_STRUCT_0208BC3C_H
#define POKEHEARTGOLD_UNK_STRUCT_0208BC3C_H

// UnkStruct_0208BC3C: merged by tools/hg/promote.py from 3 m2c local copies (1 variants, base
// src/unk_02087A78_02087B64.c) and 27 accesses; fields at every accessed offset, pads elsewhere.

struct ApplicationManager;
struct FieldBattleDTO;
struct FieldSystem;
struct SaveData;

typedef struct UnkStruct_0208BC3C {
    int unk_00;
    int unk_04;
    struct ApplicationManager *appMan;
    struct SaveData *saveData;
    struct FieldBattleDTO *unk_10;
    void *unk_14;
    struct { BOOL unk_00; int unk_04; u8 padding_08[8]; } unk_18;
    struct FieldSystem *fieldSystem;
} UnkStruct_0208BC3C;

// sizeof(UnkStruct_0208BC3C) == 0x2c

#endif // POKEHEARTGOLD_UNK_STRUCT_0208BC3C_H
