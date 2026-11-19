#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "options.h"
#include "overlay_manager.h"
#include "palette.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_87_scratch_off_card_app.h"
#include "message_printer.h"
#include "msgdata.h"
#include "yes_no_prompt.h"

struct OverlayManager;

struct MessagePrinter;

struct MsgData;

struct PaletteAnimator;

struct ScratchOffCardsAppSprite;

struct SpriteList;

struct SpriteResourceCollection;

struct MessageFormat;

struct YesNoPrompt;

ScratchOffCardsAppSprite *ov87_021E7F6C(ScratchOffCardsAppSpriteManager *spriteMan, u32 resourceID, u32 animID, u16 x, u16 y, u8 onSubScreen, u32 resourcePriority, u32 priority);

void ov87_021E7FD4(ScratchOffCardsAppSprite *appSprite, int draw);

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

extern const CoordinatesS16 ov87_021E8194[3];

void ov87_021E66B8(ScratchOffCardApp *app);

void ov87_021E66B8(ScratchOffCardApp *app) {
    for (int i = 0; i < 3; i++) {
        app->winMonSprites[i] = ov87_021E7F6C(&app->spriteMan, 1, 0, ov87_021E8194[i].x, ov87_021E8194[i].y, 0, 0, 0);
        ov87_021E7FD4(app->winMonSprites[i], 0);
    }
}
