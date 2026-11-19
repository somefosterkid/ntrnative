#ifndef POKEHEARTGOLD_OVERLAY_96_APP_0221040C_H
#define POKEHEARTGOLD_OVERLAY_96_APP_0221040C_H

// Ov96App_0221040C: the 0xE8-byte gauge/name-plate work allocated by ov96_02210240
// and kept in UnkStruct_ov96_0220E3F0.unk_C4 (and Hud.unk_0). Offsets below are DS offsets.

#include "constants/heap.h"

#include "pokeathlon/pokeathlon.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "message_format.h"
#include "msgdata.h"
#include "sprite.h"
#include "sprite_system.h"

typedef struct Ov96App_0221040C {
    PokeathlonCourseData *course; // 0x0
    enum HeapID heapId; // 0x4
    SpriteSystem *spriteSystem; // 0x8
    SpriteManager *spriteManager; // 0xc
    BgConfig *bgConfig; // 0x10
    MsgData *msgData; // 0x14
    MessageFormat *msgFormat; // 0x18
    u8 unk_1C; // 0x1c participant index
    void *scrnBuf; // 0x20 GfGfxLoader_GetScrnData result
    NNSG2dScreenData *scrnData; // 0x24
    Window windows[4]; // 0x28
    ManagedSprite *sprites[31]; // 0x68 [0] bg, [1..2] digits, [3..6] cursors, [7..30] name plates
    u8 unk_E4; // 0xe4
    u8 unk_E5; // 0xe5
    u16 unk_E6; // 0xe6
} Ov96App_0221040C;

// sizeof(Ov96App_0221040C) == 0xE8

Ov96App_0221040C *ov96_02210240(SpriteSystem *spriteSystem, SpriteManager *spriteManager, BgConfig *bgConfig, MsgData *msgData, MessageFormat *msgFormat, u8 unk1C, enum HeapID heapId, PokeathlonCourseData *course);
void ov96_022102D4(Ov96App_0221040C *app);
void ov96_0221031C(Ov96App_0221040C *app);
u32 ov96_02210324(Ov96App_0221040C *app, u8 index, u16 value);
u32 ov96_02210390(Ov96App_0221040C *app, u8 index, u16 value);
void ov96_0221040C(Ov96App_0221040C *app, s32 value);
u16 ov96_022104C4(Ov96App_0221040C *app);
void ov96_0221058C(Ov96App_0221040C *app, s32 value);
void ov96_0221065C(Ov96App_0221040C *app);
void ov96_0221069C(Ov96App_0221040C *app);
void ov96_022107D4(Ov96App_0221040C *app);
void ov96_0221075C(Ov96App_0221040C *app);
void ov96_02210538(Ov96App_0221040C *app);

#endif // POKEHEARTGOLD_OVERLAY_96_APP_0221040C_H
