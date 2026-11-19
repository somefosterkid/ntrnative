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
#include "overlay_87_scratch_off_card_app.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "message_printer.h"
#include "msgdata.h"
#include "yes_no_prompt.h"

void *ov87_021E7F6C(void *, u32, u32, u16, u16, u8, u32, u32);

void ov87_021E7FD4(void *, u32);

void ov87_021E6704(ScratchOffCardApp *app);

struct OverlayManager;
struct MessagePrinter;
struct MsgData;
struct PaletteAnimator;
struct ScratchOffCardsAppSprite;
struct SpriteList;
struct SpriteResourceCollection;
struct MessageFormat;
struct YesNoPrompt;
void *ov87_021E7FC0(ScratchOffCardsAppSprite *appSprite);
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
void ov87_021E6760(ScratchOffCardApp *app);

void ov87_021E6704(ScratchOffCardApp *app) {
    app->skipCardButtonSprite = ov87_021E7F6C(&app->spriteMan, 3, 1, 0x44, 0xa0, 0, 1, 0);
    ov87_021E7FD4(app->skipCardButtonSprite, 0);
    app->centerMessageBoxSprite = ov87_021E7F6C(&app->spriteMan, 3, 3, 0x44, 0x44, 0, 1, 0);
    ov87_021E7FD4(app->centerMessageBoxSprite, 0);
}

void ov87_021E6760(ScratchOffCardApp *app) {
    for (int i = 0; i < 4; i++) {
        if (app->cardSprites[i] != 0L) {
            ov87_021E7FC0(app->cardSprites[i]);
        }
    }
}
