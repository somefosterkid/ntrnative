#ifndef POKEHEARTGOLD_OVERLAY_68_MOVE_RELEARNER_H
#define POKEHEARTGOLD_OVERLAY_68_MOVE_RELEARNER_H

// MoveRelearner: the move relearner / move tutor app of overlay 68 (MoveRelearner_Init:
// OverlayManager_CreateAndGetData(man, sizeof(MoveRelearner) = 0x1dc, HEAP_ID_66)).
// One canonical layout (hk287) for the per-file lenses MoveReminderController (Platinum layout),
// WazaOshieWork (Platinum WazaOshie layout), UnkStruct_ov68_021E773C / 021E5F68 and Ov68App_021E6320.

#include "bg_window.h"
#include "menu_input_state.h"
#include "move_relearner.h"
#include "unk_02088288.h"

struct Camera;
struct GridInputHandler;
struct ListMenuItem;
struct ManagedSprite;
struct MessageFormat;
struct MsgData;
struct OverlayManager;
struct Pokepic;
struct PokepicManager;
struct SpriteManager;
struct SpriteSystem;
struct String;
struct YesNoPrompt;

#define MOVE_RELEARNER_NUM_WINDOWS 15
#define MOVE_RELEARNER_NUM_SPRITES 12

// Camera and Pokemon picture of the 3D top screen (MoveRelearner.monPic, 0x1c).
typedef struct MoveRelearnerMonPic {
    struct Camera *camera;
    struct PokepicManager *pokepicManager;
    u8 padding_08[0x10];
    struct Pokepic *pokepic;
} MoveRelearnerMonPic;

// Flash of a touched bottom-screen button (MoveRelearner.buttonFlash, 0xc).
typedef struct MoveRelearnerButtonFlash {
    u8 x;
    u8 y;
    u8 width;
    u8 height;
    u16 timer;
    u16 state;
    int nextState;
} MoveRelearnerButtonFlash;

typedef struct MoveRelearner {
    MoveRelearnerArgs *args;                                   // 0x000
    struct BgConfig *bgConfig;                                 // 0x004
    Window windows[MOVE_RELEARNER_NUM_WINDOWS];                // 0x008
    struct MsgData *msgData;                                   // 0x0f8
    struct MessageFormat *msgFormat;                           // 0x0fc
    struct String *string;                                     // 0x100
    struct String *ppLabel;                                    // 0x104
    struct String *ppFormat;                                   // 0x108
    u8 padding_10C[4];                                         // 0x10c
    struct ListMenuItem *moveList;                             // 0x110
    u8 padding_114[4];                                         // 0x114
    struct YesNoPrompt *yesNoPrompt;                           // 0x118
    struct SpriteSystem *spriteSystem;                         // 0x11c
    struct SpriteManager *spriteManager;                       // 0x120
    struct ManagedSprite *sprites[MOVE_RELEARNER_NUM_SPRITES]; // 0x124: down arrow, up arrow, cursor, category icon, 4 list type icons, 4 known-move type icons
    MoveRelearnerMonPic monPic;                                // 0x154
    PokemonSummaryArgs summary;                                // 0x170
    struct OverlayManager *summaryManager;                     // 0x1ac
    int nextState;                                             // 0x1b0: state after the current message
    MenuInputState menuInputState;                             // 0x1b4
    u8 numMoves;                                               // 0x1b8: entries of moveList
    u8 textPrinterId;                                          // 0x1b9
    u8 question;                                               // 0x1ba: the yes/no question on screen
    u8 unk_1BB;                                                // 0x1bb
    u8 unk_1BC;                                                // 0x1bc
    u8 padding_1BD[3];                                         // 0x1bd
    void *screenFile;                                          // 0x1c0
    NNSG2dScreenData *screenData;                              // 0x1c4: button tiles copied to the bottom screen
    struct GridInputHandler *gridInput;                        // 0x1c8
    u16 lastGridInput;                                         // 0x1cc
    u16 learnButtonActive;                                     // 0x1ce
    MoveRelearnerButtonFlash buttonFlash;                      // 0x1d0
} MoveRelearner;                                               // size: 0x1dc

#endif // POKEHEARTGOLD_OVERLAY_68_MOVE_RELEARNER_H
