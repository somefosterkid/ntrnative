#ifndef POKEHEARTGOLD_OVERLAY_106_ARCEUS_CINEMATIC_H
#define POKEHEARTGOLD_OVERLAY_106_ARCEUS_CINEMATIC_H

// ArceusCinematic: merged by tools/hg/promote.py from 1 m2c local copies (1 variants, base
// src/overlay_106_021E601C_021E62F4_021E63A4.c) and 1 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct ManagedSprite;
struct NARC;
struct SpriteManager;
struct SpriteSystem;

typedef struct ArceusBgPalette {
    u8 data[0x2000];
    u32 size;
} ArceusBgPalette; // 0x2004

typedef struct ArceusCinematic {
    struct BgConfig *bgConfig;
    struct NARC *narc;
    struct SpriteSystem *spriteSystem;
    struct SpriteManager *spriteManager;
    struct ManagedSprite *sprites[1];
    int *scriptPos;
    u16 savedBgm;
    u8 pad_1A[0x2];
    int blendPlane1;
    int blendStartEv;
    int blendPlane2;
    int blendStartEv2;
    int blendTargetEv;
    int blendDuration;
    ArceusBgPalette bgPalettes[3];
} ArceusCinematic;

// sizeof(ArceusCinematic) == 0x6040

#endif // POKEHEARTGOLD_OVERLAY_106_ARCEUS_CINEMATIC_H
