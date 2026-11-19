#ifndef POKEHEARTGOLD_OVERLAY_56_MAIL_VIEWER_APP_H
#define POKEHEARTGOLD_OVERLAY_56_MAIL_VIEWER_APP_H

#include "bg_window.h"
#include "constants/heap.h"

struct ManagedSprite;
struct MsgData;
struct NNSG2dScreenData;
struct PaletteData;
struct SpriteManager;
struct SpriteSystem;
struct UnkStruct_ov55_021E5B08;
struct YesNoPrompt;

enum MailViewerMode {
    MAIL_VIEWER_MODE_READ = 0,
    MAIL_VIEWER_MODE_WRITE,
    MAIL_VIEWER_MODE_CONFIRM_EMPTY,
    MAIL_VIEWER_MODE_CANCEL,
};

enum MailViewerWindow {
    MAIL_VIEWER_WINDOW_SENTENCE_1 = 0,
    MAIL_VIEWER_WINDOW_SENTENCE_2,
    MAIL_VIEWER_WINDOW_SENTENCE_3,
    MAIL_VIEWER_WINDOW_CONFIRM,
    MAIL_VIEWER_WINDOW_CANCEL,
    MAIL_VIEWER_WINDOW_MESSAGE,
    MAIL_VIEWER_WINDOW_COUNT,
};

typedef struct MailViewerApp MailViewerApp;

// Input-mode switch callbacks (ov56_021E5D44 / ov56_021E60F4 call them with the app when the
// player changes between the buttons and the touch screen).
typedef s32 (*MailViewerInputModeCallback)(MailViewerApp *);

struct MailViewerApp {
    enum HeapID heapID;
    int state;
    u16 subState;
    u8 unkA;
    u8 frame;
    u8 mode;
    u8 initialMode;
    u8 padding;
    u8 printerID;
    u8 textFrameDelay;
    u8 verticalSelectionIndex;
    u8 horizontalSelectionIndex;
    u8 pauseGlowEffect;
    u8 blendFraction;
    u8 blendDirection;
    u8 blendIndex;
    u8 prevBlendIndex;
    struct BgConfig *bgConfig;
    struct UnkStruct_ov55_021E5B08 *args;
    struct MsgData *loader;
    void *dummy[1];
    u8 padding2[8];
    struct PaletteData *paletteData;
    MailViewerInputModeCallback unk34;
    MailViewerInputModeCallback unk38;
    void *backgroundNSCRBuffer;
    void *interfaceNSCRBuffer;
    struct NNSG2dScreenData *backgroundScreenData;
    struct NNSG2dScreenData *interfaceScreenData;
    Window windows[MAIL_VIEWER_WINDOW_COUNT];
    struct YesNoPrompt *yesNoPrompt;
    struct SpriteSystem *spriteSys;
    struct SpriteManager *spriteMan;
    struct ManagedSprite *managedSprites[3];
};

// sizeof(MailViewerApp) == 0xc4 on the DS

#endif // POKEHEARTGOLD_OVERLAY_56_MAIL_VIEWER_APP_H
