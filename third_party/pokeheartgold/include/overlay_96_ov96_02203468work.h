#ifndef POKEHEARTGOLD_OVERLAY_96_OV96_02203468WORK_H
#define POKEHEARTGOLD_OVERLAY_96_OV96_02203468WORK_H

#include "constants/heap.h"

#include "bg_window.h"

struct Ov96PicPanel;
struct Ov96SpriteMgr;
struct Sprite;

typedef struct Ov96_02203468Work_Ball {
    void *unk_0;      // 0x00
    void *unk_4;      // 0x04
    void *unk_8;      // 0x08
    s32 state;        // 0x0C
    VecFx32 pos;      // 0x10
    VecFx32 vel;      // 0x1C
    VecFx32 accel;    // 0x28
    VecFx32 unk_34;   // 0x34
    u8 counter;       // 0x40
    u8 index;         // 0x41
    u8 unk_42;        // 0x42
    u8 hit;           // 0x43
    u16 unk_44;       // 0x44
    u16 unk_46;       // 0x46
} Ov96_02203468Work_Ball; // size 0x48

typedef struct Ov96_02203468Work_Item {
    void *unk_0;      // 0x00
    f32 unk_4;        // 0x04
    f32 unk_8;        // 0x08
    f32 unk_C;        // 0x0C
    u32 unk_10;       // 0x10
    u16 unk_14;       // 0x14
    s16 unk_16;       // 0x16
    u8 unk_18;        // 0x18
    u8 unk_19;        // 0x19
    u8 unk_1A;        // 0x1A
    u8 unk_1B;        // 0x1B
    u16 unk_1C;       // 0x1C
    u16 unk_1E;       // 0x1E
} Ov96_02203468Work_Item; // size 0x20

typedef struct Ov96_02203468Work_Touch {
    s32 unk_0;        // 0x00
    s32 unk_4;        // 0x04
    s32 unk_8;        // 0x08
} Ov96_02203468Work_Touch; // size 0xC

typedef struct Ov96_02203468Work {
    BgConfig *bgConfig;                   // 0x00
    Window windows[4];                    // 0x04
    enum HeapID heapID;                   // 0x44
    struct Ov96SpriteMgr *unk_48;         // 0x48
    void *unk_4C;                         // 0x4C
    struct Sprite *sprites[2];            // 0x50
    void *unk_58[3];                      // 0x58
    void *unk_64[3];                      // 0x64
    void *unk_70[9];                      // 0x70
    u8 unk_94[0x24];                      // 0x94
    Ov96_02203468Work_Ball balls[12];     // 0xB8
    Ov96_02203468Work_Item items[12];     // 0x418
    void *unk_598;                        // 0x598
    Ov96_02203468Work_Touch touch[4];     // 0x59C
    u8 unk_5CC[4];                        // 0x5CC
    u8 unk_5D0;                           // 0x5D0
    u8 unk_5D1[3];                        // 0x5D1
    void *unk_5D4;                        // 0x5D4
    void *unk_5D8;                        // 0x5D8
    void *unk_5DC;                        // 0x5DC
    struct Ov96PicPanel *unk_5E0;         // 0x5E0
    u8 unk_5E4[4];                        // 0x5E4
    u16 unk_5E8;                          // 0x5E8
    u8 unk_5EA;                           // 0x5EA
    u8 unk_5EB[4];                        // 0x5EB
    u8 unk_5EF;                           // 0x5EF
    u8 unk_5F0[4];                        // 0x5F0
} Ov96_02203468Work;

// sizeof(Ov96_02203468Work) == 0x5f4 on the DS

#endif // POKEHEARTGOLD_OVERLAY_96_OV96_02203468WORK_H
