#ifndef POKEHEARTGOLD_OVERLAY_12_TRAINER_ENCOUNTER_DATA_H
#define POKEHEARTGOLD_OVERLAY_12_TRAINER_ENCOUNTER_DATA_H

// TrainerEncounterData: merged by tools/hg/promote.py from 1 m2c local copies (1 variants, base
// src/overlay_12_battle_controller_opponent_0225AE48_0225CE28_0225CE28.c) and 10 accesses; fields at every accessed offset, pads elsewhere.

struct BattleSystem;
struct ManagedSprite;
struct Terrain;

typedef struct TrainerEncounterData {
    struct BattleSystem *battleSys;
    void *pokepic;
    struct Terrain *terrain;
    struct ManagedSprite *managedSprite;
    u8 command;
    u8 battler;
    u8 state;
    u8 face;
    s16 targetX;
    u16 ballFlashStarted : 1;
    u16 padding_12_1 : 15;
    int battlerType;
    int delay;
    int enterFrameCount;
} TrainerEncounterData;

// sizeof(TrainerEncounterData) == 0x24

BOOL ov12_02261DC8(TrainerEncounterData *arg0, struct ManagedSprite *unused);

#endif // POKEHEARTGOLD_OVERLAY_12_TRAINER_ENCOUNTER_DATA_H
