#ifndef POKEHEARTGOLD_OVERLAY_33_OV33WORK_H
#define POKEHEARTGOLD_OVERLAY_33_OV33WORK_H

// Ov33Work: merged by tools/hg/promote.py from 3 m2c local copies (1 variants, base
// src/overlay_33_0225D520.c) and 2 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct Ov33Args;
struct SysTask;

typedef struct Ov33Work {
    struct BgConfig *bgConfig;
    void *unk4;
    u32 unk8;
    struct Ov33Args *unkC;
    struct Ov33Args *unk10;
    struct SysTask *sysTask;
    struct { struct BgConfig *bgConfig; u8 bgId; u8 tilemapLeft; u8 tilemapTop; u8 width; u8 height; u8 paletteNum; u16 baseTile : 15; u16 colorMode : 1; void *pixelBuffer; } windows[2];
    u8 buffer[384];
    u16 state;
    u16 count;
    u32 touchedHitbox;
} Ov33Work;

// sizeof(Ov33Work) == 0x1c0

#endif // POKEHEARTGOLD_OVERLAY_33_OV33WORK_H
