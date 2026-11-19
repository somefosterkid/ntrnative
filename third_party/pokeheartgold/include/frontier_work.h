#ifndef POKEHEARTGOLD_FRONTIER_WORK_H
#define POKEHEARTGOLD_FRONTIER_WORK_H

// FrontierWork: merged by tools/hg/promote.py from 1 m2c local copies (1 variants, base
// src/frontier_02096608.c) and 2 accesses; fields at every accessed offset, pads elsewhere.

struct FrontierLaunchArgs;
struct FrontierMap;
struct FrontierSystem;
struct OverlayManager;

typedef struct FrontierWork {
    struct FrontierLaunchArgs *launchArgs;
    struct OverlayManager *childOvy;
    void *childArg;
    void *childCallback;
    u8 freeChildArg;
    u8 padding_011[3];
    struct FrontierSystem *system;
    struct FrontierMap *map;
    u8 padding_01C;
    u8 mapCreated;
    u8 unk1E;
    u8 padding_01F;
    u16 unk20;
    u8 unk22;
    u8 padding_023;
    u16 unk24[2][24];
    struct { u8 unk0[12]; u16 unkC; u8 unkE[46]; } unk84[32];
    u8 unk804[512];
    u16 unkA04[8];
    u8 unkA14[64];
} FrontierWork;

// sizeof(FrontierWork) == 0xa54

#endif // POKEHEARTGOLD_FRONTIER_WORK_H
