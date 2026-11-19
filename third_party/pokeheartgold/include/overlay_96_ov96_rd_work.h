#ifndef POKEHEARTGOLD_OVERLAY_96_OV96_RD_WORK_H
#define POKEHEARTGOLD_OVERLAY_96_OV96_RD_WORK_H

// Ov96RdWork: the 0x814 heap object of the Pokeathlon "Rd" course (ov96_02215AC4 allocates it in the
// PokeathlonCourse_AllocPtr4FromHeap slot). Offsets are the DS ones (hk316 derived them from the asm + both
// divergent local bodies: the one with `hit[4][4]` at 0x188 is the superset, the other had pad184[0x14]).

#include "bg_window.h"
#include "overlay_96_ov96_rd_obj.h"

struct BgConfig;
struct ManagedSprite;
struct MessageFormat;
struct Ov96DigitWork;
struct MsgData;
struct Ov96SpriteSet;
struct Ov96SpriteSetElem;
struct SpriteManager;
struct SpriteSystem;
struct SysTask;
struct UnkStruct_ov96_021E9A78;

// one banner slot (0xe8) of Ov96RdSprites; ov96_02216770 / ov96_02216934 / ov96_02216A54 / ov96_02216AA4 take it
typedef struct Ov96RdSlot {
    struct Ov96SpriteSetElem *elem;
    struct ManagedSprite *sprites[5];
    u32 unk_18;
    struct {
        u16 normal[16];
        u16 gray[16];
    } pal[3];
    u8 pad_DC[6];
    s8 timer;
    u8 unk_E3;
    u32 unk_E4_0 : 8;
    u32 state : 4;
    u32 mode : 2;
    u32 unk_E4_14 : 2;
    u32 unk_E4_16 : 16;
} Ov96RdSlot;

// the 0x3bc sprite block embedded at 0x43c (ov96_02217544 initialises it)
typedef struct Ov96RdSprites {
    void *unk_0;
    struct SpriteSystem *spriteSystem;
    struct SpriteManager *spriteManager;
    struct ManagedSprite *sprite;
    Ov96RdSlot slots[4];
    u16 stat[4];
    u8 mode;
    u8 pad_3B9[3];
} Ov96RdSprites;

// the 0x1c scroll task embedded at 0x7f8 (ov96_0221587C / ov96_02215884 / ov96_02215710, shared helper code)
typedef struct Ov96RdScroll {
    struct SpriteSystem *spriteSystem;
    struct BgConfig *bgConfig;
    struct SysTask *task;
    s32 total;
    s32 step;
    s8 count;
    u8 rate;
    s8 unk_16;
    u8 pad_17;
    s32 dir;
} Ov96RdScroll;

// the 0x2b0 runner block at 0x188 (ov96_02218330 takes it): collision latches + the 4 runners
typedef struct Ov96RdCourse {
    u8 hit[4][4];
    Ov96RdPlayer players[4];
} Ov96RdCourse;

typedef struct Ov96RdWork {
    enum HeapID heapId;
    struct BgConfig *bgConfig;
    struct SpriteSystem *spriteSystem;
    struct SpriteManager *spriteManager;
    struct MessageFormat *msgFormat;
    struct MsgData *msgData;
    struct UnkStruct_ov96_021E9A78 *resSrc;
    struct Ov96SpriteSet *spriteSet;
    Window window;
    Ov96RdStats stats[4 * 3]; // [runner * 3 + event], see ov96_0221935C
    struct Ov96DigitWork *unk_180;
    void *unk_184;
    Ov96RdCourse course;
    s32 unk_438;
    Ov96RdSprites sprites;
    Ov96RdScroll scroll;
} Ov96RdWork;

// DS layout (sizeof(Ov96RdWork) == 0x814)

#endif // POKEHEARTGOLD_OVERLAY_96_OV96_RD_WORK_H
