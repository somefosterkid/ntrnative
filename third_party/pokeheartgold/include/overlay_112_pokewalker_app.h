#ifndef POKEHEARTGOLD_OVERLAY_112_POKEWALKER_APP_H
#define POKEHEARTGOLD_OVERLAY_112_POKEWALKER_APP_H

// Ov112PokewalkerApp: overlay 112's main Pokewalker app object, OverlayManager_CreateAndGetData(man, sizeof(Ov112PokewalkerApp),
// HEAP_ID_154) in ov112_021E76CC. Closed object (only ov112 code touches it). Fields sit at the DS offsets in the comments
// (one field at every offset the code reads or writes, blobs for regions only passed by address to the callees that own them,
// pads elsewhere); 0x24..0x1D750 holds no pointers, so its layout is the same on the PC. The DS size is 0x1F378 (checked below
// on the DS build), the PC size is bigger.

#include "bg_window.h"
#include "overlay_112_sub_screen.h"
#include "pokemon_types_def.h"

struct Bag;
struct BgConfig;
struct BoxPokemon;
struct ManagedSprite;
struct MessageFormat;
struct MsgData;
struct NNSG2dCharacterData;
struct Options;
struct OverlayManager;
struct PlayerProfile;
struct POKEWALKER;
struct Pokedex;
struct SaveData;
struct SpriteManager;
struct SpriteSystem;
struct String;
struct SysTask;
struct WaitingIcon;

// Ov112PokewalkerLink: the IR link block at +0x1D764 (ov112_021EF1CC / 021EF310 / 021EF03C / 021EF3F8 take its address).
typedef struct Ov112PokewalkerLink {
    s32 active;                      // 0x00
    void *unk_4;                     // 0x04: sub_020932E0
    Ov112SubScreen *subScreen;       // 0x08: ov112_021F039C
    u32 unk_C;                       // 0x0C
    u32 unk_10;                      // 0x10
    u32 unk_14;                      // 0x14
} Ov112PokewalkerLink;

typedef struct Ov112PokewalkerRec {
    u8 unk_0[0x2];
    s8 unk_2;
    s8 unk_3;
    s16 unk_4;
    s16 unk_6;
    s16 unk_8;
    s16 unk_A;
    u16 unk_C;
    s16 unk_E;
} Ov112PokewalkerRec; // 0x10

typedef struct Ov112PokewalkerMonRec {
    u16 unk_0; // species
    u8 unk_2[0xB];
    u8 unk_D; // bit-field views at the use sites
    u8 unk_E[0x2];
} Ov112PokewalkerMonRec; // 0x10

typedef struct Ov112PokewalkerItemRec {
    u16 unk_0;
    u8 unk_2[0x2];
} Ov112PokewalkerItemRec; // 0x4

