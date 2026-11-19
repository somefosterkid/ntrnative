#ifndef POKEHEARTGOLD_FRONTIER_GRAPHICS_H
#define POKEHEARTGOLD_FRONTIER_GRAPHICS_H

// FrontierGraphics: merged by tools/hg/promote.py from 14 m2c local copies (1 variants, base
// src/frontier_map_0223885C.c) and 31 accesses; fields at every accessed offset, pads elsewhere.

struct BattleFrontier;
struct BgConfig;
struct FrontierParticleSystem;
struct G3DPipelineBuffers;
struct ManagedSprite;
struct PaletteData;
struct SpriteManager;
struct SpriteSystem;
struct SysTask;
struct UnkStruct_ov63_0222BCE8_t;
struct UnkStruct_ov63_0222BE18_t;
struct UnkStruct_ov63_0222D1C0_t;
struct UnkStruct_ov63_0222D77C_t;
struct UnkStruct_ov63_0222D848_t;

typedef struct FrontierGraphics {
    struct BgConfig *bgConfig;
    struct PaletteData *plttData;
    struct BattleFrontier *frontier;
    struct G3DPipelineBuffers *g3dPipeline;
    struct FrontierParticleSystem *particleSys;
    struct UnkStruct_ov63_0222BE18_t *unk_14;
    struct UnkStruct_ov63_0222BCE8_t *unk_18;
    struct { s16 unk_00; s16 unk_02; } unk_1C;
    void *unk_20;
    struct UnkStruct_ov63_0222D1C0_t *unk_24;
    struct UnkStruct_ov63_0222D1C0_t *unk_28;
    struct UnkStruct_ov63_0222D848_t *unk_2C;
    struct UnkStruct_ov63_0222D77C_t *unk_30;
    struct SpriteSystem *spriteSystem;
    struct SpriteManager *spriteMan;
    struct { struct ManagedSprite *unk_00[8]; u16 unk_20[8]; u32 unk_30; u16 unk_34[8]; } unk_3C;
    struct ManagedSprite *unk_80[4];
    void *unk_90;
    struct SysTask *unk_94;
    struct SysTask *unk_98;
    struct SysTask *unk_9C;
    struct SysTask *dummyExecuteOnVBlank;
    struct { struct SysTask *unk_00; s16 unk_04; s16 unk_06; s8 unk_08; s8 unk_09; s8 unk_0A; u8 unk_0B; } unk_A4;
    struct { struct SysTask *unk_00; u8 unk_04; u8 unk_05; u8 unk_06; u8 unk_07; s16 unk_08; s8 unk_0A; u8 unk_0B; } unk_B0;
    u16 unk_BC;
    u16 unk_BE;
    u8 unk_C0;
    u8 sceneID;
    u8 unk_C2;
    u8 pad_C3[0x1];
} FrontierGraphics;

// sizeof(FrontierGraphics) == 0xc4

#endif // POKEHEARTGOLD_FRONTIER_GRAPHICS_H
