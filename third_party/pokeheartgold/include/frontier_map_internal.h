#ifndef POKEHEARTGOLD_FRONTIER_MAP_INTERNAL_H
#define POKEHEARTGOLD_FRONTIER_MAP_INTERNAL_H

// FrontierMapInternal: merged by tools/hg/promote.py from 2 m2c local copies (1 variants, base
// src/frontier_map_02238648.c) and 9 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct GF3DVramMan;
struct ManagedSprite;
struct PaletteData;
struct SpriteManager;
struct SpriteSystem;
struct SysTask;
struct UnkStruct_ov42_02227F68;
struct UnkStruct_ov42_022280A8;
struct UnkStruct_ov42_02228EDC;
struct UnkStruct_ov42_022293B8;
struct UnkStruct_ov42_022299C0;
struct UnkStruct_ov42_02229A40;

typedef struct FrontierMapInternal {
    struct BgConfig *bgConfig;
    struct PaletteData *paletteData;
    void *work;
    struct GF3DVramMan *vramMan;
    void *particles;
    struct UnkStruct_ov42_022280A8 *unk14;
    struct UnkStruct_ov42_02227F68 *unk18;
    struct { s16 unk0; s16 unk2; } scroll;
    struct UnkStruct_ov42_02228EDC *unk20;
    struct UnkStruct_ov42_022293B8 *unk24;
    struct UnkStruct_ov42_022293B8 *unk28;
    struct UnkStruct_ov42_02229A40 *unk2C;
    struct UnkStruct_ov42_022299C0 *unk30;
    struct SpriteSystem *spriteSystem;
    struct SpriteManager *spriteManager;
    struct { struct ManagedSprite *sprites[8]; u16 ids[8]; u32 flags; u16 resIds[8]; } spriteData;
    struct ManagedSprite *unk80[4];
    u8 unk90[4];
    struct SysTask *unk94;
    struct SysTask *unk98;
    struct SysTask *unk9C;
    struct SysTask *unkA0;
    u8 unkA4[4];
    s16 unkA8;
    s16 unkAA;
    u8 padAC[21];
    u8 scene;
    u8 padC2[2];
} FrontierMapInternal;

// sizeof(FrontierMapInternal) == 0xc4

#endif // POKEHEARTGOLD_FRONTIER_MAP_INTERNAL_H
