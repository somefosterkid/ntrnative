#ifndef POKEHEARTGOLD_OVERLAY_96_OV96_DIGIT_WORK_H
#define POKEHEARTGOLD_OVERLAY_96_OV96_DIGIT_WORK_H

// Ov96DigitWork: the 0xC8 per-runner "digit / name plate" work of the Pokeathlon Rd course
// (Ov96RdWork.unk_180, allocated by ov96_022194C4). Every pointer slot is typed; the 0x12 managed sprites
// share one array so the free loop of ov96_022195E8 can walk it:
//   sprites[0]       0x60 plate background (palette override)
//   sprites[1]       0x64 result bar (ov96_02219E60)
//   sprites[2..5]    0x68 digit sprites, one per rank 0..3
//   sprites[6..9]    0x78 rank markers
//   sprites[10..13]  0x88 rank labels
//   sprites[14..15]  0x98 icon pair
//   sprites[16..17]  0xA0 icon cursors
#include "bg_window.h"

struct BgConfig;
struct ManagedSprite;
struct MessageFormat;
struct MsgData;
struct NARC;
struct NNSG2dScreenData;
struct PokeathlonCourseData;
struct Sprite;
struct SpriteManager;
struct SpriteSystem;
struct String;
struct UnkStruct_ov96_021E9A78;

typedef struct Ov96DigitWork {
    struct PokeathlonCourseData *course;
    enum HeapID heapId;
    struct SpriteSystem *spriteSystem;
    struct SpriteManager *spriteManager;
    struct UnkStruct_ov96_021E9A78 *resSrc;
    struct BgConfig *bgConfig;
    struct MsgData *msgData;
    struct MessageFormat *msgFormat;
    u16 plttResId;
    u8 playerIdx;
    u8 pad_23;
    struct NARC *narc;
    void *scrnBuf;
    struct NNSG2dScreenData *screenData;
    Window windows[3];
    struct ManagedSprite *sprites[0x12];
    struct Sprite *unk_A8[2];
    struct String *names[3];
    u8 pad_BC[0x6];
    u16 unk_C2;
    u32 unk_C4;
} Ov96DigitWork;

// sizeof(Ov96DigitWork) == 0xc8 on the DS

#endif // POKEHEARTGOLD_OVERLAY_96_OV96_DIGIT_WORK_H
