#ifndef POKEHEARTGOLD_OVERLAY_96_OV96_MON_ICON_WORK_H
#define POKEHEARTGOLD_OVERLAY_96_OV96_MON_ICON_WORK_H

#include "constants/heap.h"

#include "bg_window.h"
#include "sprite.h"

struct MessageFormat;
struct MsgData;
struct NNSG2dCharacterData;
struct NNSG2dScreenData;
struct PokeathlonCourseData;
struct String;

// Pokeathlon course participant HUD (Heap_Alloc(heapID, 0x1A0) in ov96_021FFF3C, stored at Ov96RowerWork.unk3E0).

typedef struct Ov96Obj Ov96Obj;

typedef struct Ov96MonIconSlot {
    u8 filler_00[0xC];
    Ov96Obj *obj0C; // 0x0C
    Ov96Obj *obj10; // 0x10
    Ov96Obj *obj14; // 0x14
    Ov96Obj *obj18; // 0x18
    u8 state1C;     // 0x1C
    u8 filler_1D[3];
} Ov96MonIconSlot; // size = 0x20

typedef struct Ov96MonPicRes {
    void *charBuf; // 0x0
    void *plttBuf; // 0x4
} Ov96MonPicRes; // size = 0x8

typedef struct Ov96MonPicSpec {
    u16 species;       // 0x0
    u16 form;          // 0x2
    u8 filler_04[2];
    u8 shiny;          // 0x6
    u8 gender;         // 0x7
    u8 filler_08[4];
    u32 personality;   // 0xC
} Ov96MonPicSpec; // size = 0x10

typedef struct Ov96MonScroll {
    u16 pos;   // 0x0
    u16 stage; // 0x2
} Ov96MonScroll; // size = 0x4

typedef struct Ov96MonIconWork {
    enum HeapID heapID;                          // 0x000
    struct PokeathlonCourseData *course;         // 0x004
    BgConfig *bgConfig;                          // 0x008
    Window windows[2];                           // 0x00C
    struct MsgData *msgData;                     // 0x02C
    struct MessageFormat *msgFormat;             // 0x030
    void *scrnBuf;                               // 0x034
    struct NNSG2dScreenData *scrnData;           // 0x038
    Ov96MonIconSlot slots[4];                    // 0x03C
    void *charBuf[12];                           // 0x0BC
    struct NNSG2dCharacterData *charData[12];    // 0x0EC
    u8 palette[12];                              // 0x11C
    Ov96MonPicRes pics[3];                       // 0x128
    u32 charLoc[2];                              // 0x140
    u32 plttLoc[2];                              // 0x148
    Ov96Obj *cursor[2];                          // 0x150
    Ov96Obj *marker[2];                          // 0x158
    u16 counter[2];                              // 0x160
    u32 phase;                                   // 0x164
    s32 cursorIdx;                               // 0x168
    u8 slotState[4];                             // 0x16C
    struct String *names[3];                     // 0x170
    Sprite *timeSprites[6];                      // 0x17C
    Ov96MonScroll scroll[3];                     // 0x194
} Ov96MonIconWork; // size = 0x1A0

Ov96MonIconWork *ov96_021FFF3C(enum HeapID heapID, BgConfig *bgConfig, struct PokeathlonCourseData *course);
void ov96_021FFFE8(Ov96MonIconWork *work);
void ov96_02200068(Ov96MonIconWork *work);
void ov96_02200DF8(Ov96MonIconWork *work);
void ov96_02200BD8(Ov96MonIconWork *work, s32 idx);
void ov96_02200C40(Ov96MonIconWork *work);
void ov96_0220050C(Ov96MonIconWork *work, u8 slot, s32 angle);
void ov96_022005B4(Ov96MonIconWork *work, s32 arg1, u32 arg2);
s32 ov96_022006BC(Ov96MonIconWork *work, s32 param_2);
s32 ov96_02200900(Ov96MonIconWork *work, u32 arg1);
void ov96_02200A64(Ov96MonIconWork *work, int time);

#endif // POKEHEARTGOLD_OVERLAY_96_OV96_MON_ICON_WORK_H
