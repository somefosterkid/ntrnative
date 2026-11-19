#ifndef POKEHEARTGOLD_OVERLAY_108_SAFARI_AREA_CUSTOMIZER_H
#define POKEHEARTGOLD_OVERLAY_108_SAFARI_AREA_CUSTOMIZER_H

// SafariAreaCustomizer: the overlay 108 area customizer app (OverlayManager data, SafariAreaCustomizer_Init).

#include "bg_window.h"
#include "obj_char_transfer.h"
#include "overlay_108_safari_area_customizer_args.h"
#include "safari_zone.h"

struct ListMenuItem;
struct ManagedSprite;
struct MessageFormat;
struct MsgData;
struct NNSG2dScreenData;
struct Sprite;
struct SpriteManager;
struct SpriteSystem;
struct String;
struct TextOBJ;
struct TouchscreenListMenu;
struct TouchscreenListMenuSpawner;
struct UnkStruct_02013534;
struct UnkStruct_02013910;
struct YesNoPrompt;

typedef struct SafariAreaCustomizer {
    enum HeapID heapID;                       // 0x0
    int unk_4;                                // 0x4
    int unk_8;                                // 0x8
    int unk_C;                                // 0xC
    BOOL inTouchMode;                         // 0x10
    SafariAreaCustomizerArgs *args;           // 0x14
    SafariZone *safariZone;                   // 0x18
    SafariZoneAreaSet areaSet;                // 0x1C
    struct MsgData *msgData;                  // 0x304
    struct MessageFormat *msgFormat;          // 0x308
    struct String *string;                    // 0x30C
    struct String *unk_310[5];                // 0x310
    struct String *unk_324[4];                // 0x324
    struct String *unk_334;                   // 0x334
    struct String *unk_338;                   // 0x338
    u8 unk_33C[4];                            // 0x33C
    BgConfig *bgConfig;                       // 0x340
    struct TouchscreenListMenuSpawner *listMenuSpawner; // 0x344
    void *unk_348;                            // 0x348
    struct SpriteSystem *spriteSystem;        // 0x34C
    struct SpriteManager *spriteManager;      // 0x350
    struct Sprite *unk_354[4];                // 0x354
    struct Sprite *unk_364;                   // 0x364
    struct Sprite *unk_368;                   // 0x368
    struct ManagedSprite *sprites[12];        // 0x36C
    struct UnkStruct_02013534 *unk_39C;       // 0x39C
    struct UnkStruct_02013910 *unk_3A0;       // 0x3A0
    struct TextOBJ *unk_3A4;                  // 0x3A4
    UnkStruct_02021AC8 unk_3A8;               // 0x3A8
    Window windows[15];                       // 0x3B4
    Window textWindow;                        // 0x4A4
    struct ListMenuItem *unk_4B4;             // 0x4B4
    struct ListMenuItem *unk_4B8;             // 0x4B8
    struct TouchscreenListMenu *listMenu;     // 0x4BC
    struct YesNoPrompt *yesNoPrompt;          // 0x4C0
    void (*unk_4C4)(struct SafariAreaCustomizer *); // 0x4C4
    void (*unk_4C8)(struct SafariAreaCustomizer *); // 0x4C8
    void *unk_4CC;                            // 0x4CC
    void *unk_4D0;                            // 0x4D0
    struct NNSG2dScreenData *unk_4D4;         // 0x4D4
    struct NNSG2dScreenData *unk_4D8;         // 0x4D8
    u8 unk_4DC[0x18000];                      // 0x4DC
    s8 textSpeed;                             // 0x184DC
    s8 frame;                                 // 0x184DD
    u8 page;                                  // 0x184DE
    u8 areaIdx;                               // 0x184DF
    u8 cursor;                                // 0x184E0
    u8 unk_184E1;                             // 0x184E1
    u8 unk_184E2_0 : 1;                       // 0x184E2
    u8 unk_184E2_1 : 1;
    u8 unk_184E2_2 : 1;
    u8 unk_184E2_3 : 5;
    u8 unk_184E3;                             // 0x184E3
    s16 unk_184E4;                            // 0x184E4
    s16 unk_184E6;                            // 0x184E6
    BOOL areaChanged;                         // 0x184E8
} SafariAreaCustomizer;

// sizeof(SafariAreaCustomizer) == 0x184EC

#endif // POKEHEARTGOLD_OVERLAY_108_SAFARI_AREA_CUSTOMIZER_H
