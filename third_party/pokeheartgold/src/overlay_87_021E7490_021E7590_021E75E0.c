#include "global.h"

#include "unk_02005D10.h"
#include "save.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "bg_window.h"
#include "pm_string.h"
#include "options.h"
#include "palette.h"
#include "overlay_87_scratch_off_card_app.h"
#include "overlay_manager.h"
#include "message_printer.h"
#include "msgdata.h"
#include "yes_no_prompt.h"

struct OverlayManager;

struct OverlayManagerTemplate;

struct BgConfig;

struct CoordinatesS16;

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

extern const CoordinatesS16 ov87_021E82C0[9];

BOOL ov87_021E75E0(ScratchOffCardApp *app, u8 cellIdx);

BOOL ov87_021E75E0(ScratchOffCardApp *app, u8 cellIdx) {
    int x;

    int revealedPixels = 0;
    int left = ov87_021E82C0[cellIdx].x;
    int top = ov87_021E82C0[cellIdx].y;

    for (int y = top; y < (top + 20); y++) {
        for (x = left; x < (left + 25); x++) {
            if (app->scratchedPixels[y * 240 + x] == 1) {
                revealedPixels++;
            }
        }
    }

    if (revealedPixels >= 380) {
        if (!app->cellIsRevealed[cellIdx]) {
            PlaySE(1506);
            app->scratchedOffCellIdxs[app->numRevealedCells] = cellIdx;
            app->numRevealedCells++;
        }

        app->cellIsRevealed[cellIdx] = 1;
        return 1;
    }

    return 0;
}
