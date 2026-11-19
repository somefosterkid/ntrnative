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

struct OverlayManager;
struct MessagePrinter;
struct MsgData;
struct PaletteAnimator;
struct ScratchOffCardsAppSprite;
struct SpriteList;
struct SpriteResourceCollection;
struct MessageFormat;
struct YesNoPrompt;
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
void ov87_021E7460(ScratchOffCardApp *app, u32 bgLayer, u8 palette, u8 width, u8 height);

u8 ov87_021E7490(u8 *arg0);
void ov87_021E74B8(u8 *arg0);
void ov87_021E74D4(u8 *arg0);

void ov87_021E7460(ScratchOffCardApp *app, u32 bgLayer, u8 palette, u8 width, u8 height) {
    BgTilemapRectChangePalette(app->bgConfig, bgLayer, 0, 0, width, height, palette);
    ScheduleBgTilemapBufferTransfer(app->bgConfig, bgLayer);
}

u8 ov87_021E7490(u8 *arg0) {
    u8 v0;
    u8 v1;

    v1 = 0;
    v0 = 0;
    do {
        if (*(u8 *)((u8 *)(arg0 + v0) + 0x394) == 1) {
            v1 += 1;
        }
        v0 += 1;
    } while ((u32)v0 < 9U);
    return v1;
}

void ov87_021E74B8(u8 *arg0) {
    u8 v0;

    v0 = 0;
    do {
        *(s8 *)((u8 *)(arg0 + v0) + 0x394) = 0;
        v0 += 1;
    } while ((u32)v0 < 9U);
}

void ov87_021E74D4(u8 *arg0) {
    u8 v0;

    v0 = 0;
    do {
        *(s8 *)((u8 *)(arg0 + v0) + 0x3A5) = 0;
        v0 += 1;
    } while ((u32)v0 < 9U);
}
