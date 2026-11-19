#include "global.h"
#include "save.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_87_scratch_off_card_app.h"
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

void ov87_021E7AB0(ScratchOffCardApp *app, u32 x, u32 y);

void ov87_021E7AB0(ScratchOffCardApp *app, u32 x, u32 y) {
    u8 clearMask = 0xff;

    if (x % 2 == 0) {
        clearMask ^= 0xf;
    } else {
        clearMask ^= 0xf0;
    }

    u32 tileX = x / 8 * 32;
    u32 tileY = y / 8 * 32 * 32;
    u32 offsetX = x % 8 / 2;

    u32 offsetY;
    if (y < 8) {
        offsetY = y * 4;
    } else {
        offsetY = y % 8;
        offsetY = offsetY * 4;
    }

    u32 pos = tileY + tileX + offsetX + offsetY;
    app->scratchCellsDataBackup[pos] &= clearMask;
}
