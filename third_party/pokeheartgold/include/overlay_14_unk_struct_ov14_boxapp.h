#ifndef POKEHEARTGOLD_OVERLAY_14_UNK_STRUCT_OV14_BOXAPP_H
#define POKEHEARTGOLD_OVERLAY_14_UNK_STRUCT_OV14_BOXAPP_H

// UnkStruct_ov14_boxapp: merged by tools/hg/promote.py from 4 m2c local copies (3 variants, base
// src/overlay_14_021F15C8_021F15C8.c) and 176 accesses; fields at every accessed offset, pads elsewhere.

struct Bag;
struct BagView;
struct NamingScreenArgs;
struct Options;
struct OverlayManager;
struct Party;
struct PCBoxArgs;
struct PokemonStorageSystem;
struct UnkStruct_ov14_boxsys;

typedef struct UnkStruct_ov14_boxapp {
    struct PCBoxArgs *unk_0;          // DS 0x0 the launch args (PCBox_Init)
    struct PokemonStorageSystem *storage;
    struct Party *party;
    struct Bag *bag;                  // DS 0xC
    struct Options *options;          // DS 0x10
    struct OverlayManager *unk_14;    // DS 0x14 the sub app (naming screen / bag)
    union {                           // DS 0x18 the sub app's args
        struct NamingScreenArgs *unk_18;
        struct BagView *bagView;
    };
    u16 unk_1C;
    u8 unk_1E[0x1];
    u8 unk1F;
    u8 unk20;
    u8 unk21;
    u8 unk22;
    u8 unk23[2];
    u8 unk25;
    u8 unk26[4];
    u8 unk2A;
    u8 unk2B;
    s32 unk2C;
    int unk30;
    struct UnkStruct_ov14_boxsys *sys;
} UnkStruct_ov14_boxapp;

// sizeof(UnkStruct_ov14_boxapp) == 0x38 on the DS (pointers are 8 bytes on the host: always the field, never an offset)

// The box app's main state table (ov14_021F7D9C, indexed by the state / boxapp.unk30): returns the next state.
typedef s32 (*UnkFuncPtr_ov14_boxstate)(UnkStruct_ov14_boxapp *app);
extern UnkFuncPtr_ov14_boxstate ov14_021F7D9C[];

#endif // POKEHEARTGOLD_OVERLAY_14_UNK_STRUCT_OV14_BOXAPP_H
