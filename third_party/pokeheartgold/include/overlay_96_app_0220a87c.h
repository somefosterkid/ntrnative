#ifndef POKEHEARTGOLD_OVERLAY_96_APP_0220A87C_H
#define POKEHEARTGOLD_OVERLAY_96_APP_0220A87C_H

// Ov96App_0220A87C: the 0x184 per-event work of the Pokeathlon ov96_0220A744 app. unk_14 is an embedded 0x140-byte
// sub-object (ov96_0220B178 / ov96_0220B1B8 / ov96_0220B324 / ov96_0220B354 take it), unk_154 holds ten
// UnkStruct_ov96_0220B148 slots (ov96_0220AE40 allocates them).
// unk_17C: low 16 bits tick counter, high 16 bits value read by ov96_0220AD4C.
// unk_180: bits 0..3 state (1 = ov96_0220AD64 phase, 2 = ov96_0220AE28 phase), bits 4..7 current slot index.

struct PokeathlonCourseData;
struct ManagedSprite;
struct SpriteManager;
struct SpriteSystem;
struct UnkStruct_ov96_0220B148;

typedef struct Ov96App_0220A87C {
    struct PokeathlonCourseData *course;
    struct SpriteSystem *spriteSystem;
    struct SpriteManager *spriteManager;
    struct ManagedSprite *unk_C;
    struct ManagedSprite *unk_10;
    u8 unk_14[0x140];
    struct UnkStruct_ov96_0220B148 *unk_154[10];
    u32 unk_17C;
    s32 unk_180;
} Ov96App_0220A87C;

// sizeof(Ov96App_0220A87C) == 0x184 on the DS

#endif // POKEHEARTGOLD_OVERLAY_96_APP_0220A87C_H