typedef struct Ov112PokewalkerApp {
    u8 pad_0[0x4];
    s32 unk_4; // 0x4
    s32 unk_8; // 0x8
    u32 unk_C; // 0xC
    u32 unk_10; // 0x10
    u16 unk_14; // 0x14
    u16 unk_16; // 0x16
    struct BgConfig *bgConfig; // 0x18
    struct OverlayManager *man; // 0x1C
    struct SaveData *saveData; // 0x20
    u8 unk_24[0x1000];
    u32 unk_1024; // 0x1024
    u32 unk_1028; // 0x1028
    u16 unk_102C; // 0x102C
    u16 unk_102E; // 0x102E
    u32 unk_1030; // 0x1030
    u8 unk_1034[0x38];
    u16 unk_106C[9]; // 0x106C
    u8 unk_107E; // 0x107E
    u8 unk_107F; // 0x107F
    u8 pad_1080[0xC];
    u8 unk_108C[0x4];
    s32 unk_1090; // 0x1090
    u8 unk_1094[0x53];
    u8 unk_10E7; // 0x10E7
    u8 pad_10E8[0x8];
    u32 unk_10F0; // 0x10F0
    u8 unk_10F4[0x8C50];
    u16 unk_9D44; // 0x9D44
    u8 pad_9D46[0xB];
    u8 unk_9D51; // 0x9D51
    u8 unk_9D52[0x2];
    u16 unk_9D54[14]; // 0x9D54: nickname
    u8 unk_9D70[0x8];
    u8 unk_9D78; // 0x9D78
    u8 pad_9D79[0x1];
    u16 unk_9D7A; // 0x9D7A
    Ov112PokewalkerMonRec unk_9D7C[3]; // 0x9D7C
    Ov112PokewalkerItemRec unk_9DAC[3]; // 0x9DAC
    Ov112PokewalkerItemRec unk_9DB8[10]; // 0x9DB8
    u8 pad_9DE0[0x1C];
    u8 unk_9DFC[0xC38];
    u8 unk_AA34[0x88];
    u8 unk_AABC; // 0xAABC
    u8 pad_AABD[0x243];
    Ov112PokewalkerMonRec unk_AD00; // 0xAD00
    u8 pad_AD10[0x4];
    u32 unk_AD14; // 0xAD14
    u8 pad_AD18[0x2];
    u16 unk_AD1A; // 0xAD1A
    u8 pad_AD1C[0x2];
    u16 unk_AD1E[8];
    u8 unk_AD2E; // 0xAD2E
    u8 unk_AD2F[0x1];
    u8 unk_AD30[0x2D2];
    u16 unk_B002; // 0xB002
    u8 pad_B004[0x180];
    u8 unk_B184[0x178C];
    u8 unk_C910[0x470];
    u8 unk_CD80[0x2450];
    u32 unk_F1D0; // 0xF1D0
    u16 unk_F1D4; // 0xF1D4
    u8 pad_F1D6[0x2];
    u8 unk_F1D8[0x28];
    u8 unk_F200[0x1634];
    u8 unk_10834[0xCF1C];
    struct SaveData *unk_1D750; // 0x1D750
    void *unk_1D754; // 0x1D754
    u32 unk_1D758; // 0x1D758
    u32 unk_1D75C; // 0x1D75C
    u16 unk_1D760; // 0x1D760
    u8 pad_1D762[0x2];
    Ov112PokewalkerLink link; // 0x1D764
    u16 unk_1D77C; // 0x1D77C
    u8 pad_1D77E[0x1A];
    s32 unk_1D798; // 0x1D798
    u8 unk_1D79C[0x10];
    u8 unk_1D7AC[0xC80];
    Pokemon *unk_1E42C; // 0x1E42C: the party mon picked (ov112_021EF31C), NULL when unk_1E430 holds a box mon
    struct BoxPokemon *unk_1E430; // 0x1E430
    struct Bag *bag; // 0x1E434
    struct PlayerProfile *profile; // 0x1E438
    struct Options *options; // 0x1E43C
    struct POKEWALKER *pokewalker; // 0x1E440
    struct Pokedex *pokedex; // 0x1E444
    struct MessageFormat *msgFormat; // 0x1E448
    struct MsgData *msgData; // 0x1E44C
    struct MsgData *msgData2; // 0x1E450
    struct String *unk_1E454; // 0x1E454
    struct String *unk_1E458; // 0x1E458
    struct String *unk_1E45C; // 0x1E45C
    struct String *unk_1E460; // 0x1E460
    struct String *unk_1E464; // 0x1E464
    struct String *unk_1E468; // 0x1E468
    struct String *unk_1E46C; // 0x1E46C
    struct String *unk_1E470; // 0x1E470
    struct String *unk_1E474; // 0x1E474
    u8 pad_1E478[0x1C];
    struct String *unk_1E494; // 0x1E494
    struct String *unk_1E498; // 0x1E498
    struct String *unk_1E49C; // 0x1E49C
    struct String *unk_1E4A0; // 0x1E4A0
    struct String *unk_1E4A4; // 0x1E4A4
    struct String *unk_1E4A8; // 0x1E4A8
    struct String *unk_1E4AC; // 0x1E4AC
    struct String *unk_1E4B0; // 0x1E4B0
    struct String *unk_1E4B4; // 0x1E4B4
    u8 pad_1E4B8[0x4];
    struct String *unk_1E4BC; // 0x1E4BC
    u8 pad_1E4C0[0x4C];
    struct String *unk_1E50C; // 0x1E50C
    struct String *unk_1E510; // 0x1E510
    struct String *unk_1E514; // 0x1E514
    struct String *unk_1E518; // 0x1E518
    u8 pad_1E51C[0x8];
    s32 unk_1E524; // 0x1E524
    struct SpriteSystem *spriteSystem; // 0x1E528
    struct SpriteManager *spriteManager; // 0x1E52C
    struct ManagedSprite *sprites[217]; // 0x1E530
    u8 pad_1E894[0x1CC];
    void *unk_1EA60; // 0x1EA60
    struct NNSG2dCharacterData *unk_1EA64; // 0x1EA64
    Window windows[20]; // 0x1EA68
    Window windows2[10]; // 0x1EBA8
    u8 unk_1EC48; // 0x1EC48
    u8 unk_1EC49; // 0x1EC49
    u8 unk_1EC4A; // 0x1EC4A
    u8 unk_1EC4B; // 0x1EC4B
    struct WaitingIcon *waitingIcon; // 0x1EC4C
    s32 unk_1EC50; // 0x1EC50
    s32 unk_1EC54; // 0x1EC54
    s8 unk_1EC58[30]; // 0x1EC58
    u16 unk_1EC76; // 0x1EC76
    s32 unk_1EC78; // 0x1EC78
    s32 unk_1EC7C; // 0x1EC7C
    Ov112PokewalkerRec unk_1EC80[100]; // 0x1EC80
    u8 unk_1F2C0[0x10];
    s32 unk_1F2D0; // 0x1F2D0
    u16 unk_1F2D4; // 0x1F2D4
    u16 unk_1F2D6; // 0x1F2D6
    struct SysTask *unk_1F2D8; // 0x1F2D8
    struct SysTask *unk_1F2DC; // 0x1F2DC
    u16 unk_1F2E0; // 0x1F2E0
    u16 unk_1F2E2; // 0x1F2E2
    s16 unk_1F2E4; // 0x1F2E4
    u8 pad_1F2E6[0x2];
    u8 unk_1F2E8[0x88];
    s16 unk_1F370; // 0x1F370
    u8 pad_1F372[0x2];
    s16 unk_1F374; // 0x1F374
    u16 unk_1F376; // 0x1F376
} Ov112PokewalkerApp;

#if !defined(SDK_PORT) && !defined(NONMATCHING)
typedef char Ov112PokewalkerApp_size_is_0x1F378[(sizeof(Ov112PokewalkerApp) == 0x1F378) ? 1 : -1];
#endif

#endif // POKEHEARTGOLD_OVERLAY_112_POKEWALKER_APP_H
