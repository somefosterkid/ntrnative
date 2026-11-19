#ifndef POKEHEARTGOLD_OVERLAY_59_APRICORN_APP_H
#define POKEHEARTGOLD_OVERLAY_59_APRICORN_APP_H

#include "bg_window.h"
#include "filesystem.h"
#include "list_menu_items.h"
#include "menu_input_state.h"
#include "message_format.h"
#include "overlay_58.h"
#include "msgdata.h"
#include "pm_string.h"
#include "save.h"
#include "sprite.h"
#include "sprite_system.h"
#include "touchscreen_list_menu.h"
#include "unk_02031B0C.h"
#include "yes_no_prompt.h"

// a juice taste rank entry as written by sub_02031DA0 (2 bytes)
typedef struct Ov59JuiceRank {
    u16 lo : 4;
    u16 hi : 4;
    u16 value : 8;
} Ov59JuiceRank;

// the 0x29c object of ov59_02237D40 / ov59_02237DA8 (Apricorn Kurt selection screen)
typedef struct Ov59ApricornApp {
    enum HeapID heapId;                 // 0x00
    Ov58ApricornBoxWork *parent;        // 0x04
    u8 unk_08[8];                       // 0x08
    SaveApricornBox *apricornBox;       // 0x10
    u8 unk_14;
    u8 unk_15;
    u8 unk_16;
    u8 unk_17;
    u8 unk_18;
    u8 unk_19;
    u8 apricornCounts[7];               // 0x1A
    u8 unk_21;
    ApricornJuice juice;                // 0x22
    Ov59JuiceRank ranks[3];             // 0x2A
    u8 unk_30[6];
    u8 rankCount;                       // 0x36
    u8 unk_37;
    s32 unk_38;
    u16 state;                          // 0x3C
    u16 subState;                       // 0x3E
    enum MenuInputState menuInputState; // 0x40
    u8 mode;                            // 0x44
    u8 textFrameDelay;                  // 0x45
    u8 frame;                           // 0x46
    u8 printerId;                       // 0x47
    u8 unk_48;
    u8 unk_49;
    u8 unk_4A;
    u8 unk_4B;
    u8 unk_4C;
    u8 selected;                        // 0x4D
    u8 quantity;                        // 0x4E
    u8 maxQuantity;                     // 0x4F
    u8 animCount;                       // 0x50
    u8 unk_51;
    u8 unk_52;
    u8 unk_53;
    BgConfig *bgConfig;                 // 0x54
    TouchscreenListMenuSpawner *menuSpawner; // 0x58
    MsgData *msgData;                   // 0x5C
    MessageFormat *msgFormat;           // 0x60
    String *unk_64;
    String *unk_68;
    String *unk_6C;
    u8 unk_70[4];
    String *unk_74;
    String *unk_78[22];                 // 0x78
    String *apricornNames[7];           // 0xD0
    u8 unk_EC[0x3C];
    Window windows[18];                 // 0x128
    YesNoPrompt *yesNoPrompt;           // 0x248
    SpriteSystem *spriteSystem;         // 0x24C
    SpriteManager *spriteManager;       // 0x250
    Sprite *sprites[15];                // 0x254
    NARC *narc;                         // 0x290
    ListMenuItem *listMenuItems;        // 0x294
    TouchscreenListMenu *touchMenu;     // 0x298
} Ov59ApricornApp;

// a list menu definition of ov59_0223C668 (indexed by Ov59ApricornApp.mode - 1)
typedef struct Ov59MenuDef {
    u16 count;
    u8 x;
    u8 y;
    const u8 *items;
} Ov59MenuDef;

// a sprite-move task (one SysTask, heap-allocated)
typedef struct Ov59SpriteMoveTask {
    Ov59ApricornApp *app; // 0x00
    u8 state;             // 0x04
    u8 numSteps;          // 0x05
    u8 step;              // 0x06
    u8 selected;          // 0x07
    s16 targetX;          // 0x08
    s16 targetY;          // 0x0A
    fx32 dx;              // 0x0C
    fx32 dy;              // 0x10
    fx32 x;               // 0x14
    fx32 y;               // 0x18
    Sprite *sprite;       // 0x1C
} Ov59SpriteMoveTask;

#endif // POKEHEARTGOLD_OVERLAY_59_APRICORN_APP_H
