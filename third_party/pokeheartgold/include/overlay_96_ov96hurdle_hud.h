#ifndef POKEHEARTGOLD_OVERLAY_96_OV96HURDLE_HUD_H
#define POKEHEARTGOLD_OVERLAY_96_OV96HURDLE_HUD_H

#include "constants/heap.h"

#include "bg_window.h"

struct MessageFormat;
struct MsgData;
struct NNSG2dCharacterData;
struct NNSG2dScreenData;
struct PokeathlonCourseData;
struct Sprite;
struct String;

// Sub screen work of the Pokeathlon hurdle event (ov96_02207CCC allocates it, 0x1c8 bytes; the hurdle work keeps it at +0x370).

typedef struct Ov96HurdleHudPlayer {
    u8 unk_00[0xC];
    void *unk_0C;
    void *unk_10;
    void *unk_14;
    u8 unk_18[4];
} Ov96HurdleHudPlayer; // 0x1C

typedef struct Ov96HurdleHudPicRes {
    void *charBuf;
    void *palBuf;
} Ov96HurdleHudPicRes; // 0x8

typedef struct Ov96HurdleHudBadge {
    void *unk_0;
    void *unk_4;
    void *unk_8;
    u8 unk_C[4];
} Ov96HurdleHudBadge; // 0x10

typedef struct Ov96HurdleHudStep {
    u16 unk_0;
    u16 unk_2;
} Ov96HurdleHudStep; // 0x4

typedef struct Ov96HurdleHud {
    enum HeapID heapId;                     // 0x00
    struct PokeathlonCourseData *course;    // 0x04
    BgConfig *bgConfig;                     // 0x08
    Window window;                          // 0x0C
    struct MsgData *msgData;                // 0x1C
    struct MessageFormat *msgFormat;        // 0x20
    void *scrnFile;                         // 0x24
    struct NNSG2dScreenData *scrnData;      // 0x28
    Ov96HurdleHudPlayer players[4];         // 0x2C
    void *iconFile[12];                     // 0x9C
    struct NNSG2dCharacterData *iconChar[12]; // 0xCC
    u8 iconPal[12];                         // 0xFC
    Ov96HurdleHudPicRes picRes[3];          // 0x108
    u32 charLoc[2];                         // 0x120
    u32 palLoc[2];                          // 0x128
    void *unk_130[2];
    void *unk_138;
    Ov96HurdleHudBadge badges[4];           // 0x13C
    void *unk_17C[4][2];
    u32 unk_19C;                            // 0x19C ov96_02208448 step
    u16 unk_1A0;                            // 0x1A0
    u16 unk_1A2;                            // 0x1A2
    u8 unk_1A4[4];                          // 0x1A4 per-player state (ov96_02208658)
    struct String *names[3];                // 0x1A8
    struct Sprite *digits[2];               // 0x1B4
    Ov96HurdleHudStep unk_1BC[3];           // 0x1BC ov96_02208608
} Ov96HurdleHud; // 0x1C8

#endif // POKEHEARTGOLD_OVERLAY_96_OV96HURDLE_HUD_H
