#ifndef POKEHEARTGOLD_UNK_STRUCT_0209_C194_H
#define POKEHEARTGOLD_UNK_STRUCT_0209_C194_H

#include "global.h"

// UnkStruct_0209C194: the 0x3c app-args block of the field-launched overlay 85 family (Heap_Alloc'ed by
// sub_02097024, which copies the 7-word UnkStruct_0209C194_1 from the launcher task into +0x14 and attaches the
// UnkStruct_0209BDF8 comm state at +0x30; overlay 85 stores its two app objects at +0x34 / +0x38). The DS has no
// JournalEntry slot in the sub-struct (that is the Platinum layout, 8 words). Hoisted from 65 local copies.

struct FieldSystem;
struct GameStats;
struct Options;
struct SaveData;
struct UnkStruct_0205C22C_t;
struct UnkStruct_0209BDF8;
struct UnkStruct_ov109_021D0F70_t;
struct UnkStruct_ov109_021D5140_t;

typedef struct UnkStruct_0209C194_t UnkStruct_0209C194;

typedef struct {
    int unk_00;
    int unk_04;
    struct SaveData *saveData;
    struct UnkStruct_0205C22C_t *unk_0C;
    struct Options *options;
    struct GameStats *records;
    struct FieldSystem *fieldSystem;
} UnkStruct_0209C194_1;

// sizeof(UnkStruct_0209C194_1) == 0x1c on the DS

struct UnkStruct_0209C194_t {
    int unk_00;
    int unk_04;
    int unk_08;
    u32 unk_0C;
    BOOL unk_10;
    UnkStruct_0209C194_1 unk_14;
    struct UnkStruct_0209BDF8 *unk_30;
    struct UnkStruct_ov109_021D0F70_t *unk_34;
    struct UnkStruct_ov109_021D5140_t *unk_38;
};

// sizeof(UnkStruct_0209C194) == 0x3c on the DS

#endif // POKEHEARTGOLD_UNK_STRUCT_0209_C194_H
