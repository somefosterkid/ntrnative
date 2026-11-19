#include "global.h"
#include "overlay_87_scratch_off_card_app.h"
#include "save.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "bg_window.h"
#include "pm_string.h"
#include "options.h"
#include "palette.h"
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

struct SysTaskManager;

struct TouchPadDataBuffer;

struct Window;

struct YesNoPrompt;

typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

typedef struct SysTaskManager SysTaskManager;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

struct SysTaskManager {
    u16 maxTasks;
    u16 stackPointer;
    SysTask sentinelTask;
    SysTask **taskStack;
    SysTask *tasks;
    BOOL locked;
    SysTask *currentTask;
    SysTask *nextTask;
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

void ov87_021E7590(ScratchOffCardApp *app, int touchX, int touchY);

void ov87_021E7590(ScratchOffCardApp *app, int touchX, int touchY) {
    for (int y = -3; y < 3; y++) {
        for (int x = -3; x < 3; x++) {
            if (touchX + x > 0 && touchX + x < 240 && touchY + y > 0 && touchY + y < 160) {
                app->scratchedPixels[(touchX + x) + ((touchY + y) * 240)] = 1;
            }
        }
    }
}
