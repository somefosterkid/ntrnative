#ifndef POKEHEARTGOLD_OVERLAY_27_OV27TASK_DATA_H
#define POKEHEARTGOLD_OVERLAY_27_OV27TASK_DATA_H

#include "global.h"
#include "bg_window.h"
#include "field_types_def.h"
#include "sys_task.h"
#include "unk_02009D48.h"

struct GF_2DGfxResMan;
struct MessageFormat;
struct MsgData;
struct Sprite;
struct SpriteResource;
struct FieldMenuManager;

typedef BOOL (*Ov27TouchMenuCallback)(struct FieldMenuManager *menu, int selection);

// Data of the field's touch-screen choice-menu sub-app (index 3 of ov01_02206C60; ov27_0225C250 creates it,
// ov01_021F6ABC / ov01_021F6B20 reach into it from the script commands). Offsets are the DS ones. sizeof == 0x3A4.
typedef struct Ov27TaskData {
    s32 state;                              // 0x00 index into ov27_0225D4D4
    u16 *result;                            // 0x04 where the selected index goes (0xEEEE while waiting)
    u32 unk_08;
    struct FieldMenuManager *menu;   // 0x0C
    s32 counter;                            // 0x10
    u32 unk_14;
    BgConfig *bgConfig;                     // 0x18
    SysTask *task;                          // 0x1C
    u16 *touchInput;                        // 0x20
    FieldSystem *fieldSystem;               // 0x24
    Window windowA;                         // 0x28
    Window windowB;                         // 0x38
    u32 unk_48;
    struct MsgData *msgData;                // 0x4C
    struct MessageFormat *msgFormat;        // 0x50
    Window menuWindows[28];                 // 0x54 (the first nMenuWindows are in use)
    s32 nMenuWindows;                       // 0x214
    SpriteList *spriteList;                 // 0x218
    G2dRenderer renderer;                   // 0x21C
    struct GF_2DGfxResMan *resMan[4];       // 0x344
    struct SpriteResource *resObj[4];       // 0x354
    SpriteResourcesHeader spriteHeader;     // 0x364
    struct Sprite *cursor;                  // 0x388
    u8 pad_38C[0x8];
    s32 selection;                          // 0x394
    u8 pad_398[0x4];
    Ov27TouchMenuCallback callback;         // 0x39C (ov27_0225C41C sets it with its argument)
    struct FieldMenuManager *callbackMenu; // 0x3A0
} Ov27TaskData;

// sizeof(Ov27TaskData) == 0x3a4

#endif // POKEHEARTGOLD_OVERLAY_27_OV27TASK_DATA_H
