#ifndef POKEHEARTGOLD_OVERLAY_117_ENCOUNTER_EFFECT_117_H
#define POKEHEARTGOLD_OVERLAY_117_ENCOUNTER_EFFECT_117_H

// EncounterEffect: merged by tools/hg/promote.py from 1 m2c local copies (1 variants, base
// src/overlay_117_0225F020.c) and 3 accesses; fields at every accessed offset, pads elsewhere.

struct FieldSystem;
struct NARC;

typedef struct EncounterEffect_117 {
    int state;
    BOOL effectComplete;
    int unk_08;
    void *param;
    struct FieldSystem *fieldSystem;
    BOOL *done;
    BOOL hBlankFlag;
    BOOL unk_1C;
    struct NARC *narc;
} EncounterEffect_117;

// sizeof(EncounterEffect_117) == 0x24

#endif // POKEHEARTGOLD_OVERLAY_117_ENCOUNTER_EFFECT_117_H
