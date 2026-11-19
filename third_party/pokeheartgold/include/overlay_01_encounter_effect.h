#ifndef POKEHEARTGOLD_OVERLAY_01_ENCOUNTER_EFFECT_H
#define POKEHEARTGOLD_OVERLAY_01_ENCOUNTER_EFFECT_H

// EncounterEffect: merged by tools/hg/promote.py from 4 m2c local copies (1 variants, base
// src/overlay_01_021EFB38_021EFB64.c) and 4 accesses; fields at every accessed offset, pads elsewhere.

struct FieldSystem;
struct NARC;

typedef struct EncounterEffect {
    int state;
    BOOL effectComplete;
    int unk_08;
    void *param;
    struct FieldSystem *fieldSystem;
    BOOL *done;
    BOOL hBlankFlag;
    BOOL unk_1C;
    struct NARC *narc;
} EncounterEffect;

// sizeof(EncounterEffect) == 0x24

#endif // POKEHEARTGOLD_OVERLAY_01_ENCOUNTER_EFFECT_H
