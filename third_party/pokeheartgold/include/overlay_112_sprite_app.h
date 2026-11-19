#ifndef POKEHEARTGOLD_OVERLAY_112_SPRITE_APP_H
#define POKEHEARTGOLD_OVERLAY_112_SPRITE_APP_H

// Ov112SpriteApp: overlay 112's sprite app object, OverlayManager_CreateAndGetData(man, sizeof(Ov112SpriteApp), HEAP_ID_155)
// in ov112_021F2DE4. Closed object (only ov112 code touches it); pointer fields sit at the DS offsets in the comments,
// the DS size is 0x150 (checked below on the DS build), the PC size is bigger.

#include "bg_window.h"
#include "gf_rtc.h"

typedef struct Ov112SpriteRec {
    u32 unk_0;
    u32 unk_4;
    struct ManagedSprite *sprites[3]; // 0x08
} Ov112SpriteRec; // 0x14

typedef struct Ov112RecBits {
    u8 lo : 5;
    u8 mid : 2;
    u8 hi : 1;
} Ov112RecBits;

// Ov112Record: one per-record data block (Ov112SpriteApp.records[]), plain data (no pointers: same layout on the PC)
typedef struct Ov112Record {
    u32 timestamp;          // 0x00: seconds, ov112_021F15E4 converts it to the date/time shown
    u8 unk_4[4];
    u16 unk_8;
    u16 speciesA;           // 0x0A
    u16 speciesB;           // 0x0C
    u16 unk_E;
    u16 playerName[8];      // 0x10
    u16 nicknameA[11];      // 0x20
    u16 nicknameB[11];      // 0x36
    u16 message[21];        // 0x4C
    u8 page;                // 0x76
    u8 unk_77;
    u16 unk_78;
    u16 unk_7A;
    u32 unk_7C;
    u8 unk_80[4];
    u8 kind;                // 0x84
    Ov112RecBits bitsA;     // 0x85
    Ov112RecBits bitsB;     // 0x86
    u8 unk_87;
} Ov112Record; // 0x88

// Ov112SpriteApp.bits144: low byte = the record page last drawn (0xFF = none); the fade state machine uses 10..14
typedef struct Ov112Bits144 {
    u32 page : 8;
    u32 flag8 : 1;
    u32 flag9 : 1;
    u32 fadeActive : 1;
    u32 fadeState : 4;
    u32 unk_F : 17;
} Ov112Bits144;

// Ov112SpriteApp.bits148: the sprite slide-in animation (ov112_021F238C)
typedef struct Ov112Bits148 {
    u32 active : 1;
    u32 step : 8;
    u32 loop : 7;
    u32 timer : 16;
} Ov112Bits148;

// Ov112SpriteApp.bits14C: the draw-flag animation (ov112_021F24F8)
typedef struct Ov112Bits14C {
    u32 active : 1;
    u32 state : 7;
    u32 timer : 24;
} Ov112Bits14C;

typedef struct Ov112SpriteApp {
    void *args;                       // 0x00: OverlayManager_GetArgs
    enum HeapID heapID;               // 0x04
    const u16 *playerName;            // 0x08
    u32 playerGender;                 // 0x0C
    struct NARC *narc;                // 0x10
    struct BgConfig *bgConfig;        // 0x14
    Window windows[4];                // 0x18
    struct MessageFormat *msgFormat;  // 0x58
    struct MsgData *msgData[3];       // 0x5C
    struct SpriteSystem *spriteSystem;   // 0x68
    struct SpriteManager *spriteManager; // 0x6C
    struct ManagedSprite *sprites[10];   // 0x70
    Ov112SpriteRec spriteRecs[2];     // 0x98
    Ov112Record *records[24];         // 0xC0: per-record data blocks, indexed by curIdx
    RTCDate date;                     // 0x120
    RTCTime time;                     // 0x130
    u8 loadStep;                      // 0x13C: ov112_021F1DC0 resource-load state
    u8 curIdx;                        // 0x13D
    u8 count;                         // 0x13E
    u8 unk_13F;
    u8 unk_140;
    u8 unk_141;
    u8 unk_142;
    u8 unk_143;
    Ov112Bits144 bits144;             // 0x144
    Ov112Bits148 bits148;             // 0x148
    Ov112Bits14C bits14C;             // 0x14C
} Ov112SpriteApp;

// the plain-data views have the same size on the DS and the PC
typedef char Ov112Record_size_is_0x88[(sizeof(Ov112Record) == 0x88) ? 1 : -1];
typedef char Ov112Bits144_size_is_4[(sizeof(Ov112Bits144) == 4) ? 1 : -1];
typedef char Ov112Bits148_size_is_4[(sizeof(Ov112Bits148) == 4) ? 1 : -1];
typedef char Ov112Bits14C_size_is_4[(sizeof(Ov112Bits14C) == 4) ? 1 : -1];

#if !defined(SDK_PORT) && !defined(NONMATCHING)
typedef char Ov112SpriteApp_size_is_0x150[(sizeof(Ov112SpriteApp) == 0x150) ? 1 : -1];
#endif

#endif // POKEHEARTGOLD_OVERLAY_112_SPRITE_APP_H
