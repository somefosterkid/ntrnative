#ifndef POKEHEARTGOLD_OVERLAY_14_UNK_STRUCT_OV14_BOXSYS_H
#define POKEHEARTGOLD_OVERLAY_14_UNK_STRUCT_OV14_BOXSYS_H

// UnkStruct_ov14_boxsys (hk49): the box app's big work (UnkStruct_ov14_boxapp.sys, 0x88e0 bytes, the Heap_Alloc size in ov14_021EAFAC). Fields at every
// offset the cut code reaches through ->sys (type from the accesses), pads elsewhere; the m2c copies in 6 cut files
// stay until their files take this header.

#include "bg_window.h"
#include "obj_char_transfer.h"
#include "save_layout.h"

struct BgConfig;
struct GridInputHandler;
struct ManagedSprite;
struct MessageFormat;
struct MessagePrinter;
struct MsgData;
struct PaletteData;
struct SpriteManager;
struct SpriteSystem;
struct String;
struct SysTask;
struct NARC;
struct UnkStruct_02013534;
struct TextOBJ;
struct UnkStruct_0201956C;
struct YesNoPrompt;
struct UnkStruct_ov14_boxapp;

// The box task: run by the vblank task ov14_021E59AC with the app until it returns 0
// (set by ov14_021E5A44 / ov14_021F0234, saved by ov14_021E5A50, resumed by ov14_021E5A54).
typedef s32 (*UnkFuncPtr_ov14_boxtask)(struct UnkStruct_ov14_boxapp *app);

typedef struct UnkStruct_ov14_textobj {
    struct TextOBJ *textObj;
    UnkStruct_02021AC8 charTransfer;
} UnkStruct_ov14_textobj;

// The box scan (UnkStruct_ov14_boxsys.unk_88DC, Heap_AllocAtEnd sizeof in ov14_021E71E8): while the held mon's sprite
// pulses (ov14_021F3354/3380/33B0/33FC), ov14_021E7278 walks every box and party slot and clears the bit of each HM move
// another mon still knows.
typedef struct UnkStruct_ov14_boxscan {
    struct ManagedSprite *sprite; // DS 0x0
    u16 pos;                      // DS 0x4 scan position: box * 30 + slot, 0x21C + party slot, 0x222 = done
    u8 moveFlags;                 // DS 0x6 one bit per HM move of the held mon (ov14_021E71C8)
    u8 frame;                     // DS 0x7
    f32 scale;                    // DS 0x8
} UnkStruct_ov14_boxscan;
#ifdef __MWERKS__
LAYOUT_CHECK(UnkStruct_ov14_boxscan_size, sizeof(UnkStruct_ov14_boxscan) == 0xC);
#endif

// The cursor drag state (UnkStruct_ov14_boxsys.unk_C when allocated by ov14_021F1F24, 0x1C bytes), filled by ov14_021E7A08.
#define OV14_DRAG_XNEG 0x1 // flags: the sprite moves toward lower x
#define OV14_DRAG_YNEG 0x2 // flags: the sprite moves toward lower y
typedef struct UnkStruct_ov14_dragstate {
    u16 slot;  // DS 0x0
    u16 unk_2; // DS 0x2 (box UnkStruct_ov14_boxsys.unk_44C)
    s32 mode;  // DS 0x4
    s32 x;     // DS 0x8 current x, 24.8 fixed point
    s32 y;     // DS 0xC
    s32 dx;    // DS 0x10 per-step x delta
    s32 dy;    // DS 0x14
    u32 flags; // DS 0x18 OV14_DRAG_*
} UnkStruct_ov14_dragstate;
#ifdef __MWERKS__
LAYOUT_CHECK(UnkStruct_ov14_dragstate_size, sizeof(UnkStruct_ov14_dragstate) == 0x1C);
#endif

// The cursor glide state (UnkStruct_ov14_boxsys.unk_C when allocated by ov14_021F7AC4, 8 bytes), run by ov14_021E9F20.
typedef struct UnkStruct_ov14_glide {
    u8 x;       // DS 0x0
    u8 y;       // DS 0x1
    u8 dx;      // DS 0x2 per-step x delta
    u8 dy;      // DS 0x3
    u32 flags;  // DS 0x4 bits 0/1 = OV14_DRAG_XNEG/YNEG, bits 2.. = steps left
} UnkStruct_ov14_glide;
#ifdef __MWERKS__
LAYOUT_CHECK(UnkStruct_ov14_glide_size, sizeof(UnkStruct_ov14_glide) == 0x8);
#endif

typedef struct UnkStruct_ov14_boxsys {
    struct SysTask *unk_0;

    UnkFuncPtr_ov14_boxtask unk_4;  // DS 0x4 the running task (NULL = idle)
    UnkFuncPtr_ov14_boxtask unk_8;  // DS 0x8 the saved task
    void *unk_C;
    u16 unk_10;
    u16 unk_12;
    struct BgConfig *unk_14;
    struct PaletteData *unk_18;
    struct MessagePrinter *unk_1C;
    struct MsgData *unk_20;
    struct MessageFormat *unk_24;
    struct String *unk_28;
    struct GridInputHandler *unk_2C;
    Window windows[0x2C];
    struct UnkStruct_0201956C *unk_2F0;
    struct SpriteSystem *unk_2F4;
    struct SpriteManager *unk_2F8;
    struct ManagedSprite *sprites[0x45];
    struct UnkStruct_02013534 *unk_410;
    UnkStruct_ov14_textobj textObjs[2];
    struct YesNoPrompt *unk_434;
    u16 unk_438;
    u8 unk_43A[0x2];
    s32 unk_43C;
    s32 unk_440;
    s32 unk_444;
    u8 unk_448[0x3];
    s8 unk_44B;
    u8 unk_44C;
    u8 unk_44D;
    u8 unk_44E_0 : 4;
    u8 unk_44E_4 : 3;
    u8 unk_44E_7 : 1;
    u8 pad_44F[0x1];
    struct NARC *unk_450;
    struct NARC *unk_454;
    u8 unk_458[0x1E][0x200];  // DS 0x458 box icon char data, one per slot
    u8 unk_4058[0x1E];        // DS 0x4058 icon draw flag per slot
    u8 unk_4076[0x1E];        // DS 0x4076 icon palette per slot
    u8 unk_4094[0x24];        // DS 0x4094 sprite index per slot (30 box + 6 party at 0x40B2)
    s32 unk_40B8;
    s32 unk_40BC;
    s32 unk_40C0;
    u32 unk_40C4;
    u8 unk_40C8[0x4800];
    u16 unk_88C8;
    u16 unk_88CA;
    int unk_88CC;
    u16 unk_88D0;
    u16 unk_88D2;
    u8 unk_88D4[0x8];
    UnkStruct_ov14_boxscan *unk_88DC;
} UnkStruct_ov14_boxsys;
#ifdef __MWERKS__
LAYOUT_CHECK(UnkStruct_ov14_boxsys_size, sizeof(UnkStruct_ov14_boxsys) == 0x88E0);
#endif

// sizeof(UnkStruct_ov14_boxsys) == 0x88e0

#endif // POKEHEARTGOLD_OVERLAY_14_UNK_STRUCT_OV14_BOXSYS_H
