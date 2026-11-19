#ifndef POKEHEARTGOLD_OVERLAY_28_DOWSING_MCHN_H
#define POKEHEARTGOLD_OVERLAY_28_DOWSING_MCHN_H

#include "bg_window.h"
#include "field_types_def.h"
#include "g2d_renderer.h"
#include "pm_string.h"
#include "script_manager.h"
#include "sprite.h"
#include "sys_task_api.h"
#include "unk_0200A090.h"

// One needle of the Dowsing Machine radar (sizeof == 0xC): the angle it shows, the angle it moves to and the next step.
typedef struct DowsingMchnNeedle {
    u32 angle;
    u32 targetAngle;
    u32 nextAngle;
} DowsingMchnNeedle;

// Position / priority / animation of one Dowsing Machine sprite (an asm data record, 4 bytes).
typedef struct DowsingMchnSpriteDef {
    u8 x;
    u8 y;
    u8 drawPriority;
    u8 animSeq;
} DowsingMchnSpriteDef;

// Environment of the Dowsing Machine touch-screen sub-app (ov28_0225D520 allocs it with CreateSysTaskAndEnvironment).
typedef struct DowsingMchnApp {
    u8 unk_00[0x10];
    BgConfig *bgConfig;                  // 0x10
    u16 *lastTouchMenuInput;             // 0x14
    FieldSystem *fieldSystem;            // 0x18
    void *args;                          // 0x1C
    SysTask *task;                       // 0x20
    SpriteList *spriteList;              // 0x24
    G2dRenderer renderer;                // 0x28
    GF_2DGfxResMan *resMans[4];          // 0x150
    SpriteResource *buttonResObjs[4];    // 0x160
    SpriteResource *needleResObjs[4];    // 0x170
    Sprite *sprites[8];                  // 0x180: 0-3 buttons, 4-7 needles
    String *strings[5];                  // 0x1A0
    Window windows[5];                   // 0x1B4
    HiddenItemResponse *hiddenItems;     // 0x204
    s16 nearestX;                        // 0x208
    s16 nearestY;                        // 0x20A
    BOOL hasNearestItem;                 // 0x20C
    u32 proximity;                       // 0x210
    u32 touchX;                          // 0x214
    u32 touchY;                          // 0x218
    DowsingMchnNeedle needles[4];        // 0x21C
    u16 needleTimer : 15;                // 0x24C
    u16 needleTimerActive : 1;
    s16 signalStrength;                  // 0x24E
    u8 signalGrid[15 * 15];               // 0x250
    u32 windowSel;                       // 0x334
    s16 unk_338;                         // 0x338
    s16 unk_33A;                         // 0x33A
    s32 touching;                        // 0x33C
} DowsingMchnApp;                        // 0x340

#endif // POKEHEARTGOLD_OVERLAY_28_DOWSING_MCHN_H
