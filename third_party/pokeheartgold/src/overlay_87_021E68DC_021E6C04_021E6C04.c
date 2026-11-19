#include "global.h"

#include "bg_window.h"
#include "palette.h"
#include "unk_0200B150.h"
#include "vram_transfer_manager.h"
#include "overlay_87_scratch_off_card_app.h"
#include "save.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "pm_string.h"
#include "options.h"
#include "overlay_manager.h"
#include "message_printer.h"
#include "msgdata.h"
#include "yes_no_prompt.h"

struct OverlayManager;

struct OverlayManagerTemplate;

struct BgConfig;

struct MessagePrinter;

struct G2dRenderer;

struct MsgData;

struct NARC;

struct Options;

struct PaletteAnimator;

struct PaletteData;

struct SaveData;

struct ScratchOffCardApp;

struct ScratchOffCardsAppSprite;

struct ScratchOffCardsAppSpriteManager;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct String;

struct MessageFormat;

struct SysTask;

struct SysTaskQueue;

struct TouchPadDataBuffer;

struct Window;

struct YesNoPrompt;

typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

typedef struct PaletteAnimator PaletteAnimator;

typedef void (*UnkFuncPtr_020157E4)(PaletteAnimator *, u16 *);

struct PaletteAnimator {
    SysTask *unk_00;
    UnkFuncPtr_020157E4 unk_04;
    PaletteData *unk_08;
    u16 unk_0C[16];
    u16 unk_2C[16];
    u8 unk_4C;
    u8 unk_4D;
    u8 unk_4E;
    u8 unk_4F;
};

void ov87_021E6C04(void *data);

void ov87_021E6C04(void *data) {
    ScratchOffCardApp *app = data;

    if (app->unused != NULL) {
        return;
    }

    if (app->plttData != NULL) {
        PaletteData_PushTransparentBuffers(app->plttData);
    }

    DoScheduledBgGpuUpdates(app->bgConfig);
    GF_RunVramTransferTasks();
    OamManager_ApplyAndResetBuffers();

    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}
