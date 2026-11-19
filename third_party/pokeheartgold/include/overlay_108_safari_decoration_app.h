#ifndef POKEHEARTGOLD_OVERLAY_108_SAFARI_DECORATION_APP_H
#define POKEHEARTGOLD_OVERLAY_108_SAFARI_DECORATION_APP_H

#include "constants/heap.h"

#include "bg_window.h"
#include "menu_input_state.h"
#include "save_layout.h"
#include "safari_decoration_args.h"
#include "safari_zone.h"
#include "unk_02097268.h"

struct Camera;
struct MessageFormat;
struct MsgData;
struct NARC;
struct NNSG2dScreenData;
struct SpriteManager;
struct SpriteSystem;
struct Sprite;
struct String;
struct YesNoPrompt;

// One animation of a decoration preview model.
typedef struct SafariDecoration3dAnim {
    void *resource;       // 0x00
    void *anmRes;         // 0x04
    NNSG3dAnmObj *anmObj; // 0x08
    s32 frame;            // 0x0C
    u32 unk_10;           // 0x10
} SafariDecoration3dAnim;

// sizeof(SafariDecoration3dAnim) == 0x14

typedef struct SafariDecoration3dModel {
    NNSG3dResFileHeader *file;                // 0x00
    NNSG3dResMdlSet *mdlSet;                  // 0x04
    NNSG3dResMdl *mdl;                        // 0x08
    NNSG3dResTex *tex;                        // 0x0C
    NNSG3dRenderObj renderObj;                // 0x10
    SafariDecoration3dAnim anims[4];          // 0x64
    u16 loaded;                               // 0xB4
    u16 numAnims;                             // 0xB6
} SafariDecoration3dModel;

// sizeof(SafariDecoration3dModel) == 0xB8

// The 3D preview of the selected decoration (embedded at SafariDecorationApp::view).
typedef struct SafariDecoration3dView {
    struct Camera *camera;                    // 0x00
    NNSFndAllocator allocator;                // 0x04
    enum HeapID heapID;                       // 0x14
    struct NARC *narcModels;                  // 0x18
    struct NARC *narcAnimTable;               // 0x1C
    struct NARC *narcAnims;                   // 0x20
    SafariDecoration3dModel model;            // 0x24
    VecFx32 target;                           // 0xDC
    void *plttVramMan;                        // 0xE8
    void *texVramMan;                         // 0xEC
    u8 enabled;                               // 0xF0
    s8 unk_F1;                                // 0xF1
    u8 unk_F2[0x2];                           // 0xF2
} SafariDecoration3dView;

// sizeof(SafariDecoration3dView) == 0xF4

typedef struct SafariDecorationEntry {
    u8 id;                     // 0x0
    u8 slot;                   // 0x1
    SafariObjectConfig config; // 0x2
} SafariDecorationEntry;

// sizeof(SafariDecorationEntry) == 0x5

// The safari decoration application's work (OverlayManager data of SafariDecoration_Init).
typedef struct SafariDecorationApp {
    enum HeapID heapID;                           // 0x000
    s32 state;                                    // 0x004
    s32 initState;                                // 0x008
    s32 result;                                   // 0x00C
    MenuInputState menuInputState;                // 0x010
    s32 pendingTasks;                             // 0x014
    s8 textSpeed;                                 // 0x018
    s8 frame;                                     // 0x019
    u8 areaIndex;                                 // 0x01A
    u8 unk_01B;                                   // 0x01B
    struct SafariDecorationArgs *args;            // 0x01C
    SafariZone *safariZone;                       // 0x020
    SafariZoneAreaSet areaSet;                    // 0x024
    struct MsgData *msgData;                      // 0x30C
    struct MessageFormat *msgFormat;              // 0x310
    struct String *strBuf0;                       // 0x314
    struct String *strBuf1;                       // 0x318
    struct String *strings[4];                    // 0x31C
    struct String *unk_32C;                       // 0x32C
    struct String *unk_330;                       // 0x330
    SafariDecorationEntry *objects;               // 0x334
    SafariDecoration3dView view;                  // 0x338
    u8 timeOfDay;                                 // 0x42C
    u8 numObjects;                                // 0x42D
    u8 numPages;                                  // 0x42E
    u8 unk_42F;                                   // 0x42F
    u8 page;                                      // 0x430
    u8 cursor;                                    // 0x431
    u8 selected;                                  // 0x432
    u8 selectedId;                                // 0x433
    u8 viewDirty;                                 // 0x434
    u8 freeObjectSlots;                           // 0x435
    u8 unk_436[0x2];                              // 0x436
    struct BgConfig *bgConfig;                    // 0x438
    struct SpriteSystem *spriteSystem;            // 0x43C
    struct SpriteManager *spriteManager;          // 0x440
    struct Sprite *sprites[4];                    // 0x444
    Window windows[12];                           // 0x454
    struct YesNoPrompt *yesNoPrompt;              // 0x514
    void *scrnBuf0;                               // 0x518
    struct NNSG2dScreenData *scrn0;               // 0x51C
    void *scrnBuf1;                               // 0x520
    struct NNSG2dScreenData *scrn1;               // 0x524
    void *scrnBuf2;                               // 0x528
    struct NNSG2dScreenData *scrn2;               // 0x52C
} SafariDecorationApp;

// sizeof(SafariDecorationApp) == 0x530

// Task data of the decoration select animation (ov108_021EA584), freed by ov108_021EA5E4.
typedef struct SafariDecorationSelectTask {
    s32 *pendingTasks;        // 0x00
    SafariDecorationApp *app; // 0x04
    s8 slot;                  // 0x08
    u8 frame;                 // 0x09
    u8 unk_0A[0xA];           // 0x0A
} SafariDecorationSelectTask;

// sizeof(SafariDecorationSelectTask) == 0x14

#ifndef SDK_PORT
LAYOUT_CHECK(SafariDecorationApp_size, sizeof(SafariDecorationApp) == 0x530);
LAYOUT_CHECK(SafariDecoration3dView_size, sizeof(SafariDecoration3dView) == 0xF4);
LAYOUT_CHECK(SafariDecoration3dModel_size, sizeof(SafariDecoration3dModel) == 0xB8);
LAYOUT_CHECK(SafariDecoration3dAnim_size, sizeof(SafariDecoration3dAnim) == 0x14);
LAYOUT_CHECK(SafariDecorationEntry_size, sizeof(SafariDecorationEntry) == 0x5);
LAYOUT_CHECK(SafariDecorationSelectTask_size, sizeof(SafariDecorationSelectTask) == 0x14);
#endif

void ov108_021E8968(SafariDecorationApp *app, struct SafariDecorationArgs *args);

#endif // POKEHEARTGOLD_OVERLAY_108_SAFARI_DECORATION_APP_H
