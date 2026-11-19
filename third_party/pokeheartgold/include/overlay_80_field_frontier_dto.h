#ifndef POKEHEARTGOLD_OVERLAY_80_FIELD_FRONTIER_DTO_H
#define POKEHEARTGOLD_OVERLAY_80_FIELD_FRONTIER_DTO_H

// FieldFrontierDTO: merged by tools/hg/promote.py from 39 m2c local copies (2 variants, base
// src/overlay_80_0222BDF4_0222C52C.c) and 13 accesses; fields at every accessed offset, pads elsewhere.

struct BagCursor;
struct FieldSystem;
struct JournalEntry;
struct Options;
struct SaveData;

typedef struct FieldFrontierDTO {
    void *facilityData;
    struct Options *options;
    struct SaveData *saveData;
    struct JournalEntry *journalEntry;
    struct BagCursor *bagCursor;
    int unk_14;
    int unk_18;
    int mapHeaderID;
    u8 *subscreenCursorOn;
    u8 sceneID;
    u8 pad_25[0x3];
    struct FieldSystem *fieldSystem;
} FieldFrontierDTO;

// sizeof(FieldFrontierDTO) == 0x2c

#endif // POKEHEARTGOLD_OVERLAY_80_FIELD_FRONTIER_DTO_H
